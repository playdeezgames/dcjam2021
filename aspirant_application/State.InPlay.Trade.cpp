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
#include <sstream>
#include "Game.Audio.Mux.h"
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

	const std::vector<std::string> menuItemIds =
	{
		MENU_ITEM_TRADE_1,
		MENU_ITEM_TRADE_2,
		MENU_ITEM_TRADE_3
	};

	static void UpdateTradeMenuItemTexts()
	{
		auto shoppeIndex = *game::Shoppes::Read(game::Avatar::GetPosition());
		auto& descriptors = game::shoppe::GetDescriptor(shoppeIndex).trades;
		for (size_t tradeIndex = 0; tradeIndex < MENU_ITEM_COUNT; tradeIndex++)
		{
			const std::string menuItemId = menuItemIds[tradeIndex];
			visuals::MenuItems::SetEnabled(LAYOUT_NAME, menuItemId, descriptors.size() > tradeIndex);
			if (descriptors.size() > tradeIndex)
			{
				auto& descriptor = descriptors[tradeIndex];
				if (game::shoppe::HasStock(shoppeIndex, tradeIndex))
				{
					visuals::MenuItems::SetText(LAYOUT_NAME, menuItemId, descriptors[tradeIndex].name);
				}
				else
				{
					visuals::MenuItems::SetText(LAYOUT_NAME, menuItemId, descriptors[tradeIndex].outOfStock);
				}
			}
		}
	}

	const std::vector<std::string> costTextIds =
	{
		"Cost1",
		"Cost2",
		"Cost3"
	};

	static void UpdateCosts()
	{
		auto& inputs = game::shoppe::GetDescriptor(*game::Shoppes::Read(game::Avatar::GetPosition())).trades[visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value_or(0)].inputs;
		auto iter = costTextIds.begin();
		for (auto& input : inputs)
		{
			std::stringstream ss;
			auto& itemDescriptor = game::item::GetDescriptor(input.first);
			ss << itemDescriptor.name << " x" << input.second;
			visuals::Texts::SetText(LAYOUT_NAME, *iter, ss.str());
			if (game::avatar::Items::Read(input.first) >= input.second)
			{
				visuals::Texts::SetColor(LAYOUT_NAME, *iter, "Gray");
			}
			else
			{
				visuals::Texts::SetColor(LAYOUT_NAME, *iter, "Red");
			}
			iter++;
			if (iter == costTextIds.end())
			{
				break;
			}
		}
		while (iter != costTextIds.end())
		{
			visuals::Texts::SetText(LAYOUT_NAME, *iter, "");
			++iter;
		}
	}

	const std::vector<std::string> benefitTextIds =
	{
		"Benefit1",
		"Benefit2",
		"Benefit3"
	};

	static void UpdateBenefits()
	{
		auto shoppeIndex = *game::Shoppes::Read(game::Avatar::GetPosition());
		auto tradeIndex = visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value_or(0);
		auto& outputs = game::shoppe::GetDescriptor(shoppeIndex).trades[tradeIndex].outputs;
		auto iter = benefitTextIds.begin();
		for (auto& output : outputs)
		{
			std::stringstream ss;
			auto& itemDescriptor = game::item::GetDescriptor(output.first);
			ss << itemDescriptor.name << " x" << output.second;
			if (game::shoppe::HasStock(shoppeIndex, tradeIndex))
			{
				visuals::Texts::SetColor(LAYOUT_NAME, *iter, "Gray");
			}
			else
			{
				visuals::Texts::SetColor(LAYOUT_NAME, *iter, "Red");
			}
			visuals::Texts::SetText(LAYOUT_NAME, *iter, ss.str());
			iter++;
			if (iter == benefitTextIds.end())
			{
				break;
			}
		}
		while (iter != benefitTextIds.end())
		{
			visuals::Texts::SetText(LAYOUT_NAME, *iter, "");
			++iter;
		}
	}

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		auto iter = mouseButtonUpAreaHandlers.find(area);
		if (iter != mouseButtonUpAreaHandlers.end())
		{
			UpdateCosts();
			UpdateBenefits();
			return iter->second();
		}
		return false;
	}

	static void UpdateTitle()
	{
		auto& descriptor = game::shoppe::GetDescriptor(*game::Shoppes::Read(game::Avatar::GetPosition()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_TITLE, descriptor.name);
	}

	static void OnEnter()
	{
		auto position = game::Avatar::GetPosition();
		if (game::Shoppes::Read(position))
		{
			UpdateTitle();
			UpdateTradeMenuItemTexts();
			UpdateCosts();
			UpdateBenefits();
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
		UpdateCosts();
		UpdateBenefits();
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
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_TRADE, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_TRADE, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_TRADE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(::UIState::IN_PLAY_TRADE, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_TRADE, LAYOUT_NAME);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_TRADE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, MouseMotionInArea, MouseMotionOutsideAreas));
	}
}