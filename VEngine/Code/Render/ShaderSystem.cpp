#include "ShaderSystem.h"
#include <filesystem>
#include "Debug.h"
#include "Renderer.h"
#include "VString.h"
#include "Input.h"
#include "Log.h"
#include "Editor/DebugMenu.h"

ShaderSystem shaderSystem;

HANDLE hotreloadHandle;

ShaderSystem::ShaderSystem() : System("ShaderSystem")
{
}

void ShaderSystem::Init()
{
    CompileAllShadersFromFile();
    CreateAllShaders();
}

void ShaderSystem::Tick()
{
    if (Input::GetKeyUp(Keys::F4))
    {
        HotReloadShaders();
    }
}

ShaderItem* ShaderSystem::FindShader(std::wstring shaderName)
{
    auto shaderIt = shaderMap.find(shaderName);
    if (shaderIt == shaderMap.end())
    {
        return nullptr;
    }

    return shaderIt->second;
}

ID3DBlob* ShaderSystem::CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target)
{
	UINT compileFlags = 0;
#ifdef _DEBUG
	compileFlags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* code;
	ComPtr<ID3DBlob> error;

	D3DCompileFromFile(filename, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry, target, compileFlags, 0, &code, error.GetAddressOf());

	if (error)
	{
        const char* errMsg = (char*)error->GetBufferPointer();
        MessageBoxA(0, errMsg, "Shader Compile Error", 0);
        Log("Shader Compile Error: %s", errMsg);
    }

	return code;
}

void ShaderSystem::CreateAllShaders()
{
    for (ShaderItem& shader : shaders)
    {
        HR(renderer.device->CreateVertexShader(
            shader.vertexCode->GetBufferPointer(),
            shader.vertexCode->GetBufferSize(),
            nullptr,
            &shader.vertexShader));

        HR(renderer.device->CreatePixelShader(
            shader.pixelCode->GetBufferPointer(),
            shader.pixelCode->GetBufferSize(),
            nullptr,
            &shader.pixelShader));
    }
}

void ShaderSystem::CompileAllShadersFromFile()
{
    shaders.clear();
    shaderMap.clear();

    //File names to check for in directory (Doesn't compile the include (.hlsli) files).
    std::set<std::string> hlslFilenames = { ".hlsl" };

    for (auto& entry : std::filesystem::directory_iterator("Code/Render/Shaders/"))
    {
        ShaderItem shaderItem = {};
        if (hlslFilenames.find(entry.path().extension().string()) != hlslFilenames.end())
        {
            shaderItem.filename = entry.path().filename();
            shaders.push_back(shaderItem);
        }
    }

    UINT flags = 0;
#ifdef _DEBUG
    flags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif

    for (ShaderItem& shader : shaders)
    {
        shaderMap[shader.filename] = &shader;

        std::wstring directory = L"Code/Render/Shaders/";
        directory += shader.filename;

        const char* vsEntry = "VSMain";
        const char* vsTarget = "vs_5_0";
        shader.vertexCode = CreateShaderFromFile(directory.c_str(), vsEntry, vsTarget);

        const char* psEntry = "PSMain";
        const char* psTarget = "ps_5_0";
        shader.pixelCode = CreateShaderFromFile(directory.c_str(), psEntry, psTarget);
    }
}

void ShaderSystem::CleanUpShaders()
{
    for (int i = 0; i < shaders.size(); i++)
    {
        shaders[i].vertexCode->Release();
        shaders[i].pixelCode->Release();

        shaders[i].vertexShader->Release();
        shaders[i].pixelShader->Release();
    }
}

//@Todo: the code here is junk, but it's allowing the engine to not crash when hotreloading shader
//by using the previous shader data that was compiled without errors. Try to cleanup.
void ShaderSystem::HotReloadShaders()
{
    debugMenu.AddNotification(L"Shader reload start...");

    std::wstring shaderRecompileMsg = L"Shader reload complete";

    //@Todo: can eventually reload shaders based on their last write time.
    //For now it's ok to just reload them all, there aren't too many.
    /*for (auto& entry : std::filesystem::directory_iterator("Code/Render/Shaders/"))
    {
        auto writeTime = entry.last_write_time();
    }*/

    UINT flags = 0;
#ifdef _DEBUG
    flags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif

    std::vector<ShaderItem*> shadersToRecompile;

    for (ShaderItem& shader : shaders)
    {
        std::wstring directory = L"Code/Render/Shaders/";
        directory += shader.filename;

        const char* vsEntry = "VSMain";
        const char* vsTarget = "vs_5_0";
        ID3DBlob* vertexCode = CreateShaderFromFile(directory.c_str(), vsEntry, vsTarget);
        if (vertexCode == nullptr)
        {
            shaderRecompileMsg = L"Shader reload failed.";
            continue;
        }

        const char* psEntry = "PSMain";
        const char* psTarget = "ps_5_0";
        ID3DBlob* pixelCode = CreateShaderFromFile(directory.c_str(), psEntry, psTarget);
        if (pixelCode == nullptr)
        {
            shaderRecompileMsg = L"Shader reload failed.";
            vertexCode->Release();
            continue;
        }

        shader.pixelCode->Release();
        shader.pixelShader->Release();

        shader.vertexCode->Release();
        shader.vertexShader->Release();

        shader.vertexCode = vertexCode;
        shader.pixelCode = pixelCode;

        shadersToRecompile.push_back(&shader);
    }

    for (ShaderItem* shader : shadersToRecompile)
    {
        HR(renderer.device->CreateVertexShader(
            shader->vertexCode->GetBufferPointer(),
            shader->vertexCode->GetBufferSize(),
            nullptr,
            &shader->vertexShader));

        HR(renderer.device->CreatePixelShader(
            shader->pixelCode->GetBufferPointer(),
            shader->pixelCode->GetBufferSize(),
            nullptr,
            &shader->pixelShader));
    }

    debugMenu.AddNotification(shaderRecompileMsg);
}
