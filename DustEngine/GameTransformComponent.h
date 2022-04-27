#ifndef _GAME_TRANSFORM_COMPONENT_H_
#define _GAME_TRANSFORM_COMPONENT_H_

#include "GameComponent.h"
#include "GameMath.h"

class GameTransformComponent : public GameComponent
{
public:
	struct Def
	{
		GameFVec2	vec2Position;
		float		fRotation;
		GameFVec2	vec2Scale;

		Def(GameFVec2 vec2PositionIn = { 0.0f, 0.0f }, float fRotationIn = 0.0f, GameFVec2 vec2ScaleIn = { 1.0f, 1.0f }) :
			vec2Position(vec2PositionIn), fRotation(fRotationIn), vec2Scale(vec2ScaleIn) {}
	};

	const GameFVec2& GetPosition() { return m_vec2Position; }
	void SetPosition(const GameFVec2& vec2Position) { m_vec2Position = vec2Position; }

	float GetRotation() { return m_fRotation; }
	void SetRotation(float fRotation) { m_fRotation = fRotation; }

	const GameFVec2& GetScale() { return m_vec2Scale; }
	void SetScale(const GameFVec2& vec2Scale) { m_vec2Scale = vec2Scale; }

private:
	GameFVec2	m_vec2Position;
	float		m_fRotation;
	GameFVec2	m_vec2Scale;

public:
	GameTransformComponent(const Def& defTransform) :
		GameComponent("Transform", sizeof(GameTransformComponent))
	{
		m_vec2Position = defTransform.vec2Position;
		m_fRotation = defTransform.fRotation;
		m_vec2Scale = defTransform.vec2Scale;
	}
};

#endif // !_GAME_TRANSFORM_COMPONENT_H_