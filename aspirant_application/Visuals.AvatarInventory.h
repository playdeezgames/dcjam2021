#pragma once
#include "Game.Item.h"
#include <optional>
#include "Common.XY.h"
#include <SDL.h>
namespace visuals::AvatarInventory
{
	void ResetIndex(const std::string&);
	void NextIndex(const std::string&);
	void PreviousIndex(const std::string&);
	std::optional<int> GetItem(const std::string&);
	void OnMouseMotion(const std::string&, const std::string&, const common::XY<Sint32>&);
	std::optional<int> OnMouseButtonUp(const std::string&, const std::string&, const common::XY<Sint32>&, Uint8);
}