#include "Game.h"
namespace game::World { void Reset(); }
namespace game::Avatar { void Reset(); }
namespace game::avatar::Statistics { void Reset(); }
namespace game
{
	void Reset()
	{
		game::World::Reset();
		game::Avatar::Reset();
		game::avatar::Statistics::Reset();
	}

	void Start()
	{
		Reset();
	}
}