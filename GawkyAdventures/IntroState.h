#ifndef INTROSTATE_H
#define INTROSTATE_H

#include <memory>
#include <iostream>

#include "DefaultGameStateManager.h"
#include "GameState.h"

using namespace Gawky::Game;
using namespace Gawky::Game::States;

class IntroState : public GameState
{
public:
	void Entered();
	void Exiting();
	void Update(float elapsedTime);

private:
	std::shared_ptr<GameStateManager> gameStateManager;
};
#endif //INTROSTATE_H
