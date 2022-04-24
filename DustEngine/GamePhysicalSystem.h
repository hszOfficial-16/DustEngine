#ifndef _GAME_PHYSICAL_SYSTEM_H_
#define _GAME_PHYSICAL_SYSTEM_H_

#include "GameSystem.h"
#include "GameTransformComponent.h"

#include <iostream>

class GamePhysicalSystem : public GameSystem
{
public:
	virtual void Update()
	{
		for (GameTransformComponent* pTransform = (GameTransformComponent*)GetScene()->GetComponentList("Transform");
			pTransform; pTransform = (GameTransformComponent*)pTransform->GetSceneNext())
		{
			std::cout << pTransform->GetRotation() << std::endl;
		}
	}
private:
	GamePhysicalSystem() :
		GameSystem({ "Physical", sizeof(GamePhysicalSystem) }) {}

	~GamePhysicalSystem() = default;
};

#endif // !_GAME_PHYSICAL_SYSTEM_H_