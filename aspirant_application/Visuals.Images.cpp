#include "Visuals.Images.h"
#include <map>
#include "json.hpp"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Data.Types.h"
#include "Visuals.Layouts.h"
#include "Visuals.Sprites.h"
#include "Visuals.Colors.h"
#include <functional>
namespace visuals::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace visuals::Image
{
	struct InternalImage
	{
		std::string sprite;
		SDL_Color color;
		common::XY<int> xy;
		bool visible;
	};

	static std::vector<InternalImage> images;
	static std::map<std::string, std::map<std::string, size_t>> imageTable;

	const SDL_Color defaultColor = { 255, 255, 255, 255 };

	static void DrawInternalImage(std::shared_ptr<SDL_Renderer> renderer, size_t imageIndex)
	{
		auto& image = images[imageIndex];
		if (image.visible)
		{
			Sprites::Draw(
				image.sprite,
				renderer,
				image.xy,
				image.color);
		}
	}

	std::function<void(std::shared_ptr<SDL_Renderer>)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		auto imageIndex = images.size();
		images.push_back(
			{
				model[visuals::data::Properties::SPRITE],
				(model.count(data::Properties::COLOR) > 0) ? (::visuals::Colors::Read(model[data::Properties::COLOR])) : (defaultColor),
				common::XY<int>(model[common::data::Properties::X], model[common::data::Properties::Y]),
				model.count(data::Properties::VISIBLE) == 0 || model[data::Properties::VISIBLE] == true
			});
		if (model.count(visuals::data::Properties::IMAGE_ID) > 0)
		{
			std::string imageId = model[visuals::data::Properties::IMAGE_ID];
			imageTable[layoutName][imageId] = imageIndex;
		}
		return [imageIndex](std::shared_ptr<SDL_Renderer> renderer)
		{
			DrawInternalImage(renderer, imageIndex);
		};
	}

	void Draw(std::shared_ptr<SDL_Renderer> renderer, const nlohmann::json& model)
	{
		if (model.count(data::Properties::VISIBLE) == 0 || model[data::Properties::VISIBLE] == true)
		{
			std::string spriteName = model[data::Properties::SPRITE];
			int x = model[common::data::Properties::X];
			int y = model[common::data::Properties::Y];
			SDL_Color color = (model.count(data::Properties::COLOR) > 0) ?
				(::visuals::Colors::Read(model[data::Properties::COLOR])) :
				(defaultColor);

			Sprites::Draw(
				spriteName,
				renderer,
				common::XY<int>(
					x,
					y),
				color);
		}
	}
}
namespace visuals::Images
{
	template<typename TResult>
	static TResult WithImage(const std::string& layoutName, const std::string& imageId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : visuals::Layouts::GetLayout(layoutName))
		{
			if (visuals::data::Types::FromString(thingie[common::data::Properties::TYPE]) == visuals::data::Type::IMAGE)
			{
				if (thingie.count(visuals::data::Properties::IMAGE_ID) > 0 &&
					thingie[visuals::data::Properties::IMAGE_ID] == imageId)
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
			thingie[visuals::data::Properties::SPRITE] = spriteName;
		}, []() {});
	}

	void SetVisible(const std::string& layoutName, const std::string& imageId, bool visible)
	{
		WithImage<void>(layoutName, imageId, [visible](auto& thingie) 
		{
			thingie[visuals::data::Properties::VISIBLE] = visible;
		}, []() {});
	}
}