#pragma once
#include <vector>
#include <string>
#include <functional>
#include <map>
namespace common::Utility
{
	std::vector<std::string> ParseCommandLine(int, char**);
	int StringToInt(const std::string&);
	int ToPercentage(int, int);
	template<typename TEnum>
	void Dispatch(const std::map<TEnum, std::function<void()>> handlers, const TEnum& key)
	{
		auto iter = handlers.find(key);
		if (iter != handlers.end())
		{
			iter->second();
		}
	}
	bool FileExists(const std::string&);
}

