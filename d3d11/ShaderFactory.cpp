#include "ShaderFactory.h"
#include <Windows.h>
#include "RenderSystem.h"
#include "DebugMenu.h"

void ShaderFactory::CreateAllShaders(ID3D11Device* device)
{
    for (int i = 0; i < shaders.size(); i++)
    {
        HR(device->CreateVertexShader(shaders[i].vertexCode->GetBufferPointer(), shaders[i].vertexCode->GetBufferSize(), nullptr, &shaders[i].vertexShader));
        HR(device->CreatePixelShader(shaders[i].pixelCode->GetBufferPointer(), shaders[i].pixelCode->GetBufferSize(), nullptr, &shaders[i].pixelShader));
    }
}

void ShaderFactory::CompileAllShadersFromFile()
{
    //https://www.bfilipek.com/2019/04/dir-iterate.html
    //Was something wrong with microsofts docs on this one, too verbose

    WIN32_FIND_DATAW data;
    HANDLE file = FindFirstFileW(L"Shaders/*.hlsl", &data);

    //TODO: even though going to move to .cso files later text files still good for debugging
    //Above at FindFirstFile at the wildcard, maybe utilise .vertex/.pixel filenames and determine type off that

    if (file == INVALID_HANDLE_VALUE)
    {
        int err = GetLastError();
        return;
    }

    ShaderItem shaderItem;

    shaders.clear();
    shadersMap.clear();

    do
    {
        wcscpy_s(shaderItem.filename, data.cFileName);
        shaders.push_back(shaderItem);
    } while (FindNextFileW(file, &data) != 0);

    FindClose(file);

    UINT flags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
    ID3DBlob* error;

    //TODO: Come back to this code. See if its any better than the static shader file declarations
    //Also check whether its worth splitting up shader stages (vertex, geom, etc) per file
    //Right now its okay, shader input and output might be a problem if that changes
    for (int i = 0; i < shaders.size(); i++)
    {
        shadersMap[shaders[i].filename] = &shaders[i];

        const char* vsEntry = "VSMain";
        const char* vsTarget = "vs_5_0";

        wchar_t directory[256] = {};
        wcscat_s(directory, L"Shaders/");
        wcscat_s(directory, shaders[i].filename);

        HR(D3DCompileFromFile(directory, nullptr, nullptr, vsEntry, vsTarget, flags, 0, &shaders[i].vertexCode, &error));
        if (error)
        {
            const char* errMsg = (char*)error->GetBufferPointer();
            OutputDebugString(errMsg);
            MessageBox(0, errMsg, vsEntry, 0);
            error = nullptr;
        }

        const char* psEntry = "PSMain";
        const char* psTarget = "ps_5_0";
        HR(D3DCompileFromFile(directory, nullptr, nullptr, psEntry, psTarget, flags, 0, &shaders[i].pixelCode, &error));
        if (error)
        {
            const char* errMsg = (char*)error->GetBufferPointer();
            OutputDebugString(errMsg);
            MessageBox(0, errMsg, psEntry, 0);
        }
    }
}

void ShaderFactory::InitHotLoading()
{
    hotreloadHandle = FindFirstChangeNotificationA("Shaders", false, FILE_NOTIFY_CHANGE_LAST_WRITE);
    if (hotreloadHandle == INVALID_HANDLE_VALUE)
    {
        DebugPrint("Handle for shader reload file tracking invalid\n");
    }
}

void ShaderFactory::CleanUpShaders()
{
    //Seems to work for now. I'm sure there's a caveat in D3D11
    for (int i = 0; i < shaders.size(); i++)
    {
        shaders[i].vertexCode->Release();
        shaders[i].pixelCode->Release();

        shaders[i].vertexShader->Release();
        shaders[i].pixelShader->Release();
    }
}

void ShaderFactory::HotReloadShaders(ID3D11Device* device, DebugMenu* debugMenu)
{
    //TODO: right now Im stuck recompiling every single shader if I can't get the filename
    //Reloading the shaders also pushes the camera back on the z-axis. Figure that one out (random negative val?)

    //Potentially engine could let the previous vertex/shader object buffers be used until new compile is ready

    //https://docs.microsoft.com/en-us/windows/win32/fileio/obtaining-directory-change-notifications

    BOOL nextChange = FindNextChangeNotification(hotreloadHandle);
    if (nextChange)
    {
        DebugPrint("Shader reload start...\n");
        CleanUpShaders();
        CompileAllShadersFromFile();
        CreateAllShaders(device);
        DebugPrint("Shader reload complete\n");

        debugMenu->notifications.push_back(DebugNotification(L"Shader reload complete."));
    }

    FindCloseChangeNotification(hotreloadHandle);
    InitHotLoading();
}
