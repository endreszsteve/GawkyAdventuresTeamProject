#include <memory>
#include <iostream>

#include "IntroState.h"

using namespace std;
using namespace Gawky::Game;
using namespace Gawky::Game::States;



void IntroState::Entered()
{
	cout << "Intro game state entered" << endl;
}

void IntroState::Exiting()
{
	cout << "Intro state is exiting" << endl;
}

void IntroState::Update(float elapsedTime)
{
	cout << "Intro state has been updated, switching to MainMenuState" << endl;
	//this->gameStateManager->Switch(std::shared_ptr<GameState>(new MainMenuState()));
}