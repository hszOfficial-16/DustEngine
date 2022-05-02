#include "GameEntity.h"
#include "GameScene.h"
#include "GameComponent.h"
#include "GameBlockAllocator.h"

#include <new>
#include <unordered_map>

class GameEntity::Impl
{
public:
	// 实体的所有组件
	std::unordered_map<std::string, GameComponent*> m_mapComponents;

	// 实体的组件列表头(用于遍历)
	GameComponent* m_pComponentHead;

	// 实体的父子关系
	GameEntity* m_pParent;
	GameEntity* m_pHead;
	GameEntity* m_pPrev;
	GameEntity* m_pNext;

	// 实体所属的场景
	GameScene* m_pScene;
	// 实体在场景中对应的下一个实体
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
	// 如果该节点原来已有父节点，则删除关系
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

	// 若该子节点已有父子关系，则移除这段关系
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

	// 若 pEntity 不是该节点的子节点，则返回
	if (pChild->m_pImpl->m_pParent != this) return;

	pChild->m_pImpl->m_pParent = nullptr;

	// 如果子节点前面存在节点，则说明它并不位于链表头
	if (pChild->m_pImpl->m_pPrev)
	{
		pChild->m_pImpl->m_pPrev->m_pImpl->m_pNext = pChild->m_pImpl->m_pNext;
		if (pChild->m_pImpl->m_pNext)
		{
			pChild->m_pImpl->m_pNext->m_pImpl->m_pPrev = pChild->m_pImpl->m_pPrev;
		}
		return;
	}

	// 如果子节点前面不存在节点，则说明它位于链表头
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

	// 将组件加入到该实体中
	pComponent->SetEntity(this);
	m_pImpl->m_mapComponents[pComponent->GetName()] = pComponent;

	if (m_pImpl->m_pComponentHead)
	{
		m_pImpl->m_pComponentHead->SetEntityNext(pComponent);
	}
	m_pImpl->m_pComponentHead = pComponent;

	// 如果实体在场景中，则将该组件加入场景中
	if (GetScene()) GetScene()->AddComponent(pComponent);
}

void GameEntity::DeleteComponent(const std::string& strName)
{
	// 寻找该组件，如果没找到就返回
	std::unordered_map<std::string, GameComponent*>::iterator iter =
		m_pImpl->m_mapComponents.find(strName);
	if (iter == m_pImpl->m_mapComponents.end()) return;

	GameComponent* pComponent = (*iter).second;

	// 如果实体在场景中，则从场景中删除该组件
	if (m_pImpl->m_pScene) m_pImpl->m_pScene->DeleteComponent(pComponent);

	// 将组件从实体中删除,并析构掉
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