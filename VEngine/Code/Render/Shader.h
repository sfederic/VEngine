#pragma once

#include <vector>

class Shader
{
private:
	std::vector<uint8_t> byteCode;

public:
	uint64_t GetByteCodeSize() { return byteCode.size() * sizeof(uint8_t); }
	uint8_t* GetByteCodeData() { return byteCode.data(); }

	void ReadData(const wchar_t* filename);
};
