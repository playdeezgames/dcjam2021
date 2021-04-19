#include "Graphics.Textures.h"
#include "Common.Utility.h"
#include <SDL_image.h>
#include "Data.JSON.h"
#include <map>
#include <memory>
namespace graphics::Textures
{
	static std::map<std::string, std::shared_ptr<SDL_Texture>> textures;

	static void Add(const std::string& name, std::shared_ptr<SDL_Texture> texture)
	{
		textures[name] = texture;
	}

	void InitializeFromFile(std::shared_ptr<SDL_Renderer> renderer, const std::string& fileName)
	{
		auto properties = data::JSON::Load(fileName);
		for (auto& entry : properties.items())
		{
			std::string imageFileName = entry.value();
			Add(entry.key(), std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer.get(), imageFileName.c_str()), SDL_DestroyTexture));
		}
	}

	std::shared_ptr<SDL_Texture> Read(std::shared_ptr<SDL_Renderer>, const std::string& name)
	{
		auto iter = textures.find(name);
		if (iter != textures.end())
		{
			return iter->second;
		}
		return nullptr;
	}
}