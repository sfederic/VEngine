#pragma once
#include <vector>
#include <typeindex>
#include <string>

//Base class for VFunctions
class IFunction
{
private:
	std::vector<std::type_index> argTypes;
	std::vector<std::string> argNames;

public:
	auto GetArgTypes()
	{
		return argTypes;
	}
};
