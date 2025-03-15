import Render.ShaderSystem;
import Render.VertexShader;
import Render.PixelShader;
import Render.ShaderItem;

import <filesystem>;
import <unordered_map>;
import <memory>;

void CompileAllShadersFromFile();

static std::unordered_map<std::wstring, std::unique_ptr<VertexShader>> vertexShaders;
static std::unordered_map<std::wstring, std::unique_ptr<PixelShader>> pixelShaders;
static std::unordered_map<std::string, std::unique_ptr<ShaderItem>> shaderItems;

static void CreateShaderItem(std::string name, std::wstring vertexShader, std::wstring pixelShader)
{
	shaderItems.emplace(name, std::make_unique<ShaderItem>(name, vertexShader, pixelShader));
}

void ShaderSystem::Init()
{
	CompileAllShadersFromFile();

	CreateShaderItem("Default", L"Default_vs.cso", L"Default_ps.cso");
	CreateShaderItem("DefaultClip", L"Default_vs.cso", L"TextureClip_ps.cso");
	CreateShaderItem("Unlit", L"Default_vs.cso", L"TextureClip_ps.cso");
	CreateShaderItem("Animation", L"Animation_vs.cso", L"Default_ps.cso");
	CreateShaderItem("Shadow", L"Shadows_vs.cso", L"Shadows_ps.cso");
	CreateShaderItem("ShadowAnimation", L"ShadowAnimation_vs.cso", L"Shadows_ps.cso");
	CreateShaderItem("Instance", L"Instance_vs.cso", L"Instance_ps.cso");
	CreateShaderItem("LightProbe", L"LightProbe_vs.cso", L"LightProbe_ps.cso");
	CreateShaderItem("SolidColour", L"Default_vs.cso", L"SolidColour_ps.cso");
	CreateShaderItem("UI", L"UI_vs.cso", L"TextureClip_ps.cso");
	CreateShaderItem("PostProcess", L"PostProcess_vs.cso", L"PostProcess_ps.cso");
	CreateShaderItem("Water", L"Water_vs.cso", L"Default_ps.cso");
	CreateShaderItem("Floating", L"Floating_vs.cso", L"Default_ps.cso");
	CreateShaderItem("Outline", L"Outline_vs.cso", L"SolidColour_ps.cso");
	CreateShaderItem("ScreenSpaceTexture", L"Default_vs.cso", L"ScreenSpaceTexture_ps.cso");
	CreateShaderItem("Grass", L"Grass_vs.cso", L"Default_ps.cso");
}

VertexShader* ShaderSystem::FindVertexShader(const std::wstring& filename)
{
	return vertexShaders.find(filename)->second.get();
}

PixelShader* ShaderSystem::FindPixelShader(const std::wstring& filename)
{
	return pixelShaders.find(filename)->second.get();
}

ShaderItem* ShaderSystem::FindShaderItem(const std::string& shaderItemName)
{
	return shaderItems.find(shaderItemName)->second.get();
}

std::vector<ShaderItem*> ShaderSystem::GetAllShaderItems()
{
	std::vector<ShaderItem*> output;
	for (auto& [name, shaderItem] : shaderItems)
	{
		output.push_back(shaderItem.get());
	}
	return output;
}

bool ShaderSystem::DoesShaderItemExist(std::string shaderItemName)
{
	return shaderItems.find(shaderItemName) != shaderItems.end();
}

void ShaderSystem::ClearShaders()
{
	vertexShaders.clear();
	pixelShaders.clear();
}

void CompileAllShadersFromFile()
{
	ShaderSystem::ClearShaders();

	for (auto& entry : std::filesystem::directory_iterator("Shaders/Vertex"))
	{
		auto vertexShader = std::make_unique<VertexShader>();
		vertexShader->Create(entry.path().c_str());
		vertexShaders.emplace(entry.path().filename(), std::move(vertexShader));
	}

	for (auto& entry : std::filesystem::directory_iterator("Shaders/Pixel"))
	{
		auto pixelShader = std::make_unique<PixelShader>();
		pixelShader->Create(entry.path().c_str());
		pixelShaders.emplace(entry.path().filename(), std::move(pixelShader));
	}
}
