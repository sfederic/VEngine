#pragma once

#include <vector>
#include <string>

class Shader
{
public:
	virtual void Create(const std::wstring filename) = 0;
	virtual void Reset() = 0;

	uint64_t GetByteCodeSize() { return byteCode.size() * sizeof(uint8_t); }
	uint8_t* GetByteCodeData() { return byteCode.data(); }

	std::wstring hlslFilepath;

protected:
	void ReadData(const std::wstring filename);

	std::vector<uint8_t> byteCode;
};
