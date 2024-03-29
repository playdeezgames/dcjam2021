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
		NO_BRIBE
	};
	struct AttitudeDescriptor
	{
		Attitude attitude;
		std::string sfx;
	};
	struct Descriptor
	{
		std::string imageId;
		int maximumHealth;
		int attack;
		int defend;
		std::vector<size_t> numberAppearing;
		std::map<int, size_t> bribes;
		std::map<Sfx, std::string> sfx;
		Attitude attitude;
		std::map<int, AttitudeDescriptor> attitudes;
		bool preferDeadEnds;
		std::map<std::optional<int>, size_t> dropWeights;
		std::map<int, size_t> dropCounts;
		std::string name;
	};

	const Descriptor& GetDescriptor(int);
	const std::vector<Descriptor>& All();
}
