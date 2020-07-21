#include "RenderSystem.h"
#include "CoreSystem.h"
#include "Actor.h"
#include "Camera.h"
#include "UISystem.h"
#include "Input.h"
#include "Console.h"
#include "DebugMenu.h"
#include "ShaderFactory.h"
#include "MathHelpers.h"
#include "World.h"
#include "Debug.h"

RenderSystem renderSystem;
ShaderFactory g_ShaderFactory;

UINT strides = sizeof(Vertex);
UINT offsets = 0;

ActorSystem debugBox;
ActorSystem debugSphere;

struct Matrices
{
	XMMATRIX model;
	XMMATRIX view;
	XMMATRIX proj;
	XMMATRIX mvp;
}matrices;

Vertex debugLineData[2];

void RenderSystem::Tick()
{
	if (inputSystem.GetKeyUpState('1'))
	{
		activeRastState = rastStateWireframe;
	}
	if (inputSystem.GetKeyUpState('2'))
	{
		activeRastState = rastStateSolid;
	}

	if (inputSystem.GetKeyUpState('B'))
	{
		bDrawBoundingBoxes = !bDrawBoundingBoxes;
	}
	if (inputSystem.GetKeyUpState('V'))
	{
		bDrawBoundingSpheres = !bDrawBoundingSpheres;
	}
}

void RenderSystem::Init()
{
	viewport = { 0.f, 0.f, (float)coreSystem.windowWidth, (float)coreSystem.windowHeight, 0.f, 1.f };

	CreateDevice();
	CreateSwapchain();
	CreateRTVAndDSV();
	CreateShaders();
	CreateInputLayout();
	CreateRasterizerStates();
	CreateConstantBuffer();
}

void RenderSystem::CreateDevice()
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

	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//BGRA support needed for DirectWrite and Direct2D
	HR(D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags,
		featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &device, &featureLevel, &context));


	debugLineData[0].pos = XMFLOAT3(0.f, 0.f, 0.f);
	debugLineData[1].pos = XMFLOAT3(0.f, 0.f, 100.f);


	g_ShaderFactory.CompileAllShadersFromFile();
	g_ShaderFactory.CreateAllShaders(device);
	g_ShaderFactory.InitHotLoading();

	D3D11_QUERY_DESC qd = {};
	qd.Query = D3D11_QUERY_TIMESTAMP;
	HR(device->CreateQuery(&qd, &startTimeQuery));
	HR(device->CreateQuery(&qd, &endTimeQuery));

	qd.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	HR(device->CreateQuery(&qd, &disjointQuery));

	debugBox.CreateActors("Models/cube.obj", this, 1);
	debugBox.shaderName = L"debugDraw.hlsl";

	debugSphere.CreateActors("Models/ico_sphere.obj", this, 1);
	debugSphere.shaderName = L"debugDraw.hlsl";
}

void RenderSystem::CreateSwapchain()
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc = { (UINT)coreSystem.windowWidth, (UINT)coreSystem.windowHeight, {60, 1}, DXGI_FORMAT_R8G8B8A8_UNORM };
	sd.Windowed = TRUE;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = coreSystem.mainWindow;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.BufferCount = frameCount;

	IDXGISwapChain* tmpSwapchain;
	HR(dxgiFactory->CreateSwapChain(device, &sd, &tmpSwapchain));
	HR(tmpSwapchain->QueryInterface(&swapchain));
	tmpSwapchain->Release();
}

void RenderSystem::CreateRTVAndDSV()
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
	dsDesc.Width = coreSystem.windowWidth;
	dsDesc.Height = coreSystem.windowHeight;

	ID3D11Texture2D* depthStencilBuffer;
	HR(device->CreateTexture2D(&dsDesc, nullptr, &depthStencilBuffer));
	HR(device->CreateDepthStencilView(depthStencilBuffer, nullptr, &dsv));
}

void RenderSystem::CreateShaders()
{
	//TODO: fix this for later. for now, all shaders are using the same Inputlayout so its fine
	vertexCode = CreateShaderFromFile(L"Shaders/shaders.hlsl", "VSMain", "vs_5_0");
	pixelCode = CreateShaderFromFile(L"Shaders/shaders.hlsl", "PSMain", "ps_5_0");

	HR(device->CreateVertexShader(vertexCode->GetBufferPointer(), vertexCode->GetBufferSize(), nullptr, &vertexShader));
	HR(device->CreatePixelShader(pixelCode->GetBufferPointer(), pixelCode->GetBufferSize(), nullptr, &pixelShader));
}

void RenderSystem::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HR(device->CreateInputLayout(inputDesc, _countof(inputDesc), vertexCode->GetBufferPointer(), vertexCode->GetBufferSize(), &inputLayout));
	context->IASetInputLayout(inputLayout);
}

void RenderSystem::CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FrontCounterClockwise = FALSE;

	HR(device->CreateRasterizerState(&rastDesc, &rastStateSolid));
	activeRastState = rastStateSolid;
	context->RSSetState(activeRastState);

	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	rastDesc.CullMode = D3D11_CULL_NONE;
	HR(device->CreateRasterizerState(&rastDesc, &rastStateWireframe));
}

void RenderSystem::CreateVertexBuffer(UINT size, const void* data, ActorSystem* system)
{
	system->vertexBuffer = CreateDefaultBuffer(size, D3D11_BIND_VERTEX_BUFFER, data);
}

void RenderSystem::CreateConstantBuffer()
{
	matrices.model = XMMatrixIdentity();
	matrices.view = XMMatrixIdentity();
	matrices.proj = XMMatrixPerspectiveFovLH(XM_PI / 3, coreSystem.GetAspectRatio(), 0.01f, 1000.f);

	//TODO: Get rid of this too. terrible
	editorCamera.proj = matrices.proj;
	matrices.mvp = matrices.model * matrices.view * matrices.proj;

	cbMatrices = CreateDefaultBuffer(sizeof(Matrices), D3D11_BIND_CONSTANT_BUFFER, &matrices);
}

void RenderSystem::RenderActorSystem(ActorSystem* actorSystem)
{
	auto shader = g_ShaderFactory.shaderMap.find(actorSystem->shaderName);

	if (shader == g_ShaderFactory.shaderMap.end())
	{
		DebugPrint("vertex shader file name %ls not found\n", actorSystem->shaderName);
	}

	context->VSSetShader(shader->second->vertexShader, nullptr, 0);
	context->PSSetShader(shader->second->pixelShader, nullptr, 0);

	context->IASetVertexBuffers(0, 1, &actorSystem->vertexBuffer, &strides, &offsets);

	for (int i = 0; i < actorSystem->actors.size(); i++)
	{
		if (actorSystem->actors[i].bRender)
		{
			matrices.view = GetPlayerCamera()->view;
			matrices.model = actorSystem->actors[i].transform;
			matrices.mvp = matrices.model * matrices.view * matrices.proj;
			context->UpdateSubresource(cbMatrices, 0, nullptr, &matrices, 0, 0);
			context->VSSetConstantBuffers(0, 1, &cbMatrices);

			context->Draw(actorSystem->modelData.verts.size(), 0);
		}
	}
}

void RenderSystem::RenderBounds()
{
	World* world = GetWorld();
	Camera* camera = GetPlayerCamera();

	if (bDrawBoundingBoxes || bDrawBoundingSpheres)
	{
		context->RSSetState(rastStateWireframe);
	}

	if (bDrawBoundingBoxes)
	{
		auto boxIt = g_ShaderFactory.shaderMap.find(debugBox.shaderName);
		context->VSSetShader(boxIt->second->vertexShader, nullptr, 0);
		context->PSSetShader(boxIt->second->pixelShader, nullptr, 0);

		context->IASetVertexBuffers(0, 1, &debugBox.vertexBuffer, &strides, &offsets);

		for (int systemIndex = 0; systemIndex < world->actorSystems.size(); systemIndex++)
		{
			for (int actorIndex = 0; actorIndex < world->actorSystems[systemIndex]->actors.size(); actorIndex++)
			{
				matrices.view = camera->view;
				matrices.model = world->actorSystems[systemIndex]->actors[actorIndex].transform;
				matrices.mvp = matrices.model * matrices.view * matrices.proj;
				context->UpdateSubresource(cbMatrices, 0, nullptr, &matrices, 0, 0);
				context->VSSetConstantBuffers(0, 1, &cbMatrices);

				context->Draw(debugBox.modelData.verts.size(), 0);
			}
		}
	}

	if (bDrawBoundingSpheres)
	{
		auto sphereIt = g_ShaderFactory.shaderMap.find(debugSphere.shaderName);
		context->VSSetShader(sphereIt->second->vertexShader, nullptr, 0);
		context->PSSetShader(sphereIt->second->pixelShader, nullptr, 0);

		context->IASetVertexBuffers(0, 1, &debugSphere.vertexBuffer, &strides, &offsets);

		for (int systemIndex = 0; systemIndex < world->actorSystems.size(); systemIndex++)
		{
			for (int actorIndex = 0; actorIndex < world->actorSystems[systemIndex]->actors.size(); actorIndex++)
			{
				matrices.view = camera->view;
				matrices.model = world->actorSystems[systemIndex]->actors[actorIndex].transform;
				matrices.mvp = matrices.model * matrices.view * matrices.proj;
				context->UpdateSubresource(cbMatrices, 0, nullptr, &matrices, 0, 0);
				context->VSSetConstantBuffers(0, 1, &cbMatrices);

				context->Draw(debugSphere.modelData.verts.size(), 0);
			}
		}
	}
}

void RenderSystem::RenderEnd(float deltaTime)
{
	//DRAW DEBUG LINES
	/*context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	context->IASetVertexBuffers(0, 1, &debugBuffer, &strides, &offsets);

	for (int i = 0; i < debugLines.size(); i++)
	{
		matrices.view = camera->view;
		matrices.model = XMMatrixIdentity();
		matrices.mvp = matrices.model * matrices.view * matrices.proj;
		context->UpdateSubresource(cbMatrices, 0, nullptr, &matrices, 0, 0);
		context->VSSetConstantBuffers(0, 1, &cbMatrices);

		context->Draw(debugLines.size(), 0);
	}*/

	//UI RENDERING 
	//TODO: Put render and d2d stuff UISystem
	uiSystem.d2dRenderTarget->BeginDraw();

	//Test console rendering and work. Might need to put it into a system
	console.Tick();
	console.DrawViewItems();

	//Debug menu testing (really need to fix this d2d stuff in Render)
	debugMenu.Tick(GetWorld(), deltaTime);

	//UI View testing
	for (int viewIndex = 0; viewIndex < uiSystem.uiViews.size(); viewIndex++)
	{
		uiSystem.uiViews[viewIndex].Tick();
	}


	//END UI RENDERING
	uiSystem.d2dRenderTarget->EndDraw();

	//PRESENT
	HR(swapchain->Present(1, 0));

	//END QUERY
	if (bQueryGPUInner)
	{
		context->End(endTimeQuery);
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

		renderTime = time;
	}
}

void RenderSystem::RenderSetup(float deltaTime)
{
	if (bQueryGPU)
	{
		context->Begin(disjointQuery);
		context->End(startTimeQuery);
		bQueryGPUInner = true;
	}
	else
	{
		bQueryGPUInner = false;
	}

	context->RSSetViewports(1, &viewport);

	const float clearColour[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	UINT frameIndex = swapchain->GetCurrentBackBufferIndex();

	context->ClearRenderTargetView(rtvs[frameIndex], clearColour);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	context->OMSetRenderTargets(1, &rtvs[frameIndex], dsv);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->RSSetState(activeRastState);

	if (inputSystem.GetKeyUpState('3'))
	{
		g_ShaderFactory.HotReloadShaders();
	}
}

ID3DBlob* RenderSystem::CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target)
{
	UINT compileFlags = 0;
#ifdef _DEBUG
	compileFlags = D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#endif
	ID3DBlob* code;
	ID3DBlob* error;
	
	D3DCompileFromFile(filename, nullptr, nullptr, entry, target, compileFlags, 0, &code, &error);
	if (error)
	{
		const char* errMsg = (char*)error->GetBufferPointer();
		OutputDebugString(errMsg);
		MessageBox(0, errMsg, entry, 0);
	}

	return code;
}

ID3D11Buffer* RenderSystem::CreateDefaultBuffer(UINT byteWidth, UINT bindFlags, const void* initData)
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

ID3D11Buffer* RenderSystem::CreateDyamicBuffer(UINT byteWidth, UINT bindFlags, const void* initData)
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


