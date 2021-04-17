#pragma once
#include <optional>
#include "Game.Item.h"
namespace graphics::FloorInventory 
{ 
	//TODO: this is a singleton, but needs to be referenced by Layout/Name
	//TODO: can this be combined with AvatarInventory? and both rolled into Menu
	void ResetIndex(); //TODO: becomes Reset(l,n)
	void NextIndex();//TODO: becomes Next(l,n)
	void PreviousIndex();//TODO: becomes Previous(l,n)
	std::optional<int> GetItem();//TODO: becomes Read(l,n)
}
