export module Render.VertexShader;

#include <wrl.h>
#include <d3d11.h>

import Render.Shader;

class VertexShader : public Shader
{
public:
	virtual void Create(const std::wstring filename) override;
	virtual void Reset() override;

	ID3D11VertexShader* GetShader() { return shader.Get(); }
	ID3D11VertexShader** GetShaderAddress() { return shader.GetAddressOf(); }

	ID3D11InputLayout* GetInputLayout() const { return inputLayout.Get(); }

private:
	void CreateInputLayoutDescFromVertexShaderSignature();

	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};
