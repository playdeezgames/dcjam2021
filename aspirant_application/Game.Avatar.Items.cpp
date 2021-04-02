#include "Game.Avatar.Items.h"
#include <map>
namespace game::avatar::Items
{
	static std::map<game::Item, size_t> avatarInventory;

	void Reset()
	{
		avatarInventory.clear();
	}


	void Start()
	{
		Reset();
	}
}