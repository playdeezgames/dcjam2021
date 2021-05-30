#pragma once
#include <vector>
#include "Maze.Direction.h"
#include "Maze.Door.h"
#include "Maze.Cell.h"
#include <memory>
#include <optional>
namespace maze
{
	class Maze
	{
	private:
		size_t columns;
		size_t rows;
		std::vector<std::shared_ptr<Cell<Direction, Door>>> cells;
		std::vector<std::shared_ptr<Door>> doors;
		const std::vector<Direction>& allDirections;
		std::function<Direction(const Direction&)> opposer;
		void Clear();
		void PopulateCells();
		void InitializeCells();
		void InitializeCell(size_t, size_t);
	public:
		Maze(size_t, size_t, const std::vector<Direction>&, std::function<Direction(const Direction&)>);
		std::optional<std::shared_ptr<Cell<Direction, Door>>> GetCell(size_t, size_t);
		std::optional<const std::shared_ptr<Cell<Direction, Door>>> GetCell(size_t, size_t) const;
		void Generate();
		size_t GetColumns() const;
		size_t GetRows() const;
	};
}
