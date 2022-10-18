#include "vpch.h"
#include "Memory.h"

bool Memory::CheckCondition()
{
	if (!conditionFuncName.empty())
	{
		conditionFunc = conditionSystem.FindCondition(conditionFuncName);

		if (!conditionFunc(conditionArg))
		{
			return false;
		}
	}

	return true;
}
