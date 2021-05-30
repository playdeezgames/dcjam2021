#include "Maze.h"
#include <set>
#include "Common.RNG.h"
namespace maze
{
	Maze::Maze(size_t columns, size_t rows, const std::vector<Direction>& allDirections, std::function<Direction(const Direction&)> opposer)
		: cells()
		, doors()
		, columns(columns)
		, rows(rows)
		, allDirections(allDirections)
		, opposer(opposer)
	{
		PopulateCells();
		InitializeCells();
	}

	void Maze::PopulateCells()
	{

		while (cells.size() < columns * rows)
		{
			cells.push_back(std::make_shared<Cell<Direction, Door>>());
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
		auto cell = GetCell(column, row);
		for (auto direction : allDirections)
		{
			if (!cell.value()->GetNeighbor(direction))
			{
				int nextColumn = Directions::NextColumn(column, row, direction);
				int nextRow = Directions::NextRow(column, row, direction);
				if (nextColumn >= 0 && nextColumn < columns && nextRow >= 0 && nextRow < rows)
				{
					auto neighbor = GetCell(nextColumn, nextRow);
					auto door = std::make_shared<Door>();
					doors.push_back(door);
					cell.value()->PlaceNeighbor(direction, neighbor.value());
					cell.value()->PlaceDoor(direction, door);
					neighbor.value()->PlaceNeighbor(opposer(direction), cell.value());
					neighbor.value()->PlaceDoor(opposer(direction), door);
				}
			}
		}
	}

	std::optional<std::shared_ptr<Cell<Direction, Door>>> Maze::GetCell(int column, int row)
	{
		if (column >= 0 && column < columns && row >= 0 && row < rows)
		{
			return cells[column + row * columns];
		}
		return std::nullopt;
	}

	std::optional<const std::shared_ptr<Cell<Direction, Door>>> Maze::GetCell(int column, int row) const
	{
		if (column >= 0 && column < columns && row >= 0 && row < rows)
		{
			return cells[column + row * columns];
		}
		return std::nullopt;
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
		std::set<std::shared_ptr<Cell<Direction, Door>>> outside(cells.begin(), cells.end());
		std::vector<std::shared_ptr<Cell<Direction, Door>>> frontier;
		std::set<std::shared_ptr<Cell<Direction, Door>>> inside;
		auto cell = cells[common::RNG::FromRange(0, (int)cells.size())];
		outside.erase(cell);
		inside.insert(cell);
		for (auto direction : allDirections)
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
			for (auto direction : allDirections)
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
			for (auto direction : allDirections)
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