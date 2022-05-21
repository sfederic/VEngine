#include "vpch.h"
#include "AudioBase.h"

AudioBase::AudioBase(std::string filename)
{
	audioFilename = filename;
}

AudioBase::~AudioBase()
{
	free((void*)buffer.pAudioData);
}
