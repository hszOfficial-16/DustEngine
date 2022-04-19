#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <string>
#include <functional>

class GameScene
{
public:
	struct Def
	{
		std::function<void()> m_funcInit;
		std::function<void()> m_funcUpdate;
		std::function<void()> m_funcQui10t;
	};

	void Update();
	void Draw();

private:
	class Impl;
	Impl* m_pImpl;

private:
	GameScene(const Def& defScene);
	~GameScene();

	friend class GameSceneSystem;
};

class GameSceneSystem
{
public:
	GameScene* CreateScene(const GameScene::Def& defScene);
	void DestroyScene(GameScene* pScene);

public:
	~GameSceneSystem() = default;
	GameSceneSystem(const GameSceneSystem&) = delete;
	GameSceneSystem& operator=(const GameSceneSystem&) = delete;
	static GameSceneSystem& GetInstance()
	{
		static GameSceneSystem instance;
		return instance;
	}
private:
	GameSceneSystem() = default;
};

#endif // !_GAME_SCENE_SYSTEM_H_