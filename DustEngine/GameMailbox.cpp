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
	// Ͷ�ݵ�����������Ҫ���������
	std::unordered_map<std::string, std::string> m_mapPairs;
	
	// �Ѿ�Ͷ�ݵ��������е���Ϣ
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
	// �������������л���
	GameMailboxManager::GetInstance().ClearCache(this);

	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}



class GameMailboxManager::Impl
{
public:
	std::unordered_set<GameMailbox*> m_setMailboxes;

	// �����ϸ���������������Ļ���
	std::unordered_map<std::pair<std::string, std::string>, std::vector<GameMailbox*>> m_mapUnqualified;
};

void GameMailboxManager::Publish(GameMessage* pMessage)
{
	std::unordered_set<GameMailbox*> setCandidate(m_pImpl->m_setMailboxes);

	// �����в�����������������̭��ȥ
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

	// ����Ϣ������������Ϣ��������ȥ
	for (std::unordered_set<GameMailbox*>::iterator iter = setCandidate.begin();
		iter != setCandidate.end(); iter++)
	{
		(*iter)->m_pImpl->m_vecMessages.push_back(pMessage);
	}
}

void GameMailboxManager::Register(GameMailbox* pMailbox)
{
	m_pImpl->m_setMailboxes.insert(pMailbox);

	// �����������Ҫ��Ϣ���������
	GameMailboxManager::GetInstance().Cache(pMailbox);
}

void GameMailboxManager::Unregister(GameMailbox* pMailbox)
{
	// ���������Ļ���
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
			// ���������䲻���ϸ��������򻺴浽�����ϸ������ı���
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
		
		// �ڻ����������ҵ��û���
		while ((*iterFound) != pMailbox) iterFound++;

		// ɾ���û���
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