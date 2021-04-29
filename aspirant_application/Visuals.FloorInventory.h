#pragma once
#include <optional>
#include "Game.Item.h"
#include <SDL.h>
#include "Common.XY.h"
namespace graphics::FloorInventory 
{ 
	void ResetIndex();
	void NextIndex();
	void PreviousIndex();
	std::optional<int> GetItem();
	void OnMouseMotion(const std::string&, const std::string&, const common::XY<Sint32>&);
	std::optional<int> OnMouseButtonUp(const std::string&, const std::string&, const common::XY<Sint32>&, Uint8);
}
