#include "vpch.h"
#include "ShaderItem.h"
#include <cassert>
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderSystem.h"

ShaderItem::ShaderItem(const std::string shaderItemName_, 
    const std::wstring vertexShaderFilename_,
    const std::wstring pixelShaderFilename_) :
    shaderItemName(shaderItemName_),
    pixelShaderFilename(pixelShaderFilename_),
    vertexShaderFilename(vertexShaderFilename_)
{
    vertexShader = shaderSystem.FindVertexShader(vertexShaderFilename);
    pixelShader = shaderSystem.FindPixelShader(pixelShaderFilename);

    assert(vertexShader);
    assert(pixelShader);

    shaderSystem.AddShaderItem(this);
}

ID3D11VertexShader* ShaderItem::GetVertexShader()
{
    return vertexShader->GetShader();
}

ID3D11PixelShader* ShaderItem::GetPixelShader()
{
    return pixelShader->GetShader();
}
