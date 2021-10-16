#pragma once
#include "AudioBase.h"

//AudioInstance is essentially a song that's being played in-world, an audio file that needs to be stopped, played.
//Only one instance of a an AudioInstance filename can exist in the world.
struct AudioInstance : AudioBase
{
	bool isPlaying = false;
};
