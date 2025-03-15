export module Render.Shader;

import <vector>;
import <string>;
import <fstream>;

export class Shader
{
public:
	virtual void Create(const std::wstring filename) = 0;
	virtual void Reset() = 0;

	uint64_t GetByteCodeSize() { return byteCode.size() * sizeof(uint8_t); }
	uint8_t* GetByteCodeData() { return byteCode.data(); }

protected:
	void ReadData(const std::wstring filename);

	std::vector<uint8_t> byteCode;
};

void Shader::ReadData(const std::wstring filename)
{
	std::ifstream inFile(filename, std::ios::in | std::ios::binary | std::ios::ate);

	if (!inFile)
	{
		throw std::runtime_error("No file found");
	}

	const std::streampos len = inFile.tellg();

	byteCode.clear();
	byteCode.resize(size_t(len));

	inFile.seekg(0, std::ios::beg);

	inFile.read(reinterpret_cast<char*>(byteCode.data()), len);

	inFile.close();
}
