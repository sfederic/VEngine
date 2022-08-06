#include "vpch.h"
#include "ShaderSystem.h"
#include <filesystem>
#include <fstream>
#include <d3dcompiler.h>
#include <d3dcommon.h>
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

ID3DBlob* CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target)
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

void ShaderSystem::HotReloadShaders()
{
    //@Todo:
}
