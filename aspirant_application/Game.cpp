#include "Game.h"
#include "json.hpp"
namespace game::World { void Reset(); }
namespace game::Avatar { void Reset(); }
namespace game::avatar::Statistics { void Reset(); }
namespace game::world::Items { void Reset(); }
namespace game::avatar::Items { void Reset(); }
namespace game::Creatures { void Reset(); }
namespace game
{
	nlohmann::json data;

	nlohmann::json& GetData()
	{
		return data;
	}

	void Reset()
	{
		game::World::Reset();
		game::world::Items::Reset();
		game::avatar::Statistics::Reset();
		game::avatar::Items::Reset();
		game::Creatures::Reset();
		game::Avatar::Reset();
	}

	void Start()
	{
		Reset();
	}
}