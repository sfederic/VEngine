#pragma once
#include <string>

//@Todo: C++'s std::format is good, but it's fucked up currently.
//Soon as it's green, think about replacing va_args here.

void Log(std::string msg, ...);
void Log(std::wstring msg, ...);
