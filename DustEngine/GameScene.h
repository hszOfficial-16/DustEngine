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
		std::function<void(GameScene*)> funcInit;	// ��ʼ������

		Def() : funcInit([](GameScene*) {}) {}

		Def(std::function<void(GameScene*)> funcInitIn) :
			funcInit(funcInitIn) {}
	};

	// ���³���
	void Update();

	GameEntity* CreateEntity(const GameEntity::Def& defEntity)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameEntity));
		GameEntity* pEntity = new (pMem) GameEntity(defEntity);
		AddEntity(pEntity);
		return pEntity;
	}

	// ɾ��һ��ʵ��
	void DestroyEntity(GameEntity* pEntity);

	// ��¡һ��ʵ��
	GameEntity* CloneEntity(GameEntity* pEntity);

	// ��ȡʵ���б�
	GameEntity* GetEntityList();

	// ����һ��ϵͳ
	template<typename System>
	GameSystem* CreateSystem(const typename System::Def& defSystem)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(System));
		typename System::Def* pDefSystem = (typename System::Def*) & defSystem;
		GameSystem* pSystem = new (pMem) System(*pDefSystem);
		AddSystem(pSystem);
		return pSystem;
	}

	// ����һ��ϵͳ
	void DestroySystem(GameSystem* pSystem);

	// ��ȡϵͳ�б�
	GameSystem* GetSystemList();

	// ���һ�������ϵͳ����
	void AddComponent(GameComponent* pComponent);

	// ɾ��ϵͳ��һ������ķ���
	void DeleteComponent(GameComponent* pComponent);

	// ��ȡ����б�
	GameComponent* GetComponentList(std::string strComponentName);

private:
	// ���һ��ʵ��
	void AddEntity(GameEntity* pEntity);

	// ���һ��ϵͳ
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