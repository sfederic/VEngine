#include "vpch.h"
#include "ShaderSystem.h"
#include <filesystem>
#include "VertexShader.h"
#include "PixelShader.h"

ShaderSystem shaderSystem;

void ShaderSystem::Init()
{
    CompileAllShadersFromFile();
}

VertexShader* ShaderSystem::FindVertexShader(const std::wstring filename)
{
    return vertexShaders[filename].get();
}

PixelShader* ShaderSystem::FindPixelShader(const std::wstring filename)
{
    return pixelShaders[filename].get();
}

ShaderItem ShaderSystem::FindShader(ShaderItemNames shaderPairNames)
{
    VertexShader* vs = vertexShaders[shaderPairNames.first].get();
    PixelShader* ps = pixelShaders[shaderPairNames.second].get();

    return ShaderItem(vs, ps);
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
