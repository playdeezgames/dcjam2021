#include "Game.Shoppes.h"
#include "Data.Stores.h"
#include "Common.Data.Properties.h"
#include "Game.Data.Properties.h"
#include "Common.Utility.h"
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

	const Descriptor& GetDescriptor(int index)
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