#pragma once
import <map>;
#include "IFunction.h"

class VFunctionSystem
{
private:
	std::map<std::string, IFunction*> functionMap;

public:
	static auto& Get()
	{
		static VFunctionSystem instance;
		return instance;
	}

	void AddFunction(std::string name, IFunction* func)
	{
		functionMap.insert(std::make_pair(name, func));
	}

	IFunction* FindFunction(std::string name)
	{
		return functionMap[name];
	}

	std::vector<std::string> GetFunctionNames();
};
