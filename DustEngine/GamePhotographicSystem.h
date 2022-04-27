#ifndef _GAME_PHOTOGRAPHIC_SYSTEM_H_
#define _GAME_PHOTOGRAPHIC_SYSTEM_H_

#include "GameSystem.h"
#include "GameEntity.h"
#include "GameScene.h"

#include "GameTransformComponent.h"
#include "GameCameraComponent.h"
#include "GameSpriteComponent.h"

#include <iostream>

class GamePhotographicSystem : public GameSystem
{
public:
	struct Def {};

	virtual void Update() override
	{
		for (GameComponent* pCamera = GetScene()->GetComponentList("Camera"); pCamera; pCamera = pCamera->GetSceneNext())
		{
			GameCameraComponent* _pCamera = (GameCameraComponent*)pCamera;
			GameTransformComponent* _pCameraTransform = (GameTransformComponent*)pCamera->GetEntity()->GetComponent("Transform");

			GameGraphicModule::GetInstance().RenderSetViewport(_pCamera->GetViewport());

			for (GameComponent* pSprite = GetScene()->GetComponentList("Sprite"); pSprite; pSprite = pSprite->GetSceneNext())
			{
				GameSpriteComponent* _pSprite = (GameSpriteComponent*)pSprite;
				GameTransformComponent* _pSpriteTransform = (GameTransformComponent*)pSprite->GetEntity()->GetComponent("Transform");

				GameGraphicModule::GetInstance().RenderCopy(
					_pSprite->GetTexture(),
					_pSprite->GetClipRect(),
					{
						(GameGraphicModule::GetInstance().GetWindowSize().x / 2 + 
						(_pSpriteTransform->GetPosition().x - _pCameraTransform->GetPosition().x) * 100.0f -
						_pSprite->GetTexture()->GetSize().x * _pSpriteTransform->GetScale().x * _pSprite->GetAnchor().x) *
						_pCamera->GetZoom(),

						(GameGraphicModule::GetInstance().GetWindowSize().y / 2 - 
							(_pSpriteTransform->GetPosition().y - _pCameraTransform->GetPosition().y) * 100.0f -
						_pSprite->GetTexture()->GetSize().y * _pSpriteTransform->GetScale().y * _pSprite->GetAnchor().y) *
						_pCamera->GetZoom(),

						_pSprite->GetTexture()->GetSize().x * _pSpriteTransform->GetScale().x * _pCamera->GetZoom(),

						_pSprite->GetTexture()->GetSize().y * _pSpriteTransform->GetScale().y * _pCamera->GetZoom()
					},
					360.0f - (_pSpriteTransform->GetRotation() * 180.0f / GAMEMATH_PI),
					_pSprite->GetAnchor(),
					_pSprite->GetFlipMode()
				);
			}
		}
	}

public:
	GamePhotographicSystem(const Def& defSystem) :
		GameSystem("Photographic", sizeof(GamePhotographicSystem)) {}
};

#endif // !_GAME_PHOTOGRAPHIC_SYSTEM_H_