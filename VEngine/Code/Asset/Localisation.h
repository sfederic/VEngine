#pragma once

#include <string>

namespace Localise
{
	std::wstring GetString(const std::string& key);
	void SetLanguage(const std::string_view language);
};
