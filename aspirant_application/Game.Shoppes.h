#pragma once
#include <string>
#include <vector>
#include <map>
#include <optional>
#include "Common.XY.h"
namespace game::shoppe
{
	struct TradeDescriptor
	{
		std::string name;
		std::string sfx;
		std::map<int, size_t> inputs;
		std::map<int, size_t> outputs;
	};
	struct Descriptor
	{
		std::string name;
		std::string imageId;
		std::vector<TradeDescriptor> trades;
	};
	const Descriptor& GetDescriptor(size_t);
	const std::vector<Descriptor>& All();
}
namespace game::Shoppes
{
	std::optional<std::string> AttemptTrade(const common::XY<size_t>&, size_t);
	std::optional<size_t> Read(const common::XY<size_t>&);
}
