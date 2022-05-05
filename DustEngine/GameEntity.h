#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include "GameBlockAllocator.h"

#include <string>
#include <functional>

class GameScene;
class GameComponent;

class GameEntity
{
public:
	struct Def
	{
		std::function<void(GameEntity*)> funcInit;

		Def() : funcInit([](GameEntity*) {}) {}

		Def(std::function<void(GameEntity*)> funcInitIn) :
			funcInit(funcInitIn) {}
	};

	// 获取父实体
	GameEntity* GetParent();
	// 设置父实体
	void SetParent(GameEntity* pParent);

	// 添加子实体
	void AddChild(GameEntity* pChild);
	// 删除子实体
	void DeleteChild(GameEntity* pChild);

	// 获取子实体列表头
	GameEntity* GetChildList();
	// 获取下一个子实体
	GameEntity* GetNext();

	template<typename Component>
	GameComponent* CreateComponent(const typename Component::Def& defComponent)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(Component));
		typename Component::Def* pDefComponent = (typename Component::Def*) & defComponent;
		GameComponent* pComponent = new (pMem) Component(*pDefComponent);
		AddComponent(pComponent);
		return pComponent;
	}

	// 销毁组件
	void DestroyComponent(const std::string& strName);

	// 获取组件
	GameComponent* GetComponent(const std::string& strName);
	// 获取组件列表头
	GameComponent* GetComponentList();

	// 获取实体所属的场景
	GameScene* GetScene();
	// 获取实体在场景中的下一个实体
	GameEntity* GetSceneNext();

protected:
	// 设置实体在场景中的下一个实体
	void SetSceneNext(GameEntity* pEntity);

	// 设置实体所属的场景
	void SetScene(GameScene* pScene);

	// 添加组件
	void AddComponent(GameComponent* pComponent);

private:
	class Impl;
	Impl* m_pImpl;

protected:
	GameEntity(const GameEntity::Def& defEntity);
	~GameEntity();

	friend class GameScene;
};

#endif // !_GAME_ENTITY_H_