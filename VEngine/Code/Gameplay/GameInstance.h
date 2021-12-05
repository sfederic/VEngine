#pragma once

//Instance holding data over the entirety of the game
struct GameInstance
{
	inline static int currentHour;
	inline static int currentMinute;

	static void ProgressTime();
	static void ResetTime();
};
