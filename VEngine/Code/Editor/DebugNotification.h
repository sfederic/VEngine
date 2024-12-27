#pragma once

#include <string>

struct DebugNotification
{
	DebugNotification(std::wstring_view note) : text(note) {}

	std::wstring text;
	float timeOnScreen = 0.f;
	bool staticNotification = false; //Means the message won't get removed on timer.
};
