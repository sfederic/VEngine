#include "vpch.h"
#include "IFunction.h"
#include "VFunctionSystem.h"

IFunction::IFunction(std::string name_)
{
	functionSystem.AddFunction(name_, this);
}
