#include "Game.Shoppes.h"
#include "Data.Stores.h"
#include "Common.Data.Properties.h"
#include "Game.Data.Properties.h"
#include "Common.Utility.h"
#include "Game.h"
#include <sstream>
#include "Game.World.h"
#include "Common.RNG.h"
namespace game::shoppe
{
	static std::vector<Descriptor> descriptors;

	static void Initialize()
	{
		static bool initialized = false;
		if (!initialized)
		{
			auto& shoppes = ::data::Stores::GetStore(::data::Store::SHOPPES);
			for (auto& shoppe : shoppes)
			{
				std::vector<TradeDescriptor> trades;
				auto& entries = shoppe[game::data::Properties::TRADES];
				for (auto& entry : entries)
				{
					std::map<int, size_t> inputs;
					for (auto& input : entry[game::data::Properties::INPUTS].items())
					{
						inputs[common::Utility::StringToInt(input.key())] = input.value();
					}
					std::map<int, size_t> outputs;
					for (auto& input : entry[game::data::Properties::INPUTS].items())
					{
						inputs[common::Utility::StringToInt(input.key())] = input.value();
					}
					trades.push_back(
						{
							entry[common::data::Properties::NAME],
							entry[game::data::Properties::SFX],
							inputs,
							outputs
						});
				}
				descriptors.push_back
				(
					{
						shoppe[common::data::Properties::NAME],
						shoppe[game::data::Properties::IMAGE_ID],
						trades
					}
				);
			}
			initialized = true;
		}
	}

	const Descriptor& GetDescriptor(size_t index)
	{
		Initialize();
		return descriptors[index];
	}

	const std::vector<Descriptor>& All()
	{
		Initialize();
		return descriptors;
	}
}
namespace game::Shoppes
{
	static nlohmann::json& GetShoppes()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::SHOPPES) == 0)
		{
			data[game::data::Properties::SHOPPES] = nlohmann::json();
		}
		return data[game::data::Properties::SHOPPES];
	}

	static std::string XYToString(const common::XY<size_t>& location)
	{
		std::stringstream ss;
		ss << "(" << location.GetX() << "," << location.GetY() << ")";
		return ss.str();
	}

	static std::optional<size_t> Get(const common::XY<size_t>& location)
	{
		auto place = XYToString(location);
		auto& shoppes = GetShoppes();
		if (shoppes.count(place) > 0)
		{
			auto& shoppe = GetShoppes()[place];
			return std::optional<int>(shoppe);
		}
		return std::nullopt;
	}

	void Put(const common::XY<size_t>& location, size_t instance)
	{
		auto place = XYToString(location);
		auto& shoppes = GetShoppes();
		if (shoppes.count(place) == 0)
		{
			shoppes[place] = instance;
		}
	}

	std::optional<size_t> Read(const common::XY<size_t>& location)
	{
		return Get(location);
	}
	void Reset()
	{
		auto worldSize = game::World::GetSize();
		GetShoppes().clear();
		auto deadEnds = game::World::GetDeadEnds();
		for (size_t index = 0; index < game::shoppe::All().size(); ++index)
		{
			bool available = false;
			size_t x;
			size_t y;
			while (!available)
			{
				size_t deadEndIndex = common::RNG::FromRange(0u, deadEnds.size());
				x = deadEnds[deadEndIndex].GetX();
				y = deadEnds[deadEndIndex].GetY();
				available = !Get({ x,y });
			}
			Put({ x,y }, index);
		}
	}
}