#pragma once

#include <string>

class VertexShader;
class PixelShader;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

//Vertex shader filename and pixel shader filename
typedef std::pair<std::wstring, std::wstring> ShaderItemNames;

//Helper structure when finding shaders from Shader System
class ShaderItem
{
public:
	ShaderItem(VertexShader* vs, PixelShader* ps) : vertexShader(vs), pixelShader(ps) {}

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();

private:
	VertexShader* const vertexShader = nullptr;
	PixelShader* const pixelShader = nullptr;
};

struct ShaderItems
{
	inline static const ShaderItemNames Default = std::make_pair(L"Default_vs.cso", L"Default_ps.cso");
	inline static const ShaderItemNames DefaultClip = std::make_pair(L"Default_vs.cso", L"TextureClip_ps.cso");
	inline static const ShaderItemNames Unlit = std::make_pair(L"Unlit_vs.cso", L"TextureClip_ps.cso");
	inline static const ShaderItemNames Animation = std::make_pair(L"Animation_vs.cso", L"Default_ps.cso");
	inline static const ShaderItemNames Shadow = std::make_pair(L"Shadows_vs.cso", L"Shadows_ps.cso");
	inline static const ShaderItemNames Instance = std::make_pair(L"Instance_vs.cso", L"Instance_ps.cso");
	inline static const ShaderItemNames SolidColour = std::make_pair(L"Default_vs.cso", L"SolidColour_ps.cso");
	inline static const ShaderItemNames UI = std::make_pair(L"ui_vs.cso", L"TextureClip_ps.cso");
	inline static const ShaderItemNames PostProcess = std::make_pair(L"PostProcess_vs.cso", L"PostProcess_ps.cso");
};
