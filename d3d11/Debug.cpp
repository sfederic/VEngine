#include "Debug.h"
#include <comdef.h>
#include <Windows.h>

void DXTrace(HRESULT hr, const char* filename, const char* func, int line)
{
	_com_error err(hr);
	char errmsg[1024];
	snprintf(errmsg, sizeof(errmsg), "HR: %s\nFile: %s\nFunction: %s\nLine: %d", err.ErrorMessage(), filename, func, line);
	MessageBox(0, errmsg, "Error", 0);
}

int Print(const char* format, va_list args)
{
	char buff[1024];
	int charCount = vsnprintf(buff, sizeof(buff), format, args);
	OutputDebugString(buff);
	return charCount;
}

int DebugPrint(const char* format, ...)
{
	va_list argList;
	va_start(argList, format);
	int charsWritten = Print(format, argList);
	return charsWritten;
}