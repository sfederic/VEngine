export module Core.Debug;

#include <comdef.h>

import <stdexcept>;
import <source_location>;

export void HR(HRESULT hr)
{
	const std::source_location& location = std::source_location::current();

	if (hr != S_OK)
	{
		_com_error err(hr);
		char errMsg[1024];
		snprintf(errMsg, sizeof(errMsg), "HR: %s\nFile: %s\nFunction: %s\nLine: %u",
			err.ErrorMessage(), location.file_name(), location.function_name(), location.line());

		OutputDebugStringA(errMsg);
		throw std::runtime_error(errMsg);
	}
}
