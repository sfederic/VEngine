#include "vpch.h"
#include "AudioBase.h"

AudioBase::AudioBase()
{
}

AudioBase::~AudioBase()
{
	//@Todo: don't trust this raw delete, or the destructor.
	delete buffer.pAudioData;
}
