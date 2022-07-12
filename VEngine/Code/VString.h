#pragma once
#include <string>

//string conversion functions taken from:
//https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string

namespace VString
{
	//convert from std::string to std::wstring
	std::wstring stows(const std::string str);

	//convert from std::wstring to std::string
	std::string wstos(const std::wstring wstr);

	//String formatting functions
	std::wstring wformat(const std::wstring wstr, ...);
	std::string format(const std::string str, ...);

	//Returns the string that is found at the matching input to the end of the string.
	//Using this mainly for file path stuff to get the path from a specific folder's name.
	std::string GetSubStringAtFoundOffset(std::string mainString, std::string findCase);
}
