
#include "VFunctionSystem.h"

VFunctionSystem* functionSystem;

std::vector<std::string> VFunctionSystem::GetFunctionNames()
{
	std::vector<std::string> names;
	for (auto& funcPair : functionMap)
	{
		names.emplace_back(funcPair.first);
	}
	return names;
}
