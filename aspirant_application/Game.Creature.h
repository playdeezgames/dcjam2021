#pragma once
#include "Common.XY.h"
#include <optional>
#include <string>
#include <vector>
#include <map>
#include "Game.Item.h"
#include <optional>
namespace game::creature
{
	enum class Attitude
	{
		NEUTRAL,
		UNIMPRESSED,
		AMOROUS,
		DISGUST,
		HOSTILE,
		EMBARRASSED,
		DISTRACTED
	};
	enum class Sfx
	{
		HIT,
		DEATH,
		BLOCK,
		BRIBE,
		NO_BRIBE,
		ATTITUDE,
		NO_ATTITUDE
	};
	struct Descriptor
	{
		std::string imageId;
		int maximumHealth;
		int attack;
		int defend;
		size_t numberAppearing;
		std::map<int, size_t> bribes;
		std::map<Sfx, std::string> sfx;
		Attitude attitude;
		std::map<int, Attitude> attitudes;
		bool preferDeadEnds;
		std::map<std::optional<int>, size_t> dropWeights;
		std::map<int, size_t> dropCounts;
	};

	const Descriptor& GetDescriptor(int);
	const std::vector<Descriptor>& All();
}
