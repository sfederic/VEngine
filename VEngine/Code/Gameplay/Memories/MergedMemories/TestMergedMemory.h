#pragma once

#include "Gameplay/Memory.h"

//@Todo: remove
struct TestMergedMemory : public Memory
{
	TestMergedMemory(std::string name_,
		std::string description_,
		std::string conditionFuncName_,
		std::string conditionArg_,
		std::string imageFile_) :
		Memory(name_,
			description_,
			conditionFuncName_,
			conditionArg_,
			imageFile_)
	{}
};
