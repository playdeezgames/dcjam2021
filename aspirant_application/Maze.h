#pragma once
#include <vector>
#include "Maze.Cell.h"
#include "Maze.Door.h"
#include <memory>
#include <optional>
namespace maze
{
	class Maze
	{
	private:
		size_t columns;
		size_t rows;
		std::vector<std::shared_ptr<Cell>> cells;
		std::vector<std::shared_ptr<Door>> doors;
		void Clear();
		void PopulateCells();
		void InitializeCells();
		void InitializeCell(int, int);
	public:
		Maze(size_t, size_t);
		std::optional<std::shared_ptr<Cell>> GetCell(int, int);
		std::optional<const std::shared_ptr<Cell>> GetCell(int, int) const;
		void Generate();
		size_t GetColumns() const;
		size_t GetRows() const;
	};
}
