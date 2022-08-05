#pragma once

#include <string>

class VertexShader;
class PixelShader;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

//Vertex shader filename and pixel shader filename
typedef std::pair<std::wstring, std::wstring> ShaderPairNames;

//Helper structure when finding shaders from Shader System
class ShaderPair
{
public:
	ShaderPair(VertexShader* vs, PixelShader* ps) : vertexShader(vs), pixelShader(ps) {}

	ID3D11VertexShader* GetVertexShader();
	ID3D11PixelShader* GetPixelShader();

private:
	VertexShader* const vertexShader = nullptr;
	PixelShader* const pixelShader = nullptr;
};

struct ShaderPairs
{
	inline static const ShaderPairNames Default = std::make_pair(L"Default_vs.cso", L"Default_ps.cso");
	inline static const ShaderPairNames DefaultClip = std::make_pair(L"Default_vs.cso", L"TextureClip_ps.cso");
	inline static const ShaderPairNames Unlit = std::make_pair(L"Unlit_vs.cso", L"TextureClip_ps.cso");
	inline static const ShaderPairNames Animation = std::make_pair(L"Animation_vs.cso", L"Default_ps.cso");
	inline static const ShaderPairNames Shadow = std::make_pair(L"Shadows_vs.cso", L"TextureClip_ps.cso");
	inline static const ShaderPairNames Instance = std::make_pair(L"Instance_vs.cso", L"Instance_ps.cso");
	inline static const ShaderPairNames SolidColour = std::make_pair(L"Default_vs.cso", L"SolidColour_ps.cso");
	inline static const ShaderPairNames UI = std::make_pair(L"ui_vs.cso", L"TextureClip_ps.cso");
	inline static const ShaderPairNames PostProcess = std::make_pair(L"PostProcess_vs.cso", L"PostProcess_ps.cso");
};
