#pragma once

#include <wrl.h>
#include "Shader.h"

struct ID3D11PixelShader;

class PixelShader : public Shader
{
public:
	ID3D11PixelShader* GetShader() { return shader.Get(); }
	ID3D11PixelShader** GetShaderAddress() { return shader.GetAddressOf(); }

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;
};
