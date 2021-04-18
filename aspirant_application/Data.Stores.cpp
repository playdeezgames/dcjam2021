#include "Data.Stores.h"
#include <map>
#include <string>
#include "Data.JSON.h"
namespace data::Stores
{
	static std::map<data::Store, nlohmann::json> stores;

	nlohmann::json& GetStore(const data::Store& store)
	{
		return stores[store];
	}

	const std::map<data::Store, std::string> storeFiles =
	{
		{data::Store::CREATURE_DESCRIPTORS, "config/creatures.json"}
	};

	void Start()
	{
		for (auto& entry : storeFiles)
		{
			stores[entry.first] = ::data::JSON::Load(entry.second);
		}
	}
}
