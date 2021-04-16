#pragma once
#include <string>
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
		struct Descriptor
		{
			const std::string name;
			const std::string imageId;
		};
		Descriptor GetDescriptor(game::Item);
		void InitializeFromFile(const std::string&);
	}
}

