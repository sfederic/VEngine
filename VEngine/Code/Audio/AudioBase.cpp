#include "vpch.h"
#include "AudioBase.h"

AudioBase::AudioBase()
{
}

AudioBase::~AudioBase()
{
	delete buffer.pAudioData;
}
