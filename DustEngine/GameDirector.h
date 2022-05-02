#ifndef _GAME_DIRECTOR_H_
#define _GAME_DIRECTOR_H_

#include "GameScene.h"

class GameDirector
{
public:
	// 开始运行
	void Run();

	// 注册一个场景
	size_t RegisterScene(std::string strSceneName, GameSceneConstructor funcConstructor);

	// 切换为索引的场景
	void ShiftScene(std::string strSceneName);

private:
	class Impl;
	Impl* m_pImpl;

public:
	~GameDirector();
	GameDirector(const GameDirector&) = delete;
	GameDirector& operator=(const GameDirector&) = delete;
	static GameDirector& GetInstance()
	{
		static GameDirector instance;
		return instance;
	}
private:
	GameDirector();
};

#endif // !_GAME_DIRECTOR_H_