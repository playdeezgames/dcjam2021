#include "Visuals.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
namespace graphics::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace graphics::Texts
{
	template <typename TResult>
	static TResult WithText(const std::string& layoutName, const std::string& textId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : graphics::Layouts::GetLayout(layoutName))
		{
			if (graphics::data::Types::FromString(thingie[common::data::Properties::TYPE]) == graphics::data::Type::TEXT)
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

	void SetColor(const std::string& layoutName, const std::string& textId, const std::string& color)
	{
		WithText<void>(layoutName, textId, [color](auto& thingie)
		{
			thingie[graphics::data::Properties::COLOR] = color;
		}, []() {});
	}
}