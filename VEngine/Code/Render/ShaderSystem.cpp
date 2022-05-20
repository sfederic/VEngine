#include "ShaderSystem.h"
#include <filesystem>
#include <set>
#include "Debug.h"
#include "RenderUtils.h"
#include "Input.h"
#include "Log.h"
#include "Profile.h"
#include "Editor/DebugMenu.h"

ShaderSystem shaderSystem;

std::wstring shaderDirectory = L"Code/Render/Shaders/";

const char* vsEntry = "VSMain";
const char* vsTarget = "vs_5_0";

const char* psEntry = "PSMain";
const char* psTarget = "ps_5_0";

const char* csEntry = "CSMain";
const char* csTarget = "cs_5_0";

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
        Log("%S shader not found.", shaderName);
        //@Todo: keep an eye on this. Might be better to always return nullptr
        return shaderMap.find(L"DefaultShader.hlsl")->second;
    }

    return shaderIt->second;
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

void ShaderSystem::CreateAllShaders()
{
    for (ShaderItem* shader : shaders)
    {
        HR(RenderUtils::device->CreateVertexShader(
            shader->vertexCode->GetBufferPointer(),
            shader->vertexCode->GetBufferSize(),
            nullptr,
            &shader->vertexShader));

        HR(RenderUtils::device->CreatePixelShader(
            shader->pixelCode->GetBufferPointer(),
            shader->pixelCode->GetBufferSize(),
            nullptr,
            &shader->pixelShader));
    }

    for (ShaderItem* shader : computeShaders)
    {
        HR(RenderUtils::device->CreateComputeShader(
            shader->computeCode->GetBufferPointer(),
            shader->computeCode->GetBufferSize(),
            nullptr,
            &shader->computeShader));
    }
}

void ShaderSystem::CompileAllShadersFromFile()
{
    shaders.clear();
    computeShaders.clear();
    shaderMap.clear();

    //File names to check for in directory (Doesn't compile the include (.hlsli) files).
    std::set<std::string> hlslFilenames = { ".hlsl" };

    for (auto& entry : std::filesystem::directory_iterator("Code/Render/Shaders/"))
    {
        if (hlslFilenames.find(entry.path().extension().string()) != hlslFilenames.end())
        {
            auto shaderItem = new ShaderItem();
            shaderItem->filename = entry.path().filename();
            shaders.push_back(shaderItem);
        }
    }

    for (ShaderItem* shader : shaders)
    {
        shaderMap[shader->filename] = shader;

        std::wstring path = shaderDirectory + shader->filename;

        shader->vertexCode = CreateShaderFromFile(path.c_str(), vsEntry, vsTarget);
        shader->pixelCode = CreateShaderFromFile(path.c_str(), psEntry, psTarget);
    }

    //Get compute shaders
    for (auto& entry : std::filesystem::directory_iterator("Code/Render/Shaders/Compute"))
    {
        auto shaderItem = new ShaderItem();
        if (hlslFilenames.find(entry.path().extension().string()) != hlslFilenames.end())
        {
            shaderItem->filename = L"Compute/" + entry.path().filename().wstring();
            computeShaders.push_back(shaderItem);
        }
    }

    for (ShaderItem* computeShader : computeShaders)
    {
        shaderMap[computeShader->filename] = computeShader;

        std::wstring path = shaderDirectory + computeShader->filename;

        computeShader->computeCode = CreateShaderFromFile(path.c_str(), csEntry, csTarget);
    }
}

void ShaderSystem::CleanUpShaders()
{
    for (int i = 0; i < shaders.size(); i++)
    {
        shaders[i]->vertexCode->Release();
        shaders[i]->pixelCode->Release();

        shaders[i]->vertexShader->Release();
        shaders[i]->pixelShader->Release();
    }

    for (int i = 0; i < computeShaders.size(); i++)
    {
        computeShaders[i]->computeCode->Release();
        computeShaders[i]->computeShader->Release();
    }
}

void ShaderSystem::HotReloadShaders()
{
    auto startTime = Profile::QuickStart();

    debugMenu.AddNotification(L"Shader reload start...");

    std::wstring shaderRecompileMsg = L"Shader reload complete";

    //@Todo: can reload shaders based on their last write time.
    //For now it's ok to just reload them all, there aren't too many.
    /*for (auto& entry : std::filesystem::directory_iterator("Code/Render/Shaders/"))
    {
        auto writeTime = entry.last_write_time();
        //recompile shaders within a timeframe of 1 minute from current time for example
    }*/

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

        shader->pixelCode->Release();
        shader->pixelShader->Release();

        shader->vertexCode->Release();
        shader->vertexShader->Release();

        shader->vertexCode = vertexCode;
        shader->pixelCode = pixelCode;

        shadersToRecompile.push_back(shader);
    }

    for (ShaderItem* shader : shadersToRecompile)
    {
        HR(RenderUtils::device->CreateVertexShader(
            shader->vertexCode->GetBufferPointer(),
            shader->vertexCode->GetBufferSize(),
            nullptr,
            &shader->vertexShader));

        HR(RenderUtils::device->CreatePixelShader(
            shader->pixelCode->GetBufferPointer(),
            shader->pixelCode->GetBufferSize(),
            nullptr,
            &shader->pixelShader));
    }

    debugMenu.AddNotification(shaderRecompileMsg);

    auto elapsedTime = Profile::QuickEnd(startTime);
    Log(L"Shader hotreload took [%f] seconds", elapsedTime);
}
