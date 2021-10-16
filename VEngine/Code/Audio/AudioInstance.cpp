#include "AudioInstance.h"

void AudioInstance::OnBufferStart(void* pBufferContext)
{
	isPlaying = true;
}

void AudioInstance::OnBufferEnd(void* pBufferContext)
{
	isPlaying = false;
}
