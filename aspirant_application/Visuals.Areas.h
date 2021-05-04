#include "Common.XY.h"
#include <string>
#include <set>
#pragma once
namespace visuals
{
	struct Area
	{
		std::string areaId;
		common::XY<int> xy;
		common::XY<size_t> size;
	};
}
namespace visuals::Areas
{
	std::set<std::string> Get(const std::string&, const common::XY<int>&);
	Area Get(const std::string&, const std::string&);
}
