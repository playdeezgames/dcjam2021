#pragma once
namespace game::World
{
	enum class Border
	{
		WALL,
		DOOR
	};
	Border GetLeftSide();
	Border GetAhead();
	Border GetRightSide();
}