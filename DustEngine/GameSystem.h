#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include "GameScene.h"

#include <string>

class GameSystem
{
public:
	class Def
	{
	private:
		std::string		strName;
		uint16_t		nSize;

	public:
		Def(std::string strNameIn, uint16_t nSizeIn) :
			strName(strNameIn), nSize(nSizeIn) {}

		friend class GameSystem;
		friend class GameSystemFactory;
	};

	// 系统更新函数
	virtual void Update() = 0;

	// 获取系统的名称
	const std::string& GetName() { return m_strName; }

	// 获取系统所属场景
	GameScene* GetScene() { return m_pScene; }

	// 获取系统在场景中的下一个系统
	GameSystem* GetSceneNext() { return m_pSceneNext; }

protected:
	// 获取系统的大小
	uint16_t GetSize() { return m_nSize; }

	// 设置系统所属场景
	void SetScene(GameScene* pScene) { m_pScene = pScene; }

	// 设置系统在场景中的下一个系统
	void SetSceneNext(GameSystem* pSystem) { m_pSceneNext = pSystem; }

private:
	std::string		m_strName;
	uint16_t		m_nSize;
	GameScene*		m_pScene;
	GameSystem*		m_pSceneNext;

protected:
	GameSystem(const GameSystem::Def& defSystem)
	{
		m_strName = defSystem.strName;
		m_nSize = defSystem.nSize;
		m_pScene = nullptr;
		m_pSceneNext = nullptr;
	}
	virtual ~GameSystem() = default;

	friend class GameScene;
	friend class GameSystemFactory;
};

typedef GameSystem* (*GameSystemConstructor)(const GameSystem::Def& defSystem);

class GameSystemFactory
{
public:
	int RegisterSystem(std::string strSystemName, GameSystemConstructor funcSystemConstructor);

	GameSystem* CreateSystem(const GameSystem::Def& defSystem);

	void DestroySystem(GameSystem* pSystem);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameSystemFactory();
	GameSystemFactory(const GameSystemFactory&) = delete;
	GameSystemFactory& operator=(const GameSystemFactory&) = delete;
	static GameSystemFactory& GetInstance()
	{
		static GameSystemFactory instance;
		return instance;
	}
private:
	GameSystemFactory();
};

#endif // !_GAME_SYSTEM_H_