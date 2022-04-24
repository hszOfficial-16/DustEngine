#ifndef _GAME_SCENE_TEST_H_
#define _GAME_SCENE_TEST_H_

#include "GameDirector.h"

class Test : public GameScene
{
private:
	Test()
	{

	}
	~Test()
	{

	}

public:
	virtual void Update()
	{

	}

	static GameScene* Create()
	{
		return new Test();
	}
};

static int s_nSceneIndex = GameDirector::GetInstance().RegisterScene("Test", Test::Create);

#endif // !_GAME_SCENE_TEST_H_