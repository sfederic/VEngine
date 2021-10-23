#include "ShaderSystem.h"
#include "Editor/Editor.h"
#include "Debug.h"
#include "Renderer.h"
#include "VString.h"
#include "Input.h"

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
    return shaderMap.find(shaderName)->second;
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

//TODO: come back here and swap all this win32 shit to <filesystem>
void ShaderSystem::CompileAllShadersFromFile()
{
    WIN32_FIND_DATAW data;
    HANDLE file = FindFirstFileW(L"Code/Render/Shaders/*.hlsl", &data);

    if (file == INVALID_HANDLE_VALUE)
    {
        HR(GetLastError());
    }

    ShaderItem shaderItem;

    shaders.clear();
    shaderMap.clear();

    do
    {
        shaderItem.filename = data.cFileName;
        shaders.push_back(shaderItem);
    } while (FindNextFileW(file, &data) != 0);

    FindClose(file);

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
        editor->Log(L"Shader hot reload init failed");
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
    //TODO: right now Im stuck recompiling every single shader if I can't get the filename
    //Reloading the shaders also pushes the camera back on the z-axis. Figure that one out (random negative val?)

    //Potentially engine could let the previous vertex/shader object buffers be used until new compile is ready

    //REF: https://docs.microsoft.com/en-us/windows/win32/fileio/obtaining-directory-change-notifications

    bool nextChange = FindNextChangeNotification(hotreloadHandle);
    if (nextChange)
    {
        editor->Log(L"Shader reload start...\n");

        CleanUpShaders();
        CompileAllShadersFromFile();
        CreateAllShaders();

        editor->Log(L"Shader reload complete\n");
    }

    FindCloseChangeNotification(hotreloadHandle);
    InitHotLoading();
}
