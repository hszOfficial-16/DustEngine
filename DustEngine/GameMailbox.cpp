#include "GameMailbox.h"
#include "GameBlockAllocator.h"

#include <new>
#include <unordered_set>
#include <unordered_map>

size_t GamePair::Hasher::Combine(size_t seed, std::string strValue) const
{
	return seed ^ std::hash<std::string>()(strValue) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class GameMessage::Impl
{
public:
	std::unordered_map<std::string, std::string> m_mapPair;

	// ���ü���
	uint16_t m_nReferenceCount;

public:
	Impl() : m_nReferenceCount(0) {}
};

std::string& GameMessage::operator[](const std::string& strKey)
{
	return m_pImpl->m_mapPair[strKey];
}

uint16_t& GameMessage::GetReferenceCount()
{
	return m_pImpl->m_nReferenceCount;
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
	// ���������
	std::string m_strName;

	// ����Ϣ�ַ�������������Ҫ���������
	std::unordered_map<std::string, std::string> m_mapRequirement;

	// �Ѿ��ַ����������е���Ϣ
	std::vector<GameMessage*> m_vecMessage;

public:
	Impl(const Def& defMailbox)
	{
		m_strName = defMailbox.strName;

		for (std::initializer_list<GamePair>::iterator iter = defMailbox.listRequirement.begin();
			iter != defMailbox.listRequirement.end(); iter++)
		{
			m_mapRequirement[(*iter).strKey] = (*iter).strValue;
		}
	}
	~Impl()
	{
		for (std::vector<GameMessage*>::iterator iter = m_vecMessage.begin();
			iter != m_vecMessage.end(); iter++)
		{
			if (--(*iter)->GetReferenceCount() > 0) continue;

			(*iter)->~GameMessage();
			GameBlockAllocator::GetInstance().Free(*iter, sizeof(GameMessage));
		}
	}
};

const std::string& GameMailbox::operator[](const std::string& strKey)
{
	return m_pImpl->m_mapRequirement[strKey];
}

const std::string& GameMailbox::GetName()
{
	return m_pImpl->m_strName;
}

GameMessage* GameMailbox::GetMessage(size_t nIndex)
{
	return m_pImpl->m_vecMessage[nIndex];
}

size_t GameMailbox::GetMessageCount()
{
	return m_pImpl->m_vecMessage.size();
}

void GameMailbox::Clear()
{
	m_pImpl->m_vecMessage.clear();
}

GameMailbox::GameMailbox(const Def& defMailbox)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl(defMailbox);
}

GameMailbox::~GameMailbox()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}

class GameMailboxManager::Impl
{
public:
	std::unordered_set<GameMailbox*> m_setMailbox;

	// ���в����ϸ�����������
	std::unordered_map<GamePair, std::unordered_set<GameMailbox*>, GamePair::Hasher, GamePair::Equalizer> m_mapQualified;

public:
	~Impl()
	{
		for (std::unordered_set<GameMailbox*>::iterator iter = m_setMailbox.begin();
			iter != m_setMailbox.end(); iter++)
		{
			(*iter)->~GameMailbox();
			GameBlockAllocator::GetInstance().Free(*iter, sizeof(GameMailbox));
		}
	}
};

void GameMailboxManager::Publish(GameMessage* pMessage)
{
	std::unordered_set<GameMailbox*> setCandidate;

	// �����з�����Ϣ����һ����������������ѡ����
	for (std::unordered_map<std::string, std::string>::iterator iterPair = pMessage->m_pImpl->m_mapPair.begin();
		iterPair != pMessage->m_pImpl->m_mapPair.end(); iterPair++)
	{
		GamePair pairTemp = { (*iterPair).first, (*iterPair).second };

		if (m_pImpl->m_mapQualified.find(pairTemp) != m_pImpl->m_mapQualified.end())
		{
			for (std::unordered_set<GameMailbox*>::iterator iterMailbox = m_pImpl->m_mapQualified[pairTemp].begin();
				iterMailbox != m_pImpl->m_mapQualified[pairTemp].end(); iterMailbox++)
			{
				setCandidate.insert(*iterMailbox);
			}
		}
	}

	// �Ժ�ѡ�����ÿһ������������ϸ�ıȽ�
	for (std::unordered_set<GameMailbox*>::iterator iterMailbox = setCandidate.begin();
		iterMailbox != setCandidate.end(); iterMailbox++)
	{
		for (std::unordered_map<std::string, std::string>::iterator iterPair = (*iterMailbox)->m_pImpl->m_mapRequirement.begin();
			iterPair != (*iterMailbox)->m_pImpl->m_mapRequirement.end(); iterPair++)
		{
			if (pMessage->m_pImpl->m_mapPair.find((*iterPair).first) == pMessage->m_pImpl->m_mapPair.end())
			{
				setCandidate.erase(*iterMailbox);
			}
			else if ((*pMessage)[(*iterPair).first] != (*iterPair).second)
			{
				setCandidate.erase(*iterMailbox);
			}
		}
	}

	// ����Ϣ���͵����з���������������
	for (std::unordered_set<GameMailbox*>::iterator iter = setCandidate.begin();
		iter != setCandidate.end(); iter++)
	{
		(*iter)->m_pImpl->m_vecMessage.push_back(pMessage);
	}
}

void GameMailboxManager::AddMailbox(GameMailbox* pMailbox)
{
	if (!pMailbox)
	{
		return;
	}

	if (m_pImpl->m_setMailbox.find(pMailbox) != m_pImpl->m_setMailbox.end())
	{
		return;
	}

	m_pImpl->m_setMailbox.insert(pMailbox);

	// ����������ע����ʲô��������Ϣ
	for (std::unordered_map<std::string, std::string>::iterator iterPair = pMailbox->m_pImpl->m_mapRequirement.begin();
		iterPair != pMailbox->m_pImpl->m_mapRequirement.end(); iterPair++)
	{
		m_pImpl->m_mapQualified[{(*iterPair).first, (*iterPair).second}].insert(pMailbox);
	}
}

void GameMailboxManager::DeleteMailbox(GameMailbox* pMailbox)
{
	if (!pMailbox)
	{
		return;
	}

	if (m_pImpl->m_setMailbox.find(pMailbox) == m_pImpl->m_setMailbox.end())
	{
		return;
	}

	m_pImpl->m_setMailbox.erase(pMailbox);

	// �������
	for (std::unordered_map<std::string, std::string>::iterator iterPair = pMailbox->m_pImpl->m_mapRequirement.begin();
		iterPair != pMailbox->m_pImpl->m_mapRequirement.end(); iterPair++)
	{
		m_pImpl->m_mapQualified[{(*iterPair).first, (*iterPair).second}].erase(pMailbox);
	}

	// �ͷ��ڴ�
	pMailbox->~GameMailbox();
	GameBlockAllocator::GetInstance().Free(pMailbox, sizeof(GameMailbox));
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