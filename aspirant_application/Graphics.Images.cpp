#include "Graphics.Images.h"
#include <map>
#include "json.hpp"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
#include "Graphics.Types.h"
namespace graphics::Layouts
{
	extern std::map<std::string, nlohmann::json> layouts;
}
namespace graphics::Images
{
	void SetSprite(const std::string& layoutName, const std::string& imageId, const std::string& spriteName)
	{
		for (auto& thingie : graphics::Layouts::layouts[layoutName])
		{
			if (thingie[common::Properties::TYPE] == graphics::Types::IMAGE)
			{
				if (thingie.count(graphics::Properties::IMAGE_ID) > 0 &&
					thingie[graphics::Properties::IMAGE_ID] == imageId)
				{
					thingie[graphics::Properties::SPRITE] = spriteName;
				}
			}
		}
	}

	void SetVisible(const std::string& layoutName, const std::string& imageId, bool visible)
	{
		for (auto& thingie : graphics::Layouts::layouts[layoutName])
		{
			if (thingie[common::Properties::TYPE] == graphics::Types::IMAGE)
			{
				if (thingie.count(graphics::Properties::IMAGE_ID) > 0 &&
					thingie[graphics::Properties::IMAGE_ID] == imageId)
				{
					thingie[graphics::Properties::VISIBLE] = visible;
				}
			}
		}
	}
}