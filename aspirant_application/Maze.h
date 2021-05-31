#pragma once
#include <vector>
#include "Maze.Direction.h"
#include "Maze.Cell.h"
#include <memory>
#include <optional>
#include <set>
#include <functional>
namespace maze
{
	template<typename TDirection, typename TDoor>
	class Maze
	{
	private:
		size_t columns;
		size_t rows;
		std::vector<std::shared_ptr<Cell<TDirection, TDoor>>> cells;
		std::vector<std::shared_ptr<TDoor>> doors;
		const std::vector<TDirection>& allDirections;
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
				cells.push_back(std::make_shared<Cell<TDirection, TDoor>>());
			}
		}
		void InitializeCells(std::function<TDirection(const TDirection&)> opposer,
			std::function<size_t(size_t, size_t, const TDirection&)> columnStepper,
			std::function<size_t(size_t, size_t, const TDirection&)> rowStepper)
		{
			for (int column = 0; column < columns; ++column)
			{
				for (int row = 0; row < rows; ++row)
				{
					InitializeCell(column, row, opposer, columnStepper, rowStepper);
				}
			}
		}
		void InitializeCell(size_t column, size_t row, std::function<TDirection(const TDirection&)> opposer,
			std::function<size_t(size_t, size_t, const TDirection&)> columnStepper,
			std::function<size_t(size_t, size_t, const TDirection&)> rowStepper)
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
			const std::vector<TDirection>& allDirections,
			std::function<TDirection(const TDirection&)> opposer,
			std::function<size_t(size_t, size_t, const TDirection&)> columnStepper,
			std::function<size_t(size_t, size_t, const TDirection&)> rowStepper)
			: cells()
			, doors()
			, columns(columns)
			, rows(rows)
			, allDirections(allDirections)
		{
			PopulateCells();
			InitializeCells(opposer, columnStepper, rowStepper);
		}
		std::optional<std::shared_ptr<Cell<TDirection, TDoor>>> GetCell(size_t column, size_t row)
		{
			if (column >= 0 && column < columns && row >= 0 && row < rows)
			{
				return cells[column + row * columns];
			}
			return std::nullopt;
		}
		std::optional<const std::shared_ptr<Cell<TDirection, TDoor>>> GetCell(size_t column, size_t row) const
		{
			if (column >= 0 && column < columns && row >= 0 && row < rows)
			{
				return cells[column + row * columns];
			}
			return std::nullopt;
		}
		void Generate(const TDoor& closed, const TDoor& open, std::function<size_t(size_t,size_t)> rng)
		{
			Clear(closed);
			std::set<std::shared_ptr<Cell<TDirection, TDoor>>> outside(cells.begin(), cells.end());
			std::vector<std::shared_ptr<Cell<TDirection, TDoor>>> frontier;
			std::set<std::shared_ptr<Cell<TDirection, TDoor>>> inside;
			auto cell = cells[rng(0, (int)cells.size())];
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
				size_t index = rng(0, (int)frontier.size());
				cell = frontier[index];
				frontier[index] = frontier[frontier.size() - 1];
				frontier.pop_back();
				std::vector<TDirection> candidates;
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
				TDirection direction = candidates[rng(0, (int)candidates.size())];
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
