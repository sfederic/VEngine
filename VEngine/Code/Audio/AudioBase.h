#pragma once
#include <xaudio2.h>
#include <string>

//Base class for audio data
struct AudioBase
{
	WAVEFORMATEXTENSIBLE waveFormat = {};
	XAUDIO2_BUFFER buffer = {};

	AudioBase();
	~AudioBase();
};
