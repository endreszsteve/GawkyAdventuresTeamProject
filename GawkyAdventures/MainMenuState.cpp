#include <memory>
#include <iostream>

#include "MainMenuState.h"

using namespace std;
using namespace Gawky::Game;
using namespace Gawky::Game::States;



void MainMenuState::Entered()
{
	cout << "MainMenu game state entered" << endl;
}

void MainMenuState::Exiting()
{
	cout << "MainMenu state is exiting" << endl;
}

void MainMenuState::Update(float elapsedTime)
{
	cout << "MainMenu state has been updated" << endl;
}

void MainMenuState::Draw(float elapsedFrameTime)
{
	cout << "MainMenu state has been drawn" << endl;
}
