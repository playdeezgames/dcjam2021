#include "Visuals.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Fonts.h"

namespace visuals::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}

namespace visuals::Text
{
	struct InternalText
	{
		std::string font;
		std::string text;
		std::string color;
		common::XY<int> xy;
		HorizontalAlignment alignment;
		bool dropShadow;
		std::string dropShadowColor;
		common::XY<int> dropShadowXY;
	};

	static std::vector<InternalText> internalTexts;
	static std::map<std::string, std::map<std::string, size_t>> textTable;

	static void DrawInternalText(std::shared_ptr<SDL_Renderer> renderer, size_t textIndex)
	{
		auto& text = internalTexts[textIndex];
		if (text.dropShadow)
		{
			visuals::Fonts::WriteText(
				text.font,
				renderer,
				text.xy + text.dropShadowXY,
				text.text,
				text.dropShadowColor,
				text.alignment);
		}
		visuals::Fonts::WriteText(
			text.font,
			renderer,
			text.xy,
			text.text,
			text.color,
			text.alignment);
	}

	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t textIndex = internalTexts.size();
		internalTexts.push_back(
			{
				model[visuals::data::Properties::FONT],
				model[visuals::data::Properties::TEXT],
				model[visuals::data::Properties::COLOR],
				common::XY<int>(
					model[common::data::Properties::X],
					model[common::data::Properties::Y]),
				(HorizontalAlignment)(int)model[visuals::data::Properties::HORIZONTAL_ALIGNMENT],
				(bool)model[visuals::data::Properties::DROP_SHADOW],
				model[visuals::data::Properties::DROP_SHADOW_COLOR],
				common::XY<int>(
					model[visuals::data::Properties::DROP_SHADOW_X],
					model[visuals::data::Properties::DROP_SHADOW_Y])
			});
		return [textIndex](std::shared_ptr<SDL_Renderer> renderer) 
		{
			DrawInternalText(renderer, textIndex);
		};
	}

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		if ((bool)model[visuals::data::Properties::DROP_SHADOW])
		{
			visuals::Fonts::WriteText(
				model[visuals::data::Properties::FONT],
				renderer,
				common::XY<int>
				(
					(int)model[common::data::Properties::X] + (int)model[visuals::data::Properties::DROP_SHADOW_X],
					(int)model[common::data::Properties::Y] + (int)model[visuals::data::Properties::DROP_SHADOW_Y]
					),
				model[visuals::data::Properties::TEXT],
				model[visuals::data::Properties::DROP_SHADOW_COLOR],
				(HorizontalAlignment)(int)model[visuals::data::Properties::HORIZONTAL_ALIGNMENT]);
		}
		visuals::Fonts::WriteText(
			model[visuals::data::Properties::FONT],
			renderer,
			common::XY<int>
			(
				model[common::data::Properties::X],
				model[common::data::Properties::Y]
				),
			model[visuals::data::Properties::TEXT],
			model[visuals::data::Properties::COLOR],
			(HorizontalAlignment)(int)model[visuals::data::Properties::HORIZONTAL_ALIGNMENT]);
	}
}

namespace visuals::Texts
{
	template <typename TResult>
	static TResult WithText(const std::string& layoutName, const std::string& textId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : visuals::Layouts::GetLayout(layoutName))
		{
			if (visuals::data::Types::FromString(thingie[common::data::Properties::TYPE]) == visuals::data::Type::TEXT)
			{
				if (thingie.count(visuals::data::Properties::TEXT_ID) > 0 &&
					thingie[visuals::data::Properties::TEXT_ID] == textId)
				{
					return func(thingie);
				}
			}
		}
		return notFound();
	}

	void SetText(const std::string& layoutName, const std::string& textId, const std::string& text)
	{
		WithText<void>(layoutName, textId, [text](auto& thingie) 
		{
			thingie[visuals::data::Properties::TEXT] = text;
		}, []() {});
	}

	void SetColor(const std::string& layoutName, const std::string& textId, const std::string& color)
	{
		WithText<void>(layoutName, textId, [color](auto& thingie)
		{
			thingie[visuals::data::Properties::COLOR] = color;
		}, []() {});
	}
}