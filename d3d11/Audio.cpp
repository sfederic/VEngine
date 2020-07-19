#include "Audio.h"
#include "AudioSystem.h"

void AudioBase::SetPitch(float ratio)
{
	if (ratio <= 0.f) { ratio = 1.f; }
	sourceVoice->SetFrequencyRatio(ratio);
}
