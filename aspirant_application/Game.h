#pragma once
namespace game
{
	enum class Border
	{
		WALL,
		DOOR
	};
	Border GetLeftSide();
	Border GetAhead();
	Border GetRightSide();
	void TurnLeft();
	void TurnRight();

	void Start();
}
