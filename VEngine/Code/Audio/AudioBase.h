#pragma once

#include <xaudio2.h>

//Base class for audio data
struct AudioBase
{
	WAVEFORMATEXTENSIBLE waveFormat = {};
	XAUDIO2_BUFFER buffer = {};

	AudioBase() {}
	~AudioBase()
	{
		//@Todo: there's a potential issue here with how vengine is seperating out AudioBase and AudioChannel.
		//From the XAudio2 docs (https://learn.microsoft.com/en-us/windows/win32/api/xaudio2/ns-xaudio2-xaudio2_buffer#remarks),
		//they're saying that, "The data the structure points to (pAudioData and pDecodedPacketCumulativeBytes, respectively)
		//can't be freed until the buffer completes (as signaled by the IXAudio2VoiceCallback::OnBufferEnd callback)
		// or the voice is stopped or destroyed." This might mean that you'll have to look again into the audio code
		//and bring together both AudioBase and AudioChannel, that or have references to ensure that AudioBases are 
		//deleted only when everything is unloaded, which is fine for small scale projects, but will blow up
		//as soon as streaming is introduced.
		delete buffer.pAudioData;
	}
};
