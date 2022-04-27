#include "GameDirector.h"

#include "GameFactory.h"
#include "GameFileModule.h"
#include "GamePhotographicSystem.h"

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
	}

public:
	static GameScene* Create()
	{
		return new Test();
	}
};

static int s_nSceneIndex = GameDirector::GetInstance().RegisterScene("Test", Test::Create);