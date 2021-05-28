#include "Common.XY.h"
#include <string>
#include <set>
#include <functional>
#pragma once
namespace visuals
{
	struct Area
	{
		std::string areaId;
		common::XY<int> xy;
		common::XY<size_t> size;
		bool Contains(const common::XY<int>&) const;
	};
}
namespace visuals::Areas
{
	std::set<std::string> Get(const std::string&, const common::XY<int>&);
	Area Get(const std::string&, const std::string&);
	std::function<void(const common::XY<int>&)> HandleMouseMotion(const std::string&, std::function<void(const std::string&, const common::XY<int>&)>, std::function<void(const common::XY<int>&)>);
	std::function<void(const common::XY<int>&)> HandleMouseMotion(const std::string&, std::function<void(const std::string&, const common::XY<int>&)>);
	std::function<bool(const common::XY<int>&, unsigned char)> HandleMouseButtonUp(const std::string&, std::function<bool(const std::string&)>);
}
