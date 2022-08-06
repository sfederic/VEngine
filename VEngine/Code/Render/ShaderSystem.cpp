#include "vpch.h"
#include "ShaderSystem.h"
#include <filesystem>
#include "VertexShader.h"
#include "PixelShader.h"
#include "ShaderItem.h"

ShaderSystem shaderSystem;

void ShaderSystem::Init()
{
    CompileAllShadersFromFile();

    ShaderItems::Default = new ShaderItem("Default", L"Default_vs.cso", L"Default_ps.cso");
    ShaderItems::DefaultClip = new ShaderItem("DefaultClip", L"Default_vs.cso", L"TextureClip_ps.cso");
    ShaderItems::Unlit = new ShaderItem("Unlit", L"Default_vs.cso", L"TextureClip_ps.cso");
    ShaderItems::Animation = new ShaderItem("Animation", L"Animation_vs.cso", L"Default_ps.cso");
    ShaderItems::Shadow = new ShaderItem("Shadow", L"Shadows_vs.cso", L"Shadows_ps.cso");
    ShaderItems::Instance = new ShaderItem("Instance", L"Instance_vs.cso", L"Instance_ps.cso");
    ShaderItems::SolidColour = new ShaderItem("SolidColour", L"Default_vs.cso", L"SolidColour_ps.cso");
    ShaderItems::UI = new ShaderItem("UI", L"Default_vs.cso", L"TextureClip_ps.cso");
    ShaderItems::PostProcess = new ShaderItem("PostProcess", L"PostProcess_vs.cso", L"PostProcess_ps.cso");
}

VertexShader* ShaderSystem::FindVertexShader(const std::wstring filename)
{
    assert(vertexShaders.find(filename) != vertexShaders.end());
    return vertexShaders[filename].get();
}

PixelShader* ShaderSystem::FindPixelShader(const std::wstring filename)
{
    assert(pixelShaders.find(filename) != pixelShaders.end());
    return pixelShaders[filename].get();
}

void ShaderSystem::AddShaderItem(ShaderItem* shaderItem)
{
    shaderItems.emplace(shaderItem->GetName(), shaderItem);
}

ShaderItem* ShaderSystem::FindShaderItem(std::string shaderItemName)
{
    assert(shaderItems.find(shaderItemName) != shaderItems.end());
    return shaderItems[shaderItemName];
}

void ShaderSystem::CompileAllShadersFromFile()
{
    ClearShaders();

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

void ShaderSystem::ClearShaders()
{
    vertexShaders.clear();
    pixelShaders.clear();
}
