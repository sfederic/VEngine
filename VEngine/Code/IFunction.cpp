#include "vpch.h"
#include "IFunction.h"
#include "VFunctionSystem.h"

IFunction::IFunction(std::string name_)
{
	if (functionSystem == nullptr)
	{
		functionSystem = new VFunctionSystem();
	}

	functionSystem->AddFunction(name_, this);
}
