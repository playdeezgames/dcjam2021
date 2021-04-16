#pragma once
#include <string>
#include <optional>
namespace game
{
	//TODO: eliminate and replace with int
	enum class Item
	{
		FOOD,
		POTION,
		BEER,
		WINE,
		COFFEE,
		JOOLS
	};
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
		};
		Descriptor GetDescriptor(game::Item);
		void InitializeFromFile(const std::string&);
	}
}

