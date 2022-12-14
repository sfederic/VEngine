#pragma once

#include <string>

//A setup for essentially calling Start() & Tick() functions for a world on load or at runtime respectively.
namespace WorldFunctions
{
	void SetupWorldStartFunctions();
	void CallWorldStartFunction(const std::string worldName);
}
