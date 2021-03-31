#include "Graphics.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Types.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
namespace graphics::Layouts
{
	extern std::map<std::string, nlohmann::json> layouts;
}
namespace graphics::Texts
{
	void SetTextText(const std::string& layoutName, const std::string& textId, const std::string& text)
	{
		for (auto& thingie : graphics::Layouts::layouts[layoutName])
		{
			if (thingie[common::Properties::TYPE] == graphics::Types::TEXT)
			{
				if (thingie.count(graphics::Properties::TEXT_ID) > 0 &&
					thingie[graphics::Properties::TEXT_ID] == textId)
				{
					thingie[graphics::Properties::TEXT] = text;
				}
			}
		}
	}
}