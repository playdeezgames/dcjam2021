#include <string>
#include "Game.Avatar.h"
#include <map>
#include "Game.World.h"
#include "Graphics.Texts.h"
#include "Graphics.Images.h"
#include "Game.Item.h"
#include "Game.World.Items.h"
#include "Application.Update.h"
#include "Game.Creatures.h"
#include <tuple>
#include "Graphics.Areas.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Game.Avatar.Items.h"
namespace sublayout::POV
{
	const std::string LEFT_SIDE_IMAGE_ID = "LeftSide";
	const std::string AHEAD_IMAGE_ID = "Ahead";
	const std::string RIGHT_SIDE_IMAGE_ID = "RightSide";
	const std::string DIRECTION_TEXT_ID = "Direction";
	const std::string POSITION_TEXT_ID = "Position";
	const std::string POV_LAYOUT_NAME = "Sublayout.POV";

	const std::string AREA_TAKE_FOOD = "TakeFood";
	const std::string IMAGE_TAKE_FOOD = "TakeFood";

	const std::map<game::world::Border, std::string> leftSides =
	{
		{game::world::Border::DOOR, "LeftDoor"},
		{game::world::Border::WALL, "LeftWall"}
	};

	const std::map<game::world::Border, std::string> rightSides =
	{
		{game::world::Border::DOOR, "RightDoor"},
		{game::world::Border::WALL, "RightWall"}
	};

	const std::map<game::world::Border, std::string> aheads =
	{
		{game::world::Border::DOOR, "AheadDoor"},
		{game::world::Border::WALL, "AheadWall"}
	};

	const std::map<maze::Direction, std::string> directionNames =
	{
		{maze::Direction::NORTH, "N"},
		{maze::Direction::EAST,  "E"},
		{maze::Direction::SOUTH, "S"},
		{maze::Direction::WEST, "W"}
	};

	static void UpdateDirection(maze::Direction facing	)
	{
		::graphics::Texts::SetText(POV_LAYOUT_NAME, DIRECTION_TEXT_ID, directionNames.find(facing)->second);
	}

	static void UpdateCreatures(const common::XY<size_t> position)
	{
		auto creatures = game::creature::All();
		for (auto creature : creatures)
		{
			auto imageId = game::creature::GetDescriptor(creature).imageId;
			auto instance = game::Creatures::GetInstance(position);
			bool visible = instance && instance.value().creature == creature;
			graphics::Images::SetVisible(POV_LAYOUT_NAME, imageId, visible);
		}
	}

	static void UpdateRoom(const common::XY<size_t> position, const maze::Direction& facing)
	{
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, LEFT_SIDE_IMAGE_ID, leftSides.find(game::World::GetBorderLeft(position, facing))->second);
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, AHEAD_IMAGE_ID, aheads.find(game::World::GetBorderAhead(position, facing))->second);
		::graphics::Images::SetSprite(POV_LAYOUT_NAME, RIGHT_SIDE_IMAGE_ID, rightSides.find(game::World::GetBorderRight(position, facing))->second);
	}

	static void UpdateItems(const common::XY<size_t> position)
	{

		for (auto& item : game::item::All())
		{
			::graphics::Images::SetVisible(POV_LAYOUT_NAME, game::item::GetDescriptor(item).imageId, game::world::Items::IsPresent(position, item));
		}
	}

	static void UpdatePOV(const Uint32&)
	{
		auto position = game::Avatar::GetPosition();
		auto facing = game::Avatar::GetFacing();
		UpdateRoom(position, facing);
		UpdateItems(position);
		UpdateDirection(facing);
		UpdateCreatures(position);
	}

	const std::vector<::UIState> states = 
	{
		::UIState::IN_PLAY_MAP,
		::UIState::IN_PLAY_FLOOR,
		::UIState::IN_PLAY_INVENTORY,
		::UIState::IN_PLAY_STATUS,
		::UIState::IN_PLAY_COMBAT,
		::UIState::IN_PLAY_COMBAT_RESULT
	};

	static void OnMouseMotion(const common::XY<Sint32>& xy)
	{
		auto areas = graphics::Areas::Get(POV_LAYOUT_NAME, xy);
		auto position = game::Avatar::GetPosition();
		for (auto& item : game::item::All())
		{
			auto descriptor = game::item::GetDescriptor(item);
			::graphics::Images::SetVisible(POV_LAYOUT_NAME, descriptor.takeImageId, areas.contains(descriptor.takeAreaId) && game::world::Items::IsPresent(position, item));
		}
	}

	static bool OnMouseButtonUp(const common::XY<Sint32>& xy, Uint8)
	{
		auto areas = graphics::Areas::Get(POV_LAYOUT_NAME, xy);
		auto position = game::Avatar::GetPosition();
		int index = 0;
		for (auto& item : game::item::All())
		{
			auto descriptor = game::item::GetDescriptor(item);
			if (areas.contains(descriptor.takeAreaId) && game::world::Items::IsPresent(position, item))
			{
				//TODO: this is duplicated code
				auto inventory = game::world::Items::FloorInventory(game::Avatar::GetPosition());
				size_t amount = game::world::Items::Remove(game::Avatar::GetPosition(), item, inventory[item]);
				game::avatar::Items::Add(item, amount);

				::graphics::Images::SetVisible(POV_LAYOUT_NAME, descriptor.takeImageId, false);
				return true;
			}
			index++;
		}
		return false;
	}

	void Start()
	{
		application::MouseMotion::AddHandler(::UIState::IN_PLAY_MAP, OnMouseMotion);
		application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_MAP, OnMouseButtonUp);
		for (auto state : states)
		{
			::application::Update::AddHandler(state, UpdatePOV);
		}
	}
}