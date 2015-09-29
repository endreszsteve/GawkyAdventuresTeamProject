#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <memory>
#include <iostream>

#include "DefaultGameStateManager.h"
#include "GameState.h"

using namespace Gawky::Game;
using namespace Gawky::Game::States;

class PlayState : public GameState
{
public:
	PlayState(const std::shared_ptr<GameStateManager> &gameStateManager);
	void Entered();
	void Exiting();
	void Update(float elapsedTime);
	void Draw(float elapsedFrameTime);

private:
	std::shared_ptr<GameStateManager> gameStateManager;
};

#endif //PLAYSTATE_H