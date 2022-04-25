#ifndef _GAME_BODY_COMPONENT_H_
#define _GAME_BODY_COMPONENT_H_

#include "GameComponent.h"
#include "GamePhysicalModule.h"

class GameBodyComponent : public GameComponent
{
public:
	struct Def : GameComponent::Def
	{

	};

	static GameComponent* Create(const GameComponent::Def& defBody)
	{
		void* pMem = GameBlockAllocator::GetInstance().Allocate(sizeof(GameBodyComponent));
		Def* pDefBody = (Def*)(&defBody);
		GameBodyComponent* pBody = new (pMem) GameBodyComponent(*pDefBody);
		return pBody;
	}

private:

private:
	GameBodyComponent(const Def& defBody) :
		GameComponent(defBody)
	{

	}
};

#endif // !_GAME_BODY_COMPONENT_H_