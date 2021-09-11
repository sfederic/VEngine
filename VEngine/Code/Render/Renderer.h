#pragma once
#include <vector>
#include <unordered_map>
#include <d3d11.h>
#include <dxgi1_6.h>
#include "RenderTypes.h"
#include "PipelineObjects.h"

struct MeshComponent;

struct Renderer
{
	static const int swapchainCount = 2;

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	ID3D11RenderTargetView* rtvs[swapchainCount];
	
	ID3D11DepthStencilView* dsv;
	ID3D11InputLayout* inputLayout;

	//Rasterizer states
	std::unordered_map<std::string, RastState*> rastStateMap;
	ID3D11RasterizerState* rastStateSolid;
	ID3D11RasterizerState* rastStateWireframe;
	ID3D11RasterizerState* rastStateNoBackCull;

	//Blendstates
	ID3D11BlendState* blendStateAlphaToCoverage;

	//DXGI
	IDXGISwapChain3* swapchain;
	IDXGIFactory6* dxgiFactory;

	//Constant buffers and data
	ID3D11Buffer* cbMatrices;
	ID3D11Buffer* cbMaterial;
	ID3D11Buffer* cbLights;

	//Viewport
	D3D11_VIEWPORT viewport;

	//Queries for GPU profiling
	ID3D11Query* startTimeQuery;
	ID3D11Query* endTimeQuery;
	ID3D11Query* timeDisjointQuery;

	//GPU structures
	std::vector<IDXGIAdapter1*> gpuAdapters;
	std::vector<DXGI_ADAPTER_DESC1> gpuAdaptersDesc;

	bool drawBoundingBoxes = false;
	bool drawAllAsWireframe = false;

	void Init(void* window, int viewportWidth, int viewportHeight);
	void Tick();

private:
	void CreateFactory();
	void CreateDevice();
	void CreateSwapchain(HWND window);
	void CreateRTVAndDSV();
	void CreateInputLayout();
	void CreateRasterizerStates();
	void CreateBlendStates();
	void CreateQueries();
	void CreateMainConstantBuffers();
	void CheckSupportedFeatures();
	void RenderMeshComponents();
	void RenderInstanceMeshComponents();
	void RenderBounds();
	void RenderLightMeshes();
	void UpdateLights();

public:
	void Present();
	void RenderSetup();
	void Render();
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
