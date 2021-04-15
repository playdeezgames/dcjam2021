#include "Game.Creatures.h"
#include "json.hpp"
#include "Data.JSON.h"
#include <map>
#include "Common.RNG.h"
#include "Game.World.h"
#include "Game.Properties.h"
#include <sstream>
#include "Game.h"
namespace game::Creatures
{
	nlohmann::json& GetCreatures()
	{
		auto& data = game::GetData();
		if (data.count(game::Properties::CREATURES) == 0)
		{
			data[game::Properties::CREATURES] = nlohmann::json();
		}
		return data[game::Properties::CREATURES];
	}


	struct CreatureInstance
	{
		int creature;
		int wounds;
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
			return std::optional<CreatureInstance>({ (int)creature[game::Properties::CREATURE], (int)creature[game::Properties::WOUNDS] });
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
		creatures[place][game::Properties::CREATURE] = (int)instance.creature;
		creatures[place][game::Properties::WOUNDS] = instance.wounds;
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

	void Advance(const common::XY<size_t>& location)
	{
		auto instance = GetInstance(location);
		if (instance && instance.value().health<=0)
		{
			Remove(location);
		}
	}

	void Reset()
	{
		auto worldSize = game::World::GetSize();
		GetCreatures().clear();
		for (auto creature : game::creature::All())
		{
			auto descriptor = game::creature::GetDescriptor(creature);
			size_t numberAppearing = descriptor.numberAppearing;
			while (numberAppearing > 0)
			{
				bool available = false;
				size_t x;
				size_t y;
				while (!available)
				{
					x = (size_t)common::RNG::FromRange(0, (int)worldSize.GetX());
					y = (size_t)common::RNG::FromRange(0, (int)worldSize.GetY());
					available = !Get({ x,y });
				}
				Put({ x,y }, { creature, 0 });
				numberAppearing--;
			}
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
				descriptor
			});
		}
		return std::nullopt;
	}
}