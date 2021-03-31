#include "Maze.h"
#include <set>
#include "Common.RNG.h"
namespace maze
{
	Maze::Maze(size_t columns, size_t rows)
		: cells()
		, doors()
		, columns(columns)
		, rows(rows)
	{
		PopulateCells();
		InitializeCells();
	}

	void Maze::PopulateCells()
	{

		while (cells.size() < columns * rows)
		{
			cells.push_back(new Cell());
		}
	}


	void Maze::InitializeCells()
	{
		for (int column = 0; column < columns; ++column)
		{
			for (int row = 0; row < rows; ++row)
			{
				InitializeCell(column, row);
			}
		}
	}


	void Maze::InitializeCell(int column, int row)
	{
		Cell* cell = GetCell(column, row);
		for (auto direction : MazeDirectionHelper::GetAll())
		{
			if (!cell->GetNeighbor(direction))
			{
				int nextColumn = MazeDirectionHelper::GetNextColumn(column, row, direction);
				int nextRow = MazeDirectionHelper::GetNextRow(column, row, direction);
				if (nextColumn >= 0 && nextColumn < columns && nextRow >= 0 && nextRow < rows)
				{
					Cell* neighbor = GetCell(nextColumn, nextRow);
					Door* door = new Door();
					doors.push_back(door);
					cell->SetNeighbor(direction, neighbor);
					cell->SetDoor(direction, door);
					neighbor->SetNeighbor(MazeDirectionHelper::GetOpposite(direction), cell);
					neighbor->SetDoor(MazeDirectionHelper::GetOpposite(direction), door);
				}
			}
		}
	}


	Maze::~Maze()
	{
		for (auto& ptr : cells)
		{
			delete ptr;
		}
		for (auto& ptr : doors)
		{
			delete ptr;
		}
	}

	Cell* Maze::GetCell(int column, int row)
	{
		if (column >= 0 && column < columns && row >= 0 && row < rows)
		{
			return cells[column + row * columns];
		}
		return nullptr;
	}

	const Cell* Maze::GetCell(int column, int row) const
	{
		if (column >= 0 && column < columns && row >= 0 && row < rows)
		{
			return cells[column + row * columns];
		}
		return nullptr;
	}

	void Maze::Clear()
	{
		for (auto& door : doors)
		{
			(*door) = Door::CLOSED;
		}
	}

	void Maze::Generate()
	{
		Clear();
		std::set<Cell*> outside(cells.begin(), cells.end());
		std::vector<Cell*> frontier;
		std::set<Cell*> inside;
		Cell* cell = cells[common::RNG::FromRange(0, (int)cells.size())];
		outside.erase(cell);
		inside.insert(cell);
		for (auto direction : MazeDirectionHelper::GetAll())
		{
			auto neighbor = cell->GetNeighbor(direction);
			if (neighbor)
			{
				outside.erase(neighbor.value());
				frontier.push_back(neighbor.value());
			}
		}
		while (!frontier.empty())
		{
			size_t index = common::RNG::FromRange(0, (int)frontier.size());
			cell = frontier[index];
			frontier[index] = frontier[frontier.size() - 1];
			frontier.pop_back();
			std::vector<Direction> candidates;
			for (auto direction : MazeDirectionHelper::GetAll())
			{
				auto neighbor = cell->GetNeighbor(direction);
				if (neighbor)
				{
					if (inside.contains(neighbor.value()))
					{
						candidates.push_back(direction);
					}
				}
			}
			Direction direction = candidates[common::RNG::FromRange(0, (int)candidates.size())];
			*(cell->GetDoor(direction).value()) = Door::OPEN;
			inside.insert(cell);
			for (auto direction : MazeDirectionHelper::GetAll())
			{
				auto neighbor = cell->GetNeighbor(direction);
				if (neighbor)
				{
					if (outside.contains(neighbor.value()))
					{
						outside.erase(neighbor.value());
						frontier.push_back(neighbor.value());
					}
				}
			}
		}
	}

	size_t Maze::GetColumns() const
	{
		return columns;
	}

	size_t Maze::GetRows() const
	{
		return rows;
	}
}