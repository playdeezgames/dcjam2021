#include "Visuals.Textures.h"
#include "Common.Utility.h"
#include <SDL_image.h>
#include "Data.JSON.h"
#include <map>
#include <memory>
#include "Data.Stores.h"
namespace visuals::Textures
{
	static std::map<std::string, std::shared_ptr<SDL_Texture>> textures;

	static void Add(const std::string& name, std::shared_ptr<SDL_Texture> texture)
	{
		textures[name] = texture;
	}

	static bool initialized = false;

	static void Initialize(std::shared_ptr<SDL_Renderer> renderer)
	{
		if (!initialized)
		{
			auto& properties = data::Stores::GetStore(data::Store::TEXTURES);
			for (auto& entry : properties.items())
			{
				std::string imageFileName = entry.value();
				Add(entry.key(), std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer.get(), imageFileName.c_str()), SDL_DestroyTexture));
			}
			initialized = true;
		}
	}

	std::shared_ptr<SDL_Texture> Read(std::shared_ptr<SDL_Renderer> renderer, const std::string& name)
	{
		Initialize(renderer);
		auto iter = textures.find(name);
		if (iter != textures.end())
		{
			return iter->second;
		}
		return nullptr;
	}
}