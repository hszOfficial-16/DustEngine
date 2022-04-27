#ifndef _GAME_CAMERA_COMPONENT_H_
#define _GAME_CAMERA_COMPONENT_H_

#include "GameComponent.h"
#include "GameGraphicModule.h"

class GameCameraComponent : public GameComponent
{
public:
	struct Def
	{
		GameRect	rectViewport;
		float		fZoom;

		Def(GameRect rectViewportIn = { 0, 0,
			GameGraphicModule::GetInstance().GetWindowSize().x,
			GameGraphicModule::GetInstance().GetWindowSize().y },
			float fZoomIn = 1.0f) :
			rectViewport(rectViewportIn), fZoom(fZoomIn) {}
	};

	const GameRect& GetViewport() { return m_rectViewport; }
	void SetViewport(const GameRect& rectViewport) { m_rectViewport = rectViewport; }

	float GetZoom() { return m_fZoom; }
	void SetZoom(float fZoom) { m_fZoom = fZoom; }

private:
	GameRect	m_rectViewport;
	float		m_fZoom;

public:
	GameCameraComponent(const Def& defCamera) :
		GameComponent("Camera", sizeof(GameCameraComponent))
	{
		m_rectViewport = defCamera.rectViewport;
		m_fZoom = defCamera.fZoom;
	}
};

#endif // !_GAME_CAMERA_COMPONENT_H_