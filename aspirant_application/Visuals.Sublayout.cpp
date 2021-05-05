#include "json.hpp"
#include <SDL.h>
#include "Visuals.Layouts.h"
#include "Common.Data.Properties.h"
namespace visuals::Sublayout
{
	struct InternalSublayout
	{
		std::string name;
	};

	static std::vector<InternalSublayout> internalSublayouts;

	static void DrawInternalSublayout(std::shared_ptr<SDL_Renderer> renderer, size_t index)
	{
		auto& sublayout = internalSublayouts[index];
		visuals::Layouts::Draw(renderer, sublayout.name);
	}

	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t index = internalSublayouts.size();
		internalSublayouts.push_back(
			{ 
				model[common::data::Properties::NAME]
			});
		return [index](std::shared_ptr<SDL_Renderer> renderer) 
		{
			DrawInternalSublayout(renderer, index);
		};
	}
}