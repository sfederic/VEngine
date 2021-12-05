#include "Log.h"
#include <stdarg.h>
#include <vadefs.h>
#include "Editor/Editor.h"
#include "VString.h"

void Log(std::string logMessage, ...)
{
	va_list args;
	va_start(args, logMessage);

	char msg[1024];
	_vsnprintf_s(msg, 1024, logMessage.c_str(), args);
	va_end(args);

	editor->Log(msg);
}

void Log(std::wstring logMessage, ...)
{
	va_list args;
	va_start(args, logMessage);

	wchar_t msg[1024];
	_vsnwprintf_s(msg, 1024, logMessage.c_str(), args);
	va_end(args);

	editor->Log(msg);
}
