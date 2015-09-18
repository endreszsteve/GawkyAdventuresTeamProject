#ifndef GAMESTATEMANAGER_H
#define GAMESTATEMANAGER_H

#include <memory>
#include "Modality.h"

namespace Gawky {
	namespace Game {
		namespace States
		{

			// --------------
			class GameState;  // forward declaration so we don't need a header
			// --------------

			// Interface for stack based game state manager

			class GameStateManager
			{
			public:
				virtual ~GameStateManager(){}
				virtual std::shared_ptr<GameState> Peek() const = 0;
				virtual void Push(const std::shared_ptr<GameState> &state, Modality::Enum modality = Modality::Exclusive) = 0;
				virtual std::shared_ptr<GameState> Pop() = 0;
				virtual std::shared_ptr<GameState>Switch(const std::shared_ptr<GameState> &state, Modality::Enum modality = Modality::Exclusive)
				{
					std::shared_ptr<GameState> currentState = Peek();
					if (currentState)
					{
						Pop();
					}

					Push(state, modality);

					return currentState;
				}
			};
		}}}
#endif