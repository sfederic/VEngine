#include "vpch.h"
#include "Shader.h"

//Ref:https://raw.githubusercontent.com/wiki/microsoft/DirectXTK/ReadData.h
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
