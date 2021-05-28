#include "Visuals.Areas.h"
#include "json.hpp"
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
namespace visuals::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace visuals
{
	bool Area::Contains(const common::XY<int>& location) const
	{
		int x = location.GetX() - xy.GetX();
		int y = location.GetY() - xy.GetY();
		return(x >= 0 && y >= 0 && x < size.GetX() && y < size.GetY());
	}
}
namespace visuals::Areas
{
	static std::map<std::string, std::vector<visuals::Area>> areaLists;
	static std::map<std::string, std::map<std::string, visuals::Area>> areaTable;

	static void Initialize(const std::string& layoutName)
	{
		if (areaLists.find(layoutName) == areaLists.end())
		{
			areaLists[layoutName] = {};
			for (auto& thingie : visuals::Layouts::GetLayout(layoutName))
			{
				if (visuals::data::Types::FromString(thingie[common::data::Properties::TYPE]) == visuals::data::Type::AREA)
				{
					int x = thingie[common::data::Properties::X];
					int y = thingie[common::data::Properties::Y];
					size_t width = thingie[common::data::Properties::WIDTH];
					size_t height = thingie[common::data::Properties::HEIGHT];
					std::string areaId = thingie[visuals::data::Properties::AREA_ID];
					visuals::Area area =
					{
						areaId,
						common::XY<int>(x,y),
						common::XY<size_t>(width, height)
					};
					areaLists[layoutName].push_back(area);
					areaTable[layoutName][areaId] = area;
				}
			}
		}
	}

	static const std::vector<visuals::Area>& GetForLayout(const std::string& layoutName)
	{
		Initialize(layoutName);
		return areaLists.find(layoutName)->second;
	}

	std::set<std::string> Get(const std::string& layoutName, const common::XY<int>& xy)
	{
		std::set<std::string> result;
		for (auto& area : GetForLayout(layoutName))
		{
			if (area.Contains(xy))
			{
				result.insert(area.areaId);
			}
		}
		return result;
	}

	Area Get(const std::string& layoutName, const std::string& areaName)
	{
		Initialize(layoutName);
		return areaTable.find(layoutName)->second.find(areaName)->second;
	}

	std::function<void(const common::XY<int>& xy)> 
		HandleMouseMotion(
			const std::string& layoutName, 
			std::function<void(const std::string&, const common::XY<int>&)> areaHandler,
			std::function<void(const common::XY<int>&)> noAreaHandler
		)
	{
		Initialize(layoutName);
		return [layoutName, areaHandler, noAreaHandler](const common::XY<int>& xy)
		{
			auto areas = visuals::Areas::Get(layoutName, xy);
			if (areas.empty())
			{
				noAreaHandler(xy);
			}
			else
			{ 			
				for (auto& area : areas)
				{
					auto a = Get(layoutName, area);
					areaHandler(area, xy - a.xy);
				}
			}
		};
	}

	std::function<void(const common::XY<int>& xy)>
		HandleMouseMotion(
			const std::string& layoutName,
			std::function<void(const std::string&, const common::XY<int>&)> areaHandler
		)
	{
		return HandleMouseMotion(layoutName, areaHandler, [](const common::XY<int>&) {});
	}


	std::function<bool(const common::XY<int>&, unsigned char)> HandleMouseButtonUp(const std::string& layoutName, std::function<bool(const std::string&)> areaHandler)
	{
		return [layoutName, areaHandler](const common::XY<int>& xy, unsigned char)
		{
			auto areas = visuals::Areas::Get(layoutName, xy);
			for (auto& area : areas)
			{
				if (areaHandler(area))
				{
					return true;
				}
			}
			return false;
		};
	}
}