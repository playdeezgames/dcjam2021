#pragma once
#include <vector>
#include "Maze.Cell.h"
#include "Maze.Door.h"
namespace maze
{
	class Maze
	{
	private:
		size_t columns;
		size_t rows;
		std::vector<Cell*> cells;
		std::vector<Door*> doors;
		void Clear();
		void PopulateCells();
		void InitializeCells();
		void InitializeCell(int, int);
	public:
		Maze(size_t, size_t);
		~Maze();
		Cell* GetCell(int, int);
		const Cell* GetCell(int, int) const;
		void Generate();
		size_t GetColumns() const;
		size_t GetRows() const;
	};
}
