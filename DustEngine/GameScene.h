#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <string>
#include <functional>

class GameEntity;
class GameComponent;
class GameSystem;

class GameScene
{
public:
	// 更新场景
	void Update();

	// 添加一个实体
	void AddEntity(GameEntity* pEntity);
	// 删除一个实体
	void DeleteEntity(GameEntity* pEntity);
	// 获取实体列表
	GameEntity* GetEntityList();
	
	// 添加一个组件供系统访问
	void AddComponent(GameComponent* pComponent);
	// 删除一个组件
	void DeleteComponent(GameComponent* pComponent);
	// 获取组件列表
	GameComponent* GetComponentList(std::string strComponentName);

	// 添加一个系统
	void AddSystem(GameSystem* pSystem);
	// 删除一个系统
	void DeleteSystem(GameSystem* pSystem);
	// 获取系统列表
	GameSystem* GetSystemList();

private:
	class Impl;
	Impl* m_pImpl;

protected:
	GameScene();
	~GameScene();

	friend class GameDirector;
};

typedef GameScene* (*GameSceneConstructor)();

#endif // !_GAME_SCENE_SYSTEM_H_