#include "Visuals.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Fonts.h"
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
		if (model.count(visuals::data::Properties::TEXT_ID) > 0)
		{
			textTable[layoutName][model[visuals::data::Properties::TEXT_ID]] = textIndex;
		}
		return [textIndex](std::shared_ptr<SDL_Renderer> renderer) 
		{
			DrawInternalText(renderer, textIndex);
		};
	}
}

namespace visuals::Texts
{
	void SetText(const std::string& layoutName, const std::string& textId, const std::string& text)
	{
		auto textIndex = visuals::Text::textTable[layoutName][textId];
		visuals::Text::internalTexts[textIndex].text = text;
	}

	void SetColor(const std::string& layoutName, const std::string& textId, const std::string& color)
	{
		auto textIndex = visuals::Text::textTable[layoutName][textId];
		visuals::Text::internalTexts[textIndex].color = color;
	}
}