#include "Graphics.Images.h"
#include <map>
#include "json.hpp"
#include "Common.Data.Properties.h"
#include "Graphics.Data.Properties.h"
#include "Graphics.Data.Types.h"
#include "Graphics.Layouts.h"
namespace graphics::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
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