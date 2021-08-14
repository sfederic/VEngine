#pragma once
#include <vector>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "RenderTypes.h"
#include "PipelineObjects.h"

using namespace Microsoft::WRL;

struct Renderer
{
	static const int swapchainCount = 2;

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;

	//ComPtr can't be array or whatever REF:https://github-wiki-see.page/m/microsoft/DirectXTK/wiki/ComPtr
	ID3D11RenderTargetView* rtvs[swapchainCount];
	
	ComPtr<ID3D11DepthStencilView> dsv;
	ComPtr<ID3D11InputLayout> inputLayout;

	ComPtr<ID3D11RasterizerState> activeRastState;
	ComPtr<ID3D11RasterizerState> rastStateSolid;
	ComPtr<ID3D11RasterizerState> rastStateWireframe;
	ComPtr<ID3D11RasterizerState> rastStateNoBackCull;

	ComPtr<ID3D11BlendState> blendStateAlphaToCoverage;

	ComPtr<IDXGISwapChain3> swapchain;
	ComPtr<IDXGIFactory6> dxgiFactory;

	ComPtr<ID3DBlob> defaultVertexCode;
	ComPtr<ID3DBlob> defaultPixelCode;
	ComPtr<ID3D11VertexShader> defaultVertexShader;
	ComPtr<ID3D11PixelShader> defaultPixelShader;

	ComPtr<ID3D11Buffer> cbMatrices;

	ShaderMatrices shaderMatrices;

	D3D11_VIEWPORT viewport;

	std::vector<IDXGIAdapter1*> gpuAdapters;
	std::vector<DXGI_ADAPTER_DESC1> gpuAdaptersDesc;

	void Init(void* window, int viewportWidth, int viewportHeight);
	void CreateFactory();
	void CreateDevice();
	void CreateSwapchain(HWND window);
	void CreateRTVAndDSV();
	void CreateDefaultShaders();
	void CreateInputLayout();
	void CreateRasterizerStates();
	void CreateBlendStates();
	void CreateMainConstantBuffers();
	void CheckSupportedFeatures();

	//TODO: think about seperating these into a Renderutils file
	ID3D11Buffer* CreateDefaultBuffer(UINT byteWidth, UINT bindFlags, const void* initData);
	ID3D11Buffer* CreateVertexBuffer(MeshData& meshData);
	ID3D11Buffer* CreateIndexBuffer(MeshData& meshData);
	ID3D11SamplerState* CreateSampler();

	void RenderSetup();
	void Render();
	void Present();
	void* GetSwapchain();
	float GetAspectRatio();
	float GetViewportWidth();
	float GetViewportHeight();
	void SetViewportWidthHeight(float width, float height);
	void ResizeSwapchain(int newWidth, int newHeight);
};

extern Renderer renderer;
