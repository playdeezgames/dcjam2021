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
	template<typename TEnum, typename TResult>
	TResult Dispatch(const std::map<TEnum, std::function<TResult()>> handlers, const TEnum& key, const TResult& defaultResult)
	{
		auto iter = handlers.find(key);
		if (iter != handlers.end())
		{
			return iter->second();
		}
		return defaultResult;
	}
	bool FileExists(const std::string&);
	unsigned char GetFileCheckSum(const std::string&);
}

