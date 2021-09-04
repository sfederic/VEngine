#pragma once
#include <vector>
#include <unordered_map>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "RenderTypes.h"
#include "PipelineObjects.h"

using namespace Microsoft::WRL;

struct MeshComponent;

struct Renderer
{
	static const int swapchainCount = 2;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	//ComPtr can't be array or whatever REF:https://github-wiki-see.page/m/microsoft/DirectXTK/wiki/ComPtr
	ID3D11RenderTargetView* rtvs[swapchainCount];
	
	ComPtr<ID3D11DepthStencilView> dsv;
	ComPtr<ID3D11InputLayout> inputLayout;

	//Rasterizer states
	std::unordered_map<std::string, RastState*> rastStateMap;
	ID3D11RasterizerState* rastStateSolid;
	ID3D11RasterizerState* rastStateWireframe;
	ID3D11RasterizerState* rastStateNoBackCull;

	//Blendstates
	ComPtr<ID3D11BlendState> blendStateAlphaToCoverage;

	//DXGI
	ComPtr<IDXGISwapChain3> swapchain;
	ComPtr<IDXGIFactory6> dxgiFactory;

	//Constant buffers and data
	ComPtr<ID3D11Buffer> cbMatrices;
	ComPtr<ID3D11Buffer> cbMaterial;

	//Viewport
	D3D11_VIEWPORT viewport;

	//GPU structures
	std::vector<IDXGIAdapter1*> gpuAdapters;
	std::vector<DXGI_ADAPTER_DESC1> gpuAdaptersDesc;

	bool drawBoundingBoxes = false;

	void Init(void* window, int viewportWidth, int viewportHeight);
	void Tick();
	void CreateFactory();
	void CreateDevice();
	void CreateSwapchain(HWND window);
	void CreateRTVAndDSV();
	void CreateInputLayout();
	void CreateRasterizerStates();
	void CreateBlendStates();
	void CreateMainConstantBuffers();
	void CheckSupportedFeatures();
	void RenderSetup();
	void Render();
	void RenderMeshComponents();
	void RenderInstanceMeshComponents();
	void RenderBounds();
	void Present();
	void* GetSwapchain();
	float GetAspectRatio();
	float GetViewportWidth();
	float GetViewportHeight();
	void SetViewportWidthHeight(float width, float height);
	void ResizeSwapchain(int newWidth, int newHeight);

private:
	void SetRenderPipelineStates(MeshComponent* mesh);
};

extern Renderer renderer;
