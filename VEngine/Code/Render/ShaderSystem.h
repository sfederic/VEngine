#pragma once

#include <string>
#include <memory>
#include <map>
#include "System.h"

struct VertexShader;
struct PixelShader;
class ShaderItem;

class ShaderSystem : public System
{
public:
	ShaderSystem() : System("ShaderSystem") {}
	void Init();
	void Tick();

	VertexShader* FindVertexShader(const std::wstring filename);
	PixelShader* FindPixelShader(const std::wstring filename);

	void AddShaderItem(ShaderItem* shaderItem);
	std::shared_ptr<ShaderItem> FindShaderItem(std::string shaderItemName);

	void ClearShaders();

private:
	void CompileAllShadersFromFile();
	void HotreloadShaders();
	void RecompileShaderTypesForHotreload(const std::string shaderType, const std::string version);

	std::map<std::wstring, std::unique_ptr<VertexShader>> vertexShaders;
	std::map<std::wstring, std::unique_ptr<PixelShader>> pixelShaders;

	std::map<std::string, std::shared_ptr<ShaderItem>> shaderItems;
};

extern ShaderSystem shaderSystem;
