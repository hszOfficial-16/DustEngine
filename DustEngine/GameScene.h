#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <string>
#include <functional>

#include "GameEntity.h"
#include "GameComponent.h"
#include "GameSystem.h"

class GameScene
{
public:
	struct Def
	{
		std::function<void(GameScene*)> funcInit;	// 初始化函数

		Def() : funcInit([](GameScene*) {}) {}

		Def(std::function<void(GameScene*)> funcInitIn) :
			funcInit(funcInitIn) {}
	};

	// 更新场景
	void Update();

	GameEntity* CreateEntity(const GameEntity::Def& defEntity)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEntity));
		GameEntity* pEntity = new (pMem) GameEntity(defEntity);
		AddEntity(pEntity);
		return pEntity;
	}

	// 删除一个实体
	void DestroyEntity(GameEntity* pEntity);

	// 克隆一个实体
	GameEntity* CloneEntity(GameEntity* pEntity);

	// 获取实体列表
	GameEntity* GetEntityList();

	// 创建一个系统
	template<typename System>
	GameSystem* CreateSystem(const typename System::Def& defSystem)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(System));
		typename System::Def* pDefSystem = (typename System::Def*) & defSystem;
		GameSystem* pSystem = new (pMem) System(*pDefSystem);
		AddSystem(pSystem);
		return pSystem;
	}

	// 销毁一个系统
	void DestroySystem(GameSystem* pSystem);

	// 获取系统列表
	GameSystem* GetSystemList();

	// 添加一个组件供系统访问
	void AddComponent(GameComponent* pComponent);

	// 删除系统对一个组件的访问
	void DeleteComponent(GameComponent* pComponent);

	// 获取组件列表
	GameComponent* GetComponentList(std::string strComponentName);

private:
	// 添加一个实体
	void AddEntity(GameEntity* pEntity);

	// 添加一个系统
	void AddSystem(GameSystem* pSystem);

private:
	class Impl;
	Impl* m_pImpl;

protected:
	GameScene(const GameScene::Def& defScene);
	~GameScene();

	friend class GameDirector;
};

#endif // !_GAME_SCENE_SYSTEM_H_