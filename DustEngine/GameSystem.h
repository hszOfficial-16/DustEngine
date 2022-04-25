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

	// ϵͳ���º���
	virtual void Update() = 0;

	// ��ȡϵͳ������
	const std::string& GetName() { return m_strName; }

	// ��ȡϵͳ��������
	GameScene* GetScene() { return m_pScene; }

	// ��ȡϵͳ�ڳ����е���һ��ϵͳ
	GameSystem* GetSceneNext() { return m_pSceneNext; }

protected:
	// ��ȡϵͳ�Ĵ�С
	uint16_t GetSize() { return m_nSize; }

	// ����ϵͳ��������
	void SetScene(GameScene* pScene) { m_pScene = pScene; }

	// ����ϵͳ�ڳ����е���һ��ϵͳ
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