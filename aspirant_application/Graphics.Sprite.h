#pragma once
#include <SDL.h>
#include "Common.XY.h"
#include "json.hpp"
namespace graphics
{
	class Sprite
	{
	private:
		const nlohmann::json& model;
	public:
		Sprite(const nlohmann::json&);
		void Draw(SDL_Renderer*, const ::common::XY<int>&, const SDL_Color&) const;
		void Draw(SDL_Renderer*, const ::common::XY<int>&) const;
		int GetWidth() const;
	};
}

