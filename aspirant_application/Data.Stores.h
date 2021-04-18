#pragma once
#include "Data.Store.h"
#include "json.hpp"
namespace data::Stores
{
	nlohmann::json& GetStore(const data::Store&);
	void Start();
}
