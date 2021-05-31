#pragma once
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <vector>
#include <set>
namespace maze
{
	template<typename TDirection, typename TDoor>
	class Cell
	{
	private:
		std::map<TDirection, std::shared_ptr<TDoor>> exits;
		std::map<TDirection, std::shared_ptr<Cell<TDirection, TDoor>>> neighbors;
	public:
		Cell()
			: exits()
			, neighbors()
		{

		}
		void PlaceNeighbor(TDirection direction, std::shared_ptr<Cell<TDirection, TDoor>> cell)
		{
			neighbors[direction] = cell;
		}
		void PlaceDoor(TDirection direction, std::shared_ptr<TDoor> door)
		{
			exits[direction] = door;
		}
		bool SetDoor(TDirection direction, TDoor door)
		{
			auto iter = exits.find(direction);
			if (iter != exits.end())
			{
				(*iter->second) = door;
				return true;
			}
			return false;
		}
		std::optional<const std::shared_ptr<Cell<TDirection, TDoor>>> GetNeighbor(TDirection direction) const
		{
			auto neighbor = neighbors.find(direction);
			return
				(neighbor != neighbors.end()) ?
				(std::make_optional(neighbor->second)) :
				(std::nullopt);
		}
		std::optional<std::shared_ptr<Cell<TDirection, TDoor>>> GetNeighbor(TDirection direction)
		{
			auto neighbor = neighbors.find(direction);
			return
				(neighbor != neighbors.end()) ?
				(std::make_optional(neighbor->second)) :
				(std::nullopt);
		}
		std::optional<const std::shared_ptr<TDoor>> GetDoor(TDirection direction) const
		{
			auto door = exits.find(direction);
			return
				(door != exits.end()) ?
				(std::optional<const std::shared_ptr<TDoor>>(door->second)) :
				(std::nullopt);
		}
		std::optional<std::shared_ptr<TDoor>> GetDoor(TDirection direction)
		{
			auto door = exits.find(direction);
			return
				(door != exits.end()) ?
				(std::optional<std::shared_ptr<TDoor>>(door->second)) :
				(std::nullopt);
		}
		size_t CountDoorsThat(std::function<bool(const TDoor&)> condition) const
		{
			size_t count = 0u;
			for (auto& entry : exits)
			{
				if (condition(*entry.second))
				{
					count++;
				}
			}
			return count;
		}
	};
	template<typename TDirection>
	struct Initializer
	{
		std::function<TDirection(const TDirection&)> opposer;
		std::function<size_t(size_t, size_t, const TDirection&)> columnStepper;
		std::function<size_t(size_t, size_t, const TDirection&)> rowStepper;
	};
	template<typename TDoor>
	struct Generator
	{
		const TDoor& closed;
		const TDoor& open; 
		std::function<size_t(size_t, size_t)> rng;
	};
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
		void InitializeCells(const Initializer<TDirection>& initializer)
		{
			for (int column = 0; column < columns; ++column)
			{
				for (int row = 0; row < rows; ++row)
				{
					InitializeCell(column, row, initializer);
				}
			}
		}
		void InitializeCell(size_t column, size_t row, const Initializer<TDirection>& initializer)
		{
			auto cell = GetCell(column, row);
			for (auto direction : allDirections)
			{
				if (!cell.value()->GetNeighbor(direction))
				{
					size_t nextColumn = initializer.columnStepper(column, row, direction);
					size_t nextRow = initializer.rowStepper(column, row, direction);
					if (nextColumn >= 0 && nextColumn < columns && nextRow >= 0 && nextRow < rows)
					{
						auto neighbor = GetCell(nextColumn, nextRow);
						auto door = std::make_shared<TDoor>();
						doors.push_back(door);
						cell.value()->PlaceNeighbor(direction, neighbor.value());
						cell.value()->PlaceDoor(direction, door);
						neighbor.value()->PlaceNeighbor(initializer.opposer(direction), cell.value());
						neighbor.value()->PlaceDoor(initializer.opposer(direction), door);
					}
				}
			}
		}
	public:
		Maze(
			size_t columns, 
			size_t rows, 
			const std::vector<TDirection>& allDirections,
			const Initializer<TDirection>& initializer)
			: cells()
			, doors()
			, columns(columns)
			, rows(rows)
			, allDirections(allDirections)
		{
			PopulateCells();
			InitializeCells(initializer);
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
		void Generate(const Generator<TDoor>& generator)
		{
			Clear(generator.closed);
			std::set<std::shared_ptr<Cell<TDirection, TDoor>>> outside(cells.begin(), cells.end());
			std::vector<std::shared_ptr<Cell<TDirection, TDoor>>> frontier;
			std::set<std::shared_ptr<Cell<TDirection, TDoor>>> inside;
			auto cell = cells[generator.rng(0, (int)cells.size())];
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
				size_t index = generator.rng(0, (int)frontier.size());
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
				TDirection direction = candidates[generator.rng(0, (int)candidates.size())];
				*(cell->GetDoor(direction).value()) = generator.open;
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
