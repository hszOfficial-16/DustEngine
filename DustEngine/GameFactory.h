#ifndef _GAME_FACTORY_H_
#define _GAME_FACTORY_H_

#include "GameEntity.h"
#include "GameComponent.h"
#include "GameSystem.h"

#include "GameMailbox.h"

#include "GameBlockAllocator.h"

#include <new>
#include <string.h>

class GameFactory
{
public:
	GameEntity* CreateEntity()
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEntity));
		return new (pMem) GameEntity();
	}

	void DestroyEntity(GameEntity* pEntity)
	{
		pEntity->~GameEntity();
		GameBlockAllocator::GetInstance().Free(pEntity, sizeof(GameEntity));
	}

	GameEntity* CloneEntity(GameEntity* pEntity)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEntity));

		GameEntity* pCloneEntity = new (pMem) GameEntity();

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

	template<typename Component>
	GameComponent* CreateComponent(const typename Component::Def& defComponent)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Component));
		typename Component::Def* pDefComponent = (typename Component::Def*)&defComponent;
		return new (pMem) Component(*pDefComponent);
	}

	void DestroyComponent(GameComponent* pComponent)
	{
		GameBlockAllocator::GetInstance().Free(pComponent, pComponent->GetSize());
	}

	template<typename System>
	GameSystem* CreateSystem(const typename System::Def& defSystem)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(System));
		typename System::Def* pDefSystem = (typename System::Def*)&defSystem;
		return new (pMem) System(*pDefSystem);
	}

	void DestroySystem(GameSystem* pSystem)
	{
		GameBlockAllocator::GetInstance().Free(pSystem, pSystem->GetSize());
	}

	GameMessage* CreateMessage()
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameMessage));
		return new (pMem) GameMessage();
	}

	void DestroyMessage(GameMessage* pMessage)
	{
		pMessage->~GameMessage();
		GameBlockAllocator::GetInstance().Free(pMessage, sizeof(GameMessage));
	}

	GameMailbox* CreateMailbox(const std::initializer_list<std::pair<std::string, std::string>>& listPairs)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameMailbox));
		return new (pMem) GameMailbox(listPairs);
	}

	void DestroyMailbox(GameMailbox* pMailbox)
	{
		pMailbox->~GameMailbox();
		GameBlockAllocator::GetInstance().Free(pMailbox, sizeof(GameMailbox));
	}

public:
	~GameFactory() = default;
	GameFactory(const GameFactory&) = delete;
	GameFactory& operator=(const GameFactory&) = delete;
	static GameFactory& GetInstance()
	{
		static GameFactory instance;
		return instance;
	}
private:
	GameFactory() = default;
};

#endif // !_GAME_FACTORY_H_