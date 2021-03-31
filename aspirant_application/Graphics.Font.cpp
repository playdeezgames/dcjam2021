#include "Graphics.Font.h"
#include "Common.Utility.h"
#include "Graphics.Sprites.h"
#include "Graphics.Colors.h"
#include <sstream>
namespace graphics
{
	Font::Font
	(
		const nlohmann::json& model
	)
		: model(model)
	{
	}

	std::optional<graphics::Sprite> Font::GetGlyphSprite(char ch) const
	{
		std::stringstream ss;
		ss << (int)ch;
		if (model.count(ss.str()) > 0)
		{
			return Sprites::Read(model[ss.str()]);
		}
		else
		{
			return std::nullopt;
		}
	}

	common::XY<int> Font::WriteGlyph(SDL_Renderer* renderer, const common::XY<int>& xy, char ch, const std::string& color) const
	{
		const auto& sprite = GetGlyphSprite(ch);
		sprite.value().Draw(renderer, xy, ::graphics::Colors::Get(color));
		return common::XY(xy.GetX() + sprite.value().GetWidth(), xy.GetY());
	}

	common::XY<int> Font::WriteTextLeft(SDL_Renderer* renderer, const common::XY<int>& xy, const std::string& text, const std::string& color) const
	{
		common::XY<int> temp = xy;
		for (auto ch : text)
		{
			temp = WriteGlyph(renderer, temp, ch, color);
		}
		return temp;
	}

	void Font::WriteTextCentered(SDL_Renderer* renderer, const common::XY<int>& xy, const std::string& text, const std::string& color) const
	{
		int width = 0;
		for (auto ch : text)
		{
			const auto& sprite = GetGlyphSprite(ch);
			width += sprite.value().GetWidth();
		}
		auto adjustedXY = common::XY<int>(xy.GetX() - width / 2, xy.GetY());
		WriteTextLeft(renderer, adjustedXY, text, color);
	}

	void Font::WriteTextRight(SDL_Renderer* renderer, const common::XY<int>& xy, const std::string& text, const std::string& color) const
	{
		int width = 0;
		for (auto ch : text)
		{
			const auto& sprite = GetGlyphSprite(ch);
			width += sprite.value().GetWidth();
		}
		auto adjustedXY = common::XY<int>(xy.GetX() - width, xy.GetY());
		WriteTextLeft(renderer, adjustedXY, text, color);
	}

	void Font::WriteText
	(
		SDL_Renderer* renderer,
		const common::XY<int>& xy,
		const std::string& text,
		const std::string& color,
		const HorizontalAlignment& alignment
	) const
	{
		switch (alignment)
		{
		case HorizontalAlignment::LEFT:
			WriteTextLeft(renderer, xy, text, color);
			break;
		case HorizontalAlignment::RIGHT:
			WriteTextRight(renderer, xy, text, color);
			break;
		case HorizontalAlignment::CENTER:
			WriteTextCentered(renderer, xy, text, color);
			break;
		}
	}
}
