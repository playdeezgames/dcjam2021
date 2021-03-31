#pragma once
#include <map>
#include "UIState.h"
#include "Application.UIState.h"
namespace application::Handlers
{
	template<typename THandler, typename TAction>
	void WithCurrent(const std::map<::UIState, THandler> handlers, TAction action)
	{
		auto iter = handlers.find(application::UIState::Read());
		if (iter != handlers.end())
		{
			action(iter->second);
		}
	}
}