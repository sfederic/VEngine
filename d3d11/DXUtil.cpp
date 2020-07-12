#include "DXUtil.h"
#include "Win32Util.h"
#include "Actor.h"
#include "Camera.h"
#include "UIContext.h"
#include "Input.h"
#include "Console.h"
#include "DebugMenu.h"
#include "ShaderFactory.h"
#include <thread>

//GLOBALS
DebugMenu g_DebugMenu;
ShaderFactory g_ShaderFactory;

UINT strides = sizeof(Vertex);
UINT offsets = 0;

ActorSystem debugBox;

//Temp constant buffer data for base shader
struct Matrices
{
	XMMATRIX model;
	XMMATRIX view;
	XMMATRIX proj;
	XMMATRIX mvp;
}matrices;

Vertex debugLineData[2];

void DXUtil::CreateDevice()
{
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

	IDXGIFactory* tmpFactory;
	HR(CreateDXGIFactory(IID_PPV_ARGS(&tmpFactory)));
	HR(tmpFactory->QueryInterface(&dxgiFactory));
	tmpFactory->Release();

	//Reference for EnumAdapterByGpuPerformance
	//https://github.com/walbourn/directx-vs-templates/blob/master/d3d11game_win32_dr/DeviceResources.cpp
	
	IDXGIAdapter1* adapter;
	for (int i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_MINIMUM_POWER, IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(adapter);
		DXGI_ADAPTER_DESC1 desc = {};
		adapter->GetDesc1(&desc);
		adaptersDesc.push_back(desc);
	}

	//BGRA support needed for DirectWrite and Direct2D
	HR(D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &device, &featureLevel, &context));

	debugLineData[0].pos = XMFLOAT3(0.f, 0.f, 0.f);
	debugLineData[1].pos = XMFLOAT3(0.f, 0.f, 100.f);

	//TODO: just doing it here for now
	g_ShaderFactory.CompileAllShadersFromFile();
	g_ShaderFactory.CreateAllShaders(device);

	debugBox.CreateActors("Models/cube.obj", this, 1);
	debugBox.shaderName = L"debugDraw.hlsl";
}

void DXUtil::CreateSwapchain()
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc = { (UINT)windowWidth, (UINT)windowHeight, {60, 1}, DXGI_FORMAT_R8G8B8A8_UNORM };
	sd.Windowed = TRUE;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = mainWindow;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.BufferCount = frameCount;

	IDXGISwapChain* tmpSwapchain;
	HR(dxgiFactory->CreateSwapChain(device, &sd, &tmpSwapchain));
	HR(tmpSwapchain->QueryInterface(&swapchain));
	tmpSwapchain->Release();
}

void DXUtil::CreateRTVAndDSV()
{
	for (int i = 0; i < frameCount; i++)
	{
		ID3D11Texture2D* backBuffer;
		swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
		HR(device->CreateRenderTargetView(backBuffer, nullptr, &rtvs[i]));
		backBuffer->Release();
	}

	D3D11_TEXTURE2D_DESC dsDesc = {};
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsDesc.ArraySize = 1;
	dsDesc.MipLevels = 1;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.SampleDesc.Count = 1;
	dsDesc.Width = windowWidth;
	dsDesc.Height = windowHeight;

	ID3D11Texture2D* depthStencilBuffer;
	HR(device->CreateTexture2D(&dsDesc, nullptr, &depthStencilBuffer));
	HR(device->CreateDepthStencilView(depthStencilBuffer, nullptr, &dsv));
}

void DXUtil::CreateShaders()
{
	vertexCode = CreateShaderFromFile(L"Shaders/shaders.hlsl", "VSMain", "vs_5_0");
	pixelCode = CreateShaderFromFile(L"Shaders/shaders.hlsl", "PSMain", "ps_5_0");

	HR(device->CreateVertexShader(vertexCode->GetBufferPointer(), vertexCode->GetBufferSize(), nullptr, &vertexShader));
	HR(device->CreatePixelShader(pixelCode->GetBufferPointer(), pixelCode->GetBufferSize(), nullptr, &pixelShader));

	//context->VSSetShader(vertexShader, nullptr, 0);
	//context->PSSetShader(pixelShader, nullptr, 0);
}

void DXUtil::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HR(device->CreateInputLayout(inputDesc, _countof(inputDesc), vertexCode->GetBufferPointer(), vertexCode->GetBufferSize(), &inputLayout));
	context->IASetInputLayout(inputLayout);
}

void DXUtil::CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FrontCounterClockwise = FALSE;

	HR(device->CreateRasterizerState(&rastDesc, &rastStateSolid));
	context->RSSetState(rastStateSolid);

	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	rastDesc.CullMode = D3D11_CULL_NONE;
	HR(device->CreateRasterizerState(&rastDesc, &rastStateWireframe));
}

void DXUtil::CreateVertexBuffer(UINT size, const void* data, ActorSystem* system)
{
	system->vertexBuffer = CreateDefaultBuffer(size, D3D11_BIND_VERTEX_BUFFER, data);
}

void DXUtil::CreateConstantBuffer(Camera& camera)
{
	//TODO: cleanup constant buffer code to work later with multiple shaders and buffers
	matrices.model = XMMatrixIdentity();
	matrices.view = XMMatrixIdentity();
	matrices.proj = XMMatrixPerspectiveFovLH(XM_PI / 3, Win32Util::GetAspectRatio(), 0.01f, 1000.f);

	//TODO: Get rid of this too. terrible
	camera.proj = matrices.proj;

	matrices.mvp = matrices.model * matrices.view * matrices.proj;

	cbMatrices = CreateDefaultBuffer(sizeof(Matrices), D3D11_BIND_CONSTANT_BUFFER, &matrices);
}

void DXUtil::Render(Camera* camera, UIContext* ui, ActorSystem* actorSystem, DXUtil* dx, ID3D11Buffer* debugBuffer)
{
	//context->Begin(disjointQuery);
	//context->End(startTimeQuery);

	context->RSSetViewports(1, &viewport);

	const float clearColour[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	UINT frameIndex = swapchain->GetCurrentBackBufferIndex();

	context->ClearRenderTargetView(rtvs[frameIndex], clearColour);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	context->OMSetRenderTargets(1, &rtvs[frameIndex], dsv);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->RSSetState(rastStateSolid);

	if (GetKeyUpState('3'))
	{
		std::thread reloadThread(&ShaderFactory::HotReloadShaders, &g_ShaderFactory, device);
		reloadThread.join();
		//g_ShaderFactory.HotReloadShaders(device);
	}

	//Using one shader per stage for now
	auto vs = g_ShaderFactory.shadersMap.find(actorSystem->shaderName);
	//auto ps = g_ShaderFactory.shadersMap.find(actorSystem->shaderName);

	//TODO: put this error checking into shader construction
	if (vs == g_ShaderFactory.shadersMap.end())
	{
		debugPrint("vertex shader file name %ls not found\n", actorSystem->shaderName);
		throw;
	}
	/*if (ps == g_ShaderFactory.shadersMap.end())
	{
		debugPrint("pixel shader file name %ls not found\n", actorSystem->shaderName);
		throw;
	}*/

		//Keep in mind that with actor systems, only need once pass of setting all d3d11 state
	context->VSSetShader(vs->second->vertexShader, nullptr, 0);
	context->PSSetShader(vs->second->pixelShader, nullptr, 0);

	context->IASetVertexBuffers(0, 1, &actorSystem->vertexBuffer, &strides, &offsets);
	//context->IASetIndexBuffer(actorSystem->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Only a test system. End needs to go through different rasterizer passes and group actor systems together
	for (int i = 0; i < actorSystem->actors.size(); i++)
	{
		if (actorSystem->actors[i].bRender)
		{
			//Constant buffer work
			matrices.view = camera->view;
			matrices.model = actorSystem->actors[i].transform;
			matrices.mvp = matrices.model * matrices.view * matrices.proj;
			context->UpdateSubresource(cbMatrices, 0, nullptr, &matrices, 0, 0);
			context->VSSetConstantBuffers(0, 1, &cbMatrices);

			//Draw all actors of system
			context->Draw(actorSystem->modelData.verts.size(), 0);
		}
	}

	auto boxIt = g_ShaderFactory.shadersMap.find(debugBox.shaderName);
	context->VSSetShader(boxIt->second->vertexShader, nullptr, 0);
	context->PSSetShader(boxIt->second->pixelShader, nullptr, 0);

	context->RSSetState(rastStateWireframe);

	//Draw bounding boxes
	for (int i = 0; i < debugBox.actors.size(); i++)
	{
		matrices.view = camera->view;
		matrices.model = debugBox.actors[i].transform;
		matrices.model = XMMatrixScaling(1.01f, 1.01f, 1.01f); //Trying to make the debug boxes slightly larger than the actor
		matrices.mvp = matrices.model * matrices.view * matrices.proj;
		context->UpdateSubresource(cbMatrices, 0, nullptr, &matrices, 0, 0);
		context->VSSetConstantBuffers(0, 1, &cbMatrices);

		context->Draw(debugBox.modelData.verts.size(), 0);
	}

	//TODO: the lines work fine, find a way to make them thicker? (Geom shader?)
	//Lifetime fix (add a timer into the raycast func (this needs a global engine timer/clocl now))
	//Draw debug shapes
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	context->IASetVertexBuffers(0, 1, &debugBuffer, &strides, &offsets);

	for (int i = 0; i < debugLines.size(); i++)
	{
		matrices.view = camera->view;
		matrices.model = XMMatrixIdentity(); //Lines don't need their own model matrix
		matrices.mvp = matrices.model * matrices.view * matrices.proj;
		context->UpdateSubresource(cbMatrices, 0, nullptr, &matrices, 0, 0);
		context->VSSetConstantBuffers(0, 1, &cbMatrices);

		context->Draw(debugLines.size(), 0);
	}

	//TODO: FIX THIS TOO THIS IS AWFUL
	//UI RENDERING 
	//TODO: Put render and d2d stuff into func for profiling
	ui->d2dRenderTarget->BeginDraw();

	//Test console rendering and work. Might need to put it into a system
	Console::Tick(ui, dx);
	Console::DrawViewItems(ui);

	//Debug menu testing (really need to fix this d2d stuff in Render)
	g_DebugMenu.Tick(ui, dx, actorSystem);


	ui->d2dRenderTarget->EndDraw();


	//PRESENT
	HR(swapchain->Present(1, 0));


	//TODO: Queries blocking the GPU. Find a way to poll the query states on a thread?
	//END QUERY
	/*context->End(endTimeQuery);
	context->End(disjointQuery);

	//POLL QUERY
	while (context->GetData(disjointQuery, nullptr, 0, 0) == S_FALSE)
	{
		Sleep(1);
	}

	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT freq = {};
	HR(context->GetData(disjointQuery, &freq, sizeof(freq), 0));

	//Is the thread polling necessary?
	while (context->GetData(startTimeQuery, nullptr, 0, 0) == S_FALSE)
	{
		Sleep(1);
	}

	while (context->GetData(endTimeQuery, nullptr, 0, 0) == S_FALSE)
	{
		Sleep(1);
	}

	//TODO: fucking clean this up
	UINT64 endTime = 0, startTime = 0;
	HR(context->GetData(startTimeQuery, &startTime, sizeof(UINT64), 0));
	HR(context->GetData(endTimeQuery, &endTime, sizeof(UINT64), 0));

	UINT64 realTime = endTime - startTime;
	double tick = 1.0 / freq.Frequency;
	double time = tick * (realTime);

	renderTime = time;*/
}

ID3DBlob* DXUtil::CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target)
{
	UINT flags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
	ID3DBlob* code;
	ID3DBlob* error;
	
	D3DCompileFromFile(filename, nullptr, nullptr, entry, target, flags, 0, &code, &error);
	if (error)
	{
		const char* errMsg = (char*)error->GetBufferPointer();
		OutputDebugString(errMsg);
		MessageBox(0, errMsg, entry, 0);
	}

	return code;
}

ID3D11Buffer* DXUtil::CreateDefaultBuffer(UINT byteWidth, UINT bindFlags, const void* initData)
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = byteWidth;
	desc.BindFlags = bindFlags;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = initData;

	HR(device->CreateBuffer(&desc, &data, &buffer));

	return buffer;
}

ID3D11Buffer* DXUtil::CreateDyamicBuffer(UINT byteWidth, UINT bindFlags, const void* initData)
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = byteWidth;
	desc.BindFlags = bindFlags;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = initData;

	HR(device->CreateBuffer(&desc, &data, &buffer));

	return buffer;
}

void DXTrace(HRESULT hr, const char* filename, const char* func, int line)
{
	_com_error err(hr);
	char errmsg[1024];
	snprintf(errmsg, sizeof(errmsg), "HR: %s\nFile: %s\nFunction: %s\nLine: %d", err.ErrorMessage(), filename, func, line);
	MessageBox(0, errmsg, "Error", 0);
}
