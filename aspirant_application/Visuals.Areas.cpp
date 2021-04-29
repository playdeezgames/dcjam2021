#include "Visuals.Areas.h"
#include "json.hpp"
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
namespace graphics::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace graphics::Areas
{
	std::set<std::string> Get(const std::string& layoutName, const common::XY<int>& xy)
	{
		std::set<std::string> result;
		for (auto& thingie : graphics::Layouts::GetLayout(layoutName))
		{
			if (graphics::data::Types::FromString(thingie[common::data::Properties::TYPE]) == graphics::data::Type::AREA)
			{
				int x = thingie[common::data::Properties::X];
				int y = thingie[common::data::Properties::Y];
				int width = thingie[common::data::Properties::WIDTH];
				int height = thingie[common::data::Properties::HEIGHT];
				if (xy.GetX() >= x && xy.GetY() >= y && xy.GetX() < x + width && xy.GetY() < y + height)
				{
					std::string areaId = thingie[graphics::data::Properties::AREA_ID];
					result.insert(areaId);
				}
			}
		}
		return result;
	}
}