#include "Visuals.Sprites.h"
#include "Visuals.Textures.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Data.Stores.h"
namespace visuals::Sprites
{
	struct InternalSprite
	{
		std::string textureName;
		SDL_Rect destination;
		SDL_Rect source;
	};

	static std::map<std::string, InternalSprite> internalSprites;

	static void Initialize()
	{
		static bool initialized = false;
		if (!initialized)
		{
			auto& spriteStore = ::data::Stores::GetStore(::data::Store::SPRITES);
			for (auto& sprite : spriteStore.items())
			{
				auto& model = sprite.value();
				std::string textureName = model[visuals::data::Properties::TEXTURE];
				SDL_Rect destination = 
				{
					((model.count(data::Properties::OFFSET_X) > 0) ? ((int)model[data::Properties::OFFSET_X]) : (0)),
					((model.count(data::Properties::OFFSET_Y) > 0) ? ((int)model[data::Properties::OFFSET_Y]) : (0)),
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
				internalSprites[sprite.key()] =
				{
					textureName,
					destination,
					source
				};
			}
			initialized = true;
		}
	}

	void Draw(const std::string& spriteName, const std::shared_ptr<SDL_Renderer>& renderer, const ::common::XY<int>& xy, const SDL_Color& color)
	{
		Initialize();
		auto iter = internalSprites.find(spriteName);
		if (iter != internalSprites.end())
		{
			auto& interalSprite = iter->second;
			auto texture = visuals::Textures::Read(renderer, interalSprite.textureName);
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
			interalSprite.destination.x += xy.GetX();
			interalSprite.destination.y += xy.GetY();
			SDL_RenderCopy(renderer.get(), texture.get(), &interalSprite.source, &interalSprite.destination);
			interalSprite.destination.x -= xy.GetX();
			interalSprite.destination.y -= xy.GetY();
		}
	}

	std::optional<int> GetWidth(const std::string& spriteName)
	{
		Initialize();
		auto iter = internalSprites.find(spriteName);
		if (iter != internalSprites.end())
		{
			return iter->second.destination.w;
		}
		return std::nullopt;
	}
}
