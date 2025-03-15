export module Core.VString;

#include <vadefs.h>
#include <codecvt>
#include <cstdarg>

import <string>;

export namespace VString
{
	std::wstring stows(const std::string str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(str);
	}

	std::string wstos(const std::wstring wstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(wstr);
	}

	std::wstring wformat(const std::wstring wstr, ...)
	{
		va_list args;
		va_start(args, wstr.c_str());

		wchar_t output[1024];
		_vsnwprintf_s(output, 1024, wstr.c_str(), args);
		va_end(args);

		return std::wstring(output);
	}

	std::string format(const std::string str, ...)
	{
		va_list args;
		va_start(args, str.c_str());

		char output[1024];
		_vsnprintf_s(output, 1024, str.c_str(), args);
		va_end(args);

		return std::string(output);
	}

	std::string GetSubStringAtFoundOffset(std::string mainString, std::string findCase)
	{
		if (mainString.find(findCase) != std::string::npos)
		{
			return mainString.substr(mainString.find(findCase) + findCase.size());
		}
		return mainString;
	}

	std::string GetSubStringWithFoundOffset(std::string mainString, std::string findCase)
	{
		if (mainString.find(findCase) != std::string::npos)
		{
			return mainString.substr(mainString.find(findCase));
		}
		return mainString;
	}

	std::string GetSubStringBeforeFoundOffset(std::string mainString, std::string findCase)
	{
		if (mainString.find(findCase) != std::string::npos)
		{
			return mainString.substr(0, mainString.find(findCase));
		}
		return mainString;
	}

	std::string ReplaceFileExtesnion(std::string mainString, std::string newExtension)
	{
		return VString::GetSubStringBeforeFoundOffset(mainString, ".") + newExtension;
	}
}
