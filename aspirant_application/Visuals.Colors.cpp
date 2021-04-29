#include "Visuals.Colors.h"
#include "Data.JSON.h"
#include <map>
#include <vector>
#include "Visuals.Data.Properties.h"
#include "Data.Stores.h"
namespace visuals::Colors
{
	static SDL_Color ParseDescriptor(const nlohmann::json& properties)
	{
		SDL_Color result;
		result.r = (Uint8)properties[data::Properties::RED];
		result.g = (Uint8)properties[data::Properties::GREEN];
		result.b = (Uint8)properties[data::Properties::BLUE];
		result.a = (Uint8)properties[data::Properties::ALPHA];
		return result;
	}

	SDL_Color Read(const std::string& key)
	{
		return ParseDescriptor(::data::Stores::GetStore(::data::Store::COLORS)[key]);
	}
}