#pragma once
#include "Game.Item.h"
#include <optional>
namespace graphics::AvatarInventory
{
	//TODO: this is a singleton, but needs to be referenced by Layout/Name
	void ResetIndex();//TODO: becomes Reset(l,n)
	void NextIndex();//TODO: becomes Next(l,n)
	void PreviousIndex();//TODO: becomes Previous(l,n)
	std::optional<int> GetItem();//TODO: becomes Read(l,n)
	//TODO: set XY
	//TODO: set font
	//TODO: set row height
	//TODO: set colors
	//TODO: set drop shadow flag
	//TODO: set drop shadow color
	//TODO: set drop shadow XY
	//TODO: set visible
}