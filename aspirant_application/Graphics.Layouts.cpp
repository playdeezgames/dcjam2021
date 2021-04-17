#include "Graphics.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Types.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
//TODO: dont like this here
namespace graphics::Layout { void Draw(std::shared_ptr<SDL_Renderer>, const nlohmann::json&); }
namespace graphics::Layouts
{
	std::map<std::string, nlohmann::json> layouts;

	nlohmann::json& GetLayout(const std::string& layoutName)
	{
		return layouts.find(layoutName)->second;
	}

	static nlohmann::json table;

	void InitializeFromFile(const std::string& fileName)
	{
		table = data::JSON::Load(fileName);
		for (auto& item : table.items())
		{
			layouts[item.key()] = data::JSON::Load(item.value());
		}
	}

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const std::string& layoutName)
	{
		graphics::Layout::Draw(renderer, layouts[layoutName]);
	}
}