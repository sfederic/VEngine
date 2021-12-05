#include "VString.h"
#include <locale>
#include <stdarg.h>
#include <vadefs.h>
#include <codecvt>

namespace VString
{
    std::wstring stows(const std::string& str)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.from_bytes(str);
    }

    std::string wstos(const std::wstring& wstr)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(wstr);
    }

    std::wstring wformat(const std::wstring wstr, va_list args)
    {
        wchar_t msg[1024];
        _vsnwprintf_s(msg, 1024, wstr.c_str(), args);

        return std::wstring(msg);
    }

    std::string format(const std::string str, va_list args)
    {
        char msg[1024];
        vsnprintf_s(msg, 1024, str.c_str(), args);

        return std::string(msg);
    }
}
