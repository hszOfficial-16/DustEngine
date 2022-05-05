#ifndef _GAME_DIRECTOR_H_
#define _GAME_DIRECTOR_H_

#include "GameScene.h"

class GameDirector
{
public:
	// ��ʼ����
	void Run();

	// �л���ǰ����
	void ShiftScene(const GameScene::Def& defScene);

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