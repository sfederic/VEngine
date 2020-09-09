#include "D3D12RenderSystem.h"
#include "CoreSystem.h"

void D3D12RenderSystem::Tick()
{
}

void D3D12RenderSystem::Init(HWND window)
{
	//Viewport and rect
	viewport = { 0.f, 0.f, (float)gCoreSystem.windowWidth, (float)gCoreSystem.windowHeight, 0.f, 1.f };
	scissorRect = { 0, 0, gCoreSystem.windowWidth, gCoreSystem.windowHeight };

	ID3D12Debug* debug;
	HR(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)));
	debug->EnableDebugLayer();
	debug->Release();

	//DEVICE
	HR(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));
	HR(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)));

	//CMD Q
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HR(device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue)));

	//SWAPCHAIN
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc = { (UINT)gCoreSystem.windowWidth, (UINT)gCoreSystem.windowHeight, {60, 1}, DXGI_FORMAT_R8G8B8A8_UNORM };
	sd.Windowed = TRUE;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.BufferCount = swapchainCount;

	IDXGISwapChain* tmpSwapchain;
	HR(factory->CreateSwapChain(cmdQueue, &sd, &tmpSwapchain));
	tmpSwapchain->QueryInterface<IDXGISwapChain3>(&swapchain);
	currentBackBufferIndex = swapchain->GetCurrentBackBufferIndex();

	//DESC HEAPS
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = swapchainCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	HR(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap)));

	rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	assert(rtvHeapSize > 0);

	//RTV
	for (int i = 0; i < swapchainCount; i++)
	{
		HR(swapchain->GetBuffer(i, IID_PPV_ARGS(&rtvs[i])));
		rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += (i * rtvHeapSize);
		device->CreateRenderTargetView(rtvs[i], nullptr, rtvHandle);
	}

	//CMD ALLOC
	HR(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc)));
}

void D3D12RenderSystem::RenderSetup(float deltaTime)
{
}

void D3D12RenderSystem::Render(float deltaTime)
{
}

void D3D12RenderSystem::RenderEnd(float deltaTime)
{
}

void D3D12RenderSystem::CreateDefaultBuffer()
{
}

void D3D12RenderSystem::CreateVertexBuffer(unsigned int size, const void* data, ActorSystem* actor)
{
}

void D3D12RenderSystem::CreateSamplerState(ActorSystem* actorSystem)
{
}

void D3D12RenderSystem::CreateTexture(ActorSystem* actorSystem)
{
}

void D3D12RenderSystem::CreateVertexShader()
{
}

void D3D12RenderSystem::CreatePixelShader()
{
}

void D3D12RenderSystem::CreateAllShaders()
{
}

void* D3D12RenderSystem::GetSwapchain()
{
	return nullptr;
}

void D3D12RenderSystem::Present()
{
}
