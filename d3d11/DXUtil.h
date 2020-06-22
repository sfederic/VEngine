#pragma once

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1.lib")

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>
#include <d2d1_1.h>
#include <comdef.h>
#include <DirectXMath.h>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
};

void DXTrace(HRESULT hr, const char* filename, const char* func, int line);
#define HR(hr) if(hr != S_OK) { DXTrace(hr, __FILE__, #hr, __LINE__); }
#define HRThrow(hr) if(hr != S_OK) { DXTrace(hr, __FILE__, #hr, __LINE__); throw; }

class DXUtil
{
public:
	void CreateDevice();
	void CreateSwapchain();
	void CreateRTVAndDSV();
	void CreateShaders();
	void CreateInputLayout();
	void CreateRasterizerState();
	void CreateVertexBuffer();

	void Render();

	ID3DBlob* CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target);
	ID3D11Buffer* CreateDefaultBuffer(UINT byteWidth, UINT bindFlags, const void* initData);

	static const int frameCount = 2;

	D3D11_VIEWPORT viewport = { 0.f, 0.f, 800.f, 600.f, 0.f, 1.f };

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain3* swapchain;
	ID3D11RenderTargetView* rtvs[frameCount];
	ID3D11DepthStencilView* dsv;
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11RasterizerState* rastState;
	IDXGIFactory* dxgiFactory;

	ID3DBlob* vertexCode;
	ID3DBlob* pixelCode;

	D3D_FEATURE_LEVEL featureLevel;
};

