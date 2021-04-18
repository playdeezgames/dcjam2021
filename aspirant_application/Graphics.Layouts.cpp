#include "Graphics.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Graphics.Data.Properties.h"
#include "Graphics.Layout.h"
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
		table = ::data::JSON::Load(fileName);
		for (auto& item : table.items())
		{
			layouts[item.key()] = ::data::JSON::Load(item.value());
		}
	}

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const std::string& layoutName)
	{
		graphics::Layout::Draw(renderer, layouts[layoutName]);
	}
}