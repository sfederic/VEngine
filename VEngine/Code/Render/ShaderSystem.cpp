#include "vpch.h"
#include "ShaderSystem.h"
#include <filesystem>
#include <algorithm>
#include <execution>
#include <execution>
#include <fstream>
#include <set>
#include "Debug.h"
#include "RenderUtils.h"
#include "Input.h"
#include "Log.h"
#include "Profile.h"
#include "Editor/DebugMenu.h"
#include "Render/VertexShader.h"
#include "Render/PixelShader.h"

ShaderSystem shaderSystem;

void ShaderSystem::Init()
{
    CompileAllShadersFromFile();
}

void ShaderSystem::Tick()
{
    if (Input::GetKeyUp(Keys::F4))
    {
        HotReloadShaders();
    }
}

VertexShader* ShaderSystem::FindVertexShader(const std::wstring filename)
{
    return vertexShaders[filename].get();
}

PixelShader* ShaderSystem::FindPixelShader(const std::wstring filename)
{
    return pixelShaders[filename].get();
}

ShaderPair ShaderSystem::FindShaderPair(ShaderPairNames shaderPairNames)
{
    VertexShader* vs = vertexShaders[shaderPairNames.first].get();
    PixelShader* ps = pixelShaders[shaderPairNames.second].get();

    return ShaderPair(vs, ps);
}

ID3DBlob* ShaderSystem::CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target)
{
	UINT compileFlags = 0;
#ifdef _DEBUG
	compileFlags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* code = nullptr;
	ID3DBlob* error = nullptr;

	D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry, target, compileFlags, 0, &code, &error);

	if (error)
	{
        const char* errMsg = (char*)error->GetBufferPointer();
        MessageBoxA(0, errMsg, "Shader Compile Error", 0);
        Log("Shader Compile Error: %s", errMsg);
    }

	return code;
}

void ShaderSystem::CompileAllShadersFromFile()
{
    ClearShaders();

    //Go through all vertex shaders
    for (auto& entry : std::filesystem::directory_iterator("Shaders/Vertex"))
    {
        auto vertexShader = std::make_unique<VertexShader>();

        vertexShader->ReadData(entry.path().c_str());

        HR(RenderUtils::device->CreateVertexShader(
            vertexShader->GetByteCodeData(),
            vertexShader->GetByteCodeSize(),
            nullptr,
            vertexShader->GetShaderAddress()));

        vertexShaders.emplace(entry.path().filename(), std::move(vertexShader));
    }

    //Go through all pixel shaders
    for (auto& entry : std::filesystem::directory_iterator("Shaders/Pixel"))
    {
        auto pixelShader = std::make_unique<PixelShader>();

        pixelShader->ReadData(entry.path().c_str());

        HR(RenderUtils::device->CreatePixelShader(
            pixelShader->GetByteCodeData(),
            pixelShader->GetByteCodeSize(),
            nullptr,
            pixelShader->GetShaderAddress()));

        pixelShaders.emplace(entry.path().filename(), std::move(pixelShader));
    }
}

void ShaderSystem::ClearShaders()
{
    vertexShaders.clear();
    pixelShaders.clear();
}

void ShaderSystem::HotReloadShaders()
{
   /* auto startTime = Profile::QuickStart();

    debugMenu.AddNotification(L"Shader reload start...");

    std::wstring shaderRecompileMsg = L"Shader reload complete";

    std::vector<ShaderItem*> shadersToRecompile;

    for (ShaderItem* shader : shaders)
    {
        std::wstring path = shaderDirectory + shader->filename;

        ID3DBlob* vertexCode = CreateShaderFromFile(path.c_str(), vsEntry, vsTarget);
        if (vertexCode == nullptr)
        {
            shaderRecompileMsg = L"Shader reload failed.";
            continue;
        }

        ID3DBlob* pixelCode = CreateShaderFromFile(path.c_str(), psEntry, psTarget);
        if (pixelCode == nullptr)
        {
            shaderRecompileMsg = L"Shader reload failed.";
            vertexCode->Release();
            continue;
        }

        shader->pixelShader->Release();
        shader->vertexShader->Release();

        shadersToRecompile.push_back(shader);
    }

    for (ShaderItem* shader : shadersToRecompile)
    {
    }

    debugMenu.AddNotification(shaderRecompileMsg);

    auto elapsedTime = Profile::QuickEnd(startTime);
    Log(L"Shader hotreload took [%f] seconds", elapsedTime);*/
}
