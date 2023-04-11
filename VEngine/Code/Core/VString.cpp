#include "vpch.h"
#include "VString.h"
#include <locale>
#include <algorithm>
#include <stdarg.h>
#include <vadefs.h>
#include <codecvt>

namespace VString
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
        va_start(args, wstr);

        wchar_t output[1024];
        _vsnwprintf_s(output, 1024, wstr.c_str(), args);
        va_end(args);

        return std::wstring(output);
    }

    std::string format(const std::string str, ...)
    {
        va_list args;
        va_start(args, str);

        char output[1024];
        _vsnprintf_s(output, 1024, str.c_str(), args);
        va_end(args);

        return std::string(output);
    }

    void Replace(std::string& str, std::string_view tokenToFind, std::string_view replacementToken)
    {
        std::replace(str.begin(), str.end(), tokenToFind, replacementToken);
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
