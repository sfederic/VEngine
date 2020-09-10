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

	//D3D11on12 (For D2D/DWrite)
	//NOTE: With D3D11on12 and all the debug information, some builds were giving 500MB for hello world tier programs
	{	
		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_11_0
		};
		D3D_FEATURE_LEVEL level;

		UINT d3dDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		HR(D3D11On12CreateDevice(device, d3dDeviceFlags, levels, _countof(levels), (IUnknown**)&cmdQueue,
			1, 0, &d3d11Device, &d3d11DeviceContext, &level));
		HR(d3d11Device->QueryInterface(&d3d11On12Device));
	}

	InitD2D();

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

	//CMDLIST
	HR(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc, pipelineState, IID_PPV_ARGS(&cmdList)));
	HR(cmdList->Close());

	HR(cmdAlloc->Reset());
	HR(cmdList->Reset(cmdAlloc, pipelineState));

	//FENCE
	HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	fenceVal = 1;

	fenceEvent = CreateEvent(nullptr, false, false, nullptr);
}

void D3D12RenderSystem::RenderSetup(float deltaTime)
{
}

void D3D12RenderSystem::Render(float deltaTime)
{
	HR(cmdAlloc->Reset());
	HR(cmdList->Reset(cmdAlloc, pipelineState));

	//UpdateConstantBuffer(cbUploadBuffer.Get(), sizeof(matrices), &matrices);
	//UpdateConstantBuffer(cbUploadMaterialBuffer.Get(), sizeof(Material), &material);

	cmdList->SetGraphicsRootSignature(rootSig);
	cmdList->SetDescriptorHeaps(1, &cbHeap);
	cmdList->SetGraphicsRootDescriptorTable(0, cbHeap->GetGPUDescriptorHandleForHeapStart());
	cmdList->RSSetViewports(1, &viewport);
	cmdList->RSSetScissorRects(1, &scissorRect);

	cmdList->ResourceBarrier(1, &ResourceBarrier::Transition(rtvs[currentBackBufferIndex],
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += (currentBackBufferIndex * rtvHeapSize);
	cmdList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	const float clearColour[4] = { 0.25f, 0.25f, 0.25f, 1.f };
	cmdList->ClearRenderTargetView(rtvHandle, clearColour, 0, nullptr);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetVertexBuffers(0, 1, &vertexBufferView);
	cmdList->DrawInstanced(3, 1, 0, 0);

	//Resource Barrier gone with D3D11on12, barrier is with wrappedBackBuffer creation
	//cmdList->ResourceBarrier(1, &ResourceBarrier::Transition(rtvs[currentBackBufferIndex],
		//D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	HR(cmdList->Close());

	ID3D12CommandList* cmdLists[] = { cmdList };
	cmdQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	//UI
	d3d11On12Device->AcquireWrappedResources(&wrappedBackBuffers[currentBackBufferIndex], 1);

	d2dDeviceContext->SetTarget(d2dRenderTargets[currentBackBufferIndex]);
	d2dDeviceContext->BeginDraw();
	d2dDeviceContext->DrawTextA(L"hello", 5, textFormat, { 0.f, 0.f, 100.f, 100.f }, brushText);
	d2dDeviceContext->EndDraw();

	d3d11On12Device->ReleaseWrappedResources(&wrappedBackBuffers[currentBackBufferIndex], 1);
	d3d11DeviceContext->Flush();

	HR(swapchain->Present(1, 0))

	WaitForPreviousFrame();
	currentBackBufferIndex = swapchain->GetCurrentBackBufferIndex();
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

void D3D12RenderSystem::Flush()
{
	ExecuteCommandLists();
	WaitForPreviousFrame();
}

void D3D12RenderSystem::ExecuteCommandLists()
{
	HR(cmdList->Close());

	ID3D12CommandList* cmdLists[] = { cmdList };
	cmdQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
}

void D3D12RenderSystem::InitD2D()
{
	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, options, &d2dFactory)); 

	HR(d3d11On12Device->QueryInterface(&dxgiDevice));

	assert(dxgiDevice);
	HR(d2dFactory->CreateDevice(dxgiDevice, &d2dDevice));
	assert(d2dDevice);

	HR(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &d2dDeviceContext));

	D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(writeFactory), (IUnknown**)(&writeFactory)));

	//DirectWrite Init
	HR(writeFactory->CreateTextFormat(L"Terminal", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 14.f, L"en-us", &textFormat));

	HR(writeFactory->CreateTextFormat(L"Terminal", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL, 14.f, L"en-us", &textFormat));

	D2D1_COLOR_F color = { 1.f, 0.f, 0.f, 1.f };
	HR(d2dDeviceContext->CreateSolidColorBrush(color, &brushText));
}

void D3D12RenderSystem::UpdateConstantBuffer(ID3D12Resource* constBuffer, int byteWidth, void* initData)
{
	UINT8* data = nullptr;
	constBuffer->Map(0, nullptr, (void**)&data);
	memcpy(data, initData, byteWidth);
	constBuffer->Unmap(0, nullptr);
}

void D3D12RenderSystem::WaitForPreviousFrame()
{
	const UINT64 tempFenceVal = fenceVal;
	cmdQueue->Signal(fence, tempFenceVal);
	fenceVal++;

	if (fence->GetCompletedValue() < tempFenceVal)
	{
		fence->SetEventOnCompletion(tempFenceVal, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}
}
