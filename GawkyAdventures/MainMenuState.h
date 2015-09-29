#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <memory>
#include <iostream>

#include "DefaultGameStateManager.h"
#include "GameState.h"

using namespace Gawky::Game;
using namespace Gawky::Game::States;

class MainMenuState : public GameState
{
public:
	void Entered();
	void Exiting();
	void Update(float elapsedTime);
	void Draw(float elapsedFrameTime);

private:
	std::shared_ptr<GameStateManager> gameStateManager;
};

#endif //MAINMENUSTATE_H
