#include "vpch.h"
#include "WorldFunctions.h"
#include "Core/Log.h"
#include "GameUtils.h"

std::map<std::string, std::function<void()>> worldStartFunctionMap;

void WorldFunctions::SetupWorldStartFunctions()
{

}

void WorldFunctions::CallWorldStartFunction(const std::string worldName)
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
