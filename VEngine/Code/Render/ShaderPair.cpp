#include "vpch.h"
#include "ShaderItem.h"
#include "VertexShader.h"
#include "PixelShader.h"

ID3D11VertexShader* ShaderItem::GetVertexShader()
{
    return vertexShader->GetShader();
}

ID3D11PixelShader* ShaderItem::GetPixelShader()
{
    return pixelShader->GetShader();
}
