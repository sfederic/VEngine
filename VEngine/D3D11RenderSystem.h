#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>

#include <DirectXMath.h>
#include <vector>
#include "Debug.h"
#include "RenderTypes.h"
#include "IRenderSystem.h"

using namespace DirectX;

extern Vertex debugLineData[2];

class ISampler;
class ITexture;
class IShaderView;

class D3D11RenderSystem : public IRenderSystem
{
public:
	D3D11RenderSystem();

	virtual void Tick() override;
	virtual void Init(HWND window) override;
	virtual void CreateVertexShader();
	virtual void CreatePixelShader();
	virtual void CreateAllShaders();
	virtual void CreateVertexBuffer(UINT size, const void* data, class ActorSystem* actor) override;
	virtual IDXGISwapChain3* GetSwapchain();
	virtual void Present();
	virtual void Flush();

	void CreateDefaultBuffer();
	void CreateDevice();
	void CreateSwapchain(HWND windowHandle);
	void CreateRTVAndDSV();
	void CreateShaders();
	void CreateInputLayout();
	void CreateRasterizerStates();
	void CreateConstantBuffer();

	virtual void CreateShaderView(IShaderView* shaderView, ITexture* texture) override;
	virtual void CreateSamplerState(ISampler* sampler) override;
	virtual void CreateTexture(class ActorSystem* actorSystem) override;

	virtual void RenderSetup(float deltaTime);
	void RenderActorSystem(class World* world);
	void RenderBounds();
	virtual void Render(float deltaTime);
	virtual void RenderEnd(float deltaTime);

	std::vector<IDXGIAdapter1*> adapters;
	std::vector<DXGI_ADAPTER_DESC1> adaptersDesc;

	std::vector<Vertex> debugLines;
	std::vector<XMMATRIX> debugLineMatrices;

	ID3DBlob* CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target);
	ID3D11Buffer* CreateDefaultBuffer(UINT byteWidth, UINT bindFlags, const void* initData);
	ID3D11Buffer* CreateDyamicBuffer(UINT byteWidth, UINT bindFlags, const void* initData);

	static const int frameCount = 2;

	D3D11_VIEWPORT viewport;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain3* swapchain;
	ID3D11RenderTargetView* rtvs[frameCount];
	ID3D11DepthStencilView* dsv;
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11RasterizerState* rastStateSolid;
	ID3D11RasterizerState* rastStateNoBackCull;
	ID3D11RasterizerState* rastStateWireframe;
	ID3D11RasterizerState* activeRastState;
	IDXGIFactory6* dxgiFactory;

	ID3D11Query* disjointQuery;
	ID3D11Query* startTimeQuery;
	ID3D11Query* endTimeQuery;

	ID3D11Buffer* cbMatrices;
	ID3D11Buffer* cbMaterial;

	ID3DBlob* vertexCode;
	ID3DBlob* pixelCode;

	D3D_FEATURE_LEVEL featureLevel;

	double renderTime;

	bool bDrawBoundingBoxes = false;
	bool bDrawBoundingSpheres = false;
	bool bQueryGPU = false;
	bool bQueryGPUInner = false;
};
