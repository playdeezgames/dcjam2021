#pragma once
#include <string>
namespace application::Sounds
{
	//for those that are not monster or item specific, they may remain:
	const std::string BUMP_WALL = "bumpwall";
	const std::string DEAD_HUNTER = "deadhunter";
	const std::string EXIT = "exit";
	const std::string HIT_HUNTER = "hithunter";

	//TODO: these belong on both lists vv
	const std::string HIT_BLOCKED = "hitblocked";
	const std::string WOOHOO = "woohoo";

	//TODO: these should be put as descriptor properties for items and creatures where possible
	const std::string BEER = "beer";
	const std::string COFFEE = "coffee";
	const std::string DEAD_MONSTER = "deadmonster";
	const std::string DRINK_POTION = "drinkpotion";
	const std::string GET_POTION = "getpotion";
	const std::string HIT_MONSTER = "hitmonster";
	const std::string JOOLS = "jools";
	const std::string SHUCKS = "shucks";
	const std::string WINE = "wine";
}
