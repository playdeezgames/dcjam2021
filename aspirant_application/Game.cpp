#include "Game.h"
namespace game::World { void Start(); }
namespace game::Avatar { void Start(); }
namespace game::avatar::Statistics { void Start(); }
namespace game
{
	void Start()
	{
		game::World::Start();
		game::Avatar::Start();
		game::avatar::Statistics::Start();
	}
}