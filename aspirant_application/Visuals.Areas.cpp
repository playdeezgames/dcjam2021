#include "Visuals.Areas.h"
#include "json.hpp"
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
namespace visuals::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace visuals::Areas
{
	std::set<std::string> Get(const std::string& layoutName, const common::XY<int>& xy)
	{
		std::set<std::string> result;
		for (auto& thingie : visuals::Layouts::GetLayout(layoutName))
		{
			if (visuals::data::Types::FromString(thingie[common::data::Properties::TYPE]) == visuals::data::Type::AREA)
			{
				int x = thingie[common::data::Properties::X];
				int y = thingie[common::data::Properties::Y];
				int width = thingie[common::data::Properties::WIDTH];
				int height = thingie[common::data::Properties::HEIGHT];
				if (xy.GetX() >= x && xy.GetY() >= y && xy.GetX() < x + width && xy.GetY() < y + height)
				{
					std::string areaId = thingie[visuals::data::Properties::AREA_ID];
					result.insert(areaId);
				}
			}
		}
		return result;
	}

	Area Get(const std::string& layoutName, const std::string& areaName)
	{
		for (auto& thingie : visuals::Layouts::GetLayout(layoutName))
		{
			if (visuals::data::Types::FromString(thingie[common::data::Properties::TYPE]) == visuals::data::Type::AREA &&
				thingie[visuals::data::Properties::AREA_ID]==areaName)
			{
				int x = thingie[common::data::Properties::X];
				int y = thingie[common::data::Properties::Y];
				size_t width = thingie[common::data::Properties::WIDTH];
				size_t height = thingie[common::data::Properties::HEIGHT];
				return
				{
					thingie[visuals::data::Properties::AREA_ID],
					common::XY<int>(x,y),
					common::XY<size_t>(width, height)
				};
			}
		}
		throw "DID NOT FIND!";
	}

}