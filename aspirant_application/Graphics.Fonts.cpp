#include "Graphics.Fonts.h"
#include "Data.JSON.h"
#include "Graphics.Sprites.h"
#include <sstream>
#include "Graphics.Colors.h"
namespace graphics
{
	class Font
	{
	private:
		const nlohmann::json& model;
		std::optional<std::string> GetGlyphSprite(char) const;
		void WriteTextCentered(std::shared_ptr<SDL_Renderer>, const common::XY<int>&, const std::string&, const std::string&) const;
		common::XY<int> WriteTextLeft(std::shared_ptr<SDL_Renderer>, const common::XY<int>&, const std::string&, const std::string&) const;
		void WriteTextRight(std::shared_ptr<SDL_Renderer>, const common::XY<int>&, const std::string&, const std::string&) const;
		common::XY<int> WriteGlyph(std::shared_ptr<SDL_Renderer>, const common::XY<int>&, char, const std::string&) const;
	public:
		Font(const nlohmann::json&);
		void WriteText(std::shared_ptr<SDL_Renderer>, const common::XY<int>&, const std::string&, const std::string&, const HorizontalAlignment&) const;
	};

	Font::Font
	(
		const nlohmann::json& model
	)
		: model(model)
	{
	}

	std::optional<std::string> Font::GetGlyphSprite(char ch) const
	{
		std::stringstream ss;
		ss << (int)ch;
		if (model.count(ss.str()) > 0)
		{
			return model[ss.str()];
		}
		else
		{
			return std::nullopt;
		}
	}

	common::XY<int> Font::WriteGlyph(std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, char ch, const std::string& color) const
	{
		auto sprite = GetGlyphSprite(ch);
		Sprites::Draw(sprite.value(), renderer, xy, ::graphics::Colors::Read(color));
		return common::XY(xy.GetX() + Sprites::GetWidth(sprite.value()), xy.GetY());
	}

	common::XY<int> Font::WriteTextLeft(std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, const std::string& text, const std::string& color) const
	{
		common::XY<int> temp = xy;
		for (auto ch : text)
		{
			temp = WriteGlyph(renderer, temp, ch, color);
		}
		return temp;
	}

	void Font::WriteTextCentered(std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, const std::string& text, const std::string& color) const
	{
		int width = 0;
		for (auto ch : text)
		{
			const auto& sprite = GetGlyphSprite(ch);
			width += Sprites::GetWidth(sprite.value());
		}
		auto adjustedXY = common::XY<int>(xy.GetX() - width / 2, xy.GetY());
		WriteTextLeft(renderer, adjustedXY, text, color);
	}

	void Font::WriteTextRight(std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, const std::string& text, const std::string& color) const
	{
		int width = 0;
		for (auto ch : text)
		{
			const auto& sprite = GetGlyphSprite(ch);
			width += Sprites::GetWidth(sprite.value());
		}
		auto adjustedXY = common::XY<int>(xy.GetX() - width, xy.GetY());
		WriteTextLeft(renderer, adjustedXY, text, color);
	}

	void Font::WriteText
	(
		std::shared_ptr<SDL_Renderer> renderer,
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
namespace graphics::Fonts
{
	static std::map<std::string, nlohmann::json> table;

	static void ParseDescriptor(const std::string& identifier, const nlohmann::json& properties)
	{
		table[identifier] = data::JSON::Load(properties);
	}

	static std::optional<graphics::Font> Read(const std::string& key)
	{
		if (table.count(key) > 0)
		{
			return Font(table[key]);
		}
		else
		{
			return std::nullopt;
		}
	}

	void InitializeFromFile(const std::string& fileName)
	{
		auto properties = data::JSON::Load(fileName);
		for (auto& item : properties.items())
		{
			ParseDescriptor(item.key(), item.value());
		}
	}

	void WriteText(const std::string& fontname, std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, const std::string& text, const std::string& color, const HorizontalAlignment& alignment)
	{
		auto font = Read(fontname);
		if (font)
		{
			font.value().WriteText(renderer, xy, text, color, alignment);
		}
	}
}