#pragma once
#include <string>
#include <optional>
#include <vector>
namespace game
{
	namespace item
	{
		enum class Usage
		{
			EAT,
			HEAL,
			ATTACK_BUFF,
			DEFEND_BUFF,
			BRIBE
		};
		struct Descriptor
		{
			const std::string name;
			const std::string imageId;
			std::optional<Usage> usage;
			std::optional<int> amount;
			std::optional<int> duration;
			std::optional<std::string> sfxSuccess;
			std::optional<std::string> sfxFailure;
			std::optional<size_t> initialInventory;
			std::optional<Usage> combatUsage;
			size_t numberAppearing;
			std::optional<std::string> combatText;
		};
		Descriptor GetDescriptor(int);
		std::vector<int> All();
		void InitializeFromFile(const std::string&);
	}
}

