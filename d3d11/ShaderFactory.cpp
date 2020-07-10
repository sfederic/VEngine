#include "ShaderFactory.h"
#include <Windows.h>

void ShaderFactory::CreateAllShaders()
{
    //shaders.push_back(ShaderItem(L"DefaultVertex.hlsl", EShaderType::VERTEX));
    //shadersMap.insert(std::pair<const char*, ShaderItem>("DefaultPixel.hlsl", shaders[0]));

    //shaders.push_back(ShaderItem(L"DefaultPixel.hlsl", EShaderType::PIXEL));
    //ShaderItem item = shadersMap.find("DefaultVertex.hlsl")->second;

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

    do
    {
        wcscpy_s(shaderItem.filename, data.cFileName);
        //shaderItem.filename = data.cFileName;
        //shaders.push_back(ShaderItem(data.cFileName, EShaderType::UNKNOWN));
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
