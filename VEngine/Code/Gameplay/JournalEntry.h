#pragma once

#include <string>
#include "Core/Properties.h"

struct JournalEntry
{
	std::wstring title;
	std::wstring text;
	std::string image;

	Properties GetProps()
	{
		Properties props;
		props.Add("Title", &title);
		props.Add("Text", &text);
		props.Add("Image", &image);
		return props;
	}
};
