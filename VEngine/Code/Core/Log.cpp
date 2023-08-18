#include "vpch.h"
#include "Log.h"
#include <cstdarg>
#include <fstream>
#include <chrono>
#include <vadefs.h>
#include "Editor/Editor.h"

void Log(std::string logMessage, ...)
{
	va_list args;
	va_start(args, logMessage);

	char msg[1024];
	_vsnprintf_s(msg, 1024, logMessage.c_str(), args);
	va_end(args);

	editor->Log(msg);

	std::ofstream os;
	os.open("Log.txt", std::ios_base::app);
	os << std::chrono::system_clock::now() << " | " << msg << std::endl;
}

void Log(std::wstring logMessage, ...)
{
	va_list args;
	va_start(args, logMessage);

	wchar_t msg[1024];
	_vsnwprintf_s(msg, 1024, logMessage.c_str(), args);
	va_end(args);

	editor->Log(msg);

	std::wofstream os;
	os.open("Log.txt", std::ios_base::app);
	os << std::chrono::system_clock::now() << " | " << msg << std::endl;
}
