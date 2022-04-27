#ifndef _GAME_COMPONENT_H_
#define _GAME_COMPONENT_H_

#include "GameBlockAllocator.h"

#include <new>
#include <string>

class GameEntity;
class GameScene;

class GameComponent
{
public:
	// 获取组件的名称
	const std::string& GetName() { return m_strName; }

	// 获取组件所属的实体
	GameEntity* GetEntity() { return m_pEntity; }

	// 获取组件在场景中的下一个组件(用于相同类型的组件)
	GameComponent* GetSceneNext() { return m_pSceneNext; }

	// 获取组件在实体中的下一个组件(用于遍历实体的组件)
	GameComponent* GetEntityNext() { return m_pEntityNext; }

protected:
	// 获取组件的大小
	uint16_t GetSize() { return m_nSize; }

	// 设置组件所属的实体
	void SetEntity(GameEntity* pEntity) { m_pEntity = pEntity; }

	// 设置组件在场景中的下一个组件
	void SetSceneNext(GameComponent* pComponent) { m_pSceneNext = pComponent; }

	// 设置组件在实体中的下一个组件
	void SetEntityNext(GameComponent* pComponent) { m_pEntityNext = pComponent; }

private:
	std::string		m_strName;		// 组件名称
	uint16_t		m_nSize;		// 组件大小
	GameEntity*		m_pEntity;		// 组件所属实体
	GameComponent*	m_pSceneNext;	// 组件在场景中的下一个组件
	GameComponent*  m_pEntityNext;	// 组件在实体中的下一个组件

protected:
	GameComponent(std::string strName, uint16_t nSize)
	{
		m_strName = strName;
		m_nSize = nSize;
		m_pEntity = nullptr;
		m_pSceneNext = nullptr;
	}
	virtual ~GameComponent() = default;

	friend class GameEntity;
	friend class GameScene;
	friend class GameFactory;
};

#endif // !_GAME_COMPONENT_H_