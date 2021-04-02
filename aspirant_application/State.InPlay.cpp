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
#include <sstream>
namespace state::InPlay
{
	const std::string LAYOUT_NAME = "State.InPlay";
	const std::string LEFT_SIDE_IMAGE_ID = "LeftSide";
	const std::string AHEAD_IMAGE_ID = "Ahead";
	const std::string RIGHT_SIDE_IMAGE_ID = "RightSide";
	const std::string DIRECTION_TEXT_ID = "Direction";
	const std::string POSITION_TEXT_ID = "Position";
	const std::string HEALTH_TEXT_ID = "Health";
	const std::string HUNGER_TEXT_ID = "Hunger";
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
		{maze::Direction::NORTH, "North"},
		{maze::Direction::EAST,  "East"},
		{maze::Direction::SOUTH, "South"},
		{maze::Direction::WEST, "West"}
	};

	static void OnCommand(const ::Command& command)
	{
		switch (command)
		{
		case ::Command::BACK:
		case ::Command::RED:
			application::UIState::Write(::UIState::MAIN_MENU);
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

	static void UpdatePosition()
	{
		auto position = game::Avatar::GetPosition();
		std::stringstream ss;
		ss << "(" << position.GetX() << ", " << position.GetY() << ")";
		::graphics::Texts::SetTextText(LAYOUT_NAME, POSITION_TEXT_ID, ss.str());
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
		::graphics::Texts::SetTextText(LAYOUT_NAME, HEALTH_TEXT_ID, ss.str());
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
		::graphics::Texts::SetTextText(LAYOUT_NAME, HUNGER_TEXT_ID, ss.str());
	}

	static void UpdatePOV()
	{
		::graphics::Images::SetSprite(LAYOUT_NAME, LEFT_SIDE_IMAGE_ID, leftSides[game::World::GetLeftSide()]);
		::graphics::Images::SetSprite(LAYOUT_NAME, AHEAD_IMAGE_ID, aheads[game::World::GetAhead()]);
		::graphics::Images::SetSprite(LAYOUT_NAME, RIGHT_SIDE_IMAGE_ID, rightSides[game::World::GetRightSide()]);
	}

	static void UpdateDirection()
	{
		auto facing = game::Avatar::GetFacing();
		::graphics::Texts::SetTextText(LAYOUT_NAME, DIRECTION_TEXT_ID, directionNames[facing]);
	}

	static void OnUpdate(const Uint32& ticks)
	{
		UpdatePOV();
		UpdatePosition();
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