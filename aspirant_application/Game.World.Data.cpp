#include "Game.World.Data.h"
#include <sstream>
#include "Game.h"
#include "Game.Data.Properties.h"
namespace game::world::Data
{
	std::string XYToRoomKey(const common::XY<size_t>& xy)
	{
		std::stringstream ss;
		ss << "(" << xy.GetX() << "," << xy.GetY() << ")";
		return ss.str();
	}

	nlohmann::json& GetNSBorders()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::NS_BORDERS) == 0)
		{
			data[game::data::Properties::NS_BORDERS] = nlohmann::json();
		}
		return data[game::data::Properties::NS_BORDERS];
	}

	nlohmann::json& GetEWBorders()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::EW_BORDERS) == 0)
		{
			data[game::data::Properties::EW_BORDERS] = nlohmann::json();
		}
		return data[game::data::Properties::EW_BORDERS];
	}

	nlohmann::json& GetExplored()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::EXPLORED) == 0)
		{
			data[game::data::Properties::EXPLORED] = nlohmann::json();
		}
		return data[game::data::Properties::EXPLORED];
	}

	void ClearExplored()
	{
		game::world::Data::GetExplored().clear();
	}
}
