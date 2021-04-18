#pragma once
#include <vector>
#include <string>
namespace common::Utility
{
	std::vector<std::string> ParseCommandLine(int, char**);
	int StringToInt(const std::string&);
	int ToPercentage(int, int);
}

