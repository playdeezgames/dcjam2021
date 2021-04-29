#include "Visuals.Images.h"
#include <map>
#include "json.hpp"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Data.Types.h"
#include "Visuals.Layouts.h"
#include "Visuals.Sprites.h"
#include "Visuals.Colors.h"
namespace graphics::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace graphics::Image
{
	const SDL_Color defaultColor = { 255, 255, 255, 255 };

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		if (model.count(data::Properties::VISIBLE) == 0 || model[data::Properties::VISIBLE] == true)
		{
			Sprites::Draw(
				model[data::Properties::SPRITE],
				renderer,
				common::XY<int>(
					(int)model[common::data::Properties::X],
					(int)model[common::data::Properties::Y]),
				(model.count(data::Properties::COLOR) > 0) ?
				(::graphics::Colors::Read(model[data::Properties::COLOR])) :
				(defaultColor));
		}
	}
}
namespace graphics::Images
{
	template<typename TResult>
	static TResult WithImage(const std::string& layoutName, const std::string& imageId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : graphics::Layouts::GetLayout(layoutName))
		{
			if (graphics::data::Types::FromString(thingie[common::data::Properties::TYPE]) == graphics::data::Type::IMAGE)
			{
				if (thingie.count(graphics::data::Properties::IMAGE_ID) > 0 &&
					thingie[graphics::data::Properties::IMAGE_ID] == imageId)
				{
					return func(thingie);
				}
			}
		}
		return notFound();
	}

	void SetSprite(const std::string& layoutName, const std::string& imageId, const std::string& spriteName)
	{
		WithImage<void>(layoutName, imageId, [spriteName](auto& thingie)
		{
			thingie[graphics::data::Properties::SPRITE] = spriteName;
		}, []() {});
	}

	void SetVisible(const std::string& layoutName, const std::string& imageId, bool visible)
	{
		WithImage<void>(layoutName, imageId, [visible](auto& thingie) 
		{
			thingie[graphics::data::Properties::VISIBLE] = visible;
		}, []() {});
	}
}