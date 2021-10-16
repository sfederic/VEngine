#pragma once
#include <string>
#include <map>
#include <vector>
#include "AudioBase.h"
#include "SourceVoice.h"

//TODO: wav files might be a bit too big with git, look into .ogg 
//https://github.com/nothings/stb/blob/master/stb_vorbis.c

struct AudioSystem
{
	IXAudio2* audioEngine = nullptr; //Main XAudio2 sound engine
	IXAudio2MasteringVoice* masteringVoice = nullptr; //Main track	

	//Maps audio filename to AudioBase structure
	std::map<std::string, AudioBase*> loadedAudioFilesMap;

	inline static const int MAX_SOURCE_VOICES = 16;
	//This acts as a buffer for 'One-shot' audio files being played.
	SourceVoice sourceVoices[MAX_SOURCE_VOICES];

	void Init();
	void Cleanup();

	//Needs to be called when new world's are loaded in to clear loaded audio structs
	void CleanupAllLoadedAudio();

	//Plays audio as a 'one shot'. Creates a sourcevoice and fires its Start()
	void PlayAudioOneShot(AudioBase* audio);

	//Finds an audio file by filename and creates the audio if the map doesn't contain the filename.
	AudioBase* FindAudio(const std::string filename);

private:
	HRESULT LoadWAV(const std::string filename, WAVEFORMATEXTENSIBLE& waveFormat, XAUDIO2_BUFFER& buffer);
	HRESULT FindChunk(HANDLE file, DWORD fourcc, DWORD* dwChunkSize, DWORD* dwChunkDataPosition);
	HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset);
};

extern AudioSystem audioSystem;
