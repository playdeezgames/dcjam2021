#pragma once
#include "Common.XY.h"
namespace game::world
{
	enum class KnownState
	{
		UNKNOWN,
		KNOWN,
		VISITED
	};
}
namespace game::World
{
	const size_t COLUMNS = 12;
	const size_t ROWS = 12;
	common::XY<size_t> GetSize();
	game::world::KnownState GetKnownState(const common::XY<size_t>&);
	void SetExplored(const common::XY<size_t>&);
	void SetKnown(const common::XY<size_t>&);
}