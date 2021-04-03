#include "Game.Avatar.Items.h"
#include <map>
namespace game::avatar::Items
{
	static std::map<game::Item, size_t> avatarInventory;

	const std::map<game::Item, size_t>& All()
	{
		return avatarInventory;
	}

	void Reset()
	{
		avatarInventory.clear();
	}

	void Add(game::Item item, size_t amount)
	{
		if (amount > 0)
		{
			avatarInventory[item] += amount;
		}
	}

	void Start()
	{
		Reset();
	}
}