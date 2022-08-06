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

    ShaderItems::Default = ShaderItem(L"Default_vs.cso", L"Default_ps.cso");
    ShaderItems::DefaultClip = ShaderItem(L"Default_vs.cso", L"TextureClip_ps.cso");
    ShaderItems::Unlit = ShaderItem(L"Unlit_vs.cso", L"TextureClip_ps.cso");
    ShaderItems::Animation = ShaderItem(L"Animation_vs.cso", L"Default_ps.cso");
    ShaderItems::Shadow = ShaderItem(L"Shadows_vs.cso", L"Shadows_ps.cso");
    ShaderItems::Instance = ShaderItem(L"Instance_vs.cso", L"Instance_ps.cso");
    ShaderItems::SolidColour = ShaderItem(L"Default_vs.cso", L"SolidColour_ps.cso");
    ShaderItems::UI = ShaderItem(L"ui_vs.cso", L"TextureClip_ps.cso");
    ShaderItems::PostProcess = ShaderItem(L"PostProcess_vs.cso", L"PostProcess_ps.cso");
}

VertexShader* ShaderSystem::FindVertexShader(const std::wstring filename)
{
    return vertexShaders[filename].get();
}

PixelShader* ShaderSystem::FindPixelShader(const std::wstring filename)
{
    return pixelShaders[filename].get();
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
