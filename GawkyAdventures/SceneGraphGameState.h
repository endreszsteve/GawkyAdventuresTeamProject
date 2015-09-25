#ifndef SCENEGRAPHGAMESTATE_H
#define SCENEGRAPHGAMESTATE_H

#include "GameState.h"

namespace Gawky { namespace Game { namespace States {
	// Game states for game using scene graph
	class SceneGraphGameState : public GameState
	{
	public:
		// Initialize an new scene graph game state
		SceneGraphGameState();
		// Destroy the game state
		virtual ~SceneGraphGameState();
		// notifies the game state that it has been obscured
		void Hiding()
		{
			if (this->areSceneNodesCreated)
			{
				if (this->areSceneNodesAdded)
				{
					DetachSceneNodes();
					this->areSceneNodesAdded = false;
				}
			}
		}

		// notifies the game state that it is no longer obscured
		void Revealed()
		{
			if (this->areSceneNodesCreated)
			{
				if (!this->areSceneNodesAdded)
				{
					AttachSceneNodes();
					this->areSceneNodesAdded = true;
				}
			}
		}

		// notifies the game state it is about to be exited

		void Exiting()
		{
			if (this->areSceneNodesCreated)
			{
				if (this->areSceneNodesAdded)
				{
					DetachSceneNodes();
					this->areSceneNodesAdded = false;
				}

				DestroySceneNodes();
				this->areSceneNodesCreated = false;
			}
		}

		// notiifes the game state it has been entered
		void Entered()
		{
			if (!this->areSceneNodesCreated)
			{
				CreateSceneNodes();
				this->areSceneNodesCreated = true;

				if (!this->areSceneNodesAdded)
				{
					AttachSceneNodes();
					this->areSceneNodesAdded = true;
				}
			}
		}

	protected:
		// Called when the state should create its scene nodes
		virtual void CreateSceneNodes(){}
		// Called when the state should destroy its scene nodes
		virtual void DestroySceneNodes(){}
		// Called when the state should add its nodes to the scene graph
		virtual void AttachSceneNodes(){}
		// Called when the state should remove its nodes from the scene graph
		virtual void DetachSceneNodes(){}

	private:
		// Whether the game state has currently added its contents to the scene graph
		bool areSceneNodesAdded;
		// Whether the game state has currently created its contents to the graph
		bool areSceneNodesCreated;
	};
}}}
#endif //SCENEGRAPHGAMESTATE_H