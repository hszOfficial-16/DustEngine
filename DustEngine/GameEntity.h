#ifndef _GAME_ENTITY_H_
#define _GAME_ENTITY_H_

#include "GameComponent.h"

class GameEntity
{
public:
	struct Def
	{
		
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

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameEntity(const Def& defEntity);
	~GameEntity();
};

#endif // !_GAME_ENTITY_H_