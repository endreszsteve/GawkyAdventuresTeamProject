#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace Gawky { namespace Game { namespace States {

	class GameState
	{
	public:
		// Destroy the game state
		virtual ~GameState(){}
		// Notifies the game state it is about to be exited
		virtual void Exiting() {};
		// Notifies the game state that it has been entered
		virtual void Entered() {};
		// Notifies the games state that it has been obscured by another state
		virtual void Obscuring(){};
		// Notifies the game state that it is no longer obscured by another state
		virtual void Revealed(){};
	};
		}}}
#endif //GAMESTATE_H