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
#include "Visuals.Menus.h"
#include "Common.Audio.h"
#include "Game.Avatar.Items.h"
namespace state::in_play::Trade
{
	const std::string LAYOUT_NAME = "State.InPlay.Trade";
	const std::string TEXT_TITLE = "Title";
	const std::string AREA_UI_CLOSE = "UIClose";
	const std::string AREA_TRADE_1 = "Trade1";
	const std::string AREA_TRADE_2 = "Trade2";
	const std::string AREA_TRADE_3 = "Trade3";
	const std::string MENU_ITEM_TRADE_1 = "Trade1";
	const std::string MENU_ITEM_TRADE_2 = "Trade2";
	const std::string MENU_ITEM_TRADE_3 = "Trade3";
	const std::string MENU_ID = "Trade";
	const std::string IMAGE_UI_CLOSE = "ui-close";
	const std::string SPRITE_HOVER = "UICloseHover";
	const std::string SPRITE_NORMAL = "UICloseNormal";
	const size_t MENU_ITEM_COUNT = 3;

	enum class TradeItem
	{
		TRADE_1,
		TRADE_2,
		TRADE_3
	};

	static bool CloseTrade()
	{
		application::UIState::Write(::UIState::IN_PLAY_MAP);
		return true;
	}

	static void CompleteTrade(size_t index)
	{
		common::audio::Sfx::Play(game::Shoppes::AttemptTrade(game::Avatar::GetPosition(), index));
		application::OnEnter::Handle();
	}

	static std::function<bool()> DoTrade(size_t index)
	{
		return [index]() {
			CompleteTrade(index);
			return true;
		};
	}

	const std::map<std::string, std::function<bool()>> mouseButtonUpAreaHandlers =
	{
		{AREA_UI_CLOSE, CloseTrade},
		{AREA_TRADE_1, DoTrade(0u)},
		{AREA_TRADE_2, DoTrade(1u)},
		{AREA_TRADE_3, DoTrade(2u)}
	};

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		auto iter = mouseButtonUpAreaHandlers.find(area);
		if (iter != mouseButtonUpAreaHandlers.end())
		{
			return iter->second();
		}
		return false;
	}

	const std::vector<std::string> menuItemIds =
	{
		MENU_ITEM_TRADE_1,
		MENU_ITEM_TRADE_2,
		MENU_ITEM_TRADE_3
	};

	static void UpdateTradeMenuItemTexts(const std::vector<game::shoppe::TradeDescriptor>& descriptors)
	{
		for (size_t index = 0; index < MENU_ITEM_COUNT; index++)
		{
			const std::string menuItemId = menuItemIds[index];
			visuals::MenuItems::SetEnabled(LAYOUT_NAME, menuItemId, descriptors.size() > index);
			if (descriptors.size() > index)
			{
				auto& descriptor = descriptors[index];
				if (game::avatar::Items::HasItems(descriptor.inputs))
				{
					visuals::MenuItems::SetText(LAYOUT_NAME, menuItemId, descriptors[index].name);
				}
				else
				{
					visuals::MenuItems::SetText(LAYOUT_NAME, menuItemId, descriptors[index].notAvailable);
				}
			}
		}
	}

	static void OnEnter()
	{
		auto position = game::Avatar::GetPosition();
		auto shoppe = game::Shoppes::Read(position);
		if (shoppe)
		{
			auto& descriptor = game::shoppe::GetDescriptor(*shoppe);
			visuals::Texts::SetText(LAYOUT_NAME, TEXT_TITLE, descriptor.name);

			UpdateTradeMenuItemTexts(descriptor.trades);

		}
	}

	const std::map<std::string, std::optional<TradeItem>> areaMenuItems =
	{
		{ AREA_UI_CLOSE, std::nullopt},
		{ AREA_TRADE_1, TradeItem::TRADE_1},
		{ AREA_TRADE_2, TradeItem::TRADE_2},
		{ AREA_TRADE_3, TradeItem::TRADE_3}
	};

	static void SetCurrentMenuItem(TradeItem item)
	{
		visuals::Menus::WriteIndex(LAYOUT_NAME, MENU_ID, (int)item);
	}

	static void MouseMotionInArea(const std::string& area, const common::XY<Sint32>&)
	{
		auto& menuItem = areaMenuItems.find(area)->second;
		if (menuItem)
		{
			visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_CLOSE, SPRITE_NORMAL);
			SetCurrentMenuItem(*menuItem);
		}
		else
		{
			visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_CLOSE, SPRITE_HOVER);
		}
	}

	static void MouseMotionOutsideAreas(const common::XY<Sint32>&)
	{
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_UI_CLOSE, SPRITE_NORMAL);
	}

	static void ActivateItem()
	{
		auto index = visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID);
		CompleteTrade((size_t)*index);
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_MAP) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_MAP) }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_TRADE, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_TRADE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(::UIState::IN_PLAY_TRADE, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_TRADE, LAYOUT_NAME);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_TRADE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, MouseMotionInArea, MouseMotionOutsideAreas));
	}
}