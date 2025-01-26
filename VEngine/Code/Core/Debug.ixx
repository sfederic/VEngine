module;
#include <source_location>
#include <comdef.h>
export module Core.Debug;

export void HR(HRESULT hr)
{
	const std::source_location& location = std::source_location::current();

	if (hr != S_OK)
	{
		_com_error err(hr);
		char errMsg[1024];
		snprintf(errMsg, 1024, "HR: %s\nFile: %s\nFunction: %s\nLine: %u",
			err.ErrorMessage(), location.file_name(), location.function_name(), location.line());
		OutputDebugString(errMsg);
		throw;
	}
}
