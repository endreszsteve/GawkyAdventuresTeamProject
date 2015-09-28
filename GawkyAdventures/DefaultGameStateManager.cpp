#include "DefaultGameStateManager.h"
#include "GameState.h"

#include <stdexcept>

namespace Gawky { namespace Game { namespace States {


	DefaultGameStateManager::DefaultGameStateManager(){}
	DefaultGameStateManager::~DefaultGameStateManager()
	{
		while (!this->activeStates.empty())
		{
			Pop();
		}
	}

	std::shared_ptr<GameState> DefaultGameStateManager::Peek() const
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

	void DefaultGameStateManager::Push(const std::shared_ptr<GameState> &state, Modality::Enum modality /* = Modality::Exclusive */)
	{
		this->activeStates.push_back(std::make_pair(state, modality));

		if (modality == Modality::Exclusive)
		{
			this->exposedDrawables.clear();
			this->exposedUpdateables.clear();
		}

		addToDrawablesOrUpdateables(state.get());

		notifyObscuredStates();

		state->Entered();
	}

	std::shared_ptr<GameState> DefaultGameStateManager::Pop()
	{
		if (this->activeStates.empty())
		{
			throw std::runtime_error("Attemped to pop from an empty game state stack");
		}

		GameStateModailtyPair popped = this->activeStates.at(this->activeStates.size() - 1);
		popped.first->Exiting();
		this->activeStates.pop_back();

		if (popped.second == Modality::Exclusive)
		{
			rebuildUpdateableAndDrawableQueues();
		}
		else
		{
			removeFromDrawablesOrUpdateables(popped.first.get());
		}

		notifyRevealedStates();

		return popped.first;
	}

	void DefaultGameStateManager::Update(float elapsedTime)
	{
		for (std::size_t i = 0; i < this->exposedUpdateables.size(); ++i)
		{
			this->exposedUpdateables.at(i)->Update(elapsedTime);
		}
	}

	void DefaultGameStateManager::Draw(float elapsedFrameTime)
	{
		for (std::size_t i = 0; i < this->exposedDrawables.size(); ++i)
		{
			this->exposedDrawables.at(i)->Draw(elapsedFrameTime);
		}
	}

	void DefaultGameStateManager::addToDrawablesOrUpdateables(GameState *gameState)
	{
		Drawable *drawable = dynamic_cast<Drawable *>(gameState);
		if (drawable)
		{
			this->exposedDrawables.push_back(drawable);
		}

		Updateable *updateable = dynamic_cast<Updateable *>(gameState);
		if (updateable)
		{
			this->exposedUpdateables.push_back(updateable);
		}
	}

	void DefaultGameStateManager::removeFromDrawablesOrUpdateables(GameState *gamestate)
	{
		Drawable *drawable = dynamic_cast<Drawable *>(gamestate);
		if (drawable)
		{
			this->exposedDrawables.pop_back();
		}

		Updateable *updateable = dynamic_cast<Updateable *>(gamestate);
		if (updateable)
		{
			this->exposedUpdateables.pop_back();
		}
	}

	void DefaultGameStateManager::rebuildUpdateableAndDrawableQueues()
	{
		this->exposedUpdateables.clear();
		this->exposedDrawables.clear();

		if (this->activeStates.empty())
		{
			return;
		}

		// Reverse scan the active states until we hit either the beginning or Hiding state
		std::size_t i = this->activeStates.size() - 1;
		while (i > 0)
		{
			if (this->activeStates.at(i).second == Modality::Exclusive)
			{
				break;
			}
			--i;
		}

		// Now go forward again until the lists of exposed Updateable and Drawables
		while (i < activeStates.size())
		{
			addToDrawablesOrUpdateables(this->activeStates.at(i).first.get());
			++i;
		}
	}

	void DefaultGameStateManager::notifyObscuredStates()
	{
		if (this->activeStates.size() < 2)
		{
			return;
		}

		// Reverse scan until we hit either the beginning or find the next Hiding state
		std::size_t i = this->activeStates.size() - 2;
		while (i > 0)
		{
			if (this->activeStates.at(i).second == Modality::Exclusive)
			{
				break;
			}
			--i;
		}

		// now go forward 9up until the second last state) and notify the obscured states
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

		// Reverse scan until we hit the beginning or find the next Hiding state
		std::size_t i = this->activeStates.size() - 1;
		while (i > 0)
		{
			if (this->activeStates.at(i).second == Modality::Exclusive)
			{
				break;
			}
			--i;
		}

		// Now go forward and notify all revealed state
		while (i < this->activeStates.size())
		{
			this->activeStates.at(i).first->Revealed();
			++i;
		}
	}
}}}