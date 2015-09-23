#ifndef DEFAULTGAMESTATEMANAGER_H
#define DEFAULTGAMESTATEMANAGER_H

#include <vector>
#include <utility>

#include "GameStateManager.h"
#include "Drawable.h"
#include "Updatable.h"

namespace Gawky { namespace Game { namespace States {

	class DefaultGameStateManager : public GameStateManager, public Drawable, public Updatable
	{
	private:
		typedef std::pair<std::shared_ptr<GameState>, Modality::Enum> GameStateModalityPair;
		void rebuildUpdatableAndDrawableQueues();
		void notifyObscuredStates();
		void notifyRevealedStates();
		std::vector<GameStateModalityPair> activeStates;
		std::vector<Drawable *> exposedDrawables;
		std::vector<Updatable *> exposedUpdatables;

	public:
		DefaultGameStateManager();
		virtual ~DefaultGameStateManager();
		virtual std::shared_ptr<GameState> Peek() const;
		virtual void Push(const std::shared_ptr<GameState> &state, Modality::Enum modality = Modality::Exclusive);
		virtual std::shared_ptr<GameState> Pop();
		void Update(float elapsedTime);
		void Draw(float elapsedFrameTime);
		void addToDrawablesOrUpdatables(GameState *gameState);
		void removeFromDrawablesOrUpdatables(GameState *gameState);
		
	};
}}}
#endif //DEFAULTGAMESTATEMANAGER_H
