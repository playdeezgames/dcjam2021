#include "Application.Sounds.h"
#include "Data.Stores.h"
#include <sstream>
namespace application::Sounds
{
	static std::string SfxToString(const application::UI::Sfx& sfx)
	{
		std::stringstream ss;
		ss << (int)sfx;
		return ss.str();
	}

	std::optional<std::string> Read(const application::UI::Sfx& sfx)
	{
		auto& store = data::Stores::GetStore(data::Store::UI_SFX);
		auto id = SfxToString(sfx);
		if (store.count(id) > 0)
		{
			return std::optional<std::string>(store[id]);
		}
		return std::nullopt;
	}
}