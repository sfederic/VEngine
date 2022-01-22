#include "ShaderSystem.h"
#include <filesystem>
#include "Debug.h"
#include "Renderer.h"
#include "VString.h"
#include "Input.h"
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
    InitHotLoading();
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

    for (int i = 0; i < shaders.size(); i++)
    {
        shaderMap[shaders[i].filename] = &shaders[i];

        std::wstring directory;
        directory += L"Code/Render/Shaders/";
        directory += shaders[i].filename;

        const char* vsEntry = "VSMain";
        const char* vsTarget = "vs_5_0";
        shaders[i].vertexCode = CreateShaderFromFile(directory.c_str(), vsEntry, vsTarget);
        assert(shaders[i].vertexCode);

        const char* psEntry = "PSMain";
        const char* psTarget = "ps_5_0";
        shaders[i].pixelCode = CreateShaderFromFile(directory.c_str(), psEntry, psTarget);
        assert(shaders[i].pixelCode);
    }
}

void ShaderSystem::InitHotLoading()
{
    hotreloadHandle = FindFirstChangeNotificationA("Code/Render/Shaders", false, FILE_NOTIFY_CHANGE_LAST_WRITE);
    if (hotreloadHandle == INVALID_HANDLE_VALUE)
    {
        throw;
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

void ShaderSystem::HotReloadShaders()
{
    //@Todo: right now Im stuck recompiling every single shader if I can't get the filename.
    //Potentially engine could let the previous vertex/shader object buffers be used until new compile is ready
    //REF: https://docs.microsoft.com/en-us/windows/win32/fileio/obtaining-directory-change-notifications

    bool nextChange = FindNextChangeNotification(hotreloadHandle);
    if (nextChange)
    {
        debugMenu.AddNotification(L"Shader reload start...");

        CleanUpShaders();
        CompileAllShadersFromFile();
        CreateAllShaders();

        debugMenu.AddNotification(L"Shader reload complete");
    }

    FindCloseChangeNotification(hotreloadHandle);
    InitHotLoading();
}
