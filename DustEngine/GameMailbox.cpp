#include "GameMailbox.h"
#include "GameBlockAllocator.h"

#include <new>
#include <unordered_set>
#include <unordered_map>

class GameMessage::Impl
{
public:
	std::unordered_map<std::string, std::string> m_mapPairs;
};

const std::string& GameMessage::operator[](const std::string strKey)
{
	return m_pImpl->m_mapPairs[strKey];
}

void GameMessage::AddPair(const std::string& strKey, const std::string& strValue)
{
	m_pImpl->m_mapPairs[strKey] = strValue;
}

void GameMessage::DeletePair(const std::string& strKey)
{
	m_pImpl->m_mapPairs.erase(strKey);
}

GameMessage::GameMessage()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameMessage::~GameMessage()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

class GameMailbox::Impl
{
public:
	// 投递到该信箱所需要满足的条件
	std::unordered_map<std::string, std::string> m_mapPairs;
	
	// 已经投递到该信箱中的消息
	std::vector<GameMessage*> m_vecMessages;

public:
	Impl(const std::initializer_list<std::pair<std::string, std::string>>& listPairs)
	{
		m_mapPairs.clear();

		for (std::initializer_list<std::pair<std::string, std::string>>::iterator iter = listPairs.begin();
			iter != listPairs.end(); iter++)
		{
			m_mapPairs[(*iter).first] = (*iter).second;
		}
	}
};

GameMessage* GameMailbox::GetMessage(size_t nIndex)
{
	return m_pImpl->m_vecMessages[nIndex];
}

void GameMailbox::Clear()
{
	m_pImpl->m_vecMessages.clear();
}

GameMailbox::GameMailbox(const std::initializer_list<std::pair<std::string, std::string>>& listPairs)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl(listPairs);
}

GameMailbox::~GameMailbox()
{
	// 清除该信箱的所有缓存
	GameMailboxManager::GetInstance().ClearCache(this);

	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

// from boost (functional/hash):
// see http://www.boost.org/doc/libs/1_35_0/doc/html/hash/combine.html template
template <typename T>
inline void hash_combine(std::size_t& seed, const T& val) {
	seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
// auxiliary generic functions to create a hash value using a seed
template <typename T> inline void hash_val(std::size_t& seed, const T& val) {
	hash_combine(seed, val);
}
template <typename T, typename... Types>
inline void hash_val(std::size_t& seed, const T& val, const Types &... args) {
	hash_combine(seed, val);
	hash_val(seed, args...);
}

template <typename... Types>
inline std::size_t hash_val(const Types &... args) {
	std::size_t seed = 0;
	hash_val(seed, args...);
	return seed;
}

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& p) const {
		return hash_val(p.first, p.second);
	}
};

class GameMailboxManager::Impl
{
public:
	std::unordered_set<GameMailbox*> m_setMailboxes;

	// 不符合该条件的所有信箱的缓存
	std::unordered_map<std::pair<std::string, std::string>, std::vector<GameMailbox*>, pair_hash> m_mapUnqualified;
};

void GameMailboxManager::Publish(GameMessage* pMessage)
{
	std::unordered_set<GameMailbox*> setCandidate(m_pImpl->m_setMailboxes);

	// 将所有不符合条件的信箱淘汰出去
	for (std::unordered_set<GameMailbox*>::iterator iterMailbox = m_pImpl->m_setMailboxes.begin();
		iterMailbox != m_pImpl->m_setMailboxes.end(); iterMailbox++)
	{
		for (std::unordered_map<std::string, std::string>::iterator iterPair = (*iterMailbox)->m_pImpl->m_mapPairs.begin();
			iterPair != (*iterMailbox)->m_pImpl->m_mapPairs.end(); iterPair++)
		{
			for (std::vector<GameMailbox*>::iterator iterEliminated = m_pImpl->m_mapUnqualified[*iterPair].begin();
				iterEliminated != m_pImpl->m_mapUnqualified[*iterPair].end(); iterEliminated++)
			{
				setCandidate.erase(*iterEliminated);
			}
		}
	}

	// 将消息发布到符合消息的信箱中去
	for (std::unordered_set<GameMailbox*>::iterator iter = setCandidate.begin();
		iter != setCandidate.end(); iter++)
	{
		(*iter)->m_pImpl->m_vecMessages.push_back(pMessage);
	}
}

void GameMailboxManager::Register(GameMailbox* pMailbox)
{
	m_pImpl->m_setMailboxes.insert(pMailbox);

	// 缓存该信箱需要信息满足的条件
	GameMailboxManager::GetInstance().Cache(pMailbox);
}

void GameMailboxManager::Unregister(GameMailbox* pMailbox)
{
	// 清除该信箱的缓存
	GameMailboxManager::GetInstance().ClearCache(pMailbox);

	m_pImpl->m_setMailboxes.erase(pMailbox);
}

void GameMailboxManager::Cache(GameMailbox* pMailbox)
{
	for (std::unordered_map<std::string, std::string>::iterator iterPair = pMailbox->m_pImpl->m_mapPairs.begin();
		iterPair != pMailbox->m_pImpl->m_mapPairs.end(); iterPair++)
	{
		for (std::unordered_set<GameMailbox*>::iterator iterMailbox = m_pImpl->m_setMailboxes.begin();
			iterMailbox != m_pImpl->m_setMailboxes.end(); iterMailbox++)
		{
			// 如果这个信箱不符合该条件，则缓存到不符合该条件的表中
			if ((*iterMailbox)->m_pImpl->m_mapPairs[(*iterPair).first] != (*iterPair).second)
			{
				m_pImpl->m_mapUnqualified[*iterPair].push_back(*iterMailbox);
			}
		}
	}
}

void GameMailboxManager::ClearCache(GameMailbox* pMailbox)
{
	for (std::unordered_map<std::string, std::string>::iterator iterPair = pMailbox->m_pImpl->m_mapPairs.begin();
		iterPair != pMailbox->m_pImpl->m_mapPairs.end(); iterPair++)
	{
		std::vector<GameMailbox*>::iterator iterFound = m_pImpl->m_mapUnqualified[*iterPair].begin();
		
		// 在缓存向量中找到该缓存
		while ((*iterFound) != pMailbox) iterFound++;

		// 删除该缓存
		m_pImpl->m_mapUnqualified[*iterPair].erase(iterFound);
	}
}

GameMailboxManager::GameMailboxManager()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameMailboxManager::~GameMailboxManager()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}