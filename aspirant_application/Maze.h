#pragma once
#include <vector>
#include "Maze.Direction.h"
#include "Maze.Cell.h"
#include <memory>
#include <optional>
#include <set>
#include "Common.RNG.h"
namespace maze
{
	template<typename TDoor>
	class Maze
	{
	private:
		size_t columns;
		size_t rows;
		std::vector<std::shared_ptr<Cell<Direction, TDoor>>> cells;
		std::vector<std::shared_ptr<TDoor>> doors;
		const std::vector<Direction>& allDirections;
		void Clear(const TDoor& closed)
		{
			for (auto& door : doors)
			{
				(*door) = closed;
			}
		}
		void PopulateCells()
		{

			while (cells.size() < columns * rows)
			{
				cells.push_back(std::make_shared<Cell<Direction, TDoor>>());
			}
		}
		void InitializeCells(std::function<Direction(const Direction&)> opposer,
			std::function<size_t(size_t, size_t, const Direction&)> columnStepper,
			std::function<size_t(size_t, size_t, const Direction&)> rowStepper)
		{
			for (int column = 0; column < columns; ++column)
			{
				for (int row = 0; row < rows; ++row)
				{
					InitializeCell(column, row, opposer, columnStepper, rowStepper);
				}
			}
		}
		void InitializeCell(size_t column, size_t row, std::function<Direction(const Direction&)> opposer,
			std::function<size_t(size_t, size_t, const Direction&)> columnStepper,
			std::function<size_t(size_t, size_t, const Direction&)> rowStepper)
		{
			auto cell = GetCell(column, row);
			for (auto direction : allDirections)
			{
				if (!cell.value()->GetNeighbor(direction))
				{
					size_t nextColumn = columnStepper(column, row, direction);
					size_t nextRow = rowStepper(column, row, direction);
					if (nextColumn >= 0 && nextColumn < columns && nextRow >= 0 && nextRow < rows)
					{
						auto neighbor = GetCell(nextColumn, nextRow);
						auto door = std::make_shared<TDoor>();
						doors.push_back(door);
						cell.value()->PlaceNeighbor(direction, neighbor.value());
						cell.value()->PlaceDoor(direction, door);
						neighbor.value()->PlaceNeighbor(opposer(direction), cell.value());
						neighbor.value()->PlaceDoor(opposer(direction), door);
					}
				}
			}
		}
	public:
		Maze(
			size_t columns, 
			size_t rows, 
			const std::vector<Direction>& allDirections, 
			std::function<Direction(const Direction&)> opposer, 
			std::function<size_t(size_t, size_t, const Direction&)> columnStepper, 
			std::function<size_t(size_t, size_t, const Direction&)> rowStepper)
			: cells()
			, doors()
			, columns(columns)
			, rows(rows)
			, allDirections(allDirections)
		{
			PopulateCells();
			InitializeCells(opposer, columnStepper, rowStepper);
		}
		std::optional<std::shared_ptr<Cell<Direction, TDoor>>> GetCell(size_t column, size_t row)
		{
			if (column >= 0 && column < columns && row >= 0 && row < rows)
			{
				return cells[column + row * columns];
			}
			return std::nullopt;
		}
		std::optional<const std::shared_ptr<Cell<Direction, TDoor>>> GetCell(size_t column, size_t row) const
		{
			if (column >= 0 && column < columns && row >= 0 && row < rows)
			{
				return cells[column + row * columns];
			}
			return std::nullopt;
		}
		void Generate(const TDoor& closed, const TDoor& open)
		{
			Clear(closed);
			std::set<std::shared_ptr<Cell<Direction, TDoor>>> outside(cells.begin(), cells.end());
			std::vector<std::shared_ptr<Cell<Direction, TDoor>>> frontier;
			std::set<std::shared_ptr<Cell<Direction, TDoor>>> inside;
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
				*(cell->GetDoor(direction).value()) = open;
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
		size_t GetColumns() const
		{
			return columns;
		}
		size_t GetRows() const
		{
			return rows;
		}
	};
}
