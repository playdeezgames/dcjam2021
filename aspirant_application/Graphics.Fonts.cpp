#include "Graphics.Fonts.h"
#include "Data.JSON.h"
#include "Graphics.Sprites.h"
#include <sstream>
#include "Graphics.Colors.h"
#include "Data.Stores.h"
namespace graphics::Fonts
{
	static std::map<std::string, nlohmann::json> table;

	static void InitializeFont(const std::string& name)
	{
		if (!table.contains(name))
		{
			if (::data::Stores::GetStore(::data::Store::FONTS).count(name) > 0)
			{
				table[name] = data::JSON::Load(::data::Stores::GetStore(::data::Store::FONTS)[name]);
			}
		}
	}

	const nlohmann::json empty = nlohmann::json();

	static const nlohmann::json& NameToData(const std::string name)
	{
		if (::data::Stores::GetStore(::data::Store::FONTS).count(name) > 0)
		{
			InitializeFont(name);
			return table[name];
		}
		else
		{
			return empty;
		}
	}

	static std::optional<std::string> GetGlyphSprite(const std::string& fontname, char ch)
	{
		std::stringstream ss;
		ss << (int)ch;
		if (NameToData(fontname).count(ss.str()) > 0)
		{
			return NameToData(fontname)[ss.str()];
		}
		else
		{
			return std::nullopt;
		}
	}

	static common::XY<int> WriteGlyph(const std::string& fontname, std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, char ch, const std::string& color)
	{
		auto sprite = GetGlyphSprite(fontname, ch);
		Sprites::Draw(sprite.value(), renderer, xy, ::graphics::Colors::Read(color));
		return common::XY(xy.GetX() + Sprites::GetWidth(sprite.value()).value_or(0), xy.GetY());
	}

	void WriteTextLeft(const std::string& fontname, std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, const std::string& text, const std::string& color)
	{
		common::XY<int> temp = xy;
		for (auto ch : text)
		{
			temp = WriteGlyph(fontname, renderer, temp, ch, color);
		}
	}

	static int GetTextWidth(const std::string& fontname, const std::string& text)
	{
		int width = 0;
		for (auto ch : text)
		{
			const auto& sprite = GetGlyphSprite(fontname, ch);
			width += Sprites::GetWidth(sprite.value()).value_or(0);
		}
		return width;
	}

	static void WriteTextCentered(const std::string& fontname, std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, const std::string& text, const std::string& color)
	{
		auto adjustedXY = common::XY<int>(xy.GetX() - GetTextWidth(fontname, text) / 2, xy.GetY());
		WriteTextLeft(fontname, renderer, adjustedXY, text, color);
	}

	static void WriteTextRight(const std::string& fontname, std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, const std::string& text, const std::string& color)
	{
		auto adjustedXY = common::XY<int>(xy.GetX() - GetTextWidth(fontname, text), xy.GetY());
		WriteTextLeft(fontname, renderer, adjustedXY, text, color);
	}

	typedef std::function<void(const std::string&, std::shared_ptr<SDL_Renderer>, const common::XY<int>&, const std::string&, const std::string&)> WriteFunction;

	const std::map<HorizontalAlignment, WriteFunction> writers =
	{
		{HorizontalAlignment::LEFT, WriteTextLeft},
		{HorizontalAlignment::RIGHT, WriteTextRight},
		{HorizontalAlignment::CENTER, WriteTextCentered}
	};

	void WriteText(const std::string& fontname, std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, const std::string& text, const std::string& color, const HorizontalAlignment& alignment)
	{
		writers.find(alignment)->second(fontname, renderer, xy, text, color);
	}
}