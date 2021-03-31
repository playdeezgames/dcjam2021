#include "Graphics.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Types.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
namespace graphics::Layout { void Draw(SDL_Renderer*, const nlohmann::json&); }
namespace graphics::Layouts
{
	std::map<std::string, nlohmann::json> layouts;
	static nlohmann::json table;

	void Start(const std::string& fileName)
	{
		table = data::JSON::Load(fileName);
		for (auto& item : table.items())
		{
			layouts[item.key()] = data::JSON::Load(item.value());
		}
	}

	void Draw(SDL_Renderer* renderer, const std::string& layoutName)
	{
		graphics::Layout::Draw(renderer, layouts[layoutName]);
	}
}