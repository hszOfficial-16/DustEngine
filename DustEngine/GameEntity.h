#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include <string>

class GameScene;
class GameComponent;

class GameEntity
{
public:
	struct Def
	{
		GameScene* pScene;
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

	// 添加组件
	void AddComponent(GameComponent* pComponent);
	// 删除组件
	void DeleteComponent(const std::string& strName);

	// 获取组件
	GameComponent* GetComponent(const std::string& strName);

	// 获取实体在场景中的下一个实体
	GameEntity* GetSceneNext();

	// 获取实体所属的场景
	GameScene* GetScene();

protected:
	// 设置实体在场景中的下一个实体
	void SetSceneNext(GameEntity* pEntity);

	// 设置实体所属的场景
	void SetScene(GameScene* pScene);

	// 将实体的所有组件全部添加到场景中
	void AddComponentToScene(GameScene* pScene);

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameEntity(const Def& defEntity);
	~GameEntity();

	friend class GameScene;
	friend class GameEntityFactory;
};

class GameEntityFactory
{
public:
	GameEntity* CreateEntity(GameScene* pScene);

	void DestroyEntity(GameEntity* pEntity);

	GameEntity* CloneEntity(GameEntity* pEntity);

public:
	~GameEntityFactory() = default;
	GameEntityFactory(const GameEntityFactory&) = delete;
	GameEntityFactory& operator=(const GameEntityFactory&) = delete;
	static GameEntityFactory& GetInstance()
	{
		static GameEntityFactory instance;
		return instance;
	}

private:
	GameEntityFactory() = default;
};

#endif // !_GAME_ENTITY_H_