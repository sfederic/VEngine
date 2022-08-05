#pragma once

#include <wrl.h>
#include "Shader.h"

struct ID3D11VertexShader;

class VertexShader : public Shader
{
public:
	ID3D11VertexShader* GetShader() { return shader.Get(); }
	ID3D11VertexShader** GetShaderAddress() { return shader.GetAddressOf(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
};
