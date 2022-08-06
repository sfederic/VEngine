#pragma once

#include <vector>

class Shader
{
public:
	virtual void Create(const wchar_t* filename) = 0;
	virtual void Reset() = 0;

	uint64_t GetByteCodeSize() { return byteCode.size() * sizeof(uint8_t); }
	uint8_t* GetByteCodeData() { return byteCode.data(); }

protected:
	void ReadData(const wchar_t* filename);

	std::vector<uint8_t> byteCode;
};
