#include "Graphics.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Graphics.Data.Properties.h"
#include "Graphics.Layout.h"
#include "Data.Stores.h"
namespace graphics::Layouts
{
	std::map<std::string, nlohmann::json> layouts;

	void InitializeLayout(const std::string& layoutName)
	{
		if (!layouts.contains(layoutName))
		{
			layouts[layoutName] = ::data::JSON::Load(::data::Stores::GetStore(::data::Store::LAYOUTS)[layoutName]);
		}
	}

	nlohmann::json& GetLayout(const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		return layouts.find(layoutName)->second;
	}

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const std::string& layoutName)
	{
		InitializeLayout(layoutName);
		graphics::Layout::Draw(renderer, layouts[layoutName]);
	}
}