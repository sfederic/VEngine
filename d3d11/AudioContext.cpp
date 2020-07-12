#include "AudioContext.h"
#include "Audio.h"

//Little-Endian (Docs say that XBOX uses big endian)
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

void AudioContext::Init()
{
	HR(XAudio2Create(&audioEngine));
	//CoInitialize(NULL); //Needs to be called before CreateMasteringVoice. Comdef throws an error.
	HR(audioEngine->CreateMasteringVoice(&masteringVoice));
}

void AudioContext::PlayAudio(AudioChunk* chunk)
{
	if (!chunk->bIsPlaying)
	{
		HR(chunk->sourceVoice->SubmitSourceBuffer(&chunk->buffer));
		HR(chunk->sourceVoice->Start(0));
	}
}

HRESULT AudioContext::FindChunk(HANDLE file, DWORD fourcc, DWORD* dwChunkSize, DWORD* dwChunkDataPosition)
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

HRESULT AudioContext::ReadChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset)
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

HRESULT AudioContext::LoadWAV(const char* filename, WAVEFORMATEXTENSIBLE& waveFormat, XAUDIO2_BUFFER& buffer)
{
	HANDLE file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if (INVALID_HANDLE_VALUE == file) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (INVALID_SET_FILE_POINTER == SetFilePointer(file, 0, NULL, FILE_BEGIN)) {
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD chunkSize;
	DWORD chunkPosition;
	HR(FindChunk(file, fourccRIFF, &chunkSize, &chunkPosition));

	DWORD fileType;
	HR(ReadChunkData(file, &fileType, sizeof(DWORD), chunkPosition));
	if (fileType != fourccWAVE) {
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

bool AudioContext::CreateAudio(const char* filename, AudioChunk* chunk)
{
	//Initilization of audio is bad if nothing is zeroed out. Source voice fails
	HR(LoadWAV(filename, chunk->waveFormat, chunk->buffer));

	HR(audioEngine->CreateSourceVoice(&chunk->sourceVoice, (WAVEFORMATEX*)&chunk->waveFormat, 0, 2.0f, &chunk->callback));

	//HR(chunk->sourceVoice->SubmitSourceBuffer(&chunk->buffer));

	return true;
}
