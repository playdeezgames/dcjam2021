#include "Game.Avatar.Items.h"
#include <map>
#include "json.hpp"
#include "Game.Data.Properties.h"
#include <sstream>
#include "Common.Utility.h"
#include "Game.World.Items.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Avatar.h"
#include "Application.Sounds.h"
#include "Game.Creatures.h"
#include "Game.World.h"
#include "Common.RNG.h"
#include "Game.CombatDeck.h"
namespace game::Avatar
{
	nlohmann::json& GetAvatar();
}
namespace game::avatar::Items
{
	static nlohmann::json& GetAvatarInventory()
	{
		auto& avatar = game::Avatar::GetAvatar();
		if (avatar.count(game::data::Properties::INVENTORY) == 0)
		{
			avatar[game::data::Properties::INVENTORY] = nlohmann::json();
		}
		return avatar[game::data::Properties::INVENTORY];
	}

	static std::string ItemToKey(int item)
	{
		std::stringstream ss;
		ss << (int)item;
		return ss.str();
	}

	size_t Read(int item)
	{
		auto itemKey = ItemToKey(item);
		auto& inventory = GetAvatarInventory();
		if (inventory.count(itemKey) > 0)
		{
			return inventory[itemKey];
		}
		return 0;
	}

	std::map<int, size_t> All()
	{
		std::map<int, size_t> result;
		for (auto& item : GetAvatarInventory().items())
		{
			result[(int)common::Utility::StringToInt(item.key())] = item.value();
		}
		return result;
	}

	void Reset()
	{
		GetAvatarInventory().clear();
		for (auto item : game::item::All())
		{
			auto descriptor = game::item::GetDescriptor(item);
			if (descriptor.initialInventory)
			{
				Add(item, descriptor.initialInventory.value());
			}
		}
	}

	void Add(int item, size_t amount)
	{
		if (amount > 0)
		{
			auto itemKey = ItemToKey(item);
			GetAvatarInventory()[itemKey] = Read(item) + amount;
		}
	}

	void Start()
	{
		Reset();
	}

	static size_t Remove(int item, size_t quantity)
	{
		auto total = Read(item);
		if (total >0)
		{
			auto itemKey = ItemToKey(item);
			if (quantity >= total)
			{
				quantity = total;
				GetAvatarInventory().erase(itemKey);
			}
			else
			{
				GetAvatarInventory()[itemKey] = Read(item) -quantity;
			}
			return quantity;
		}
		else
		{
			return 0;
		}

	}

	void Drop(std::optional<int> item)
	{
		if (item)
		{
			size_t amount = game::avatar::Items::Remove(*item, 1);
			game::world::Items::Add(game::Avatar::GetPosition(), *item, 1);
		}
	}

	enum class ConsumeItemResult
	{
		CONSUMED,
		DROPPED,
		NOT_CONSUMED
	};

	static std::optional<std::string> ResultToSfx(const game::item::Descriptor& descriptor, ConsumeItemResult result)
	{
		switch (result)
		{
		case ConsumeItemResult::CONSUMED:
		case ConsumeItemResult::DROPPED:
			return descriptor.sfxSuccess;
		}
		return descriptor.sfxFailure;
	}

	static std::optional<std::string> ConsumeItem
	(
		int item, 
		std::function<ConsumeItemResult(const game::item::Descriptor&)> action,
		std::function<std::optional<std::string>(const game::item::Descriptor&, ConsumeItemResult)> translator
	)
	{
		auto descriptor = game::item::GetDescriptor(item);
		ConsumeItemResult result = ConsumeItemResult::NOT_CONSUMED;
		if (game::avatar::Items::Read(item) > 0)
		{
			result = action(descriptor);
			switch (result)
			{
			case ConsumeItemResult::CONSUMED:
				Remove(item, 1);
				break;
			case ConsumeItemResult::DROPPED:
				Drop(item);
				break;
			}
		}
		return translator(descriptor, result);
	}

	static void AdjustBowel(const game::item::Descriptor& descriptor)
	{
		if (descriptor.bowel)
		{
			game::avatar::Statistics::Increase(game::avatar::Statistic::BOWEL, *descriptor.bowel);
		}
	}

	static void AdjustDrunkennessAndNausea(const game::item::Descriptor& descriptor)
	{
		if (descriptor.drunkenness)
		{
			int capacity =
				game::avatar::Statistics::Maximum(game::avatar::Statistic::DRUNKENNESS) -
				game::avatar::Statistics::Read(game::avatar::Statistic::DRUNKENNESS);
			int nausea = (*descriptor.drunkenness > capacity) ? (*descriptor.drunkenness - capacity) : (0);
			game::avatar::Statistics::Increase(game::avatar::Statistic::DRUNKENNESS, *descriptor.drunkenness);
			game::avatar::Statistics::Increase(game::avatar::Statistic::NAUSEA, nausea);
		}
	}

	static ConsumeItemResult ToConsumeItemResult(const game::item::Descriptor& descriptor)
	{
		AdjustBowel(descriptor);
		AdjustDrunkennessAndNausea(descriptor);
		if (descriptor.dropOnUse)
		{
			return ConsumeItemResult::DROPPED;
		}
		else
		{
			return ConsumeItemResult::CONSUMED;
		}
	}

	static std::optional<std::string> Eat(int item)
	{
		return ConsumeItem(item, [](const game::item::Descriptor& descriptor) 
		{
			if (game::avatar::Statistics::Read(game::avatar::Statistic::HUNGER) < game::avatar::Statistics::Maximum(game::avatar::Statistic::HUNGER))
			{
				game::avatar::Statistics::Increase(game::avatar::Statistic::HUNGER, descriptor.amount.value());
				return ToConsumeItemResult(descriptor);
			}
			return ConsumeItemResult::NOT_CONSUMED;
		}, ResultToSfx);
	}

	static std::optional<std::string> Heal(int item)
	{
		return ConsumeItem(item, [](const game::item::Descriptor& descriptor)
		{
			if (game::avatar::Statistics::Read(game::avatar::Statistic::HEALTH) < game::avatar::Statistics::Maximum(game::avatar::Statistic::HEALTH))
			{
				game::avatar::Statistics::Increase(game::avatar::Statistic::HEALTH, descriptor.amount.value());
				return ToConsumeItemResult(descriptor);
			}
			return ConsumeItemResult::NOT_CONSUMED;
		}, ResultToSfx);
	}

	static 	std::optional<std::string> BuffAttack(int item)
	{
		return ConsumeItem(item, [](const game::item::Descriptor& descriptor)
		{
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK, descriptor.amount.value());
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK_TIMER, descriptor.duration.value());
			return ToConsumeItemResult(descriptor);
		}, ResultToSfx);
	}

	static std::optional<std::string> BuffDefend(int item)
	{
		return ConsumeItem(item, [](const game::item::Descriptor& descriptor)
		{
			game::avatar::Statistics::Write(game::avatar::Statistic::DEFEND, descriptor.amount.value());
			game::avatar::Statistics::Write(game::avatar::Statistic::DEFEND_TIMER, descriptor.duration.value());
			return ToConsumeItemResult(descriptor);
		}, ResultToSfx);
	}

	static void LoseTeleportItems()
	{
		auto worldSize = game::World::GetSize();
		auto inventory = All();
		for (auto& item : inventory)
		{
			auto descriptor = game::item::GetDescriptor(item.first);
			if (descriptor.loseOnTeleport)
			{
				while (Read(item.first)>0)
				{
					Drop(item.first);
					size_t x = (size_t)common::RNG::FromRange(0, (int)worldSize.GetX());
					size_t y = (size_t)common::RNG::FromRange(0, (int)worldSize.GetY());
					auto xy = common::XY<size_t>(x, y);
					game::world::Items::Add(xy, item.first, 1);
				}
			}
		}
	}

	static std::optional<std::string> Teleport(int item)
	{
		return ConsumeItem(item, [](const game::item::Descriptor& descriptor)
		{
			auto worldSize = game::World::GetSize();
			common::XY<size_t> xy;
			do
			{
				size_t x = (size_t)common::RNG::FromRange(0, (int)worldSize.GetX());
				size_t y = (size_t)common::RNG::FromRange(0, (int)worldSize.GetY());
				xy = common::XY<size_t>(x, y);
			} while (game::Creatures::GetInstance(xy).has_value());
			LoseTeleportItems();
			game::Avatar::SetPosition(xy);
			game::World::SetExplored(xy);
			return ToConsumeItemResult(descriptor);
		}, ResultToSfx);
	}

	static std::map<game::item::Usage, std::function<std::optional<std::string>(int)>> nonCombatVerbs =
	{
		{game::item::Usage::EAT, Eat},
		{game::item::Usage::HEAL, Heal},
		{game::item::Usage::ATTACK_BUFF, BuffAttack},
		{game::item::Usage::DEFEND_BUFF, BuffDefend},
		{game::item::Usage::TELEPORT, Teleport},
		{game::item::Usage::BRIBE, [](int) { return std::nullopt; }},
		{game::item::Usage::ATTITUDE, [](int) { return std::nullopt; }},
		{game::item::Usage::JOKER, [](int) { return std::nullopt; }}
	};

	std::optional<std::string> Use(std::optional<int> item)
	{
		if (item)
		{
			auto usage = game::item::GetDescriptor(*item).usage;
			if (usage)
			{
				return nonCombatVerbs.find(*usage)->second(*item);
			}
		}
		return std::nullopt;
	}

	static std::optional<std::tuple<std::string, CombatUseResult>> CombatEat(int item)
	{
		auto result = Eat(item);
		if (result)
		{
			return std::make_tuple(*result, CombatUseResult::SKIP);
		}
		return std::nullopt;
	}

	static std::optional<std::tuple<std::string, CombatUseResult>> CombatHeal(int item)
	{
		auto result = Heal(item);
		if (result)
		{
			return std::make_tuple(*result, CombatUseResult::SKIP);
		}
		return std::nullopt;
	}

	static std::optional<std::tuple<std::string, CombatUseResult>> CombatTeleport(int item)
	{
		auto result = Teleport(item);
		if (result)
		{
			return std::make_tuple(*result, CombatUseResult::REFRESH);
		}
		return std::nullopt;
	}

	static std::optional<std::tuple<std::string, CombatUseResult>> CombatBuffAttack(int item)
	{
		auto result = BuffAttack(item);
		if (result)
		{
			return std::make_tuple(*result, CombatUseResult::SKIP);
		}
		return std::nullopt;
	}

	static std::optional<std::tuple<std::string, CombatUseResult>> CombatBuffDefend(int item)
	{
		auto result = BuffDefend(item);
		if (result)
		{
			return std::make_tuple(*result, CombatUseResult::SKIP);
		}
		return std::nullopt;
	}

	static std::optional<std::tuple<std::string, CombatUseResult>> CombatBribe(int item)
	{
		auto instance = game::Creatures::GetInstance(game::Avatar::GetPosition());
		if (instance && instance.value().descriptor.bribes.contains(item))
		{
			size_t amount = instance.value().descriptor.bribes.find(item)->second;
			if (amount > 0 && game::avatar::Items::Read(item) >= amount)
			{
				game::avatar::Items::Remove(item, (size_t)amount);
				game::Creatures::Remove(game::Avatar::GetPosition());
				return std::make_tuple(instance.value().descriptor.sfx[game::creature::Sfx::BRIBE], CombatUseResult::REFRESH);
			}
		}
		return std::make_tuple(instance.value().descriptor.sfx[game::creature::Sfx::NO_BRIBE], CombatUseResult::SKIP);
	}

	static std::optional<std::tuple<std::string, CombatUseResult>> CombatAttitude(int item)
	{
		auto result = ConsumeItem(item, [item](const game::item::Descriptor& descriptor)
		{
			game::Creatures::ChangeAttitude(game::Avatar::GetPosition(), item);
			return ToConsumeItemResult(descriptor);
		}, ResultToSfx);
		if (result)
		{
			return std::make_tuple(*result, CombatUseResult::REFRESH);
		}
		return std::nullopt;
	}

	static std::optional<std::tuple<std::string, CombatUseResult>> CombatJoker(int item)
	{
		auto result = ConsumeItem(item, [](const game::item::Descriptor& descriptor)
		{
			game::CombatDeck::Deal();
			return ToConsumeItemResult(descriptor);
		}, ResultToSfx);
		if (result)
		{
			return std::make_tuple(*result, CombatUseResult::SKIP);
		}
		return std::nullopt;
	}


	static std::map<game::item::Usage, std::function<std::optional<std::tuple<std::string, CombatUseResult>>(int)>> combatVerbs =
	{
		{game::item::Usage::EAT, CombatEat},
		{game::item::Usage::HEAL, CombatHeal},
		{game::item::Usage::ATTACK_BUFF, CombatBuffAttack},
		{game::item::Usage::DEFEND_BUFF, CombatBuffDefend},
		{game::item::Usage::TELEPORT, CombatTeleport},
		{game::item::Usage::BRIBE, CombatBribe},
		{game::item::Usage::ATTITUDE, CombatAttitude},
		{game::item::Usage::JOKER, CombatJoker}
	};

	std::optional<std::tuple<std::string, CombatUseResult>> CombatUse(std::optional<int> item)
	{
		if (item)
		{
			auto usage = game::item::GetDescriptor(*item).combatUsage;
			if (usage)
			{
				return combatVerbs.find(*usage)->second(*item);
			}
		}
		return std::nullopt;
	}
}