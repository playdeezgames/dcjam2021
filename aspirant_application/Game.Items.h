#pragma once
#include "Game.Item.h"
#include <string>
namespace game::Items
{
	//TODO: this needs a descriptor type
	std::string GetName(const game::Item&);
	std::string GetImageId(const game::Item&);
	void InitializeFromFile(const std::string&);
}
