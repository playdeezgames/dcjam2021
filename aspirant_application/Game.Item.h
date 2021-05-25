#pragma once
#include <string>
#include <optional>
#include <vector>
#include <map>
namespace game::item
{
	enum class Usage
	{
		EAT,
		HEAL,
		ATTACK_BUFF,
		DEFEND_BUFF,
		BRIBE,
		TELEPORT,
		ATTITUDE,
		JOKER,
		SOAP
	};
	enum class ItemSfx
	{
		USE,
		PICK_UP
	};
	struct Descriptor
	{
		const std::string name;
		const std::string imageId;
		const std::string takeImageId;
		const std::string takeAreaId;
		std::optional<Usage> usage;
		std::optional<int> amount;
		std::optional<int> duration;
		std::optional<std::vector<size_t>> initialInventory;
		std::optional<Usage> combatUsage;
		std::vector<size_t> numberAppearing;
		std::optional<std::string> combatText;
		bool loseOnTeleport;
		bool dropOnUse;
		std::optional<int> bowel;
		std::optional<int> drunkenness;
		std::map<ItemSfx, std::string> sfx;

		std::optional<std::string> GetSfx(const ItemSfx&) const;
	};
	const Descriptor& GetDescriptor(int);
	const std::vector<int>& All();
}

