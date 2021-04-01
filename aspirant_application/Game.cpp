#include "Game.h"
namespace game::World { void Start(); }
namespace game::Avatar { void Start(); }
namespace game
{
	void Start()
	{
		game::World::Start();
		game::Avatar::Start();
	}
}