#include "Graphics.Images.h"
#include <map>
#include "json.hpp"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
#include "Graphics.Types.h"
namespace graphics::Layouts
{
	extern std::map<std::string, nlohmann::json> layouts;
}
namespace graphics::Images
{
	template<typename TResult>
	static TResult WithImage(const std::string& layoutName, const std::string& imageId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : graphics::Layouts::layouts[layoutName])
		{
			if (thingie[common::data::Properties::TYPE] == graphics::Types::IMAGE)
			{
				if (thingie.count(graphics::Properties::IMAGE_ID) > 0 &&
					thingie[graphics::Properties::IMAGE_ID] == imageId)
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
			thingie[graphics::Properties::SPRITE] = spriteName;
		}, []() {});
	}

	void SetVisible(const std::string& layoutName, const std::string& imageId, bool visible)
	{
		WithImage<void>(layoutName, imageId, [visible](auto& thingie) 
		{
			thingie[graphics::Properties::VISIBLE] = visible;
		}, []() {});
	}
}