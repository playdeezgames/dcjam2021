#include "Graphics.Sprites.h"
#include "Common.Utility.h"
#include "Data.JSON.h"
#include "Graphics.Textures.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
namespace graphics
{
	class Sprite
	{
	private:
		const nlohmann::json& model;
	public:
		Sprite(const nlohmann::json&);
		void Draw(std::shared_ptr<SDL_Renderer>, const ::common::XY<int>&, const SDL_Color&) const;
		void Draw(std::shared_ptr<SDL_Renderer>, const ::common::XY<int>&) const;
		int GetWidth() const;
	};
	Sprite::Sprite
	(
		const nlohmann::json& model
	)
		: model(model)
	{

	}

	void Sprite::Draw(std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy) const
	{
		Draw(renderer, xy, { 255,255,255,255 });
	}


	void Sprite::Draw(std::shared_ptr<SDL_Renderer> renderer, const common::XY<int>& xy, const SDL_Color& color) const
	{
		auto texture = graphics::Textures::Read(model[graphics::Properties::TEXTURE]);
		SDL_SetTextureColorMod
		(
			texture.get(),
			color.r,
			color.g,
			color.b
		);
		SDL_SetTextureAlphaMod
		(
			texture.get(),
			color.a
		);
		SDL_Rect rcDst =
		{
			xy.GetX() + ((model.count(Properties::OFFSET_X) > 0) ? ((int)model[Properties::OFFSET_X]) : (0)),
			xy.GetY() + ((model.count(Properties::OFFSET_Y) > 0) ? ((int)model[Properties::OFFSET_Y]) : (0)),
			model[common::Properties::WIDTH],
			model[common::Properties::HEIGHT]
		};
		SDL_Rect source =
		{
			model[common::Properties::X],
			model[common::Properties::Y],
			model[common::Properties::WIDTH],
			model[common::Properties::HEIGHT]
		};
		SDL_RenderCopy(renderer.get(), texture.get(), &source, &rcDst);
	}

	int Sprite::GetWidth() const
	{
		return model[common::Properties::WIDTH];
	}
}
namespace graphics::Sprites
{
	static std::map<std::string, Sprite> sprites;
	static nlohmann::json table;

	std::optional<Sprite> Read(const std::string& name)
	{
		if (table.count(name) > 0)
		{
			return Sprite(table[name]);
		}
		else
		{
			return std::nullopt;
		}
	}

	void Draw(const std::string& spriteName, std::shared_ptr<SDL_Renderer> renderer, const ::common::XY<int>& xy, const SDL_Color& color)
	{
		auto sprite = Read(spriteName);
		if (sprite)
		{
			sprite.value().Draw(renderer, xy, color);
		}
	}

	void Draw(const std::string& spriteName, std::shared_ptr<SDL_Renderer> renderer, const ::common::XY<int>& xy)
	{
		auto sprite = Read(spriteName);
		if (sprite)
		{
			sprite.value().Draw(renderer, xy);
		}
	}

	int GetWidth(const std::string& spriteName)
	{
		auto sprite = Read(spriteName);
		if (sprite)
		{
			return sprite.value().GetWidth();
		}
		return 0;
	}

	void InitializeFromFile(const std::string& fileName)
	{
		table = data::JSON::Load(fileName);
	}
}
