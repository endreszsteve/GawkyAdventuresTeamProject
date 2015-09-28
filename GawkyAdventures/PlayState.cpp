#include <memory>
#include <iostream>

#include "PlayState.h"
#include "d3dApp.h"
#include "LevelBuilder.h"

using namespace std;
using namespace Gawky::Game;
using namespace Gawky::Game::States;

PlayState::PlayState(const std::shared_ptr<GameStateManager> &gameStateManager) : gameStateManager(gameStateManager){};
void PlayState::Entered()
{
	cout << "Playing game state entered" << endl;
}

void PlayState::Exiting()
{
	cout << "Playing state is exiting" << endl;
}

void PlayState::Update(float elapsedTime)
{
	cout << "Playing state has been updated" << endl;
	
}

void PlayState::Draw(float elapsedFrameTime)
{
	cout << "Playing state has been drawn" << endl;
	
}