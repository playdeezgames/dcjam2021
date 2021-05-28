#pragma once
#include "Game.Item.h"
#include <optional>
#include "Common.XY.h"
#include <functional>
namespace visuals::AvatarInventory
{
	void ResetIndex(const std::string&, const std::string&);
	void NextIndex(const std::string&, const std::string&);
	void PreviousIndex(const std::string&, const std::string&);
	std::function<void()> DoIndexReset(const std::string&, const std::string&);
	std::function<void()> GoToNextIndex(const std::string&, const std::string&);
	std::function<void()> GoToPreviousIndex(const std::string&, const std::string&);
	std::optional<int> GetItem(const std::string&, const std::string&);
	void OnMouseMotion(const std::string&, const std::string&, const common::XY<int>&);
	std::optional<int> OnMouseButtonUp(const std::string&, const std::string&, const common::XY<int>&, unsigned char);
}