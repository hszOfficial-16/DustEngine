#include "GameDirector.h"
#include "GamePhysicalSystem.h"

#include <iostream>

class Test : public GameScene
{
private:
	Test()
	{
		std::cout << "TestScene" << std::endl;
	}
	~Test()
	{

	}

public:
	static GameScene* Create()
	{
		return new Test();
	}
};

static int s_nSceneIndex = GameDirector::GetInstance().RegisterScene("Test", Test::Create);