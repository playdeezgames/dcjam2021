#pragma once
#include "Maze.Direction.h"
#include "Common.XY.h"
namespace game::Avatar
{
	void TurnLeft();//TODO: this should return an optional string with a sound effect?
	void TurnRight();//TODO: this should return an optional string with a sound effect?
	void MoveAhead();//TODO: this should return an optional string with a sound effect?
	void MoveBack();//TODO: this should return an optional string with a sound effect?
	maze::Direction GetFacing();
	common::XY<size_t> GetPosition();
}