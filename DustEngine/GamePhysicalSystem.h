#ifndef _GAME_PHYSICAL_SYSTEM_H_
#define _GAME_PHYSICAL_SYSTEM_H_

#include "GameEntity.h"
#include "GameSystem.h"
#include "GameTimeModule.h"
#include "GameBodyComponent.h"
#include "GameTransformComponent.h"

class GamePhysicalSystem : public GameSystem
{
public:
	struct Def : GameSystem::Def
	{
		GameFVec2	vec2Gravity;
		int			nVelocityIterations;
		int			nPositionIterations;

		Def(const GameFVec2& vec2GravityIn = { 0.0f, 0.0f }, int nVelocityIterationsIn = 8, int nPositionIterationsIn = 3) :
			GameSystem::Def("Physical", sizeof(GamePhysicalSystem)),
			vec2Gravity(vec2GravityIn), nVelocityIterations(nVelocityIterationsIn), nPositionIterations(nPositionIterationsIn) {}
	};

	int GetVelocityIterations() { return m_nVelocityIterations; }
	void SetVelocityIterations(int nVelocityIterations) { m_nVelocityIterations = nVelocityIterations; }

	int GetPositionIterations() { return m_nPositionIterations; }
	void SetPositionIterations(int nPositionIterations) { m_nPositionIterations = nPositionIterations; }

	virtual void Update()
	{

	}

private:
	int			m_nVelocityIterations;
	int			m_nPositionIterations;

private:
	GamePhysicalSystem(const Def& defPhysicalSystem) :
		GameSystem(defPhysicalSystem)
	{
		m_nVelocityIterations = defPhysicalSystem.nVelocityIterations;
		m_nPositionIterations = defPhysicalSystem.nPositionIterations;
	}
	~GamePhysicalSystem()
	{
	}
};

#endif // !_GAME_PHYSICAL_SYSTEM_H_