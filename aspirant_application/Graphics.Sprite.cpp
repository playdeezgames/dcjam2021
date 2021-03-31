#include "Graphics.Sprite.h"
#include "Graphics.Textures.h"
#include "Graphics.Properties.h"
#include "Common.Properties.h"
namespace graphics
{
	Sprite::Sprite
	(
		const nlohmann::json& model
	)
		: model(model)
	{

	}

	void Sprite::Draw(SDL_Renderer* renderer, const common::XY<int>& xy) const
	{
		Draw(renderer, xy, { 255,255,255,255 });
	}


	void Sprite::Draw(SDL_Renderer* renderer, const common::XY<int>& xy, const SDL_Color& color) const
	{
		auto texture = graphics::Textures::Read(model[graphics::Properties::TEXTURE]);
		SDL_SetTextureColorMod
		(
			texture,
			color.r,
			color.g,
			color.b
		);
		SDL_SetTextureAlphaMod
		(
			texture,
			color.a
		);
		SDL_Rect rcDst =
		{
			xy.GetX() + ((model.count(Properties::OFFSET_X) > 0) ? ((int)model[Properties::OFFSET_X]) : (0)),
			xy.GetY() + ((model.count(Properties::OFFSET_Y) > 0) ? ((int)model[Properties::OFFSET_Y]) : (0)),
			model[common::Properties::WIDTH],
			model[common::Properties::HEIGHT]
		};
		SDL_Rect source =
		{
			model[common::Properties::X],
			model[common::Properties::Y],
			model[common::Properties::WIDTH],
			model[common::Properties::HEIGHT]
		};
		SDL_RenderCopy(renderer, texture, &source, &rcDst);
	}

	int Sprite::GetWidth() const
	{
		return model[common::Properties::WIDTH];
	}

}
