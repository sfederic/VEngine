#pragma once

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "IRenderSystem.h"

using namespace Microsoft::WRL;

class D3D12RenderSystem : public IRenderSystem
{
public:
	virtual void Tick() override;
	virtual void Init(HWND window) override;
	virtual void RenderSetup(float deltaTime) override;
	virtual void Render(float deltaTime) override;
	virtual void RenderEnd(float deltaTime) override;
	virtual void CreateDefaultBuffer() override;
	virtual void CreateVertexBuffer(unsigned int size, const void* data, ActorSystem* actor) override;
	virtual void CreateSamplerState(ActorSystem* actorSystem) override;
	virtual void CreateTexture(ActorSystem* actorSystem) override;
	virtual void CreateVertexShader() override;
	virtual void CreatePixelShader() override;
	virtual void CreateAllShaders() override;
	virtual void* GetSwapchain() override;
	virtual void Present() override;

	static const int swapchainCount = 2;

	IDXGIFactory1* factory;
	ID3D12Device* device;
	ID3D12CommandQueue* cmdQueue;
	ID3D12GraphicsCommandList* cmdList;
	ID3D12CommandAllocator* cmdAlloc;
	IDXGISwapChain3* swapchain;
	ID3D12DescriptorHeap* rtvHeap;
	ID3D12DescriptorHeap* dsvHeap;
	ID3D12DescriptorHeap* cbHeap;
	ID3D12Resource* rtvs[swapchainCount];
	ID3D12RootSignature* rootSig;
	ID3D12PipelineState* pipelineState;
	ID3D12Fence* fence;

	//IDxcCompiler* dxcCompiler;
	//IDxcLibrary* dxcLibrary;

	ID3DBlob* vertexCode;
	ID3DBlob* pixelCode;

	//IDxcBlob* vertexCode;
	//IDxcBlob* pixelCode;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	ComPtr<ID3D12Resource> vertexBuffer;
	ComPtr<ID3D12Resource> cbUploadBuffer;
	ComPtr<ID3D12Resource> cbUploadMaterialBuffer;

	D3D12_VIEWPORT viewport;
	D3D12_RECT scissorRect;

	UINT64 fenceVal;

	HANDLE fenceEvent;

	unsigned int currentBackBufferIndex;
	UINT rtvHeapSize;
	UINT cbHeapSize;
};

