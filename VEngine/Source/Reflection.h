#pragma once

//C++ reflection helpers.

namespace Reflection
{
	template <typename Type>
	const char* GetTypeName(Type type)
	{
		return typeid(type).name();
	}
}
