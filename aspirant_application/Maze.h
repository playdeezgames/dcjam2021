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
		std::vector<Cell*> cells;//TODO: shared_ptr
		std::vector<Door*> doors;//TODO: shared_ptr
		void Clear();
		void PopulateCells();
		void InitializeCells();
		void InitializeCell(int, int);
	public:
		Maze(size_t, size_t);
		~Maze();
		Cell* GetCell(int, int);//TODO: shared_ptr
		const Cell* GetCell(int, int) const;//TODO: shared_ptr
		void Generate();
		size_t GetColumns() const;
		size_t GetRows() const;
	};
}
