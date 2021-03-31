#pragma once
#include <string>
#include "json.hpp"
namespace data::JSON
{
	nlohmann::json Load(const std::string&);
	void Save(const std::string&, const nlohmann::json&);
}