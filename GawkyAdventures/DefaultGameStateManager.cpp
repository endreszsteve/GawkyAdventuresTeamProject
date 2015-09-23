#include "DefaultGameStateManager.h"
#include "GameState.h"

#include <stdexcept>

namespace Gawky { namespace Game { namespace States {

DefaultGameStateManager:: DefaultGameStateManager(){}

DefaultGameStateManager:: ~DefaultGameStateManager()
{
	while (!this->activeStates.empty())
	{
		Pop();
	}
}

std::shared_ptr<GameState>DefaultGameStateManager::Peek()const
{
	if (this->activeStates.empty())
	{
		return std::shared_ptr<GameState>();
	}
	else
	{
		return this->activeStates.at(this->activeStates.size() - 1).first;
	}
}

void DefaultGameStateManager::Push(const std::shared_ptr<GameState> &state, Modality::Enum modality /*= Modality::Exclusive*/)
{
	this->activeStates.push_back(std::make_pair(state, modality));

	if (modality == Modality::Exclusive)
	{
		this->exposedDrawables.clear();
		this->exposedUpdatables.clear();
	}

	addToDrawablesOrUpdatables(state.get());
	notifyObscuredStates();

	state->Entered();
}

std::shared_ptr<GameState> DefaultGameStateManager::Pop()
{
	if (this->activeStates.empty())
	{
		throw std::runtime_error("Attempted to pop from an empty game state stack");
	}

	GameStateModalityPair popped = this->activeStates.at(this->activeStates.size() - 1);
	popped.first->Exiting();
	this->activeStates.pop_back();

	if (popped.second == Modality::Exclusive)
	{
		rebuildUpdatableAndDrawableQueues();
	}
	else
	{
		removeFromDrawablesOrUpdatables(popped.first.get());
	}

	notifyRevealedStates();

	return popped.first;
}

void DefaultGameStateManager::Update(float elapsedTime)
{
	for (std::size_t i = 0; i < this->exposedUpdatables.size(); ++i)
	{
		this->exposedUpdatables.at(i)->Update(elapsedTime);
	}
}

void DefaultGameStateManager::Draw(float elapsedFrameTime)
{
	for (std::size_t i = 0; i < this->exposedDrawables.size(); ++i)
	{
		this->exposedDrawables.at(i)->Draw(elapsedFrameTime);
	}
}

void DefaultGameStateManager::addToDrawablesOrUpdatables(GameState *gameState)
{
	Drawable *drawable = dynamic_cast<Drawable *>(gameState);
	if (drawable)
	{
		this->exposedDrawables.push_back(drawable);
	}

	Updatable *updatable = dynamic_cast<Updatable *>(gameState);
		if (updatable)
		{
			this->exposedUpdatables.push_back(updatable);
		}
}

void DefaultGameStateManager::rebuildUpdatableAndDrawableQueues()
{
	this->exposedUpdatables.clear();
	this->exposedDrawables.clear();

	if (this->activeStates.empty())
	{
		return;
	}

	std::size_t i = this->activeStates.size() - 1;
	while (i > 0)
	{
		if (this->activeStates.at(i).second == Modality::Exclusive)
		{
			break;
		}
		--i;
	}

	while (i < this->activeStates.size())
	{
		addToDrawablesOrUpdatables(this->activeStates.at(i).first.get());
		++i;
	}
}

void DefaultGameStateManager::notifyObscuredStates()
{
	if (this->activeStates.size() < 2)
	{
		return;
	}

	std::size_t i = this->activeStates.size() - 2;
	while (i > 0)
	{
		if (this->activeStates.at(i).second == Modality::Exclusive)
		{
			break;
		}
		--i;
	}

	while (i < this->activeStates.size() - 1)
	{
		this->activeStates.at(i).first->Obscuring();
		++i;
	}
}

void DefaultGameStateManager::notifyRevealedStates()
{
	if (this->activeStates.empty())
	{
		return;
	}

	std::size_t i = this->activeStates.size() - 1;
	while (i > 0)
	{
		if (this->activeStates.at(i).second == Modality::Exclusive)
		{
			break;
		}
		--i;
	}

	while (i < this->activeStates.size())
	{
		this->activeStates.at(i).first->Revealed();
		++i;
	}
}
}}}
