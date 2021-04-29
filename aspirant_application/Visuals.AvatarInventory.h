#pragma once
#include "Game.Item.h"
#include <optional>
#include "Common.XY.h"
#include <SDL.h>
namespace graphics::AvatarInventory
{
	void ResetIndex();
	void NextIndex();
	void PreviousIndex();
	std::optional<int> GetItem();
	void OnMouseMotion(const std::string&, const std::string&, const common::XY<Sint32>&);
	std::optional<int> OnMouseButtonUp(const std::string&, const std::string&, const common::XY<Sint32>&, Uint8);
}