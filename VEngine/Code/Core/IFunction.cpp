#include "vpch.h"
#include "IFunction.h"
#include "VFunctionSystem.h"

IFunction::IFunction(std::string name_)
{
	name = name_;
	VFunctionSystem::Get().AddFunction(name_, this);
}
