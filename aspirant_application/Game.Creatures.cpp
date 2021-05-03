#include "Game.Creatures.h"
#include "json.hpp"
#include "Data.JSON.h"
#include <map>
#include "Common.RNG.h"
#include "Game.World.h"
#include "Game.Data.Properties.h"
#include <sstream>
#include "Game.h"
#include <algorithm>
#include "Game.World.Items.h"
namespace game::Creatures
{
	nlohmann::json& GetCreatures()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::CREATURES) == 0)
		{
			data[game::data::Properties::CREATURES] = nlohmann::json();
		}
		return data[game::data::Properties::CREATURES];
	}


	struct CreatureInstance
	{
		int creature;
		int wounds;
		int attitude;
	};

	static std::string XYToString(const common::XY<size_t>& location)
	{
		std::stringstream ss;
		ss << "(" << location.GetX() << "," << location.GetY() << ")";
		return ss.str();
	}

	static std::optional<CreatureInstance> Get(const common::XY<size_t>& location)
	{
		auto place = XYToString(location);
		auto& creatures = GetCreatures();
		if (creatures.count(place) > 0)
		{
			auto& creature = GetCreatures()[place];
			return std::optional<CreatureInstance>({ (int)creature[game::data::Properties::CREATURE], (int)creature[game::data::Properties::WOUNDS], (int)creature[game::data::Properties::ATTITUDE] });
		}
		return std::nullopt;
	}

	void Put(const common::XY<size_t>& location, CreatureInstance instance)
	{
		auto place = XYToString(location);
		auto& creatures = GetCreatures();
		if (creatures.count(place) == 0)
		{
			creatures[place] = nlohmann::json();
		}
		creatures[place][game::data::Properties::CREATURE] = (int)instance.creature;
		creatures[place][game::data::Properties::WOUNDS] = instance.wounds;
		creatures[place][game::data::Properties::ATTITUDE] = instance.attitude;
	}

	void Remove(const common::XY<size_t>& location)
	{
		auto place = XYToString(location);
		auto& creatures = GetCreatures();
		if (creatures.count(place) > 0)
		{
			creatures.erase(place);
		}
	}

	std::optional<int> Read(const common::XY<size_t>& location)
	{
		auto temp = Get(location);
		if (temp)
		{
			return temp.value().creature;
		}
		else
		{
			return std::nullopt;
		}
	}
	std::optional<int> DoDamage(const common::XY<size_t>& location, int amount)
	{
		auto instance = Get(location);
		if (instance)
		{
			amount = amount - game::Creatures::GetInstance(location).value().descriptor.defend;
			amount = (amount < 0) ? (0) : (amount);
			instance.value().wounds += amount;
			Put(location, instance.value());
			return amount;
		}
		return std::nullopt;
	}

	void SetAttitude(const common::XY<size_t>& location, game::creature::Attitude attitude)
	{
		auto instance = Get(location);
		if (instance)
		{
			instance.value().attitude = (int)attitude;
			Put(location, instance.value());
		}
	}

	std::optional<std::string> ChangeAttitude(const common::XY<size_t>& location, int item)
	{
		auto instance = Get(location);
		if (instance)
		{
			auto descriptor = game::Creatures::GetInstance(location).value().descriptor;
			auto iter = descriptor.attitudes.find(item);
			if (iter != descriptor.attitudes.end())
			{
				instance.value().attitude = (int)iter->second;
				Put(location, instance.value());
				return descriptor.sfx[game::creature::Sfx::ATTITUDE];
			}
			return descriptor.sfx[game::creature::Sfx::NO_ATTITUDE];
		}
		return std::nullopt;
	}

	static void DropItems(const common::XY<size_t>& location)
	{
		auto instance = GetInstance(location).value();
		auto drop = common::RNG::FromGenerator<std::optional<int>>(instance.descriptor.dropWeights, std::nullopt);
		if (drop)
		{
			size_t count = instance.descriptor.dropCounts[drop.value()];
			game::world::Items::Add(location, drop.value(), count);
		}
	}

	void Advance(const common::XY<size_t>& location)
	{
		auto instance = GetInstance(location);
		if (instance && instance.value().health<=0)
		{
			DropItems(location);
			Remove(location);
		}
	}

	void Reset()
	{
		auto worldSize = game::World::GetSize();
		GetCreatures().clear();
		auto deadEnds = game::World::GetDeadEnds();
		int index = 0;
		for (auto& creature : game::creature::All())
		{
			auto& descriptor = creature;
			size_t numberAppearing = descriptor.numberAppearing;
			while (numberAppearing > 0)
			{
				bool available = false;
				size_t x;
				size_t y;
				while (!available)
				{
					if (descriptor.preferDeadEnds)
					{
						if (!deadEnds.empty())
						{
							auto xy = deadEnds[common::RNG::FromRange(0u, deadEnds.size())];
							x = xy.GetX();
							y = xy.GetY();
						}
						else
						{
							x = common::RNG::FromRange(0u, worldSize.GetX());
							y = common::RNG::FromRange(0u, worldSize.GetY());
						}
					}
					else
					{
						x = common::RNG::FromRange(0u, worldSize.GetX());
						y = common::RNG::FromRange(0u, worldSize.GetY());
					}
					available = !Get({ x,y });
				}
				common::XY<size_t> location = { x,y };
				Put(location, { index, 0, (int)descriptor.attitude });
				auto iter = std::find(deadEnds.begin(), deadEnds.end(), location);
				if (iter != deadEnds.end())
				{
					deadEnds.erase(iter);
				}
				numberAppearing--;
			}
			index++;
		}
	}

	bool AnyLeft()
	{
		return !GetCreatures().empty();
	}

	std::optional<Instance> GetInstance(const common::XY<size_t>& location)
	{
		auto instance = Get(location);
		if (instance)
		{
			auto descriptor = game::creature::GetDescriptor(instance.value().creature);
			return std::optional<Instance>(
			{
				instance.value().creature,
				descriptor.maximumHealth-instance.value().wounds,
				descriptor,
				(game::creature::Attitude)instance.value().attitude
			});
		}
		return std::nullopt;
	}
}