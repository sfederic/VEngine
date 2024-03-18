#include "vpch.h"
#include "ShaderSystem.h"
#include <filesystem>
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderItem.h"

void CompileAllShadersFromFile();

static std::unordered_map<std::wstring, std::unique_ptr<VertexShader>> vertexShaders;
static std::unordered_map<std::wstring, std::unique_ptr<PixelShader>> pixelShaders;
static std::unordered_map<std::string, std::unique_ptr<ShaderItem>> shaderItems;

void ShaderSystem::Init()
{
	CompileAllShadersFromFile();

	ShaderItems::Default = new ShaderItem("Default", L"Default_vs.cso", L"Default_ps.cso");
	ShaderItems::DefaultClip = new ShaderItem("DefaultClip", L"Default_vs.cso", L"TextureClip_ps.cso");
	ShaderItems::Unlit = new ShaderItem("Unlit", L"Default_vs.cso", L"TextureClip_ps.cso");
	ShaderItems::Animation = new ShaderItem("Animation", L"Animation_vs.cso", L"Default_ps.cso");
	ShaderItems::Shadow = new ShaderItem("Shadow", L"Shadows_vs.cso", L"Shadows_ps.cso");
	ShaderItems::ShadowAnimation = new ShaderItem("ShadowAnimation", L"ShadowAnimation_vs.cso", L"Shadows_ps.cso");
	ShaderItems::Instance = new ShaderItem("Instance", L"Instance_vs.cso", L"Instance_ps.cso");
	ShaderItems::LightProbe = new ShaderItem("LightProbe", L"LightProbe_vs.cso", L"LightProbe_ps.cso");
	ShaderItems::SolidColour = new ShaderItem("SolidColour", L"Default_vs.cso", L"SolidColour_ps.cso");
	ShaderItems::UI = new ShaderItem("UI", L"UI_vs.cso", L"TextureClip_ps.cso");
	ShaderItems::PostProcess = new ShaderItem("PostProcess", L"PostProcess_vs.cso", L"PostProcess_ps.cso");
	ShaderItems::Water = new ShaderItem("Water", L"Water_vs.cso", L"Default_ps.cso");
	ShaderItems::Floating = new ShaderItem("Floating", L"Floating_vs.cso", L"Default_ps.cso");
	ShaderItems::Outline = new ShaderItem("Outline", L"Outline_vs.cso", L"SolidColour_ps.cso");
	ShaderItems::ScreenSpaceTexture = new ShaderItem("ScreenSpaceTexture", L"Default_vs.cso", L"ScreenSpaceTexture_ps.cso");
}

VertexShader* ShaderSystem::FindVertexShader(const std::wstring& filename)
{
	return vertexShaders.find(filename)->second.get();
}

PixelShader* ShaderSystem::FindPixelShader(const std::wstring& filename)
{
	return pixelShaders.find(filename)->second.get();
}

void ShaderSystem::AddShaderItem(ShaderItem* shaderItem)
{
	const auto shaderItemName = shaderItem->GetName();
	//Duplicate shader item name
	assert(shaderItems.find(shaderItemName) == shaderItems.end());
	shaderItems.emplace(shaderItemName, shaderItem);
}

ShaderItem* ShaderSystem::FindShaderItem(const std::string& shaderItemName)
{
	return shaderItems.find(shaderItemName)->second.get();
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
