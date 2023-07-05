#pragma once

#include <string>
#include "Core/VEnum.h"

namespace PropertyTypes
{
	//Make sure this is only called once at engine startup.
	void SetupPropertyTypesVEnum(); 

	VEnum GetPropertyTypes();

	inline static std::string Bool = "Bool";
	inline static std::string Int = "Int";
	inline static std::string Float = "Float";
	inline static std::string String = "String";
};
