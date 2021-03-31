#include "Common.Utility.h"
#include <sstream>
namespace common::Utility
{
	std::vector<std::string> ParseCommandLine(int argc, char** argv)
	{
		std::vector<std::string> arguments;
		arguments.reserve(argc);
		for (int index = 0; index < argc; ++index)
		{
			arguments.push_back(std::string(argv[index]));
		}
		return arguments;
	}

	int StringToInt(const std::string& text)
	{
		std::stringstream ss;
		ss.str(text);
		int result = 0;
		ss >> result;
		return result;
	}

	int ToPercentage(int value, int maximum)
	{
		return value * 100 / maximum;
	}

	size_t NextIndex(size_t index, size_t count)
	{
		if (count > 0)
		{
			return ((index + 1) % count);
		}
		else
		{
			return (0);
		}
	}

	size_t PreviousIndex(size_t index, size_t count)
	{
		if (count > 0)
		{
			return ((index + count - 1) % count);
		}
		else
		{
			return (0);
		}

	}

	int PositiveModulo(int dividend, size_t divisor)
	{
		dividend %= (int)divisor;
		return (dividend < 0) ? (dividend + (int)divisor) : (dividend);
	}
}

