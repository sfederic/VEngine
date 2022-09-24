#pragma once

#include <string>

struct Mission
{
	Mission(std::wstring name_, std::wstring details_) : name(name_), details(details_) {}

	std::wstring name;
	std::wstring details;
};
