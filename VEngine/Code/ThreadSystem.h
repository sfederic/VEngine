#pragma once
#include <functional>

namespace ThreadSystem
{
	//Iterate over all active Frame Threads and join them.
	void Tick();

	uint32_t GetNumHardwareThreads();

	//Normal threads that fire off and cleanup when they're done, independant of frames.
	void CreateThread(std::function<void()> function);

	//Frame Threads are threads that join at the end of a frame.
	void CreateFrameThread(std::function<void()> function);
};
