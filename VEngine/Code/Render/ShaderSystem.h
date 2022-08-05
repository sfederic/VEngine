#pragma once

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <d3dcompiler.h>
#include "System.h"
#include "Render/ShaderPair.h"

struct VertexShader;
struct PixelShader;

struct ShaderSystem : System
{
public:
	ShaderSystem() : System("ShaderSystem") {}
	void Init();
	void Tick();

	VertexShader* FindVertexShader(const std::wstring filename);
	PixelShader* FindPixelShader(const std::wstring filename);
	ShaderPair FindShaderPair(ShaderPairNames shaderPairNames);

	void ClearShaders();

private:
	ID3DBlob* CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target);
	void CompileAllShadersFromFile();
	void HotReloadShaders();

	std::map<std::wstring, std::unique_ptr<VertexShader>> vertexShaders;
	std::map<std::wstring, std::unique_ptr<PixelShader>> pixelShaders;
};

extern ShaderSystem shaderSystem;
