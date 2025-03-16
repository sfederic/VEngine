#pragma once

import <vector>;
#include <typeindex>
import <string>;

//Base class for VFunctions
class IFunction
{
public:
	IFunction(std::string name_);

	auto GetArgTypes() { return argTypes; }
	std::string GetName(int index) { return argNames[index]; }

protected:
	std::string name;

	std::vector<std::type_index> argTypes;
	std::vector<std::string> argNames;
};
