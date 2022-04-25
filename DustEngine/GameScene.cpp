#include "GameScene.h"
#include "GameBlockAllocator.h"

#include "GameTimeModule.h"
#include "GameResourceManager.h"

#include "GameEntity.h"
#include "GameComponent.h"
#include "GameSystem.h"

#include <new>
#include <unordered_map>

class GameScene::Impl
{
public:
	GameEntity* m_pEntityHead;
	GameSystem* m_pSystemHead;

	std::unordered_map<std::string, GameComponent*> m_mapComponentHead;

public:
	Impl()
	{
		m_pEntityHead = nullptr;
		m_pSystemHead = nullptr;
	}
	~Impl()
	{
		for (GameSystem* pSystem = m_pSystemHead; pSystem; pSystem = pSystem->GetSceneNext())
		{
			GameSystemFactory::GetInstance().DestroySystem(pSystem);
		}

		for (std::unordered_map<std::string, GameComponent*>::iterator iter = m_mapComponentHead.begin();
			iter != m_mapComponentHead.end(); iter++)
		{
			for (GameComponent* pComponent = m_mapComponentHead[(*iter).first]; pComponent; pComponent = pComponent->GetSceneNext())
			{
				GameComponentFactory::GetInstance().DestroyComponent(pComponent);
			}
		}

		for (GameEntity* pEntity = m_pEntityHead; pEntity; pEntity = pEntity->GetSceneNext())
		{
			GameEntityFactory::GetInstance().DestroyEntity(pEntity);
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
	// 若实体已在该场景中，则不作处理
	if (pEntity->GetScene() == this) return;

	pEntity->SetScene(this);

	if (m_pImpl->m_pEntityHead)
	{
		pEntity->SetSceneNext(m_pImpl->m_pEntityHead);
	}
	m_pImpl->m_pEntityHead = pEntity;

	// 将实体的组件全部加入到场景中
	pEntity->AddComponentToScene(this);
}

void GameScene::DeleteEntity(GameEntity* pEntity)
{
	if (pEntity->GetScene() != this) return;

	GameEntity* pFound = GetEntityList();
	while (pFound->GetSceneNext() != pEntity)
	{
		pFound = pFound->GetSceneNext();
	}
	pFound->SetSceneNext(pEntity->GetSceneNext());

	GameEntityFactory::GetInstance().DestroyEntity(pEntity);
}

GameEntity* GameScene::GetEntityList()
{
	if (m_pImpl->m_pEntityHead) return nullptr;
	
	return m_pImpl->m_pEntityHead;
}

void GameScene::AddComponent(GameComponent* pComponent)
{
	if (m_pImpl->m_mapComponentHead[pComponent->GetName()])
	{
		pComponent->SetSceneNext(m_pImpl->m_mapComponentHead[pComponent->GetName()]);
	}
	m_pImpl->m_mapComponentHead[pComponent->GetName()] = pComponent;
}

void GameScene::DeleteComponent(GameComponent* pComponent)
{
	if (pComponent->GetEntity()->GetScene() != this) return;

	// 将组件从场景中删除
	GameComponent* pFound = GetComponentList(pComponent->GetName());
	while (pFound->GetSceneNext() != pComponent)
	{
		pFound = pFound->GetSceneNext();
	}
	pFound->SetSceneNext(pComponent->GetSceneNext());

	GameComponentFactory::GetInstance().DestroyComponent(pComponent);
}

GameComponent* GameScene::GetComponentList(std::string strComponentName)
{
	if (!m_pImpl->m_mapComponentHead[strComponentName]) return nullptr;
	
	return m_pImpl->m_mapComponentHead[strComponentName];
}

void GameScene::AddSystem(GameSystem* pSystem)
{
	if (m_pImpl->m_pSystemHead)
	{
		pSystem->SetSceneNext(m_pImpl->m_pSystemHead);
	}
	m_pImpl->m_pSystemHead = pSystem;
}

void GameScene::DeleteSystem(GameSystem* pSystem)
{
	if (pSystem->GetScene() != this) return;

	GameSystem* pFound = GetSystemList();
	while (pFound->GetSceneNext())
	{
		pFound = pFound->GetSceneNext();
	}
	pFound->SetSceneNext(pSystem->GetSceneNext());

	GameSystemFactory::GetInstance().DestroySystem(pSystem);
}

GameSystem* GameScene::GetSystemList()
{
	if (!m_pImpl->m_pSystemHead) return nullptr;

	return m_pImpl->m_pSystemHead;
}

GameScene::GameScene()
{
	void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Impl));
	m_pImpl = new (pMem) Impl();
}

GameScene::~GameScene()
{
	m_pImpl->~Impl();
	GameBlockAllocator::GetInstance().Free(m_pImpl, sizeof(Impl));
}