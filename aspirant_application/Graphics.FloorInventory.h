#pragma once
#include <optional>
#include "Game.Item.h"
namespace graphics::FloorInventory 
{ 
	//TODO: this is a singleton, but needs to be referenced by Layout/Name
	//TODO: can this be combined with AvatarInventory?
	void ResetIndex(); //TODO: becomes Reset(l,n)
	void NextIndex();//TODO: becomes Next(l,n)
	void PreviousIndex();//TODO: becomes Previous(l,n)
	std::optional<game::Item> GetItem();//TODO: becomes Read(l,n)
	//TODO: set XY
	//TODO: set font
	//TODO: set row height
	//TODO: set colors
	//TODO: set drop shadow flag
	//TODO: set drop shadow color
	//TODO: set drop shadow XY
	//TODO: set visible
}
