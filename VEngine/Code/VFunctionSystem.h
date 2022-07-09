#pragma once
#include <map>

class IFunction;

class VFunctionSystem
{
private:
	std::map<std::string, IFunction*> functionMap;

public:
	void AddFunction(std::string name, IFunction* func)
	{
		functionMap.emplace(name, func);
	}

	IFunction* FindFunction(std::string name)
	{
		return functionMap[name];
	}
};

extern VFunctionSystem functionSystem;
