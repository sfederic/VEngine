#pragma once

#include <string>

//Todo: "Localisation" here is veeeery simple. Dialogue files are the option right now when you need
//multiple lines of text and events to go through. The localisation functions here basically only
//work for once off static text. Think about where a JSON string localisation system can go and
//whether it can replace .dialogue files entirely.

namespace Localise
{
	std::wstring GetString(const std::string& key, const std::string& filename);
	void SetLanguage(const std::string_view language);

	namespace Filenames
	{
		inline static const std::string reconActorLines = "ReconActorLines.json";
	}
};
