#include "Renderer.h"
#include "Debug.h"
#include "ShaderSystem.h"
#include "Camera.h"
#include "UI/UISystem.h"
#include "Components/MeshComponent.h"
#include "Actors/Actor.h"

Renderer renderer;

void Renderer::Init(void* window, int viewportWidth, int viewportHeight)
{
	viewport.Width = viewportWidth;
	viewport.Height = viewportHeight;
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
	viewport.MaxDepth = 1.0f;
	viewport.MinDepth = 0.0f;

	CreateFactory();
	CreateDevice();
	CreateSwapchain((HWND)window);
	CreateRTVAndDSV();
	CreateInputLayout();
	CreateMainConstantBuffers();
	CheckSupportedFeatures();
}

void Renderer::CreateFactory()
{
	ComPtr<IDXGIFactory> tempDxgiFactory;
	HR(CreateDXGIFactory(IID_PPV_ARGS(tempDxgiFactory.GetAddressOf())));
	HR(tempDxgiFactory->QueryInterface(dxgiFactory.GetAddressOf()));
}

void Renderer::CreateDevice()
{
	//BGRA support needed for DirectWrite and Direct2D
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
	D3D_FEATURE_LEVEL selectedFeatureLevel;

	ComPtr<IDXGIAdapter1> adapter;
	for (int i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_MINIMUM_POWER,
		IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf())) != DXGI_ERROR_NOT_FOUND; i++)
	{
		gpuAdapters.push_back(adapter.Get());
		DXGI_ADAPTER_DESC1 desc = {};
		adapter->GetDesc1(&desc);
		gpuAdaptersDesc.push_back(desc);
	}

	//TODO: selecting adapters here isn't working.
	HR(D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
		featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, device.GetAddressOf(),
		&selectedFeatureLevel, context.GetAddressOf()));
}

void Renderer::CreateSwapchain(HWND window)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc = { (UINT)viewport.Width, (UINT)viewport.Height, {60, 1}, DXGI_FORMAT_R8G8B8A8_UNORM };
	sd.Windowed = TRUE;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.BufferCount = swapchainCount;

	ComPtr<IDXGISwapChain> tempSwapchain;
	HR(dxgiFactory->CreateSwapChain(device.Get(), &sd, tempSwapchain.GetAddressOf()));
	HR(tempSwapchain->QueryInterface(swapchain.GetAddressOf()));
}

void Renderer::CreateRTVAndDSV()
{
	//Create Render target views
	for (int i = 0; i < swapchainCount; i++)
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		swapchain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		HR(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &rtvs[i]));
		backBuffer.Reset();
	}

	//Create depth stencil view
	D3D11_TEXTURE2D_DESC dsDesc = {};
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsDesc.ArraySize = 1;
	dsDesc.MipLevels = 1;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.SampleDesc.Count = 1;
	dsDesc.Width = viewport.Width;
	dsDesc.Height = viewport.Height;

	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	HR(device->CreateTexture2D(&dsDesc, nullptr, depthStencilBuffer.GetAddressOf()));
	HR(device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, dsv.GetAddressOf()));
	depthStencilBuffer.Reset();
}

void Renderer::CreateDefaultShaders()
{
	defaultVertexCode = shaderSystem.CreateShaderFromFile(L"Code/Render/Shaders/DefaultShader.hlsl", "VSMain", "vs_5_0");
	defaultPixelCode = shaderSystem.CreateShaderFromFile(L"Code/Render/Shaders/DefaultShader.hlsl", "PSMain", "ps_5_0");

	HR(device->CreateVertexShader(defaultVertexCode->GetBufferPointer(), defaultVertexCode->GetBufferSize(), nullptr, defaultVertexShader.GetAddressOf()));
	HR(device->CreatePixelShader(defaultPixelCode->GetBufferPointer(), defaultPixelCode->GetBufferSize(), nullptr, defaultPixelShader.GetAddressOf()));
}

void Renderer::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	CreateDefaultShaders();

	HR(device->CreateInputLayout(inputDesc, _countof(inputDesc), defaultVertexCode->GetBufferPointer(), defaultVertexCode->GetBufferSize(), &inputLayout));
	context->IASetInputLayout(inputLayout.Get());
}

void Renderer::CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FrontCounterClockwise = FALSE;

	//SOLID
	HR(device->CreateRasterizerState(&rastDesc, &rastStateSolid));
	activeRastState = rastStateSolid;
	context->RSSetState(activeRastState.Get());

	//WIREFRAME
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	rastDesc.CullMode = D3D11_CULL_NONE;
	HR(device->CreateRasterizerState(&rastDesc, &rastStateWireframe));

	//SOLID, NO BACK CULL
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	HR(device->CreateRasterizerState(&rastDesc, &rastStateNoBackCull));
}

void Renderer::CreateBlendStates()
{
	//MSAA has to be set for AlphaToCoverage to work.
	D3D11_BLEND_DESC alphaToCoverageDesc = {};
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&alphaToCoverageDesc, blendStateAlphaToCoverage.GetAddressOf()));
}

void Renderer::CreateMainConstantBuffers()
{
	//Shader matrix constant buffer
	shaderMatrices.Create(GetAspectRatio());
	editorCamera.proj = shaderMatrices.proj;

	cbMatrices = CreateDefaultBuffer(sizeof(shaderMatrices), D3D11_BIND_CONSTANT_BUFFER, &shaderMatrices);
}

void Renderer::CheckSupportedFeatures()
{
	D3D11_FEATURE_DATA_THREADING threadFeature = {};
	device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadFeature, sizeof(threadFeature));
}

ID3D11Buffer* Renderer::CreateDefaultBuffer(UINT byteWidth, UINT bindFlags, const void* initData)
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

ID3D11Buffer* Renderer::CreateVertexBuffer(MeshData& meshData)
{
	return Renderer::CreateDefaultBuffer(meshData.GetVerticesByteWidth(), 
		D3D11_BIND_VERTEX_BUFFER, meshData.vertices.data());
}

ID3D11Buffer* Renderer::CreateIndexBuffer(MeshData& meshData)
{
	return Renderer::CreateDefaultBuffer(meshData.GetIndicesByteWidth(),
		D3D11_BIND_INDEX_BUFFER, meshData.indices.data());
}

ID3D11SamplerState* Renderer::CreateSampler()
{
	ID3D11SamplerState* sampler;

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	HR(device->CreateSamplerState(&sampDesc, &sampler));

	return sampler;
}

void Renderer::RenderSetup()
{
	context->RSSetViewports(1, &viewport);

	const float clearColour[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	UINT frameIndex = swapchain->GetCurrentBackBufferIndex();

	context->ClearRenderTargetView(rtvs[frameIndex], clearColour);
	context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	context->OMSetRenderTargets(1, &rtvs[frameIndex], dsv.Get());
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->RSSetState(activeRastState.Get());
}

void Renderer::Render()
{
	const int cbMatrixRegister = 0;

	shaderMatrices.view = activeCamera->view;

	for (MeshComponent& mesh : MeshComponent::system.components)
	{
		context->RSSetState(activeRastState.Get());

		//const FLOAT blendState[4] = { 0.f };
		//context->OMSetBlendState(blendStateAlphaToCoverage, blendState, 0xFFFFFFFF);

		//auto shader = gShaderFactory.shaderMap.find(stows(actorSystem->shaderName));
		/*if (shader == gShaderFactory.shaderMap.end())
		{
			DebugPrint("vertex shader file name %ls not found\n", actorSystem->shaderName);
		}*/

		context->VSSetShader(defaultVertexShader.Get(), nullptr, 0);
		context->PSSetShader(defaultPixelShader.Get(), nullptr, 0);

		context->PSSetSamplers(0, 1, &mesh.pso.sampler.data);
		//context->PSSetShaderResources(0, _countof(shaderResourceViews), shaderResourceViews);
		//context->VSSetShaderResources(3, 1, &actorSystem->pso.instancedDataSrv->data);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh.pso.vertexBuffer.data, &stride, &offset);
		context->IASetIndexBuffer(mesh.pso.indexBuffer.data, DXGI_FORMAT_R32_UINT, 0);

		shaderMatrices.model = mesh.transform.GetAffine();

		shaderMatrices.mvp = shaderMatrices.model * shaderMatrices.view * shaderMatrices.proj;
		context->UpdateSubresource(cbMatrices.Get(), 0, nullptr, &shaderMatrices, 0, 0);
		context->VSSetConstantBuffers(cbMatrixRegister, 1, cbMatrices.GetAddressOf());

		context->DrawIndexed(mesh.data.indices.size(), 0, 0);
	}
}

void Renderer::Present()
{
	HR(swapchain->Present(1, 0));
}

void* Renderer::GetSwapchain()
{
	return swapchain.Get();
}

float Renderer::GetAspectRatio()
{
	return viewport.Width / viewport.Height;
}

float Renderer::GetViewportWidth()
{
	return viewport.Width;
}

float Renderer::GetViewportHeight()
{
	return viewport.Height;
}

void Renderer::SetViewportWidthHeight(float width, float height)
{
	viewport.Width = width;
	viewport.Height = height;
}

void Renderer::ResizeSwapchain(int newWidth, int newHeight)
{
	if (swapchain == nullptr) return;

	context->OMSetRenderTargets(0, 0, 0);

	// Release all outstanding references to the swap chain's buffers.
	for (int rtvIndex = 0; rtvIndex < swapchainCount; rtvIndex++)
	{
		rtvs[rtvIndex]->Release();
	}

	dsv.Reset();

	uiSystem.Cleanup();

	HR(swapchain->ResizeBuffers(swapchainCount, newWidth, newHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	viewport.Width = newWidth;
	viewport.Height = newHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);

	CreateRTVAndDSV();

	uiSystem.Init((void*)swapchain.Get());

	shaderMatrices.Create(GetAspectRatio());
	activeCamera->proj = shaderMatrices.proj;
}