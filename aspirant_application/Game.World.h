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
	//TODO: the constants dont quite feel right here....
	const size_t COLUMNS = 12;
	const size_t ROWS = 12;
	const size_t NS_BORDER_COUNT = ROWS * COLUMNS + COLUMNS;
	const size_t NS_BORDER_STRIDE = COLUMNS;
	const size_t EW_BORDER_COUNT = ROWS * COLUMNS + ROWS;
	const size_t EW_BORDER_STRIDE = COLUMNS + 1;
	common::XY<size_t> GetSize();
	game::world::KnownState GetKnownState(const common::XY<size_t>&);
	void SetExplored(const common::XY<size_t>&);
	void SetKnown(const common::XY<size_t>&);
}