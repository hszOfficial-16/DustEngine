#include "GameEntity.h"
#include "GameScene.h"
#include "GameComponent.h"
#include "GameBlockAllocator.h"

#include <new>
#include <unordered_map>

class GameEntity::Impl
{
public:
	// ʵ����������
	std::unordered_map<std::string, GameComponent*> m_mapComponents;

	// ʵ�������б�ͷ(���ڱ���)
	GameComponent* m_pComponentHead;

	// ʵ��ĸ��ӹ�ϵ
	GameEntity* m_pParent;
	GameEntity* m_pHead;
	GameEntity* m_pPrev;
	GameEntity* m_pNext;

	// ʵ�������ĳ���
	GameScene* m_pScene;
	// ʵ���ڳ����ж�Ӧ����һ��ʵ��
	GameEntity* m_pSceneNext;

public:
	Impl()
	{
		m_pComponentHead = nullptr;

		m_pParent = nullptr;
		m_pHead = nullptr;
		m_pPrev = nullptr;
		m_pNext = nullptr;

		m_pScene = nullptr;
		m_pSceneNext = nullptr;
	}

	~Impl()
	{
		for (std::unordered_map<std::string, GameComponent*>::iterator iter = m_mapComponents.begin();
			iter != m_mapComponents.end(); iter++)
		{
			if (m_pScene)
			{
				GameBlockAllocator::GetInstance().Free((*iter).second, (*iter).second->GetSize());
			}
			GameBlockAllocator::GetInstance().Free((*iter).second, (*iter).second->GetSize());
		}
	}
};

GameEntity* GameEntity::GetParent()
{
	return m_pImpl->m_pParent;
}

void GameEntity::SetParent(GameEntity* pParent)
{
	// ����ýڵ�ԭ�����и��ڵ㣬��ɾ����ϵ
	if (m_pImpl->m_pParent)
	{
		m_pImpl->m_pParent->DeleteChild(this);
	}
	if (pParent)
	{
		pParent->AddChild(this);
	}
}

GameEntity* GameEntity::GetChildList()
{
	return m_pImpl->m_pHead;
}

GameEntity* GameEntity::GetNext()
{
	return m_pImpl->m_pNext;
}

void GameEntity::AddChild(GameEntity* pChild)
{
	if (!pChild) return;

	// �����ӽڵ����и��ӹ�ϵ�����Ƴ���ι�ϵ
	if (pChild->GetParent())
	{
		pChild->GetParent()->DeleteChild(pChild);
	}

	pChild->m_pImpl->m_pParent = this;
	pChild->m_pImpl->m_pNext = m_pImpl->m_pHead;

	if (m_pImpl->m_pHead)
	{
		m_pImpl->m_pHead->m_pImpl->m_pPrev = pChild;
	}
	m_pImpl->m_pHead = pChild;
}

void GameEntity::DeleteChild(GameEntity* pChild)
{
	if (!pChild) return;

	// �� pEntity ���Ǹýڵ���ӽڵ㣬�򷵻�
	if (pChild->m_pImpl->m_pParent != this) return;

	pChild->m_pImpl->m_pParent = nullptr;

	// ����ӽڵ�ǰ����ڽڵ㣬��˵��������λ������ͷ
	if (pChild->m_pImpl->m_pPrev)
	{
		pChild->m_pImpl->m_pPrev->m_pImpl->m_pNext = pChild->m_pImpl->m_pNext;
		if (pChild->m_pImpl->m_pNext)
		{
			pChild->m_pImpl->m_pNext->m_pImpl->m_pPrev = pChild->m_pImpl->m_pPrev;
		}
		return;
	}

	// ����ӽڵ�ǰ�治���ڽڵ㣬��˵����λ������ͷ
	if (pChild->m_pImpl->m_pNext)
	{
		pChild->m_pImpl->m_pNext->m_pImpl->m_pPrev = nullptr;
		m_pImpl->m_pHead = pChild->m_pImpl->m_pNext;
		return;
	}

	m_pImpl->m_pHead = nullptr;
}

void GameEntity::AddComponent(GameComponent* pComponent)
{
	if (!pComponent) return;

	// ��������뵽��ʵ����
	pComponent->SetEntity(this);
	m_pImpl->m_mapComponents[pComponent->GetName()] = pComponent;

	if (m_pImpl->m_pComponentHead)
	{
		m_pImpl->m_pComponentHead->SetEntityNext(pComponent);
	}
	m_pImpl->m_pComponentHead = pComponent;

	// ���ʵ���ڳ����У��򽫸�������볡����
	if (GetScene()) GetScene()->AddComponent(pComponent);
}

void GameEntity::DeleteComponent(const std::string& strName)
{
	// Ѱ�Ҹ���������û�ҵ��ͷ���
	std::unordered_map<std::string, GameComponent*>::iterator iter =
		m_pImpl->m_mapComponents.find(strName);
	if (iter == m_pImpl->m_mapComponents.end()) return;

	GameComponent* pComponent = (*iter).second;

	// ���ʵ���ڳ����У���ӳ�����ɾ�������
	if (m_pImpl->m_pScene) m_pImpl->m_pScene->DeleteComponent(pComponent);

	// �������ʵ����ɾ��,��������
	m_pImpl->m_mapComponents.erase(iter);

	GameComponent* pFound = GetComponentList();
	while (pFound->GetEntityNext() != pComponent)
	{
		pFound = pFound->GetEntityNext();
	}
	pFound->SetEntityNext(pComponent->GetEntityNext());

	GameBlockAllocator::GetInstance().Free(pComponent, pComponent->GetSize());
}

GameComponent* GameEntity::GetComponent(const std::string& strName)
{
	if (m_pImpl->m_mapComponents.find(strName) == m_pImpl->m_mapComponents.end())
		return nullptr;

	return m_pImpl->m_mapComponents[strName];
}

GameComponent* GameEntity::GetComponentList()
{
	return m_pImpl->m_pComponentHead;
}

GameEntity* GameEntity::GetSceneNext()
{
	return m_pImpl->m_pSceneNext;
}

void GameEntity::SetSceneNext(GameEntity* pEntity)
{
	m_pImpl->m_pSceneNext = pEntity;
}

GameScene* GameEntity::GetScene()
{
	return m_pImpl->m_pScene;
}

void GameEntity::SetScene(GameScene* pScene)
{
	m_pImpl->m_pScene = pScene;
}

void GameEntity::AddComponentToScene(GameScene* pScene)
{
	for (std::unordered_map<std::string, GameComponent*>::iterator iter = m_pImpl->m_mapComponents.begin();
		iter != m_pImpl->m_mapComponents.end(); iter++)
	{
		pScene->AddComponent((*iter).second);
	}
}

GameEntity::GameEntity()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameEntity::~GameEntity()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}