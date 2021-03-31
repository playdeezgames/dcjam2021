#pragma once
#include <vector>
namespace maze
{
	enum class Direction
	{
		NORTH,
		EAST,
		SOUTH,
		WEST
	};
	class MazeDirectionHelper
	{
	public:
		static Direction GetOpposite(Direction);
		static const std::vector<Direction>& GetAll();
		static int GetNextColumn(int, int, Direction);
		static int GetNextRow(int, int, Direction);
	};
}