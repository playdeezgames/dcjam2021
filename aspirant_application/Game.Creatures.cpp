#include "Game.Creatures.h"
#include "json.hpp"
#include "Data.JSON.h"
#include <map>
#include "Common.RNG.h"
#include "Game.World.h"
namespace game::Creatures
{
	const std::string PROPERTY_NUMBER_APPEARING = "number-appearing";
	const std::string PROPERTY_INDEX = "index";
	const std::string PROPERTY_IMAGE_ID = "image-id";
	const std::string PROPERTY_HEALTH = "health";
	const std::string PROPERTY_ATTACK = "attack";
	const std::string PROPERTY_DEFEND = "defend";

	static nlohmann::json descriptors;

	std::string GetImageId(game::Creature creature)
	{
		return descriptors[(int)creature][PROPERTY_IMAGE_ID];
	}

	struct CreatureInstance
	{
		game::Creature creature;
		int wounds;
	};

	static std::map<size_t, std::map<size_t, CreatureInstance>> roomCreatures;

	std::optional<CreatureInstance> Get(const common::XY<size_t>& location)
	{
		auto column = roomCreatures.find(location.GetX());
		if (column != roomCreatures.end())
		{
			auto row = column->second.find(location.GetY());
			if (row != column->second.end())
			{
				return row->second;
			}
		}
		return std::nullopt;
	}

	void Put(const common::XY<size_t>& location, CreatureInstance instance)
	{
		roomCreatures[location.GetX()][location.GetY()] = instance;
	}

	void Remove(const common::XY<size_t>& location)
	{
		if (roomCreatures.contains(location.GetX()))
		{
			if (roomCreatures[location.GetX()].contains(location.GetY()))
			{
				roomCreatures[location.GetX()].erase(location.GetY());
			}
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
			return descriptors[(int)temp.value()][PROPERTY_HEALTH];
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
			return descriptors[(int)temp.value()][PROPERTY_ATTACK];
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
			return descriptors[(int)temp.value()][PROPERTY_DEFEND];
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

	void DecreaseHealth(const common::XY<size_t>& location, int amount)
	{
		auto instance = Get(location);
		if (instance)
		{
			instance.value().wounds += amount;
			Put(location, instance.value());
		}
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
		roomCreatures.clear();
		for (auto& descriptor : descriptors)
		{
			game::Creature creature = (game::Creature)(int)descriptor[PROPERTY_INDEX];
			size_t numberAppearing = descriptor[PROPERTY_NUMBER_APPEARING];
			while (numberAppearing > 0)
			{
				bool available = false;
				size_t x;
				size_t y;
				while (!available)
				{
					x = (size_t)common::RNG::FromRange(0, (int)worldSize.GetX());
					y = (size_t)common::RNG::FromRange(0, (int)worldSize.GetY());
					available = roomCreatures.find(x) == roomCreatures.end() || roomCreatures[x].find(y) == roomCreatures[x].end();
				}
				roomCreatures[x][y] = { creature, 0 };
				numberAppearing--;
			}
		}
	}

	void Start(const std::string& filename)
	{
		descriptors = data::JSON::Load(filename);
	}
}