#include "vpch.h"
#include "ShaderPair.h"
#include "VertexShader.h"
#include "PixelShader.h"

ID3D11VertexShader* ShaderPair::GetVertexShader()
{
    return vertexShader->GetShader();
}

ID3D11PixelShader* ShaderPair::GetPixelShader()
{
    return pixelShader->GetShader();
}
