#include "vpch.h"
#include "WorldFunctions.h"
#include "Log.h"

std::map<std::string, std::function<void()>> worldStartFunctionMap;

void HouseStart()
{
	Log("test");
}

void WorldFunctions::SetupWorldStartFunctions()
{
	worldStartFunctionMap.emplace("house.vmap", HouseStart);
}

void WorldFunctions::CallWorldStartFunction(std::string worldName)
{
	auto funcIt = worldStartFunctionMap.find(worldName);
	if (funcIt != worldStartFunctionMap.end())
	{
		funcIt->second();
	}
	else
	{
		Log("No Start function for [%s] world.", worldName.c_str());
	}
}
