#pragma once
#include "System.h"
#include <functional>

struct ThreadSystem : System
{
	ThreadSystem();

	//Iterate over all active Frame Threads and join them.
	static void Tick();

	static uint32_t GetNumHardwareThreads();

	//Normal threads that fire off and cleanup when they're done, independant of frames.
	static void CreateThread(std::function<void()> function);

	//Frame Threads are threads that join at the end of a frame.
	static void CreateFrameThread(std::function<void()> function);
};

extern ThreadSystem threadSystem;
