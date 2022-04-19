#include "GameEntity.h"
#include "GameBlockAllocator.h"

#include <new>
#include <unordered_map>

class GameEntity::Impl
{
public:
	// 实体的父子关系
	GameEntity* m_pParent;
	GameEntity* m_pHead;
	GameEntity* m_pPrev;
	GameEntity* m_pNext;

	// 实体的所有组件
	std::unordered_map<std::string, GameComponent*> m_mapComponents;

public:
	Impl(const Def& defEntity)
	{
		m_pParent = nullptr;
		m_pHead = nullptr;
		m_pPrev = nullptr;
		m_pNext = nullptr;
	}
	~Impl()
	{

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
	m_pImpl->m_mapComponents[pComponent->GetName()] = pComponent;
}

void GameEntity::DeleteComponent(const std::string& strName)
{
	std::unordered_map<std::string, GameComponent*>::iterator iter =
		m_pImpl->m_mapComponents.find(strName);

	GameBlockAllocator::GetInstance().Free((*iter).second, (*iter).second->GetSize());
	m_pImpl->m_mapComponents.erase(iter);
}

GameEntity::GameEntity(const Def& defEntity)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl(defEntity);
}

GameEntity::~GameEntity()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}