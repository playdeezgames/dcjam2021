#include "Application.Command.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Application.UIState.h"
#include "Graphics.Images.h"
#include "Graphics.Texts.h"
#include <vector>
#include <map>
#include "Game.Avatar.h"
#include "Game.World.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Items.h"
#include <sstream>
namespace state::in_play::MiniMap
{
	const std::string LAYOUT_NAME = "State.InPlay.MiniMap";
	const std::string LEFT_SIDE_IMAGE_ID = "LeftSide";
	const std::string AHEAD_IMAGE_ID = "Ahead";
	const std::string RIGHT_SIDE_IMAGE_ID = "RightSide";
	const std::string DIRECTION_TEXT_ID = "Direction";
	const std::string POSITION_TEXT_ID = "Position";
	const std::string HEALTH_TEXT_ID = "Health";
	const std::string HUNGER_TEXT_ID = "Hunger";
	const std::string FOOD_IMAGE_ID = "Food";
	const std::string POTION_IMAGE_ID = "Potion";

	static std::map<game::World::Border, std::string> leftSides =
	{
		{game::World::Border::DOOR, "LeftDoor"},
		{game::World::Border::WALL, "LeftWall"}
	};

	static std::map<game::World::Border, std::string> rightSides =
	{
		{game::World::Border::DOOR, "RightDoor"},
		{game::World::Border::WALL, "RightWall"}
	};

	static std::map<game::World::Border, std::string> aheads =
	{
		{game::World::Border::DOOR, "AheadDoor"},
		{game::World::Border::WALL, "AheadWall"}
	};

	static std::map<maze::Direction, std::string> directionNames =
	{
		{maze::Direction::NORTH, "N"},
		{maze::Direction::EAST,  "E"},
		{maze::Direction::SOUTH, "S"},
		{maze::Direction::WEST, "W"}
	};

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
		case ::Command::RED:
			application::UIState::Write(::UIState::LEAVE_PLAY);
			break;
		case ::Command::LEFT:
			game::Avatar::TurnLeft();
			break;
		case ::Command::RIGHT:
			game::Avatar::TurnRight();
			break;
		case ::Command::UP:
			game::Avatar::MoveAhead();
			break;
		case ::Command::DOWN:
			game::Avatar::MoveBack();
			break;
		}
	}

	static void UpdateHealth()
	{
		std::stringstream ss;
		ss << "Health: ";
		if (game::avatar::Statistics::IsDead())
		{
			ss << "DEAD!";
		}
		else
		{
			ss << game::avatar::Statistics::Read(game::avatar::Statistic::HEALTH) << "/" << game::avatar::Statistics::Maximum(game::avatar::Statistic::HEALTH);
		}
		::graphics::Texts::SetText(LAYOUT_NAME, HEALTH_TEXT_ID, ss.str());
	}

	static void UpdateHunger()
	{
		std::stringstream ss;
		ss << "Hunger: ";
		if (game::avatar::Statistics::IsStarving())
		{
			ss << "STARVING!";
		}
		else
		{
			ss << game::avatar::Statistics::Read(game::avatar::Statistic::HUNGER) << "/" << game::avatar::Statistics::Maximum(game::avatar::Statistic::HUNGER);
		}
		::graphics::Texts::SetText(LAYOUT_NAME, HUNGER_TEXT_ID, ss.str());
	}

	static void UpdatePOV()
	{
		::graphics::Images::SetSprite(LAYOUT_NAME, LEFT_SIDE_IMAGE_ID, leftSides[game::World::GetLeftSide()]);
		::graphics::Images::SetSprite(LAYOUT_NAME, AHEAD_IMAGE_ID, aheads[game::World::GetAhead()]);
		::graphics::Images::SetSprite(LAYOUT_NAME, RIGHT_SIDE_IMAGE_ID, rightSides[game::World::GetRightSide()]);
		auto position = game::Avatar::GetPosition();
		::graphics::Images::SetVisible(LAYOUT_NAME, FOOD_IMAGE_ID, game::Items::IsPresent(game::Item::FOOD, position));
		::graphics::Images::SetVisible(LAYOUT_NAME, POTION_IMAGE_ID, game::Items::IsPresent(game::Item::POTION, position));
	}

	static void UpdateDirection()
	{
		auto facing = game::Avatar::GetFacing();
		::graphics::Texts::SetText(LAYOUT_NAME, DIRECTION_TEXT_ID, directionNames[facing]);
	}

	static void OnUpdate(const Uint32& ticks)
	{
		UpdatePOV();
		UpdateDirection();
		UpdateHealth();
		UpdateHunger();
	}

	void Start()
	{
		::application::Command::SetHandler(::UIState::IN_PLAY, OnCommand);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY, LAYOUT_NAME);
		::application::Update::AddHandler(::UIState::IN_PLAY, OnUpdate);
	}
}