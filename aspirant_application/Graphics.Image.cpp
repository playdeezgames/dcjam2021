#include "Common.Properties.h"
#include "Common.Utility.h"
#include "Common.XY.h"
#include "json.hpp"
#include "Graphics.Colors.h"
#include "Graphics.Properties.h"
#include "Graphics.Sprites.h"
namespace graphics::Image
{
	void Draw(SDL_Renderer* renderer, const nlohmann::json& model)
	{
		Sprites::Read(model[Properties::SPRITE])
			.value()
			.Draw
			(
				renderer,
				common::XY<int>(
					(int)model[common::Properties::X], 
					(int)model[common::Properties::Y]),
				::graphics::Colors::Get(model[Properties::COLOR])
			);
	}
}
