#include "GameEntity.h"
#include "GameBlockAllocator.h"
#include "GameScene.h"
#include "GameComponent.h"

#include <new>
#include <string.h>
#include <unordered_map>

class GameEntity::Impl
{
public:
	// ʵ����������
	std::unordered_map<std::string, GameComponent*> m_mapComponents;

	// ʵ��ĸ��ӹ�ϵ
	GameEntity* m_pParent;
	GameEntity* m_pHead;
	GameEntity* m_pPrev;
	GameEntity* m_pNext;

	// ʵ���ڳ����ж�Ӧ����һ��ʵ��
	GameEntity* m_pSceneNext;

	// ʵ�������ĳ���
	GameScene* m_pScene;

public:
	Impl(const Def& defEntity)
	{
		m_pParent = nullptr;
		m_pHead = nullptr;
		m_pPrev = nullptr;
		m_pNext = nullptr;

		m_pScene = defEntity.pScene;
		m_pSceneNext = nullptr;
	}
	~Impl()
	{
		for (std::unordered_map<std::string, GameComponent*>::iterator iter = m_mapComponents.begin();
			iter != m_mapComponents.end(); iter++)
		{

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
	pComponent->SetEntity(this);
	m_pImpl->m_mapComponents[pComponent->GetName()] = pComponent;

	// ���ʵ���ڳ����У��򽫸�������볡����
	if (m_pImpl->m_pScene) m_pImpl->m_pScene->AddComponent(pComponent);
}

void GameEntity::DeleteComponent(const std::string& strName)
{
	// Ѱ�Ҹ���������û�ҵ��ͷ���
	std::unordered_map<std::string, GameComponent*>::iterator iter =
		m_pImpl->m_mapComponents.find(strName);

	if (iter == m_pImpl->m_mapComponents.end()) return;

	// ���ʵ���ڳ����У���ӳ�����ɾ�������
	if (m_pImpl->m_pScene) m_pImpl->m_pScene->DeleteComponent((*iter).second);

	m_pImpl->m_mapComponents.erase(iter);
	GameBlockAllocator::GetInstance().Free((*iter).second, (*iter).second->GetSize());
}

GameComponent* GameEntity::GetComponent(const std::string& strName)
{
	if (m_pImpl->m_mapComponents.find(strName) == m_pImpl->m_mapComponents.end())
		return nullptr;

	return m_pImpl->m_mapComponents[strName];
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

GameEntity* GameEntityFactory::CreateEntity(GameScene* pScene)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEntity));
	GameEntity* pEntity = new (pMem) GameEntity({ pScene });
	return pEntity;
}

void GameEntityFactory::DestroyEntity(GameEntity* pEntity)
{
	pEntity->~GameEntity();
	GameBlockAllocator::GetInstance().Free(pEntity, sizeof(GameEntity));
}

GameEntity* GameEntityFactory::CloneEntity(GameEntity* pEntity)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEntity));

	GameEntity::Def defEntity = { pEntity->GetScene() };
	GameEntity* pCloneEntity = new (pMem) GameEntity(defEntity);

	for (std::unordered_map<std::string, GameComponent*>::iterator iter = pEntity->m_pImpl->m_mapComponents.begin();
		iter != pEntity->m_pImpl->m_mapComponents.end(); iter++)
	{
		// ����������ڴ棬����ӵ�ʵ�嵱��
		void* pMemComponent = GameBlockAllocator::GetInstance().Allocate((*iter).second->GetSize());
		memcpy(pMemComponent, (*iter).second, (*iter).second->GetSize());
		GameComponent* pCloneComponent = (GameComponent*)pMemComponent;
		pCloneEntity->AddComponent(pCloneComponent);
	}

	if (pEntity->m_pImpl->m_pParent)
	{
		pEntity->m_pImpl->m_pParent->AddChild(pCloneEntity);
	}
	for (GameEntity* pChild = pEntity->GetChildList(); pChild; pChild = pChild->GetNext())
	{
		GameEntity* pCloneChild = CloneEntity(pChild);
		pCloneEntity->AddChild(pCloneChild);
	}

	return pCloneEntity;
}