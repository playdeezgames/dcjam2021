#include "Graphics.Sprites.h"
#include "Common.Utility.h"
#include "Data.JSON.h"
#include "Graphics.Textures.h"
#include "Common.Data.Properties.h"
#include "Graphics.Data.Properties.h"
#include "Data.Stores.h"
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
		auto texture = graphics::Textures::Read(renderer, model[graphics::data::Properties::TEXTURE]);
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
			xy.GetX() + ((model.count(data::Properties::OFFSET_X) > 0) ? ((int)model[data::Properties::OFFSET_X]) : (0)),
			xy.GetY() + ((model.count(data::Properties::OFFSET_Y) > 0) ? ((int)model[data::Properties::OFFSET_Y]) : (0)),
			model[common::data::Properties::WIDTH],
			model[common::data::Properties::HEIGHT]
		};
		SDL_Rect source =
		{
			model[common::data::Properties::X],
			model[common::data::Properties::Y],
			model[common::data::Properties::WIDTH],
			model[common::data::Properties::HEIGHT]
		};
		SDL_RenderCopy(renderer.get(), texture.get(), &source, &rcDst);
	}

	int Sprite::GetWidth() const
	{
		return model[common::data::Properties::WIDTH];
	}
}
namespace graphics::Sprites
{
	static std::map<std::string, Sprite> sprites;

	std::optional<Sprite> Read(const std::string& name)
	{
		if (::data::Stores::GetStore(::data::Store::SPRITES).count(name) > 0)
		{
			return Sprite(::data::Stores::GetStore(::data::Store::SPRITES)[name]);
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

	std::optional<int> GetWidth(const std::string& spriteName)
	{
		auto sprite = Read(spriteName);
		if (sprite)
		{
			return sprite.value().GetWidth();
		}
		return std::nullopt;
	}
}
