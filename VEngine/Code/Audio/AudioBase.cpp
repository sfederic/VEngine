#include "vpch.h"
#include "AudioBase.h"

AudioBase::AudioBase()
{
}

AudioBase::~AudioBase()
{
	free((void*)buffer.pAudioData);
}
