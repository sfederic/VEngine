#include "Audio.h"
#include "AudioContext.h"

void AudioBase::SetPitch(float ratio)
{
	if (ratio <= 0.f) { ratio = 1.f; }
	sourceVoice->SetFrequencyRatio(ratio);
}
