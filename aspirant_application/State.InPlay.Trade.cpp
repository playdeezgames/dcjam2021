#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.OnEnter.h"
#include "Application.MouseButtonUp.h"
#include "Game.Shoppes.h"
#include "Game.Avatar.h"
#include "Visuals.Texts.h"
#include "Application.MouseMotion.h"
#include "Visuals.Areas.h"
#include "Visuals.Images.h"
namespace state::in_play::Trade
{
	const std::string LAYOUT_NAME = "State.InPlay.Trade";
	const std::string TEXT_TITLE = "Title";
	const std::string AREA_UI_CLOSE = "UIClose";
	const std::string IMAGE_UI_CLOSE = "ui-close";
	const std::string SPRITE_HOVER = "UICloseHover";
	const std::string SPRITE_NORMAL = "UICloseNormal";

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, application::UIState::GoTo(::UIState::IN_PLAY_MAP) }
	};


	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		if (area == AREA_UI_CLOSE)
		{
			application::UIState::Write(::UIState::IN_PLAY_MAP);
		}
		return true;
	}

	static void OnEnter()
	{
		auto position = game::Avatar::GetPosition();
		auto shoppe = game::Shoppes::Read(position);
		if (shoppe)
		{
			auto& descriptor = game::shoppe::GetDescriptor(*shoppe);
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_TITLE, descriptor.name);
		}
	}

	static void MouseMotionInArea(const std::string& area, const common::XY<Sint32>&)
	{
		if (area == AREA_UI_CLOSE)
		{
			visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_CLOSE, SPRITE_HOVER);
		}
		else
		{
			visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_CLOSE, SPRITE_NORMAL);
		}
	}

	static void MouseMotionOutsideAreas(const common::XY<Sint32>&)
	{
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_CLOSE, SPRITE_NORMAL);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_TRADE, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_TRADE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(::UIState::IN_PLAY_TRADE, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_TRADE, LAYOUT_NAME);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_TRADE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, MouseMotionInArea, MouseMotionOutsideAreas));
	}
}