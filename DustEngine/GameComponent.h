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
	class Def
	{
	private:
		std::string		strName;
		uint16_t		nSize;

	public:
		Def(std::string strNameIn, uint16_t nSizeIn)
			: strName(strNameIn), nSize(nSizeIn) {}

		friend class GameComponent;
		friend class GameComponentFactory;
	};

	// 获取组件的名称
	const std::string& GetName() { return m_strName; }

	// 获取组件所属的实体
	GameEntity* GetEntity() { return m_pEntity; }

	// 获取组件在场景中的下一个组件(用于遍历组件)
	GameComponent* GetSceneNext() { return m_pSceneNext; }

protected:
	// 获取组件的大小
	uint16_t GetSize() { return m_nSize; }

	// 设置组件在场景中的下一个组件
	void SetSceneNext(GameComponent* pComponent) { m_pSceneNext = pComponent; }

	// 设置组件所属的实体
	void SetEntity(GameEntity* pEntity) { m_pEntity = pEntity; }

private:
	std::string		m_strName;		// 组件名称
	uint16_t		m_nSize;		// 组件大小
	GameEntity*		m_pEntity;		// 组件所属实体
	GameComponent*	m_pSceneNext;	// 组件在场景中的下一个组件

protected:
	GameComponent(const GameComponent::Def& defComponent)
	{
		m_strName = defComponent.strName;
		m_nSize = defComponent.nSize;
		m_pEntity = nullptr;
		m_pSceneNext = nullptr;
	}
	virtual ~GameComponent() = default;

	friend class GameEntity;
	friend class GameScene;
	friend class GameEntityFactory;
	friend class GameComponentFactory;
};

// 组件构造函数
typedef GameComponent* (*GameComponentConstructor)(const GameComponent::Def&);

// 组件构造工厂
class GameComponentFactory
{
public:
	// 提供信息注册一个新组件
	int RegisterComponent(std::string strComponentName, GameComponentConstructor funcComponentConstructor);

	// 根据组件信息创建一个组件
	GameComponent* CreateComponent(const GameComponent::Def& defComponent);

	// 销毁一个组件
	void DestroyComponent(GameComponent* pComponent);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameComponentFactory();
	GameComponentFactory(const GameComponentFactory&) = delete;
	GameComponentFactory& operator=(const GameComponentFactory&) = delete;
	static GameComponentFactory& GetInstance()
	{
		static GameComponentFactory instance;
		return instance;
	}
private:
	GameComponentFactory();
};

#endif // !_GAME_COMPONENT_H_