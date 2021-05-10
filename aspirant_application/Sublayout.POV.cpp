#include "Game.Avatar.h"
#include "Game.World.h"
#include "Visuals.Texts.h"
#include "Visuals.Images.h"
#include "Game.World.Items.h"
#include "Application.Update.h"//TODO: replace with OnEnter
#include "Game.Creatures.h"
#include "Visuals.Areas.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Statistics.h"
#include <sstream>
namespace sublayout::POV
{
	const std::string LAYOUT_NAME = "Sublayout.POV";
	const std::string AREA_TAKE_FOOD = "TakeFood";
	const std::string IMAGE_LEFT_SIDE = "LeftSide";
	const std::string IMAGE_AHEAD = "Ahead";
	const std::string IMAGE_RIGHT_SIDE = "RightSide";
	const std::string IMAGE_TAKE_FOOD = "TakeFood";
	const std::string TEXT_DIRECTION = "Direction";
	const std::string TEXT_POSITION = "Position";
	const std::string TEXT_AVATAR_STATE = "AvatarState";
	const std::string TEXT_ITEM_TOOL_TIP = "ItemToolTip";
	const std::string EMPTY_TOOLTIP = "";


	const std::map<game::world::Border, std::string> leftSides =
	{
		{game::world::Border::LOCK, "LeftLockedDoor"},
		{game::world::Border::DOOR, "LeftDoor"},
		{game::world::Border::WALL, "LeftWall"}
	};

	const std::map<game::world::Border, std::string> rightSides =
	{
		{game::world::Border::LOCK, "RightLockedDoor"},
		{game::world::Border::DOOR, "RightDoor"},
		{game::world::Border::WALL, "RightWall"}
	};

	const std::map<game::world::Border, std::string> aheads =
	{
		{game::world::Border::LOCK, "AheadLockedDoor"},
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
		::visuals::Texts::SetText(LAYOUT_NAME, TEXT_DIRECTION, directionNames.find(facing)->second);
	}

	static void UpdateCreatures(const common::XY<size_t> position)
	{
		auto& creatures = game::creature::All();
		int index = 0;
		for (auto& creature : creatures)
		{
			auto imageId = creature.imageId;
			auto instance = game::Creatures::GetInstance(position);
			bool visible = instance && instance.value().creature == index;
			visuals::Images::SetVisible(LAYOUT_NAME, imageId, visible);
			index++;
		}
	}

	static void UpdateRoom(const common::XY<size_t> position, const maze::Direction& facing)
	{
		::visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_LEFT_SIDE, leftSides.find(game::World::GetBorderLeft(position, facing))->second);
		::visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_AHEAD, aheads.find(game::World::GetBorderAhead(position, facing))->second);
		::visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_RIGHT_SIDE, rightSides.find(game::World::GetBorderRight(position, facing))->second);
	}

	static void UpdateItems(const common::XY<size_t> position)
	{

		for (auto& item : game::item::All())
		{
			::visuals::Images::SetVisible(LAYOUT_NAME, game::item::GetDescriptor(item).imageId, game::world::Items::IsPresent(position, item));
		}
	}

	static void UpdateAvatarState()
	{
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::NAUSEA))
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_STATE, "*Dizzy*");
		}
		else
		{
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_STATE, "");
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
		UpdateAvatarState();
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

	static void OnMouseMotionInArea(const std::string& area, const common::XY<Sint32>&)
	{
		std::string itemToolTip = "";
		auto position = game::Avatar::GetPosition();
		for (auto& item : game::item::All())
		{
			auto descriptor = game::item::GetDescriptor(item);
			bool showItem = area == descriptor.takeAreaId && game::world::Items::IsPresent(position, item);
			::visuals::Images::SetVisible(LAYOUT_NAME, descriptor.takeImageId, showItem);
			if (showItem)
			{
				std::stringstream ss;
				ss << descriptor.name;
				auto count = game::world::Items::GetRoomInventory(position, item);
				if (count > 1)
				{
					ss << "(" << count << ")";
				}
				itemToolTip = ss.str();
			}
		}
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ITEM_TOOL_TIP, itemToolTip);
	}

	static void OnMouseMotionOutsideAreas(const common::XY<Sint32>&)
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_ITEM_TOOL_TIP, EMPTY_TOOLTIP);
	}

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		auto position = game::Avatar::GetPosition();
		int index = 0;
		for (auto& item : game::item::All())
		{
			auto descriptor = game::item::GetDescriptor(item);
			if (area == descriptor.takeAreaId && game::world::Items::IsPresent(position, item))
			{
				auto inventory = game::world::Items::FloorInventory(game::Avatar::GetPosition());
				size_t amount = game::world::Items::Remove(game::Avatar::GetPosition(), item, inventory[item]);
				game::avatar::Items::Add(item, amount);

				::visuals::Images::SetVisible(LAYOUT_NAME, descriptor.takeImageId, false);
				visuals::Texts::SetText(LAYOUT_NAME, TEXT_ITEM_TOOL_TIP, EMPTY_TOOLTIP);
				return true;
			}
			index++;
		}
		return false;
	}

	void Start()
	{
		application::MouseMotion::AddHandler(::UIState::IN_PLAY_MAP, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideAreas));
		application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_MAP, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		for (auto state : states)
		{
			::application::Update::AddHandler(state, UpdatePOV);
		}
	}
}