#include "Graphics.Sprites.h"
#include "Graphics.Textures.h"
#include "Common.Data.Properties.h"
#include "Graphics.Data.Properties.h"
#include "Data.Stores.h"
namespace graphics::Sprites
{
	nlohmann::json NameToData(const std::string name)
	{
		if (::data::Stores::GetStore(::data::Store::SPRITES).count(name) > 0)
		{
			return ::data::Stores::GetStore(::data::Store::SPRITES)[name];
		}
		else
		{
			return nlohmann::json();
		}
	}

	void Draw(const std::string& spriteName, std::shared_ptr<SDL_Renderer> renderer, const ::common::XY<int>& xy, const SDL_Color& color)
	{
		auto model = NameToData(spriteName);
		if (model.type() != nlohmann::json::value_t::null)
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
	}

	std::optional<int> GetWidth(const std::string& spriteName)
	{
		auto model = NameToData(spriteName);
		if (model.type() != nlohmann::json::value_t::null)
		{
			return model[common::data::Properties::WIDTH];
		}
		return std::nullopt;
	}
}
