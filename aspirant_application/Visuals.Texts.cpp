#include "Visuals.Layouts.h"
#include "Data.JSON.h"
#include <algorithm>
#include "Visuals.Data.Types.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
namespace visuals::Layouts
{
	nlohmann::json& GetLayout(const std::string&);
}
namespace visuals::Texts
{
	template <typename TResult>
	static TResult WithText(const std::string& layoutName, const std::string& textId, std::function<TResult(nlohmann::json&)> func, std::function<TResult()> notFound)
	{
		for (auto& thingie : visuals::Layouts::GetLayout(layoutName))
		{
			if (visuals::data::Types::FromString(thingie[common::data::Properties::TYPE]) == visuals::data::Type::TEXT)
			{
				if (thingie.count(visuals::data::Properties::TEXT_ID) > 0 &&
					thingie[visuals::data::Properties::TEXT_ID] == textId)
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
			thingie[visuals::data::Properties::TEXT] = text;
		}, []() {});
	}

	void SetColor(const std::string& layoutName, const std::string& textId, const std::string& color)
	{
		WithText<void>(layoutName, textId, [color](auto& thingie)
		{
			thingie[visuals::data::Properties::COLOR] = color;
		}, []() {});
	}
}