#pragma once

#include "DXUtil.h"
#include <xaudio2.h>

void playAudio(const char* filename);
HRESULT findChunk(HANDLE file, DWORD fourcc, DWORD* dwChunkSize, DWORD* dwChunkDataPosition);
HRESULT readChunkData(HANDLE file, void* buffer, DWORD bufferSize, DWORD bufferOffset);
HRESULT loadWAV(const char* filename, WAVEFORMATEXTENSIBLE& wfx, XAUDIO2_BUFFER& buffer);