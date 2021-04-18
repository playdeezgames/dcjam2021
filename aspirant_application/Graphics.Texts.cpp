#include "Graphics.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Types.h"
#include "Common.Properties.h"
#include "Graphics.Properties.h"
namespace graphics::Layouts
{
	extern std::map<std::string, nlohmann::json> layouts;
}
namespace graphics::Texts
{
	template <typename TResult>
	static TResult WithText(const std::string& layoutName, const std::string& textId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : graphics::Layouts::layouts[layoutName])
		{
			if (thingie[common::data::Properties::TYPE] == graphics::Types::TEXT)
			{
				if (thingie.count(graphics::Properties::TEXT_ID) > 0 &&
					thingie[graphics::Properties::TEXT_ID] == textId)
				{
					return func(thingie);
				}
			}
		}
		return notFound();
	}

	void SetText(const std::string& layoutName, const std::string& textId, const std::string& text)
	{
		WithText<void>(layoutName, textId, [text](auto& thingie) 
		{
			thingie[graphics::Properties::TEXT] = text;
		}, []() {});
	}
}