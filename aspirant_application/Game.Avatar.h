#pragma once
#include "Maze.Direction.h"
#include "Common.XY.h"
#include <string>
#include <optional>
#include "json.hpp"
namespace game::Avatar
{
	struct Descriptor
	{
		int poopItemId;
		int hungerRate;
		int sobrietyRate;
		size_t mapFragmentWidth;
		size_t mapFragmentHeight;
		int keyItemId;
		int mapFragmentItemId;
		int poopThreshold;
		int joolsItemId;
		int trousersItemId;
		int soiledTrousersItemId;
	};
	const Descriptor& GetDescriptor();

	std::optional<std::string> TurnLeft();
	std::optional<std::string> TurnRight();
	std::optional<std::string> MoveAhead();
	std::optional<std::string> MoveBack();
	maze::Direction GetFacing();
	common::XY<size_t> GetPosition();
	void SetPosition(const common::XY<size_t>&);
	nlohmann::json& GetAvatar();//TODO: this is part of the DAL
	bool CanPoop();
	std::optional<std::string> Poop(bool);
}