#include <cstdarg>
#include <fstream>
#include <chrono>
#include <Windows.h>

export module Core.Log;

import std.core;

void Log(std::string logMessage, ...)
{
	va_list args;
	va_start(args, logMessage.c_str());

	char msg[1024];
	_vsnprintf_s(msg, 1024, logMessage.c_str(), args);
	va_end(args);

	Editor::Get().Log(msg);

	std::ofstream os;
	os.open("Log.txt", std::ios_base::app);
	os << std::chrono::system_clock::now() << " | " << msg << std::endl;

	OutputDebugString(logMessage.c_str());
}

void Log(std::wstring logMessage, ...)
{
	va_list args;
	va_start(args, logMessage.c_str());

	wchar_t msg[1024];
	_vsnwprintf_s(msg, 1024, logMessage.c_str(), args);
	va_end(args);

	Editor::Get().Log(msg);

	std::wofstream os;
	os.open("Log.txt", std::ios_base::app);
	os << std::chrono::system_clock::now() << " | " << msg << std::endl;
}
