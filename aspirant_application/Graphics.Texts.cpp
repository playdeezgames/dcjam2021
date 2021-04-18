#include "Graphics.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Graphics.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Graphics.Data.Properties.h"
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
			if (thingie[common::data::Properties::TYPE] == graphics::data::Types::TEXT)
			{
				if (thingie.count(graphics::data::Properties::TEXT_ID) > 0 &&
					thingie[graphics::data::Properties::TEXT_ID] == textId)
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
			thingie[graphics::data::Properties::TEXT] = text;
		}, []() {});
	}
}