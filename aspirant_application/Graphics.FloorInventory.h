#pragma once
#include <optional>
#include "Game.Item.h"
namespace graphics::FloorInventory 
{ 
	void ResetIndex(); 
	void NextIndex();
	void PreviousIndex();
	std::optional<game::Item> GetItem();
}
