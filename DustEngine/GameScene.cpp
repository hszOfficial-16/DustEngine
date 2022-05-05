#include "GameScene.h"

#include "GameTimeModule.h"

#include <new>
#include <unordered_map>

class GameScene::Impl
{
public:
	GameEntity* m_pEntityHead;
	GameSystem* m_pSystemHead;

	std::unordered_map<std::string, GameComponent*> m_mapComponentHead;

public:
	Impl(const GameScene::Def& defScene)
	{
		m_pEntityHead = nullptr;
		m_pSystemHead = nullptr;
	}
	~Impl()
	{
		for (GameSystem* pSystem = m_pSystemHead; pSystem; pSystem = pSystem->GetSceneNext())
		{
			pSystem->~GameSystem();
			GameBlockAllocator::GetInstance().Free(pSystem, pSystem->GetSize());
		}
		for (GameEntity* pEntity = m_pEntityHead; pEntity; pEntity = pEntity->GetSceneNext())
		{
			pEntity->~GameEntity();
			GameBlockAllocator::GetInstance().Free(pEntity, sizeof(GameEntity));
		}
	}
};

void GameScene::Update()
{
	for (GameSystem* pSystem = GetSystemList(); pSystem; pSystem = pSystem->GetSceneNext())
	{
		pSystem->Update();
	}
}

void GameScene::AddEntity(GameEntity* pEntity)
{
	if (!pEntity)
	{
		return;
	}

	pEntity->SetScene(this);

	if (m_pImpl->m_pEntityHead)
	{
		pEntity->SetSceneNext(m_pImpl->m_pEntityHead);
	}
	m_pImpl->m_pEntityHead = pEntity;
}

void GameScene::DestroyEntity(GameEntity* pEntity)
{
	if (!pEntity)
	{
		return;
	}

	if (pEntity->GetScene() != this)
	{
		return;
	}

	GameEntity* pFound = GetEntityList();
	while (pFound->GetSceneNext() != pEntity)
	{
		pFound = pFound->GetSceneNext();
	}
	pFound->SetSceneNext(pEntity->GetSceneNext());

	pEntity->~GameEntity();
	GameBlockAllocator::GetInstance().Free(pEntity, sizeof(GameEntity));
}

GameEntity* GameScene::CloneEntity(GameEntity* pEntity)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEntity));
	GameEntity* pCloneEntity = new (pMem) GameEntity({});

	// 拷贝实体的所有组件
	for (GameComponent* pComponent = pEntity->GetComponentList(); pComponent; pComponent = pComponent->GetEntityNext())
	{
		void* pCloneComponentMem = GameBlockAllocator::GetInstance().Allocate(pComponent->GetSize());
		memcpy(pCloneComponentMem, pComponent, pComponent->GetSize());
		GameComponent* pCloneComponent = (GameComponent*)pCloneComponentMem;
		pEntity->AddComponent(pCloneComponent);
	}

	if (pEntity->GetParent())
	{
		pEntity->GetParent()->AddChild(pCloneEntity);
	}
	for (GameEntity* pChild = pEntity->GetChildList(); pChild; pChild = pChild->GetNext())
	{
		GameEntity* pCloneChild = CloneEntity(pChild);
		pCloneEntity->AddChild(pCloneChild);
	}

	return pCloneEntity;
}

GameEntity* GameScene::GetEntityList()
{
	return m_pImpl->m_pEntityHead;
}

void GameScene::AddComponent(GameComponent* pComponent)
{
	if (!pComponent)
	{
		return;
	}

	if (m_pImpl->m_mapComponentHead[pComponent->GetName()])
	{
		pComponent->SetSceneNext(m_pImpl->m_mapComponentHead[pComponent->GetName()]);
	}
	m_pImpl->m_mapComponentHead[pComponent->GetName()] = pComponent;
}

void GameScene::DeleteComponent(GameComponent* pComponent)
{
	if (!pComponent)
	{
		return;
	}

	if (pComponent->GetEntity()->GetScene() != this)
	{
		return;
	}

	// 将组件从场景中删除
	GameComponent* pFound = GetComponentList(pComponent->GetName());
	while (pFound->GetSceneNext() != pComponent)
	{
		pFound = pFound->GetSceneNext();
	}
	pFound->SetSceneNext(pComponent->GetSceneNext());
}

GameComponent* GameScene::GetComponentList(std::string strComponentName)
{
	return m_pImpl->m_mapComponentHead[strComponentName];
}

void GameScene::AddSystem(GameSystem* pSystem)
{
	if (!pSystem)
	{
		return;
	}

	pSystem->SetScene(this);

	if (m_pImpl->m_pSystemHead)
	{
		pSystem->SetSceneNext(m_pImpl->m_pSystemHead);
	}
	m_pImpl->m_pSystemHead = pSystem;
}

void GameScene::DestroySystem(GameSystem* pSystem)
{
	if (!pSystem)
	{
		return;
	}

	if (pSystem->GetScene() != this)
	{
		return;
	}

	GameSystem* pFound = GetSystemList();
	while (pFound->GetSceneNext())
	{
		pFound = pFound->GetSceneNext();
	}
	pFound->SetSceneNext(pSystem->GetSceneNext());

	GameBlockAllocator::GetInstance().Free(pSystem, pSystem->GetSize());
}

GameSystem* GameScene::GetSystemList()
{
	return m_pImpl->m_pSystemHead;
}

GameScene::GameScene(const GameScene::Def& defScene)
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl(defScene);

	// 进行用户自定义的初始化操作
	defScene.funcInit(this);
}

GameScene::~GameScene()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}