#include "GameDirector.h"

#include "GameTransformComponent.h"
#include "GameSpriteComponent.h"
#include "GameCameraComponent.h"

int main()
{
	GameEntity::Def defCamera = {
		[](GameEntity* pEntity) {
			GameTransformComponent::Def defTransform;
			defTransform.fRotation = GAMEMATH_PI / 6;
			pEntity->CreateComponent<GameTransformComponent>(defTransform);

			GameCameraComponent::Def defCamera;
			pEntity->CreateComponent<GameCameraComponent>(defCamera);
		}
	};

	GameEntity::Def defSprite = {
		[](GameEntity* pEntity) {
			GameTransformComponent::Def defTransform;
			defTransform.fRotation = GAMEMATH_PI / 6;
			pEntity->CreateComponent<GameTransformComponent>(defTransform);

			GameCameraComponent::Def defSprite;
			pEntity->CreateComponent<GameCameraComponent>(defSprite);
		}
	};

	GameScene::Def defScene = { 
		[&defCamera](GameScene* pScene) {
			pScene->CreateEntity(defCamera);
		}
	};

	GameDirector::GetInstance().ShiftScene(defScene);

	GameDirector::GetInstance().Run();
	return 0;
}