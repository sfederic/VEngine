#include "vpch.h"
#include "ShaderItem.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderSystem.h"

ShaderItem::ShaderItem(const std::wstring vertexShaderName, const std::wstring pixelShaderName) :
    pixelShaderFilename(pixelShaderName), vertexShaderFilename(vertexShaderName)
{
    VertexShader* vs = shaderSystem.FindVertexShader(vertexShaderName);
    PixelShader* ps = shaderSystem.FindPixelShader(pixelShaderName);

    vertexShader = vs;
    pixelShader = ps;
}

ID3D11VertexShader* ShaderItem::GetVertexShader()
{
    return vertexShader->GetShader();
}

ID3D11PixelShader* ShaderItem::GetPixelShader()
{
    return pixelShader->GetShader();
}
