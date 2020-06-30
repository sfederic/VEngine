#pragma once

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d2d1.lib")

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <d2d1_1.h>
#include <comdef.h>
#include <DirectXMath.h>
#include "Win32Util.h"
#include <vector>

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
};

void DXTrace(HRESULT hr, const char* filename, const char* func, int line);
#define HR(hr) if(hr != S_OK) { DXTrace(hr, __FILE__, #hr, __LINE__); throw; }
//#define HRThrow(hr) if(hr != S_OK) { DXTrace(hr, __FILE__, #hr, __LINE__); throw; }

class DXUtil
{
private:
	void DrawActor(class Actor* actor);

public:
	void CreateDevice();
	void CreateSwapchain();
	void CreateRTVAndDSV();
	void CreateShaders();
	void CreateInputLayout();
	void CreateRasterizerStates();
	void CreateVertexBuffer(UINT size, const void* data, class ActorSystem* actor);
	void CreateConstantBuffer();

	void Render(class Camera* camera, class UIContext* ui, class ActorSystem* actorSystem);

	//TODO: put actor array in scene/world structure
	//std::vector<class Actor> actors;

	std::vector<IDXGIAdapter*> adapters;
	std::vector<DXGI_ADAPTER_DESC1> adaptersDesc;

	ID3DBlob* CreateShaderFromFile(const wchar_t* filename, const char* entry, const char* target);
	ID3D11Buffer* CreateDefaultBuffer(UINT byteWidth, UINT bindFlags, const void* initData);

	static const int frameCount = 2;

	D3D11_VIEWPORT viewport = { 0.f, 0.f, (float)windowWidth, (float)windowHeight, 0.f, 1.f };

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain3* swapchain;
	ID3D11RenderTargetView* rtvs[frameCount];
	ID3D11DepthStencilView* dsv;
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11RasterizerState* rastStateSolid;
	ID3D11RasterizerState* rastStateWireframe;
	IDXGIFactory6* dxgiFactory;

	ID3D11Counter* gpuCounter;
	ID3D11Query* disjointQuery;
	ID3D11Query* startTimeQuery;
	ID3D11Query* endTimeQuery;

	ID3D11Buffer* cbMatrices;

	ID3DBlob* vertexCode;
	ID3DBlob* pixelCode;

	D3D_FEATURE_LEVEL featureLevel;

	double renderTime;
};

