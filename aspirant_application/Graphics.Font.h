#pragma once
#include "Graphics.Sprite.h"
#include <string>
#include <map>
#include "Common.XY.h"
#include <optional>
#include "json.hpp"
namespace graphics
{
	enum class HorizontalAlignment
	{
		RIGHT = -1,
		CENTER = 0,
		LEFT = 1
	};
	class Font
	{
	private:
		const nlohmann::json& model;
		std::optional<graphics::Sprite> GetGlyphSprite(char) const;
		void WriteTextCentered(SDL_Renderer*, const common::XY<int>&, const std::string&, const std::string&) const;
		common::XY<int> WriteTextLeft(SDL_Renderer*, const common::XY<int>&, const std::string&, const std::string&) const;
		void WriteTextRight(SDL_Renderer*, const common::XY<int>&, const std::string&, const std::string&) const;
		common::XY<int> WriteGlyph(SDL_Renderer*, const common::XY<int>&, char, const std::string&) const;
	public:
		Font
		(
			const nlohmann::json&
		);
		void WriteText(SDL_Renderer*, const common::XY<int>&, const std::string&, const std::string&, const HorizontalAlignment&) const;
	};
}

