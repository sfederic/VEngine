#pragma once
#include <map>
#include "IFunction.h"

class VFunctionSystem
{
private:
	std::map<std::string, IFunction*> functionMap;

public:
	void AddFunction(std::string name, IFunction* func)
	{
		functionMap.insert(std::make_pair(name, func));
	}

	IFunction* FindFunction(std::string name)
	{
		return functionMap[name];
	}
};

extern VFunctionSystem* functionSystem;
