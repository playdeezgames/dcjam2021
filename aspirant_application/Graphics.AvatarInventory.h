#pragma once
#include "Game.Item.h"
#include <optional>
namespace graphics::AvatarInventory
{
	void ResetIndex();
	void NextIndex();
	void PreviousIndex();
	std::optional<int> GetItem();
}