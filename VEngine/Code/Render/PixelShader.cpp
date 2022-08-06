#include "vpch.h"
#include "PixelShader.h"
#include "RenderUtils.h"
#include "Debug.h"

void PixelShader::Create(const wchar_t* filename)
{
    ReadData(filename);

    HR(RenderUtils::device->CreatePixelShader(
        GetByteCodeData(),
        GetByteCodeSize(),
        nullptr,
        GetShaderAddress()));
}

void PixelShader::Reset()
{
    shader.Reset();
    byteCode.clear();
}
