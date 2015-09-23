#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace Gawky {namespace Game { namespace States
		{

			class GameState
			{
			public:
				virtual ~GameState(){}
				virtual void Exiting(){}
				virtual void Entered(){}
				virtual void Obscuring(){}
				virtual void Revealed(){}

		}}}
};
#endif // GAMESTATE_H
