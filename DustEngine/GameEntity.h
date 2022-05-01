#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include <string>

class GameScene;
class GameComponent;

class GameEntity
{
public:
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

	// 将实体的所有组件全部添加到场景中
	void AddComponentToScene(GameScene* pScene);

private:
	class Impl;
	Impl* m_pImpl;

protected:
	GameEntity();
	~GameEntity();

	friend class GameScene;
	friend class GameFactory;
};

#endif // !_GAME_ENTITY_H_