#include "vpch.h"
#include "Shader.h"
#include <fstream>

//Ref:https://raw.githubusercontent.com/wiki/microsoft/DirectXTK/ReadData.h
void Shader::ReadData(const wchar_t* filename)
{
    std::ifstream inFile(filename, std::ios::in | std::ios::binary | std::ios::ate);

    if (!inFile)
    {
        throw std::runtime_error("ReadData");
    }

    const std::streampos len = inFile.tellg();
    if (!inFile)
    {
        throw std::runtime_error("ReadData");
    }

    std::vector<uint8_t> blob;
    blob.resize(size_t(len));

    inFile.seekg(0, std::ios::beg);
    if (!inFile)
    {
        throw std::runtime_error("ReadData");
    }

    inFile.read(reinterpret_cast<char*>(blob.data()), len);
    if (!inFile)
    {
        throw std::runtime_error("ReadData");
    }

    inFile.close();

    byteCode = blob;
}
