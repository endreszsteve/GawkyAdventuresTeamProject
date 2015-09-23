#ifndef GAMESTATEPROVIDER_H
#define GAMESTATEPROVIDER_H

#include <memory>
#include <string>

namespace Gawky { namespace Game { namespace States{

	class GameState;

	class GameStateProvider
	{
	public:
		virtual ~GameStateProvider() = 0;
		virtual std::shared_ptr<GameState>GetGameState(const std::string &name) = 0;
	};

}}}
#endif //GAMESTATEPROVIDER_H
