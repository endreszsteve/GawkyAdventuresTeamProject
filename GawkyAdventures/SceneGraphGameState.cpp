#include "SceneGraphGameState.h"
#include <cassert>

namespace Gawky { namespace Game { namespace States{

	SceneGraphGameState::SceneGraphGameState() : areSceneNodesAdded(false), areSceneNodesCreated(false){}
	SceneGraphGameState::~SceneGraphGameState()
	{
		using namespace std;

		assert(!this->areSceneNodesCreated &&
			"Game state was destroyed while it was still active (scene nodes created). "
			"Sorry, the destructor of the SceneGraphGameState cannot call your virtual "
			"DoDetachSceneNodes() or DoCreateSceneNodes() methods, so it's up to your "
			"game state manager to exit its game states before destroying them"
			);
	}

}}}