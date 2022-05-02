#include "GameDirector.h"

#include "GameFactory.h"
#include "GameFileModule.h"
#include "GamePhotographicSystem.h"

#include "GameMailbox.h"

#include <iostream>

GameEntity* CreateCreature()
{
	// 创建实体
	GameEntity* pCreature = GameFactory::GetInstance().CreateEntity();

	// 添加 Transform 组件
	GameTransformComponent::Def defTransform;
	defTransform.fRotation = 0.0f;
	defTransform.vec2Scale.x = 0.4f;
	defTransform.vec2Scale.y = 0.4f;
	pCreature->AddComponent(GameFactory::GetInstance().CreateComponent<GameTransformComponent>(defTransform));

	GameData dataTexture = GameFileSystem::GetInstance().Read("resource//GurenUddo.png");
	GameTexture* pTexture = GameGraphicModule::GetInstance().LoadTexture(dataTexture);

	// 添加 Sprite 组件
	GameSpriteComponent::Def defSprite(pTexture);
	pCreature->AddComponent(GameFactory::GetInstance().CreateComponent<GameSpriteComponent>(defSprite));

	return pCreature;
}

GameEntity* CreateCamera()
{
	GameEntity* pCamera = GameFactory::GetInstance().CreateEntity();

	// 添加 Transform 组件
	GameTransformComponent::Def defTransform;
	pCamera->AddComponent(GameFactory::GetInstance().CreateComponent<GameTransformComponent>(defTransform));
	
	// 添加 Camera 组件
	GameCameraComponent::Def defCamera;
	pCamera->AddComponent(GameFactory::GetInstance().CreateComponent<GameCameraComponent>(defCamera));

	return pCamera;
}

class Test : public GameScene
{
private:
	Test()
	{
		AddEntity(CreateCreature());
		AddEntity(CreateCamera());

		GamePhotographicSystem::Def defPhotographic;
		AddSystem(GameFactory::GetInstance().CreateSystem<GamePhotographicSystem>(defPhotographic));

		GameMailbox* pMailbox1 = GameFactory::GetInstance().CreateMailbox({ "test", { {"action", "1919"} } });
		GameMailbox* pMailbox2 = GameFactory::GetInstance().CreateMailbox({ "test2", { {"entity", "114514"} } });
		GameMailbox* pMailbox3 = GameFactory::GetInstance().CreateMailbox({ "test3", { {"entity", "1919"} } });
		GameMailbox* pMailbox4 = GameFactory::GetInstance().CreateMailbox({ "test4", { {"state", "222"} } });

		GameMailboxManager::GetInstance().AddMailbox(pMailbox1);
		GameMailboxManager::GetInstance().AddMailbox(pMailbox2);
		GameMailboxManager::GetInstance().AddMailbox(pMailbox3);

		GameMessage* pMessage = GameFactory::GetInstance().CreateMessage();
		(*pMessage)["entity"] = "114514";
		(*pMessage)["action"] = "1919";
		(*pMessage)["state"] = "111";
		GameMailboxManager::GetInstance().Publish(pMessage);

		GameMessage* pMessage2 = GameFactory::GetInstance().CreateMessage();
		(*pMessage2)["entity"] = "1919";
		GameMailboxManager::GetInstance().Publish(pMessage2);

		std::cout << pMailbox1->GetMessageCount() << std::endl;
		std::cout << pMailbox2->GetMessageCount() << std::endl;
		std::cout << pMailbox3->GetMessageCount() << std::endl;
		std::cout << pMailbox4->GetMessageCount() << std::endl;
}

public:
	static GameScene* Create()
	{
		return new Test();
	}
};

static size_t s_nSceneIndex = GameDirector::GetInstance().RegisterScene("Test", Test::Create);