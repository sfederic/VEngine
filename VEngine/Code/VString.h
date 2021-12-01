#include <string>

//Both string functions taken from:
//https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string

//convert from std::string to std::wstring
std::wstring stows(const std::string& str);

//convert from std::wstring to std::string
std::string wstos(const std::wstring& wstr);
