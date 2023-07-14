#include "vpch.h"
#include "AudioSystem.h"
#include <filesystem>
#include <cassert>
#include "AudioBase.h"
#include "AudioChannel.h"
#include "Core/Debug.h"
#include "Core/Log.h"
#include "Components/AudioComponent.h"

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

uint64_t nextChannelID = 0;

IXAudio2* audioEngine = nullptr; //Main XAudio2 sound engine
IXAudio2MasteringVoice* masteringVoice = nullptr; //Main track	

//@Todo: this is ok for one track, needs some work if multiple are needed.
//These variables are for the singular track that can play between worlds loading.
std::string persistentAudioFilename;
std::unique_ptr<AudioBase> persistentAudio;
std::unique_ptr<AudioChannel> persistentChannel;

typedef std::map<std::string, std::unique_ptr<AudioBase>> AudioMap;
AudioMap loadedAudioMap;

typedef std::map<uint64_t, std::unique_ptr<AudioChannel>> ChannelMap;
ChannelMap channelMap;

HRESULT LoadWAV(const std::string filename, WAVEFORMATEXTENSIBLE& waveFormat, XAUDIO2_BUFFER& buffer);
HRESULT FindChunk(HANDLE file, DWORD fourcc, DWORD* dwChunkSize, DWORD* dwChunkDataPosition);
HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset);

AudioChannel* CreateAudioChannel();
AudioBase* CreateAudioBase(std::string audioFilename);

void AudioSystem::Init()
{
	HR(XAudio2Create(&audioEngine));
	
#ifdef _DEBUG
	XAUDIO2_DEBUG_CONFIGURATION debug = {};
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	debug.TraceMask = XAUDIO2_LOG_ERRORS;
	audioEngine->SetDebugConfiguration(&debug);
#endif

	HR(audioEngine->CreateMasteringVoice(&masteringVoice));
}

void AudioSystem::Tick()
{
	std::vector<ChannelMap::iterator> stoppedChannels;

	for (auto it = channelMap.begin(), itEnd = channelMap.end(); it != itEnd; it++)
	{
		if (!it->second->isPlaying)
		{
			stoppedChannels.emplace_back(it);
		}
	}

	for (auto& it : stoppedChannels)
	{
		channelMap.erase(it);
	}
}

void AudioSystem::Cleanup()
{
	DeleteLoadedAudioAndChannels();

	persistentAudio.reset();
	persistentChannel.reset();
	persistentAudioFilename.clear();

	masteringVoice->DestroyVoice();
	audioEngine->Release();

	//Needs to be called after XAudio2 shutdown. Only reference I could find on this is in the DirectXTK SDK samples
	//REF:https://stackoverflow.com/questions/56091616/directx-sdk-june-2010-xaudio2-crashes-on-every-app-exit
	CoUninitialize();
}

void AudioSystem::DeleteLoadedAudioAndChannels()
{
	channelMap.clear();
	loadedAudioMap.clear();
}

AudioChannel* AudioSystem::GetChannel(uint64_t channelID)
{
	return channelMap.find(channelID)->second.get();
}

void AudioSystem::MuteAllAudio()
{
	for (auto& audio : AudioComponent::system.GetComponents())
	{
		audio->SetVolume(0.f);
	}
}

void AudioSystem::StopAllAudio()
{
	for (auto& audio : AudioComponent::system.GetComponents())
	{
		channelMap.find(audio->GetChannelID())->second->sourceVoice->Stop();
	}
}

void AudioSystem::StartAllAudio()
{
	for (auto& audio : AudioComponent::system.GetComponents())
	{
		channelMap.find(audio->GetChannelID())->second->sourceVoice->Start();
	}
}

void AudioSystem::UnmuteAllAudio()
{
	for (auto& audio : AudioComponent::system.GetComponents())
	{
		audio->SetVolume(1.f);
	}
}

void AudioSystem::FadeOutAllAudio()
{
	for (auto& audio : AudioComponent::system.GetComponents())
	{
		audio->SetToFadeOut();
	}
}

void AudioSystem::FadeInAllAudio()
{
	for (auto& audio : AudioComponent::system.GetComponents())
	{
		audio->SetToFadeIn();
	}
}

uint64_t AudioSystem::PlayAudio(const std::string filename, bool loopAudio)
{
	auto audioIt = loadedAudioMap.find(filename);
	if (audioIt == loadedAudioMap.end())
	{
		LoadAudio(filename);

		//Do a second check if audio can't be loaded.
		audioIt = loadedAudioMap.find(filename);
		if (audioIt == loadedAudioMap.end())
		{
			Log("Audio file [%s] in PlayAudio() not found.", filename.c_str());
			return 0; //@Todo: could this be an issue?
		}
	}

	AudioBase* audio = audioIt->second.get();

	auto channel = CreateAudioChannel();
	channel->isPlaying = true;

	IXAudio2SourceVoice* sourceVoice = nullptr;
	HR(audioEngine->CreateSourceVoice(&sourceVoice, (WAVEFORMATEX*)&audio->waveFormat, 0, 2.0f, channel));

	channel->sourceVoice = sourceVoice;

	if (loopAudio)
	{
		channel->isLooping = true;
		audio->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	HR(sourceVoice->SubmitSourceBuffer(&audio->buffer));
	HR(sourceVoice->Start(0));

	return nextChannelID;
}

void AudioSystem::LoadAudio(const std::string filename)
{
	std::string path = "Audio/" + filename;
	if (!std::filesystem::exists(path))
	{
		Log("Audio file [%s] not found.", path.c_str());
		return;
	}

	auto audio = CreateAudioBase(filename);

	//Initilization of audio is bad if nothing is zeroed out, source voice fails
	HR(LoadWAV(path, audio->waveFormat, audio->buffer));
}

void AudioSystem::UnloadAudio(const std::string filename)
{
	auto audioIt = loadedAudioMap.find(filename);
	assert(audioIt != loadedAudioMap.end());

	loadedAudioMap.erase(audioIt);
}

void AudioSystem::PlayPersistentAudio(std::string filename)
{
	if (persistentAudioFilename == filename)
	{
		Log("Persistent track [%s] already playing.", filename.c_str());
		return;
	}

	persistentAudioFilename = filename;

	std::string path = "Audio/" + filename;
	if (!std::filesystem::exists(path))
	{
		Log("Audio file [%s] not found.", path.c_str());
		return;
	}

	persistentAudio.reset();
	persistentAudio = std::make_unique<AudioBase>();;

	HR(LoadWAV(path, persistentAudio->waveFormat, persistentAudio->buffer));

	persistentChannel.reset();
	persistentChannel = std::make_unique<AudioChannel>();

	IXAudio2SourceVoice* sourceVoice = nullptr;
	HR(audioEngine->CreateSourceVoice(&sourceVoice, (WAVEFORMATEX*)&persistentAudio->waveFormat, 0, 2.0f, persistentChannel.get()));
	
	persistentChannel->sourceVoice = sourceVoice;

	HR(sourceVoice->SubmitSourceBuffer(&persistentAudio->buffer));
	HR(sourceVoice->Start(0));
}

HRESULT FindChunk(HANDLE file, DWORD fourcc, DWORD* dwChunkSize, DWORD* dwChunkDataPosition)
{
	if (SetFilePointer(file, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	HRESULT hr = S_OK;
	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(file, &dwChunkType, sizeof(DWORD), &dwRead, NULL)) {
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (0 == ReadFile(file, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL)) {
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case fourccRIFF:
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(file, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(file, dwChunkDataSize, NULL, FILE_CURRENT))
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			*dwChunkSize = dwChunkDataSize;
			*dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
	}

	return S_OK;
}

HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(file, bufferOffset, NULL, FILE_BEGIN)) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwRead;
	if (0 == ReadFile(file, buffer, bufferSize, &dwRead, NULL)) {
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	return hr;
}

AudioChannel* CreateAudioChannel()
{
	++nextChannelID;
	channelMap.emplace(nextChannelID, std::make_unique<AudioChannel>());
	return channelMap.find(nextChannelID)->second.get();
}

AudioBase* CreateAudioBase(std::string audioFilename)
{
	loadedAudioMap.emplace(audioFilename, std::make_unique<AudioBase>());
	return loadedAudioMap.find(audioFilename)->second.get();
}

//Ref::https://docs.microsoft.com/en-us/windows/win32/xaudio2/how-to--load-audio-data-files-in-xaudio2
HRESULT LoadWAV(const std::string filename, WAVEFORMATEXTENSIBLE& waveFormat, XAUDIO2_BUFFER& buffer)
{
	HANDLE file = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == file) 
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (INVALID_SET_FILE_POINTER == SetFilePointer(file, 0, NULL, FILE_BEGIN)) 
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD chunkSize;
	DWORD chunkPosition;
	HR(FindChunk(file, fourccRIFF, &chunkSize, &chunkPosition));

	DWORD fileType;
	HR(ReadChunkData(file, &fileType, sizeof(DWORD), chunkPosition));
	if (fileType != fourccWAVE) 
	{
		return S_FALSE;
	}

	HR(FindChunk(file, fourccFMT, &chunkSize, &chunkPosition));
	HR(ReadChunkData(file, &waveFormat, chunkSize, chunkPosition));

	FindChunk(file, fourccDATA, &chunkSize, &chunkPosition);
	BYTE* pDataBuffer = (BYTE*)malloc(chunkSize);
	ReadChunkData(file, pDataBuffer, chunkSize, chunkPosition);

	buffer.AudioBytes = chunkSize;
	buffer.pAudioData = pDataBuffer;
	buffer.Flags = XAUDIO2_END_OF_STREAM;

	return S_OK;
}
