#ifndef DEFAULTGAMESTATEMANAGER_H
#define DEFAULTGAMESTATEMANAGER_H

#include <vector>
#include <utility>

#include "GameStateManager.h"
#include "Drawable.h"
#include "Updateable.h"

namespace Gawky{ namespace Game{ namespace States{

	//Stacked game state manager that forwards Draw() and Update() calls
	class DefaultGameStateManager : public GameStateManager, public virtual Drawable, public Updateable
	{
	private:
		// Stores a game state and the modality it was activated with
		typedef std::pair<std::shared_ptr<GameState>, Modality::Enum> GameStateModailtyPair;
		// adds the specified game state to the exposed Drawables or Updateables if it implements the interfaces
		void addToDrawablesOrUpdateables(GameState *gameState);
		// Removes the specified game state from the exposed Drawble or Updateables if it implements the interfaces
		void removeFromDrawablesOrUpdateables(GameState *gameState);
		// Rebuilds the separate updateable and Drawbale queues when a hiding state has been popped back from the stack
		void rebuildUpdateableAndDrawableQueues();
		// Notifies the previously exposed states that they have been obscured
		void notifyObscuredStates();
		// Notifies the previously exposed states that they have been revealed
		void notifyRevealedStates();
		// Stores the currently active game states
		std::vector<GameStateModailtyPair> activeStates;
		// All drawable game states fromt the last hiding state
		std::vector<Drawable *> exposedDrawables;
		// All updateable game states from the last hiding state
		std::vector<Updateable *> exposedUpdateables;
	public:
		//Initializes a new game state manager
		DefaultGameStateManager();
		//Destroy the game state manager, leaving and dropping any active game state
		virtual ~DefaultGameStateManager();
		// returns the lastmost game state on the stack
		virtual std::shared_ptr<GameState> Peek() const;
		// Appends a new game state to the stack
		virtual void Push(const std::shared_ptr<GameState> &state, Modality::Enum modality = Modality::Exclusive);
		// Removes the last most game state form the stack, returns the state removed from the stack
		virtual std::shared_ptr<GameState> Pop();
		// advances the time of the active game states
		void Update(float elapsedTime);
		// Instructs the active game states to render themselves or to update the scene graph
		void Draw(float elapsedTime);
	};
		}}}
#endif //DEFAULTGAMESTATEMANAGER_H