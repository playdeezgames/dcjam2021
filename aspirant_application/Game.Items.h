#pragma once
#include "Game.Item.h"
#include <string>
namespace game::Items
{
	std::string GetName(const game::Item&);
	std::string GetImageId(const game::Item&);
	void InitializeFromFile(const std::string&);
}
