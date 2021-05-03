#pragma once
#include <map>
namespace common::RNG
{
	void Seed();
	int FromRange(int, int);
	size_t FromRange(size_t, size_t);
	template <typename TResult>
	TResult FromGenerator(const std::map<TResult, size_t>& table, TResult defaultValue)
	{
		size_t total = 0u;
		for (auto& entry : table)
		{
			total += entry.second;
		}
		if (total > 0)
		{
			size_t generated = FromRange(0u, total);
			for (auto& entry : table)
			{
				if (generated < entry.second)
				{
					return entry.first;
				}
				else
				{
					generated -= entry.second;
				}
			}
		}
		return defaultValue;
	}
}