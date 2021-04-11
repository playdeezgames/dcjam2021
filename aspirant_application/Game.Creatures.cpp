#include "Game.Creatures.h"
#include "json.hpp"
#include "Data.JSON.h"
#include <map>
#include "Common.RNG.h"
#include "Game.World.h"
#include "Game.Properties.h"
#include <sstream>
namespace game
{
	extern nlohmann::json data;
}
namespace game::Creatures
{
	nlohmann::json& GetCreatures()
	{
		if (data.count(game::Properties::CREATURES) == 0)
		{
			data[game::Properties::CREATURES] = nlohmann::json();
		}
		return data[game::Properties::CREATURES];
	}

	static nlohmann::json descriptors;

	std::string GetImageId(game::Creature creature)
	{
		return descriptors[(int)creature][game::Properties::IMAGE_ID];
	}

	struct CreatureInstance
	{
		game::Creature creature;
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
			return std::optional<CreatureInstance>({ (game::Creature)(int)creature[game::Properties::CREATURE], (int)creature[game::Properties::WOUNDS] });
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

	std::optional<game::Creature> Read(const common::XY<size_t>& location)
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

	std::optional<int> GetMaximumHealth(const common::XY<size_t>& location)
	{
		auto temp = Read(location);
		if (temp)
		{
			return descriptors[(int)temp.value()][game::Properties::HEALTH];
		}
		else
		{
			return std::nullopt;
		}
	}

	std::optional<int> GetAttack(const common::XY<size_t>& location)
	{
		auto temp = Read(location);
		if (temp)
		{
			return descriptors[(int)temp.value()][game::Properties::ATTACK];
		}
		else
		{
			return std::nullopt;
		}
	}

	std::optional<int> GetDefend(const common::XY<size_t>& location)
	{
		auto temp = Read(location);
		if (temp)
		{
			return descriptors[(int)temp.value()][game::Properties::DEFEND];
		}
		else
		{
			return std::nullopt;
		}
	}

	std::optional<int> GetFoodBribe(const common::XY<size_t>& location)
	{
		auto temp = Read(location);
		if (temp)
		{
			return descriptors[(int)temp.value()][game::Properties::FOOD_BRIBE];
		}
		else
		{
			return std::nullopt;
		}
	}

	std::optional<int> GetMoneyBribe(const common::XY<size_t>& location)
	{
		auto temp = Read(location);
		if (temp)
		{
			return descriptors[(int)temp.value()][game::Properties::MONEY_BRIBE];
		}
		else
		{
			return std::nullopt;
		}
	}

	std::optional<int> GetHealth(const common::XY<size_t>& location)
	{
		auto temp = Get(location);
		if (temp)
		{
			return GetMaximumHealth(location).value() - temp.value().wounds;
		}
		else
		{
			return std::nullopt;
		}
	}

	std::optional<bool> IsDead(const common::XY<size_t>& location)
	{
		auto instance = Get(location);
		if (instance)
		{
			return instance.value().wounds >= GetMaximumHealth(location).value();
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
			amount = amount - game::Creatures::GetDefend(location).value();
			amount = (amount < 0) ? (0) : (amount);
			instance.value().wounds += amount;
			Put(location, instance.value());
			return amount;
		}
		return std::nullopt;
	}

	void Advance(const common::XY<size_t>& location)
	{
		auto health = GetHealth(location);
		if (health && health.value()<=0)
		{
			Remove(location);
		}
	}

	void Reset()
	{
		auto worldSize = game::World::GetSize();
		GetCreatures().clear();
		for (auto& descriptor : descriptors)
		{
			game::Creature creature = (game::Creature)(int)descriptor[game::Properties::INDEX];
			size_t numberAppearing = descriptor[game::Properties::NUMBER_APPEARING];
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

	void InitializeFromFile(const std::string& filename)
	{
		descriptors = data::JSON::Load(filename);
	}
}