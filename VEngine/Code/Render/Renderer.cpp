#include "vpch.h"
#include "Renderer.h"
#include <dxgi1_6.h>
#include "RenderTypes.h"
#include "PipelineObjects.h"
#include <ScreenGrab.h>
#include <SHMath/DirectXSH.h>
#include <WinCodec.h> //For GUID_ContainerFormatJpeg
#include <filesystem>
#include "Debug.h"
#include "Core.h"
#include "Log.h"
#include "VMath.h"
#include "ShaderSystem.h"
#include "Camera.h"
#include "UI/UISystem.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/InstanceMeshComponent.h"
#include "Components/Lights/DirectionalLightComponent.h"
#include "Components/Lights/PointLightComponent.h"
#include "Components/Lights/SpotLightComponent.h"
#include "Components/WidgetComponent.h"
#include "Actors/Actor.h"
#include "Actors/DiffuseProbeMap.h"
#include "Actors/ReflectionPlane.h"
#include "Actors/PostProcessInstance.h"
#include "DebugActors/DebugBox.h"
#include "DebugActors/DebugSphere.h"
#include "DebugActors/DebugIcoSphere.h"
#include "DebugActors/DebugCamera.h"
#include "DebugActors/DebugCone.h"
#include "Input.h"
#include "Material.h"
#include "Profile.h"
#include "RenderUtils.h"
#include "Editor/DebugMenu.h"
#include "ShadowMap.h"
#include "TextureSystem.h"
#include "Render/SpriteSystem.h"
#include "Particle/ParticleEmitter.h"
#include "Particle/SpriteSheet.h"
#include "Particle/Polyboard.h"
#include "Gameplay/GameInstance.h"
#include "Texture2D.h"

void CreateFactory();
void CreateDevice();
void CreateSwapchain(HWND window);
void CreateRTVAndDSV();
void CreateInputLayout();
void CreateRasterizerStates();
void CreateBlendStates();
void CreateQueries();
void CreateConstantBuffers();
void CreatePlanarReflectionBuffers();
void CreateLightProbeBuffers();
void CheckSupportedFeatures();
void RenderShadowPass();
void RenderMeshComponents();
void RenderPlanarReflections();
void RenderInstanceMeshComponents();
void RenderBounds();
void RenderCameraMeshes();
void RenderLightMeshes();
void RenderSkeletonBones();
void RenderPolyboards();
void RenderSpriteSheets();
void RenderPostProcess();
void AnimateSkeletalMesh(MeshComponent* mesh);
void UpdateLights();
void StartGPUQueries();
void EndGPUQueries();
void GetGPUQueryData();
void MapBuffer(ID3D11Resource* resource, const void* src, size_t size);
void DrawMesh(MeshComponent* mesh);

//Inner render functions to set shader resources
void SetNullRTV();
void SetShadowData();
void SetLightResources();
void SetShadowResources();
void SetReflectionResources();
void SetMatricesFromMesh(MeshComponent* mesh);
void SetShaderMeshData(MeshComponent* mesh);
void SetRenderPipelineStates(MeshComponent* mesh);
void SetRenderPipelineStatesForShadows(MeshComponent* mesh);
void SetShader(std::wstring shaderName);
void SetRastState(std::string rastStateName);
void SetConstantBufferVertexPixel(uint32_t shaderRegister, ID3D11Buffer* constantBuffer);
void SetConstantBufferVertex(uint32_t shaderRegister, ID3D11Buffer* constantBuffer);
void SetConstantBufferPixel(uint32_t shaderRegister, ID3D11Buffer* constantBuffer);
void SetVertexBuffer(Buffer* vertexBuffer);
void SetIndexBuffer(Buffer* indexBuffer);

//Changes the global ambient param passed into shaders to change based on the day-night cycle in-game.
XMFLOAT4 CalcGlobalAmbientBasedOnGameTime();

void CreatePostProcessRenderTarget();

float Renderer::frameTime;
bool Renderer::drawBoundingBoxes = false;
bool Renderer::drawTriggers = true;
bool Renderer::drawAllAsWireframe = false;

unsigned int Renderer::stride = sizeof(Vertex);
unsigned int Renderer::offset = 0;

DXGI_FORMAT indexBufferFormat = DXGI_FORMAT_R32_UINT;

ID3D11Texture2D* backBuffer;
ID3D11Texture2D* depthStencilBuffer;

static const int swapchainCount = 2;

ID3D11Device* device;
ID3D11DeviceContext* context;

ID3D11RenderTargetView* rtvs[swapchainCount];

ID3D11DepthStencilView* dsv;
ID3D11InputLayout* inputLayout;

//Rasterizer states
std::unordered_map<std::string, RastState*> rastStateMap;
std::unordered_map<std::string, BlendState*> blendStateMap;
ID3D11RasterizerState* rastStateSolid;
ID3D11RasterizerState* rastStateWireframe;
ID3D11RasterizerState* rastStateNoBackCull;
ID3D11RasterizerState* rastStateShadow;

//Blendstates
ID3D11BlendState* nullBlendState = nullptr;
ID3D11BlendState* blendStateAlphaToCoverage = nullptr;

//DXGI
IDXGISwapChain3* swapchain;
IDXGIFactory6* dxgiFactory;

//Constant buffers and data
ID3D11Buffer* cbMatrices;
ID3D11Buffer* cbMaterial;
ID3D11Buffer* cbLights;
ID3D11Buffer* cbTime;
ID3D11Buffer* cbMeshData;
ID3D11Buffer* cbSkinningData;
ID3D11Buffer* cbPostProcess;
ID3D11Buffer* linesBuffer;

//Viewport
D3D11_VIEWPORT viewport;

//Shadow maps
ShadowMap* shadowMap;

//Reflection
ID3D11RenderTargetView* reflectionRTV;
ID3D11ShaderResourceView* reflectionSRV;
ID3D11Texture2D* reflectionTex;

//Light probe buffers
ID3D11RenderTargetView* lightProbeRTVs[6]; //Cubemap
ID3D11ShaderResourceView* lightProbeSRV = nullptr;
ID3D11Texture2D* lightProbeTexture = nullptr;

//Post process
ID3D11Texture2D* postBuffer = nullptr;
ID3D11RenderTargetView* postRTV = nullptr;
ID3D11ShaderResourceView* postSRV = nullptr;

//Queries for GPU profiling (Note that the queires are double buffered to deal with two frames for the GPU
//being ahead of the GPU)
ID3D11Query* frameStartQuery[2];
ID3D11Query* frameEndQuery[2];
ID3D11Query* timeDisjointQuery[2];
int frameQueryIndex = 0;
int framesCollected = -1;

//Quality = 0 and Count = 1 are the 'default'
DXGI_SAMPLE_DESC sampleDesc;

const int cbMatrixRegister = 0;
const int cbMaterialRegister = 1;
const int cbSkinningRegister = 2;
const int cbLightsRegister = 3;
const int cbTimeRegister = 4;
const int cbMeshDataRegister = 5;

const int shadowMapTextureResgiter = 1;
const int reflectionTextureResgiter = 2;
const int instanceSRVRegister = 3;
const int environmentMapTextureRegister = 4;

const int lightProbeTextureWidth = 64;
const int lightProbeTextureHeight = 64;

ShaderMatrices shaderMatrices;
ShaderLights shaderLights;

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

	shaderSystem.Init();

	shadowMap = new ShadowMap(device, 2048, 2048);

	CheckSupportedFeatures();

	CreateSwapchain((HWND)window);
	CreateRTVAndDSV();
	CreateInputLayout();
	CreateRasterizerStates();
	CreateBlendStates();
	CreateConstantBuffers();
	CreateQueries();

	CreatePostProcessRenderTarget();

	RenderUtils::defaultSampler = RenderUtils::CreateSampler();

	Line lines[128] = {};
	linesBuffer = RenderUtils::CreateDynamicBuffer(1024, D3D11_BIND_VERTEX_BUFFER, lines);

	spriteSystem.Init();

	CreatePlanarReflectionBuffers();
}

void Renderer::Tick()
{
	//BOUNDING BOXES HOTKEY
	if (Input::GetAsyncKey(Keys::Ctrl))
	{
		if (Input::GetKeyUp(Keys::B))
		{
			drawBoundingBoxes = !drawBoundingBoxes;
		}
	}

	//RENDER TRIGGERS HOTKEY
	if (Input::GetAsyncKey(Keys::Ctrl))
	{
		if (Input::GetKeyUp(Keys::T))
		{
			drawTriggers = !drawTriggers;
		}
	}

	//DRAW ALL AS WIREFRAME HOTKEY
	if (Input::GetKeyUp(Keys::F2))
	{
		drawAllAsWireframe = !drawAllAsWireframe;
	}

	//Screenshot
	ScreenshotCapture();
}

void CreateFactory()
{
	IDXGIFactory* tempDxgiFactory = nullptr;
	HR(CreateDXGIFactory(IID_PPV_ARGS(&tempDxgiFactory)));
	HR(tempDxgiFactory->QueryInterface(&dxgiFactory));
	tempDxgiFactory->Release();
}

void CreateDevice()
{
	//BGRA support needed for DirectWrite and Direct2D
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
	D3D_FEATURE_LEVEL selectedFeatureLevel;

	//@Todo: this shit always causes problems. From HDR to graphics debugging, all sorts of issues.
	//IDXGIAdapter1* adapter = nullptr;
	//HR(dxgiFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));
	//DXGI_ADAPTER_DESC1 desc = {};
	//adapter->GetDesc1(&desc);
	//gpuAdaptersDesc.push_back(desc);
	//for (int i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_MINIMUM_POWER,
	//	IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND; i++)
	//{
	//	DXGI_ADAPTER_DESC1 desc = {};
	//	adapter->GetDesc1(&desc);
	//	gpuAdaptersDesc.push_back(desc);
	//}

	HR(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
		featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &device,
		&selectedFeatureLevel, &context));

	RenderUtils::device = device;
	RenderUtils::context = context;
}

void CreateSwapchain(HWND window)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc = { (UINT)viewport.Width, (UINT)viewport.Height, {60, 1}, DXGI_FORMAT_R16G16B16A16_FLOAT };
	sd.Windowed = TRUE;
	sd.SampleDesc = sampleDesc;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.BufferCount = swapchainCount;

	IDXGISwapChain* tempSwapchain = nullptr;
	HR(dxgiFactory->CreateSwapChain(device, &sd, &tempSwapchain));
	HR(tempSwapchain->QueryInterface(&swapchain));
	tempSwapchain->Release();

	HR(swapchain->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709));

	//Check for colour space (HDR, sRGB)
	//IDXGIOutput* output = nullptr;
	//HR(swapchain->GetContainingOutput(&output));
	//IDXGIOutput6* output6 = nullptr;
	//HR(output->QueryInterface<IDXGIOutput6>(&output6));
	//DXGI_OUTPUT_DESC1 outputDesc = {};
	//HR(output6->GetDesc1(&outputDesc));

	dxgiFactory->MakeWindowAssociation(window, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
}

void CreateRTVAndDSV()
{
	swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	//Create Render target views
	for (int i = 0; i < swapchainCount; i++)
	{
		HR(device->CreateRenderTargetView(backBuffer, nullptr, &rtvs[i]));
	}

	//Create depth stencil view
	D3D11_TEXTURE2D_DESC dsDesc = {};
	dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsDesc.ArraySize = 1;
	dsDesc.MipLevels = 1;
	dsDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsDesc.SampleDesc = sampleDesc;
	dsDesc.Width = viewport.Width;
	dsDesc.Height = viewport.Height;

	HR(device->CreateTexture2D(&dsDesc, nullptr, &depthStencilBuffer));
	assert(depthStencilBuffer);
	HR(device->CreateDepthStencilView(depthStencilBuffer, nullptr, &dsv));
}

void CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, weights), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, offsetof(Vertex, boneIndices), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ShaderItem* shader = shaderSystem.FindShader(L"DefaultShader.hlsl");

	HR(device->CreateInputLayout(inputDesc, _countof(inputDesc), shader->vertexCode->GetBufferPointer(), shader->vertexCode->GetBufferSize(), &inputLayout));
	context->IASetInputLayout(inputLayout);
}

void CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FrontCounterClockwise = FALSE;

	//SOLID
	{
		HR(device->CreateRasterizerState(&rastDesc, &rastStateSolid));
		RastState* rs = new RastState(RastStates::solid, rastStateSolid);
		rastStateMap[rs->name] = rs;
	}

	//WIREFRAME
	{
		rastDesc.FillMode = D3D11_FILL_WIREFRAME;
		rastDesc.CullMode = D3D11_CULL_NONE;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateWireframe));
		RastState* rs = new RastState(RastStates::wireframe, rastStateWireframe);
		rastStateMap[rs->name] = rs;
	}

	//SOLID, NO BACK CULL
	{
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateNoBackCull));
		RastState* rs = new RastState(RastStates::noBackCull, rastStateNoBackCull);
		rastStateMap[rs->name] = rs;
	}

	//SHADOWS
	{
		rastDesc.CullMode = D3D11_CULL_BACK;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.DepthBias = 100000;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.SlopeScaledDepthBias = 1.0f;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateShadow));
		RastState* rs = new RastState(RastStates::shadow, rastStateShadow);
		rastStateMap[rs->name] = rs;
	}
}

void CreateBlendStates()
{
	//NULL BLEND STATE
	{
		D3D11_BLEND_DESC nullBlendDesc = {};
		nullBlendDesc.RenderTarget[0].BlendEnable = false;
		HR(device->CreateBlendState(&nullBlendDesc, &blendStateAlphaToCoverage));

		BlendState* bs = new BlendState(BlendStates::null, nullBlendState);
		blendStateMap[bs->name] = bs;
	}

	//DEFAULT BLEND STATE
	{
		D3D11_BLEND_DESC alphaToCoverageDesc = {};
		//MSAA has to be set for AlphaToCoverage to work.
		//alphaToCoverageDesc.AlphaToCoverageEnable = true;
		alphaToCoverageDesc.RenderTarget[0].BlendEnable = true;
		alphaToCoverageDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		alphaToCoverageDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		alphaToCoverageDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		alphaToCoverageDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		alphaToCoverageDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		alphaToCoverageDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HR(device->CreateBlendState(&alphaToCoverageDesc, &blendStateAlphaToCoverage));

		BlendState* bs = new BlendState(BlendStates::Default, blendStateAlphaToCoverage);
		blendStateMap[bs->name] = bs;
	}
}

void CreateQueries()
{
	D3D11_QUERY_DESC qd = {};

	//Create disjoint queries
	qd.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	HR(device->CreateQuery(&qd, &timeDisjointQuery[0]));
	HR(device->CreateQuery(&qd, &timeDisjointQuery[1]));

	//Create frame start/end queries
	qd.Query = D3D11_QUERY_TIMESTAMP;

	HR(device->CreateQuery(&qd, &frameStartQuery[0]));
	HR(device->CreateQuery(&qd, &frameStartQuery[1]));

	HR(device->CreateQuery(&qd, &frameEndQuery[0]));
	HR(device->CreateQuery(&qd, &frameEndQuery[1]));
}

void CreateConstantBuffers()
{
	//Shader matrix constant buffer
	shaderMatrices.Create();

	cbMatrices = RenderUtils::CreateDynamicBuffer(sizeof(shaderMatrices), D3D11_BIND_CONSTANT_BUFFER, &shaderMatrices);
	assert(cbMatrices);

	//Material buffer
	MaterialShaderData materialShaderData;
	cbMaterial = RenderUtils::CreateDynamicBuffer(sizeof(MaterialShaderData), 
		D3D11_BIND_CONSTANT_BUFFER, &materialShaderData);
	assert(cbMaterial);

	//Lights buffer
	ShaderLights shaderLights;
	cbLights = RenderUtils::CreateDynamicBuffer(sizeof(ShaderLights),
		D3D11_BIND_CONSTANT_BUFFER, &shaderLights);
	assert(cbLights);

	//Time buffer
	ShaderTimeData timeData = {};
	cbTime = RenderUtils::CreateDynamicBuffer(sizeof(ShaderTimeData),
		D3D11_BIND_CONSTANT_BUFFER, &timeData);
	assert(cbTime);

	//Mesh data buffer
	ShaderMeshData meshData = {};
	cbMeshData = RenderUtils::CreateDynamicBuffer(sizeof(ShaderMeshData),
		D3D11_BIND_CONSTANT_BUFFER, &meshData);
	assert(cbMeshData);

	//Skinning data
	XMMATRIX skinningMatrices[96] = {};
	cbSkinningData = RenderUtils::CreateDynamicBuffer(sizeof(XMMATRIX) * 96,
		D3D11_BIND_CONSTANT_BUFFER, skinningMatrices);
	assert(cbSkinningData);

	//Post process data
	ShaderPostProcessData postProcessData = {};
	cbPostProcess = RenderUtils::CreateDynamicBuffer(sizeof(ShaderPostProcessData),
		D3D11_BIND_CONSTANT_BUFFER, &postProcessData);
	assert(cbPostProcess);
}

void CreatePlanarReflectionBuffers()
{
	//if (reflectionTex)
	//{
	//	reflectionTex->Release();
	//}

	////Create texture
	//D3D11_TEXTURE2D_DESC texDesc = {};
	//texDesc.Width = viewport.Width;
	//texDesc.Height = viewport.Height;
	//texDesc.MipLevels = 1;
	//texDesc.ArraySize = 1;
	//texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	//texDesc.SampleDesc.Count = 1;
	//texDesc.Usage = D3D11_USAGE_DEFAULT;
	//texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	//HR(device->CreateTexture2D(&texDesc, 0, &reflectionTex));

	//if (reflectionSRV)
	//{
	//	reflectionSRV->Release();
	//}

	////Create SRV
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Format = texDesc.Format;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MipLevels = 1;
	//srvDesc.Texture2D.MostDetailedMip = 0;
	//assert(reflectionTex);
	//HR(device->CreateShaderResourceView(reflectionTex, &srvDesc, &reflectionSRV));

	//if (reflectionRTV)
	//{
	//	reflectionRTV->Release();
	//}

	////Create RTV
	//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	//rtvDesc.Format = texDesc.Format;
	//rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	//rtvDesc.Texture2D.MipSlice = 0;
	//HR(device->CreateRenderTargetView(reflectionTex, &rtvDesc, &reflectionRTV));
}

void MapBuffer(ID3D11Resource* resource, const void* src, size_t size)
{
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	HR(context->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
	std::memcpy(mapped.pData, src, size);
	context->Unmap(resource, 0);
}

void SetNullRTV()
{
	ID3D11RenderTargetView* nullRTV = nullptr;
	context->OMSetRenderTargets(1, &nullRTV, nullptr);
}

void SetShadowData()
{
	if (!DirectionalLightComponent::system.components.empty())
	{
		shaderMatrices.lightMVP = shadowMap->OutputMatrix();
		shaderMatrices.lightViewProj = shadowMap->GetLightViewMatrix() * shadowMap->GetLightPerspectiveMatrix();

		shaderLights.shadowsEnabled = true;
	}
	else
	{
		shaderLights.shadowsEnabled = false;
	}
}

void DrawMesh(MeshComponent* mesh)
{
	context->DrawIndexed(mesh->meshDataProxy->indices->size(), 0, 0);
}

void CheckSupportedFeatures()
{
	//Threading check
	D3D11_FEATURE_DATA_THREADING threadFeature = {};
	HR(device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadFeature, sizeof(threadFeature)));

	//Setting sample count higher (4, 8) is ramps up GPU usage by 5-6%
	const int sampleCount = 1;

	//MSAA check and set
	UINT msaaQualityLevel;
	HR(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &msaaQualityLevel));
	assert(msaaQualityLevel > 0);

	//Quality has to be one less than what CheckMultisampleQualityLevels() spits out for some reason
	sampleDesc.Quality = msaaQualityLevel - 1;
	sampleDesc.Count = sampleCount;
}

void RenderShadowPass()
{
	PROFILE_START

	shadowMap->BindDsvAndSetNullRenderTarget(context);

	for (auto mesh : MeshComponent::system.components)
	{
		if (!mesh->castsShadow || !mesh->active)
		{
			continue;
		}

		SetRenderPipelineStatesForShadows(mesh);

		//Set matrices
		shaderMatrices.model = mesh->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		shaderMatrices.MakeTextureMatrix(mesh->material);

		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		//Set textures
		Material* mat = mesh->material;
		context->PSSetSamplers(0, 1, &mat->sampler->data);
		context->PSSetShaderResources(0, 1, &mat->texture->srv);

		//Draw
		context->DrawIndexed(mesh->meshDataProxy->indices->size(), 0, 0);
	}

	SetNullRTV();

	PROFILE_END
}

void RenderSetup()
{
	context->RSSetViewports(1, &viewport);

	const float clearColour[4] = { 0.f, 0.f, 0.f, 1.f };
	UINT frameIndex = swapchain->GetCurrentBackBufferIndex();

	context->ClearRenderTargetView(rtvs[frameIndex], clearColour);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	context->OMSetRenderTargets(1, &rtvs[frameIndex], dsv);

	context->IASetInputLayout(inputLayout);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void RenderPostProcessSetup()
{
	context->RSSetViewports(1, &viewport);

	const float clearColour[4] = { 0.f, 0.f, 0.f, 1.f };
	UINT frameIndex = swapchain->GetCurrentBackBufferIndex();

	context->ClearRenderTargetView(postRTV, clearColour);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	context->OMSetRenderTargets(1, &postRTV, dsv);

	context->IASetInputLayout(inputLayout);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void SetShadowResources()
{
	context->PSSetShaderResources(shadowMapTextureResgiter, 1, &shadowMap->depthMapSRV);
	context->PSSetSamplers(1, 1, &shadowMap->sampler);
}

void Renderer::Render()
{
	PROFILE_START

	StartGPUQueries();

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.lightMVP = shadowMap->OutputMatrix();
	shaderMatrices.lightViewProj = shadowMap->GetLightViewMatrix() * shadowMap->GetLightPerspectiveMatrix();

	ShaderTimeData timeData = {};
	timeData.deltaTime = Core::GetDeltaTime();
	timeData.timeSinceStartup = Core::timeSinceStartup;

	//Set time constant buffer
	MapBuffer(cbTime, &timeData, sizeof(ShaderTimeData));
	context->VSSetConstantBuffers(cbTimeRegister, 1, &cbTime);

	//RenderShadowPass();
	//RenderPlanarReflections();
	RenderMeshComponents();
	RenderInstanceMeshComponents();
	RenderPolyboards();
	RenderSpriteSheets();
	RenderBounds();
	//RenderSkeletonBones();
	RenderLightMeshes();
	RenderCameraMeshes();
	RenderPostProcess();

	PROFILE_END
}

void SetReflectionResources()
{
	context->PSSetShaderResources(reflectionTextureResgiter, 1, &reflectionSRV);
}

void SetMatricesFromMesh(MeshComponent* mesh)
{
	shaderMatrices.model = mesh->GetWorldMatrix();
	shaderMatrices.MakeModelViewProjectionMatrix();
	shaderMatrices.MakeTextureMatrix(mesh->material);

	MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
	context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);
}

void SetShaderMeshData(MeshComponent* mesh)
{
	ShaderMeshData meshData = {};
	meshData.position = mesh->GetPosition();

	//@Todo: light probe data should have its own constant buffer, for now in testing, it's part of ShaderMeshData
	//Set light probe resources
	if (!DiffuseProbeMap::system.actors.empty())
	{
		context->PSSetShaderResources(environmentMapTextureRegister, 1, &lightProbeSRV);

		ProbeData probeData = DiffuseProbeMap::system.actors[0]->FindClosestProbe(mesh->GetWorldPositionV());
		memcpy(meshData.SH, probeData.SH, sizeof(XMFLOAT4) * 9);
	}

	MapBuffer(cbMeshData, &meshData, sizeof(ShaderMeshData));
	context->VSSetConstantBuffers(cbMeshDataRegister, 1, &cbMeshData);
	context->PSSetConstantBuffers(cbMeshDataRegister, 1, &cbMeshData);
}

void RenderMeshComponents()
{
	PROFILE_START

	PostProcessInstance::system.GetNumActors() == 0 ? RenderSetup() : RenderPostProcessSetup();

	SetShadowData();
	UpdateLights();

	shaderMatrices.view = activeCamera->GetViewMatrix();

	for (auto mesh : MeshComponent::system.components)
	{
		if (!mesh->active || mesh->cullMesh) continue;

		SetRenderPipelineStates(mesh);

		//Shader Resources
		SetLightResources();
		SetShadowResources();
		SetReflectionResources();

		//Constant buffer data
		SetMatricesFromMesh(mesh);
		SetShaderMeshData(mesh);

		DrawMesh(mesh);
	}	

	//Set to null to remove warnings
	ID3D11ShaderResourceView* nullSRV = nullptr;
	context->PSSetShaderResources(shadowMapTextureResgiter, 1, &nullSRV);
	context->PSSetShaderResources(reflectionTextureResgiter, 1, &nullSRV);

	PROFILE_END
}

void Renderer::RenderLightProbeViews()
{
	auto startTime = Profile::QuickStart();

	int previousWiewportWidth = viewport.Width;
	int previousWiewportHeight = viewport.Height;
	ResizeSwapchain(lightProbeTextureWidth, lightProbeTextureHeight);

	//Directions match with D3D11_TEXTURECUBE_FACE
	XMVECTOR faces[6] =
	{
		XMVectorSet(1.f, 0.f, 0.f, 0.f), //+X
		XMVectorSet(-1.f, 0.f, 0.f, 0.f), //-X
		XMVectorSet(0.f, 1.f, 0.f, 0.f), //+Y
		XMVectorSet(0.f, -1.f, 0.f, 0.f), //-Y
		XMVectorSet(0.f, 0.f, 1.f, 0.f), //+Z
		XMVectorSet(0.f, 0.f, -1.f, 0.f), //-Z
	};

	CreateLightProbeBuffers();

	for (auto probeMap : DiffuseProbeMap::system.actors)
	{
		probeMap->probeData.clear();

		int probeIndex = 0;

		for (auto& probeData : probeMap->instanceMeshComponent->instanceData)
		{
			XMMATRIX& probeMatrix = probeData.world;

			for (int i = 0; i < 6; i++)
			{
				context->RSSetViewports(1, &viewport);
				const float clearColour[4] = { 0.f, 0.f, 0.f, 0.f };
				context->ClearRenderTargetView(lightProbeRTVs[i], clearColour);
				context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				context->OMSetRenderTargets(1, &lightProbeRTVs[i], nullptr);

				context->RSSetState(rastStateMap[RastStates::solid]->data);

				UpdateLights();

				//Set lights buffer
				context->PSSetConstantBuffers(cbLightsRegister, 1, &cbLights);

				//Set shadow resources (not now for lightprobes)
				//context->PSSetShaderResources(shadowMapTextureResgiter, 1, &shadowMap->depthMapSRV);
				//context->PSSetSamplers(1, 1, &shadowMap->sampler);

				ShaderItem* lightProbeShader = shaderSystem.FindShader(L"DefaultShader.hlsl");

				for (auto mesh : MeshComponent::system.components)
				{
					if (!mesh->active || mesh->cullMesh) continue;

					Material* material = mesh->material;

					const FLOAT blendState[4] = { 0.f };
					context->OMSetBlendState(nullptr, blendState, 0xFFFFFFFF);

					context->VSSetShader(lightProbeShader->vertexShader, nullptr, 0);
					context->PSSetShader(lightProbeShader->pixelShader, nullptr, 0);

					context->PSSetSamplers(0, 1, &material->sampler->data);
					context->PSSetShaderResources(0, 1, &material->texture->srv);

					context->IASetVertexBuffers(0, 1, &mesh->pso->vertexBuffer->data, &stride, &offset);
					context->IASetIndexBuffer(mesh->pso->indexBuffer->data, DXGI_FORMAT_R32_UINT, 0);

					MapBuffer(cbMaterial, &material->materialShaderData, sizeof(MaterialShaderData));
					context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);

					//Set matrices
					shaderMatrices.view = XMMatrixLookAtLH(probeMatrix.r[3],
						probeMatrix.r[3] + faces[i], VMath::XMVectorUp());
					shaderMatrices.model = mesh->GetWorldMatrix();
					shaderMatrices.MakeModelViewProjectionMatrix();
					shaderMatrices.MakeTextureMatrix(mesh->material);

					MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
					context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

					//Set mesh data to shader
					ShaderMeshData meshData = {};
					meshData.position = mesh->GetPosition();
					MapBuffer(cbMeshData, &meshData, sizeof(ShaderMeshData));
					context->VSSetConstantBuffers(cbMeshDataRegister, 1, &cbMeshData);
					context->PSSetConstantBuffers(cbMeshDataRegister, 1, &cbMeshData);

					//Draw
					context->DrawIndexed(mesh->meshDataProxy->indices->size(), 0, 0);
				}

				//Remove lightprobe RTV
				SetNullRTV();
			}

			//Remember that there are 9 coefficients with 3rd order SH per channel
			float SH_R[9] = {}, SH_G[9] = {}, SH_B[9] = {};
			HR(DirectX::SHProjectCubeMap(context, 3, lightProbeTexture, SH_R, SH_G, SH_B));

			XMFLOAT4 coefs[9] = {};

			for (int co_index = 0; co_index < 9; co_index++)
			{
				coefs[co_index] = XMFLOAT4(SH_R[co_index], SH_G[co_index], SH_B[co_index], 1.0f);
			}

			//@Todo: this is all just copied from the Common.hlsli to figure out probe colour.
			//You could probably do all this in the InstanceShader and just return the 'nearest probe' as itself.
			const float PI = 3.14159265f;

			float SQRT_PI = 1.7724538509f;
			float SQRT_5 = 2.2360679775f;
			float SQRT_15 = 3.8729833462f;
			float SQRT_3 = 1.7320508076f;

			float AO = 1.0f;

			float Y[9] =
			{
				1.0f / (2.0f * SQRT_PI),
				-SQRT_3 / (2.0f * SQRT_PI),
				SQRT_3 / (2.0f * SQRT_PI),
				-SQRT_3 / (2.0f * SQRT_PI),
				SQRT_15 / (2.0f * SQRT_PI),
				-SQRT_15 / (2.0f * SQRT_PI),
				SQRT_5 / (4.0f * SQRT_PI),
				-SQRT_15 / (2.0f * SQRT_PI),
				SQRT_15 / (4.0f * SQRT_PI)
			};

			float t = acos(sqrt(1 - AO));

			float a = sin(t);
			float b = cos(t);

			float A0 = sqrt(4 * PI) * (sqrt(PI) / 2) * a * a;
			float A1 = sqrt(4 * PI / 3) * (sqrt(3 * PI) / 3) * (1 - b * b * b);
			float A2 = sqrt(4 * PI / 5) * (sqrt(5 * PI) / 16) * a * a * (2 + 6 * b * b);

			XMFLOAT3 n = XMFLOAT3(0.f, 0.f, 0.f); //Essentially put in a random normal
			XMStoreFloat3(&n, XMVector3Normalize(probeMatrix.r[3]));

			XMVECTOR irradiance =
				XMLoadFloat4(&coefs[0]) * A0 * Y[0] +
				XMLoadFloat4(&coefs[0]) * A1 * Y[1] * n.y +
				XMLoadFloat4(&coefs[0]) * A1 * Y[2] * n.z +
				XMLoadFloat4(&coefs[0]) * A1 * Y[3] * n.x +
				XMLoadFloat4(&coefs[0]) * A2 * Y[4] * (n.y * n.x) +
				XMLoadFloat4(&coefs[0]) * A2 * Y[5] * (n.y * n.z) +
				XMLoadFloat4(&coefs[0]) * A2 * Y[6] * (3.0 * n.z * n.z - 1.0) +
				XMLoadFloat4(&coefs[0]) * A2 * Y[7] * (n.z * n.x) +
				XMLoadFloat4(&coefs[0]) * A2 * Y[8] * (n.x * n.x - n.y * n.y);

			irradiance = DirectX::XMVectorMax(irradiance, XMVectorZero());
			irradiance.m128_f32[3] = 1.0f; //Make sure alpha is set
			XMStoreFloat4(&probeData.colour, irradiance);

			ProbeData pd = {};
			pd.index = probeIndex;
			memcpy(pd.SH, coefs, sizeof(XMFLOAT4) * 9);
			XMStoreFloat3(&pd.position, probeMatrix.r[3]);
			probeMap->probeData.push_back(pd);

			probeIndex++;
		}
	}

	ResizeSwapchain(previousWiewportWidth, previousWiewportHeight);

	//Set main RTV and DSV back on
	RenderSetup();

	double endTime = Profile::QuickEnd(startTime);
	Log("Light probe bake took [%f] ms", endTime);
}

void RenderPlanarReflections()
{
	PROFILE_START

	if (ReflectionPlane::system.actors.empty()) return;
	assert(ReflectionPlane::system.actors.size() == 1);

	auto reflectionPlane = ReflectionPlane::system.actors[0];

	context->RSSetViewports(1, &viewport);
	const float clearColour[4] = { 1.f, 0.f, 0.f, 0.f };
	context->ClearRenderTargetView(reflectionRTV, clearColour);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->OMSetRenderTargets(1, &reflectionRTV, nullptr);

	context->RSSetState(rastStateMap[RastStates::solid]->data);

	UpdateLights();

	//Set lights buffer
	context->PSSetConstantBuffers(cbLightsRegister, 1, &cbLights);

	//Set shadow resources (not now for reflections)
	//context->PSSetShaderResources(shadowMapTextureResgiter, 1, &shadowMap->depthMapSRV);
	//context->PSSetSamplers(1, 1, &shadowMap->sampler);

	ShaderItem* planarReflectionShader = shaderSystem.FindShader(L"DefaultShader.hlsl");

	for (auto mesh : MeshComponent::system.components)
	{
		if (!mesh->active || mesh->cullMesh) continue;

		Material* material = mesh->material;

		const FLOAT blendState[4] = { 0.f };
		context->OMSetBlendState(nullptr, blendState, 0xFFFFFFFF);

		context->VSSetShader(planarReflectionShader->vertexShader, nullptr, 0);
		context->PSSetShader(planarReflectionShader->pixelShader, nullptr, 0);

		context->PSSetSamplers(0, 1, &material->sampler->data);
		context->PSSetShaderResources(0, 1, &material->texture->srv);

		context->IASetVertexBuffers(0, 1, &mesh->pso->vertexBuffer->data, &Renderer::stride, &Renderer::offset);
		context->IASetIndexBuffer(mesh->pso->indexBuffer->data, DXGI_FORMAT_R32_UINT, 0);

		MapBuffer(cbMaterial, &material->materialShaderData, sizeof(MaterialShaderData));
		context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);

		//Set matrices
		shaderMatrices.view = reflectionPlane->GetReflectionViewMatrix();
		shaderMatrices.model = mesh->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		shaderMatrices.MakeTextureMatrix(mesh->material);

		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		//Set mesh data to shader
		ShaderMeshData meshData = {};
		meshData.position = mesh->GetPosition();
		MapBuffer(cbMeshData, &meshData, sizeof(ShaderMeshData));
		context->VSSetConstantBuffers(cbMeshDataRegister, 1, &cbMeshData);
		context->PSSetConstantBuffers(cbMeshDataRegister, 1, &cbMeshData);

		//Draw
		context->DrawIndexed(mesh->meshDataProxy->indices->size(), 0, 0);
	}

	//Remove reflection RTV
	SetNullRTV();

	PROFILE_END
}

void RenderInstanceMeshComponents()
{
	//@Todo: shadows for instancemeshes (might not even need it since Grid nodes are the only things rendererd that way)
	//@Todo: animated instance meshes

	PROFILE_START

	//Set matrices
	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.proj = activeCamera->GetProjectionMatrix();
	shaderMatrices.MakeModelViewProjectionMatrix();

	MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
	context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

	for (InstanceMeshComponent* instanceMesh : InstanceMeshComponent::system.components)
	{
		if (!instanceMesh->active) continue;

		SetRenderPipelineStates(instanceMesh);

		//@Todo: clean this up in InstanceMeshComponent
		const float factor[4] = { 0.f };
		context->OMSetBlendState(blendStateMap.find(BlendStates::Default)->second->data, factor, 0xFFFFFFFF);

		//update texture matrix
		shaderMatrices.MakeTextureMatrix(instanceMesh->material);
		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		//Update instance data and set SRV
		MapBuffer(instanceMesh->structuredBuffer, instanceMesh->instanceData.data(), sizeof(InstanceData) * instanceMesh->instanceData.size());
		context->VSSetShaderResources(instanceSRVRegister, 1, &instanceMesh->srv);
		context->PSSetShaderResources(instanceSRVRegister, 1, &instanceMesh->srv);

		//Set lights buffer
		context->PSSetConstantBuffers(cbLightsRegister, 1, &cbLights);

		context->DrawIndexedInstanced(instanceMesh->meshDataProxy->indices->size(), instanceMesh->GetInstanceCount(), 0, 0, 0);
	}

	PROFILE_END
}

void RenderBounds()
{
	static DebugBox debugBox;

	MaterialShaderData materialShaderData;

	if (Renderer::drawBoundingBoxes)
	{
		context->RSSetState(rastStateWireframe);

		ShaderItem* shader = shaderSystem.FindShader(L"SolidColour.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		//@Todo: there's a weird error here where if you create enough lights in the map (about 40),
		//the debug mesh actors will crash here. Tried putting the Debug Actors as global pointers
		//instead of being static, but then Direct2D swapchain/rendertarget errors would happen.
		//Feels like it might be the GPU doing some funny memory thing.
		context->IASetVertexBuffers(0, 1, &debugBox.boxMesh->pso->vertexBuffer->data, &Renderer::stride, &Renderer::offset);

		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		//Set debug wireframe material colour
		materialShaderData.ambient = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
		MapBuffer(cbMaterial, &materialShaderData, sizeof(MaterialShaderData));
		context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);

		for(auto mesh : MeshComponent::system.components)
		{
			BoundingOrientedBox boundingBox = mesh->boundingBox;

			XMFLOAT3 extents = XMFLOAT3(boundingBox.Extents.x * 2.f, boundingBox.Extents.y * 2.f,
				boundingBox.Extents.z * 2.f);

			XMVECTOR center = mesh->GetWorldPositionV() + XMLoadFloat3(&boundingBox.Center);
			XMVECTOR scale = mesh->GetScaleV() * XMLoadFloat3(&extents);

			XMMATRIX boundsMatrix = XMMatrixAffineTransformation(scale,
				XMVectorSet(0.f, 0.f, 0.f, 1.f),
				mesh->GetRotationV(),
				center);

			shaderMatrices.model = boundsMatrix;

			//Set bouding box scale just slightly more than the component to avoid overlap
			shaderMatrices.model.r[0].m128_f32[0] += 0.01f;
			shaderMatrices.model.r[1].m128_f32[1] += 0.01f;
			shaderMatrices.model.r[2].m128_f32[2] += 0.01f;

			shaderMatrices.MakeModelViewProjectionMatrix();
			MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));

			context->Draw(debugBox.boxMesh->meshDataProxy->vertices->size(), 0);
		}
	}

	//DRAW TRIGGER BOUNDS
	if(Renderer::drawTriggers)
	{
		context->RSSetState(rastStateWireframe);

		ShaderItem* shader = shaderSystem.FindShader(L"SolidColour.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		context->IASetVertexBuffers(0, 1, &debugBox.boxMesh->pso->vertexBuffer->data, &Renderer::stride, &Renderer::offset);

		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		shaderMatrices.view = activeCamera->GetViewMatrix();
		shaderMatrices.proj = activeCamera->GetProjectionMatrix();

		for (auto boxTrigger : BoxTriggerComponent::system.components)
		{
			shaderMatrices.model = boxTrigger->GetWorldMatrix();

			//Set to * 2.f because of extents
			shaderMatrices.model.r[0].m128_f32[0] *= boxTrigger->boundingBox.Extents.x * 2.f;
			shaderMatrices.model.r[1].m128_f32[1] *= boxTrigger->boundingBox.Extents.y * 2.f;
			shaderMatrices.model.r[2].m128_f32[2] *= boxTrigger->boundingBox.Extents.z * 2.f;

			shaderMatrices.model.r[3] += XMLoadFloat3(&boxTrigger->boundingBox.Center);

			shaderMatrices.MakeModelViewProjectionMatrix();
			MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));

			//Set trigger wireframe material colour
			materialShaderData.ambient = boxTrigger->renderWireframeColour;
			MapBuffer(cbMaterial, &materialShaderData, sizeof(MaterialShaderData));
			context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);

			context->Draw(debugBox.boxMesh->meshDataProxy->vertices->size(), 0);
		}
	}
}

void CreateLightProbeBuffers()
{
	//Texture
	if (lightProbeTexture) lightProbeTexture->Release();

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = lightProbeTextureWidth;
	texDesc.Height = lightProbeTextureWidth;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 6;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	HR(device->CreateTexture2D(&texDesc, 0, &lightProbeTexture));
	assert(lightProbeTexture);

	//SRV
	if (lightProbeSRV) lightProbeSRV->Release();

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = 1;
	HR(device->CreateShaderResourceView(lightProbeTexture, &srvDesc, &lightProbeSRV));

	//RTVs
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.ArraySize = 1;

	for (int i = 0; i < 6; i++)
	{
		if (lightProbeRTVs[i]) lightProbeRTVs[i]->Release();
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		HR(device->CreateRenderTargetView(lightProbeTexture, &rtvDesc, &lightProbeRTVs[i]));
	}
}

void RenderCameraMeshes()
{
	static DebugCamera debugCamera;

	if (Core::gameplayOn) return;

	MaterialShaderData materialShaderData = {};

	SetRastState(RastStates::wireframe);
	SetShader(L"SolidColour.hlsl");
	SetVertexBuffer(debugCamera.mesh->GetVertexBuffer());
	SetConstantBufferVertexPixel(cbMatrixRegister, cbMatrices);

	materialShaderData.ambient = XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f); //Make cameras red
	MapBuffer(cbMaterial, &materialShaderData, sizeof(MaterialShaderData));
	SetConstantBufferVertexPixel(cbMaterialRegister, cbMaterial);

	for (auto camera : CameraComponent::system.components)
	{
		shaderMatrices.model = camera->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));

		DrawMesh(debugCamera.mesh);
	}
}

void RenderLightMeshes()
{
	static DebugSphere debugSphere;
	static DebugIcoSphere debugIcoSphere;
	static DebugCone debugCone;

	if (Core::gameplayOn) return;

	SetRastState(RastStates::wireframe);
	SetShader(L"SolidColour.hlsl");

	context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.proj = activeCamera->GetProjectionMatrix();

	//Set debug sphere wireframe material colour
	MaterialShaderData materialShaderData;
	materialShaderData.ambient = XMFLOAT4(1.f, 1.f, 0.f, 1.0f);
	MapBuffer(cbMaterial, &materialShaderData, sizeof(MaterialShaderData));
	context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);


	//DIRECTIONAL LIGHTS
	context->IASetVertexBuffers(0, 1, &debugSphere.sphereMesh->pso->vertexBuffer->data, &Renderer::stride, &Renderer::offset);

	for (auto directionalLight : DirectionalLightComponent::system.components)
	{
		shaderMatrices.model = directionalLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));

		context->Draw(debugSphere.sphereMesh->meshDataProxy->vertices->size(), 0);
	}


	//POINT LIGHTS
	context->IASetVertexBuffers(0, 1, &debugIcoSphere.mesh->pso->vertexBuffer->data, &Renderer::stride, &Renderer::offset);

	for (auto pointLight : PointLightComponent::system.components)
	{
		shaderMatrices.model = pointLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));

		context->Draw(debugIcoSphere.mesh->meshDataProxy->vertices->size(), 0);
	}


	//SPOT LIGHTS
	context->IASetVertexBuffers(0, 1, &debugCone.mesh->pso->vertexBuffer->data, &Renderer::stride, &Renderer::offset);

	for (auto spotLight : SpotLightComponent::system.components)
	{
		shaderMatrices.model = spotLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));

		context->Draw(debugCone.mesh->meshDataProxy->vertices->size(), 0);
	}
}

void RenderSkeletonBones()
{
	std::vector<Line> lines;

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	ShaderItem* shader = shaderSystem.FindShader(L"SolidColour.hlsl");
	context->VSSetShader(shader->vertexShader, nullptr, 0);
	context->PSSetShader(shader->pixelShader, nullptr, 0);

	MaterialShaderData materialShaderData = {};
	materialShaderData.ambient = XMFLOAT4(0.54f, 0.80f, 0.92f, 1.0f); //Sky blue
	MapBuffer(cbMaterial, &materialShaderData, sizeof(MaterialShaderData));
	context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);

	for (auto mesh : MeshComponent::system.components)
	{
		for (Joint& joint : mesh->GetSkeleton()->joints)
		{
			if (joint.parentIndex == -1) continue;

			Line line = {};

			Joint& parentJoint = mesh->GetSkeleton()->joints[joint.parentIndex];

			XMMATRIX transformedPose = joint.currentPose;
			XMMATRIX parentPose = parentJoint.currentPose;

			transformedPose *= mesh->GetWorldMatrix();
			parentPose *= mesh->GetWorldMatrix();

			XMStoreFloat3(&line.p1, transformedPose.r[3]);
			XMStoreFloat3(&line.p2, parentPose.r[3]);

			lines.push_back(line);
		}
	}

	D3D11_MAPPED_SUBRESOURCE sub = {};
	sub.RowPitch = sizeof(Line) * lines.size();
	HR(context->Map(linesBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub));
	memcpy(sub.pData, lines.data(), sizeof(Line) * lines.size());
	context->Unmap(linesBuffer, 0);

	context->IASetVertexBuffers(0, 1, &linesBuffer, &Renderer::stride, &Renderer::offset);

	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.proj = activeCamera->GetProjectionMatrix();
	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();

	MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
	context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

	context->Draw(lines.size() * 2, 0);
}

void RenderPolyboards()
{
	PROFILE_START

	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.proj = activeCamera->GetProjectionMatrix();
	shaderMatrices.texMatrix = XMMatrixIdentity();
	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();

	MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
	context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

	const float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	context->OMSetBlendState(blendStateMap[BlendStates::Default]->data, blendFactor, 0xFFFFFFFF);

	if (Renderer::drawAllAsWireframe)
	{
		context->RSSetState(rastStateWireframe);
	}
	else
	{
		context->RSSetState(rastStateMap["nobackcull"]->data);
	}

	ShaderItem* shader = shaderSystem.FindShader(L"Unlit.hlsl");
	context->VSSetShader(shader->vertexShader, nullptr, 0);
	context->PSSetShader(shader->pixelShader, nullptr, 0);

	for (auto polyboard : Polyboard::system.components)
	{
		polyboard->CalcVertices();

		context->PSSetSamplers(0, 1, &RenderUtils::GetDefaultSampler()->data);
		context->PSSetShaderResources(0, 1, &textureSystem.FindTexture2D(polyboard->textureData.filename)->srv);

		//@Todo: I don't know what it is with d3d11, but mapping to vertex/index buffers without NO_OVERWRITE
		//causes flickering on meshes, as if they're sharing memory space or something. Nothing really to fix
		//if I want to keep this, but worth keeping tabs on.
		//VERTEX MAP
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(context->Map(polyboard->vertexBuffer->data, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource));
			memcpy(mappedResource.pData, polyboard->vertices.data(), sizeof(Vertex) * polyboard->vertices.size());
			context->Unmap(polyboard->vertexBuffer->data, 0);
		}

		//INDEX MAP
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			HR(context->Map(polyboard->indexBuffer->data, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource));
			memcpy(mappedResource.pData, polyboard->indices.data(), sizeof(Vertex) * polyboard->indices.size());
			context->Unmap(polyboard->indexBuffer->data, 0);
		}

		context->IASetVertexBuffers(0, 1, &polyboard->vertexBuffer->data, &Renderer::stride, &Renderer::offset);
		context->IASetIndexBuffer(polyboard->indexBuffer->data, DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(polyboard->indices.size(), 0, 0);
	}

	PROFILE_END
}

void RenderSpriteSheets()
{
	PROFILE_START

	for (auto spriteSheet : SpriteSheet::system.components)
	{
		if (Renderer::drawAllAsWireframe)
		{
			context->RSSetState(rastStateWireframe);
		}
		else
		{
			context->RSSetState(rastStateMap[RastStates::noBackCull]->data);
		}

		ShaderItem* shader = shaderSystem.FindShader(L"Unlit.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		context->PSSetSamplers(0, 1, &RenderUtils::GetDefaultSampler()->data);
		context->PSSetShaderResources(0, 1, &textureSystem.FindTexture2D(spriteSheet->textureData.filename)->srv);

		spriteSheet->UpdateSprite();

		spriteSystem.BuildSpriteQuadForSpriteSheetRendering(spriteSheet->sprite);
		spriteSystem.UpdateAndSetSpriteBuffers(context);

		VMath::RotateTowardsCamera(spriteSheet->transform);

		shaderMatrices.model = spriteSheet->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();

		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		context->DrawIndexed(6, 0, 0);
	}

	PROFILE_END
}

void AnimateSkeletalMesh(MeshComponent* mesh)
{
	PROFILE_START

	Skeleton* skeleton = mesh->meshDataProxy->skeleton;

	if (skeleton && !skeleton->joints.empty())
	{
		std::vector<XMMATRIX> skinningData;

		//Set shader for skeletal animation
		ShaderItem* shader = shaderSystem.FindShader(L"Animation.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		Animation& anim = skeleton->GetCurrentAnimation();
		if (!anim.frames.empty())
		{
			mesh->currentAnimationTime += Core::GetDeltaTime();

			//Move through and animate all joints on skeleton
			for (Joint& joint : skeleton->joints)
			{
				if (mesh->currentAnimationTime >= anim.GetEndTime(joint.index))
				{
					mesh->currentAnimationTime = 0.f;
				}

				anim.Interpolate(mesh->currentAnimationTime, joint, skeleton);

				skinningData.push_back(joint.currentPose);
			}
		}

		//Update skinning constant buffers
		if (!skinningData.empty())
		{
			MapBuffer(cbSkinningData, skinningData.data(), sizeof(XMMATRIX) * skinningData.size());
			context->VSSetConstantBuffers(cbSkinningRegister, 1, &cbSkinningData);
		}
	}

	PROFILE_END
}

void Renderer::RenderParticleEmitters()
{
	PROFILE_START

	//Only need to build sprite quad once for in-world rendering
	spriteSystem.BuildSpriteQuadForParticleRendering();

	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.proj = activeCamera->GetProjectionMatrix();
	shaderMatrices.texMatrix = XMMatrixIdentity();

	for (auto emitter : ParticleEmitter::system.components)
	{
		if (drawAllAsWireframe)
		{
			context->RSSetState(rastStateWireframe);
		}
		else
		{
			context->RSSetState(rastStateMap["nobackcull"]->data);
		}

		ShaderItem* shader = shaderSystem.FindShader(L"Unlit.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		context->PSSetSamplers(0, 1, &RenderUtils::GetDefaultSampler()->data);

		//Set texture from emitter for every particle
		context->PSSetShaderResources(0, 1, &textureSystem.FindTexture2D(emitter->textureData.filename)->srv);

		spriteSystem.UpdateAndSetSpriteBuffers(context);

		for (auto& particle : emitter->particles)
		{
			//Add rotation to particle (keep in mind that rotate speed needs to match angle's +/- value)
			particle.angle += particle.rotateSpeed * Core::GetDeltaTime();

			VMath::RotateTowardsCamera(particle.transform, particle.angle);

			shaderMatrices.model = particle.transform.GetAffine();
			shaderMatrices.MakeModelViewProjectionMatrix();

			MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
			context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

			//Draw
			//@Todo: do instancing here for particles. (was reading on instancing for particles, 
			//apparently using DrawInstanced() degrades performance when the vertex count it really low
			//and DrawIndexed is actually faster.
			context->DrawIndexed(6, 0, 0);
		}
	}

	PROFILE_END
}

void Renderer::RenderSpritesInScreenSpace()
{
	PROFILE_START

	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.proj = activeCamera->GetProjectionMatrix();

	for (const Sprite& sprite : spriteSystem.screenSprites)
	{
		if (drawAllAsWireframe)
		{
			context->RSSetState(rastStateWireframe);
		}
		else
		{
			//Careful with back culling visibility here for UI (as every sprite's a quad)
			context->RSSetState(rastStateMap["solid"]->data);
		}

		ShaderItem* shader = shaderSystem.FindShader(L"ui.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		context->PSSetSamplers(0, 1, &RenderUtils::GetDefaultSampler()->data);

		//Set texture from sprite
		context->PSSetShaderResources(0, 1, &textureSystem.FindTexture2D(sprite.textureFilename)->srv);

		spriteSystem.BuildSpriteQuadForViewportRendering(sprite);

		//Update vertex buffer
		spriteSystem.UpdateAndSetSpriteBuffers(context);

		shaderMatrices.model = XMMatrixIdentity();
		shaderMatrices.MakeModelViewProjectionMatrix();

		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		//Draw
		context->DrawIndexed(6, 0, 0);
	}

	PROFILE_END
}

//Loops over every light component and moves their data into the lights constant buffer
void UpdateLights()
{
	PROFILE_START

	int shaderLightsIndex = 0;

	//Directional lights
	for (auto light : DirectionalLightComponent::system.components)
	{
		Light lightData = light->lightData;
		XMFLOAT3 forwardVector = light->GetForwardVector();
		lightData.direction = XMFLOAT4(forwardVector.x, forwardVector.y, forwardVector.z, 0.f);

		shaderLights.lights[shaderLightsIndex] = lightData;
		shaderLightsIndex++;
	}

	//Point lights
	for (auto light : PointLightComponent::system.components)
	{
		light->lightData.position = XMFLOAT4(light->transform.position.x,
			light->transform.position.y,
			light->transform.position.z, 1.0f);

		shaderLights.lights[shaderLightsIndex] = light->lightData;
		shaderLightsIndex++;
	}
	
	//Spot lights
	for (auto light : SpotLightComponent::system.components)
	{
		light->lightData.position = XMFLOAT4(light->transform.position.x,
			light->transform.position.y,
			light->transform.position.z, 1.0f);

		XMFLOAT3 forwardVector = light->GetForwardVector();
		light->lightData.direction = XMFLOAT4(forwardVector.x, forwardVector.y, forwardVector.z, 0.f);

		shaderLights.lights[shaderLightsIndex] = light->lightData;
		shaderLightsIndex++;
	}

	shaderLights.numLights = shaderLightsIndex;

	shaderLights.globalAmbient = CalcGlobalAmbientBasedOnGameTime();
	XMStoreFloat4(&shaderLights.eyePosition, activeCamera->transform.world.r[3]);

	MapBuffer(cbLights, &shaderLights, sizeof(ShaderLights));
	context->PSSetConstantBuffers(cbLightsRegister, 1, &cbLights);

	PROFILE_END
}

//Not many good references on D3D11 Querying 
//REF:https://www.reedbeta.com/blog/gpu-profiling-101/
void StartGPUQueries()
{
	if (debugMenu.fpsMenuOpen)
	{
		context->Begin(timeDisjointQuery[frameQueryIndex]);
		context->End(frameStartQuery[frameQueryIndex]);

		GetGPUQueryData();
	}
}

void EndGPUQueries()
{
	static bool firstRun = true;

	if (debugMenu.fpsMenuOpen)
	{
		if (firstRun)
		{
			firstRun = false;
			return;
		}

		context->End(frameEndQuery[frameQueryIndex]);
		context->End(timeDisjointQuery[frameQueryIndex]);

		++frameQueryIndex &= 1;
	}
}

//Called after Present()
void GetGPUQueryData()
{
	PROFILE_START

	if (debugMenu.fpsMenuOpen)
	{
		//No frame data has been collected yet
		if (framesCollected < 0)
		{
			framesCollected = 0;
			return;
		}

		while (context->GetData(timeDisjointQuery[framesCollected], nullptr, 0, 0) == S_FALSE)
		{
			Sleep(1);
		}

		int frameIndex = framesCollected;
		++framesCollected &= 1;

		//Check whether timestamps were disjoint during the last frame
		D3D11_QUERY_DATA_TIMESTAMP_DISJOINT timeStampDisjoint;
		context->GetData(timeDisjointQuery[frameIndex], &timeStampDisjoint, sizeof(timeStampDisjoint), 0);
		if (timeStampDisjoint.Disjoint)
		{
			return;
		}

		//Get all the timestamps
		UINT64 timeStampStartFrame = 0;
		while (context->GetData(frameStartQuery[frameIndex], &timeStampStartFrame, sizeof(UINT64), 0) != S_OK)
		{
		}

		UINT64 timeStampEndFrame = 0;
		while (context->GetData(frameEndQuery[frameIndex], &timeStampEndFrame, sizeof(UINT64), 0) != S_OK)
		{
		}

		Renderer::frameTime = (float)(timeStampEndFrame - timeStampStartFrame) / (float)timeStampDisjoint.Frequency * 1000.f;
	}

	PROFILE_END
}

void Renderer::Present()
{
	HR(swapchain->Present(1, 0));

	EndGPUQueries();
}

void SetLightResources()
{
	//Set lights buffer
	context->PSSetConstantBuffers(cbLightsRegister, 1, &cbLights);
}

void* Renderer::GetSwapchain()
{
	return swapchain;
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

	dsv->Release();

	uiSystem.Cleanup();

	backBuffer->Release();
	HR(swapchain->ResizeBuffers(swapchainCount, newWidth, newHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	viewport.Width = newWidth;
	viewport.Height = newHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);

	CreateRTVAndDSV();

	CreatePostProcessRenderTarget();

	CreatePlanarReflectionBuffers();

	uiSystem.Init((void*)swapchain);

	shaderMatrices.Create();
}

void Renderer::ScreenshotCapture()
{
	if (Input::GetKeyUp(Keys::F8))
	{
		//Clear previous notification so nothing appears in the screenshot.
		debugMenu.debugNotifications.clear();

		ID3D11Texture2D* backBuffer = nullptr;
		HR(swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
		assert(backBuffer);

		//Use a generated UID so that filenames are unique
		UID imageFileID = GenerateUID();
		std::wstring imageFile = L"Screenshots/" + std::to_wstring(imageFileID) + L".jpg";
		HR(SaveWICTextureToFile(context, backBuffer, GUID_ContainerFormatJpeg, imageFile.c_str()));

		debugMenu.AddNotification(L"Screen shot taken.");
	}
}

void SetRenderPipelineStates(MeshComponent* mesh)
{
	Material* material = mesh->material;
	PipelineStateObject* pso = mesh->pso;

	if (Renderer::drawAllAsWireframe)
	{
		context->RSSetState(rastStateWireframe);
	}
	else if (material->rastState)
	{
		context->RSSetState(material->rastState->data);
	}

	const FLOAT blendState[4] = { 0.f };
	context->OMSetBlendState(material->blendState->data, blendState, 0xFFFFFFFF);

	context->VSSetShader(material->shader->vertexShader, nullptr, 0);
	context->PSSetShader(material->shader->pixelShader, nullptr, 0);

	context->PSSetSamplers(0, 1, &material->sampler->data);
	context->PSSetShaderResources(0, 1, &material->texture->srv);

	context->IASetVertexBuffers(0, 1, &pso->vertexBuffer->data, &Renderer::stride, &Renderer::offset);
	context->IASetIndexBuffer(pso->indexBuffer->data, DXGI_FORMAT_R32_UINT, 0);

	MapBuffer(cbMaterial, &material->materialShaderData, sizeof(MaterialShaderData));
	//context->VSSetConstantBuffers(cbMaterialRegister, 0, &cbMaterial);
	context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);
}

void SetRenderPipelineStatesForShadows(MeshComponent* mesh)
{
	Material* material = mesh->material;
	PipelineStateObject* pso = mesh->pso;

	context->RSSetState(rastStateMap["shadow"]->data);

	ShaderItem* shader = shaderSystem.FindShader(L"Shadows.hlsl");

	context->VSSetShader(shader->vertexShader, nullptr, 0);
	context->PSSetShader(shader->pixelShader, nullptr, 0);

	context->IASetVertexBuffers(0, 1, &pso->vertexBuffer->data, &Renderer::stride, &Renderer::offset);
	context->IASetIndexBuffer(pso->indexBuffer->data, DXGI_FORMAT_R32_UINT, 0);
}

void SetShader(std::wstring shaderName)
{
	ShaderItem* shader = shaderSystem.FindShader(shaderName);
	context->VSSetShader(shader->vertexShader, nullptr, 0);
	context->PSSetShader(shader->pixelShader, nullptr, 0);
}

void SetRastState(std::string rastStateName)
{
	auto rastState = rastStateMap[rastStateName];
	context->RSSetState(rastState->data);
}

void SetConstantBufferVertexPixel(uint32_t shaderRegister, ID3D11Buffer* constantBuffer)
{
	context->VSSetConstantBuffers(shaderRegister, 1, &constantBuffer);
	context->PSSetConstantBuffers(shaderRegister, 1, &constantBuffer);
}

void SetConstantBufferVertex(uint32_t shaderRegister, ID3D11Buffer* constantBuffer)
{
	context->VSSetConstantBuffers(shaderRegister, 1, &constantBuffer);
}

void SetConstantBufferPixel(uint32_t shaderRegister, ID3D11Buffer* constantBuffer)
{
	context->PSSetConstantBuffers(shaderRegister, 1, &constantBuffer);
}

void SetVertexBuffer(Buffer* vertexBuffer)
{
	context->IASetVertexBuffers(0, 1, &vertexBuffer->data, &Renderer::stride, &Renderer::offset);
}

void SetIndexBuffer(Buffer* indexBuffer)
{
	context->IASetIndexBuffer(indexBuffer->data, indexBufferFormat, 0);
}

XMFLOAT4 CalcGlobalAmbientBasedOnGameTime()
{
	const int hour = GameInstance::currentHour;
	const int hoursPerDay = 12; //This is relative to how many game hours there are from start to end.

	//@Todo: after a certain point in the game, light will dim to show night.
	//Figure out what time the game starts (eg. 12pm, 8am) and change this if based on that.
	if (hour > 6)
	{
		float light = 1.0f - (hour / static_cast<float>(hoursPerDay));
		light *= 0.5f;
		return XMFLOAT4(light, light, light, 1.0f);
	}

	return XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f);
}

void RenderPostProcess()
{
	uint32_t numPostProcessInstances = PostProcessInstance::system.GetNumActors();
	if (numPostProcessInstances == 0) return;

	assert(numPostProcessInstances == 1);
	auto postProcessIntance = PostProcessInstance::system.actors[0];

	SetNullRTV();

	ID3D11ShaderResourceView* nullSRV = nullptr;
	ID3D11UnorderedAccessView* nullUAV = nullptr;

	context->RSSetState(rastStateMap[RastStates::solid]->data);

	context->IASetInputLayout(nullptr);
	context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	auto quadShader = shaderSystem.FindShader(L"PostProcess.hlsl");
	context->VSSetShader(quadShader->vertexShader, nullptr, 0);
	context->PSSetShader(quadShader->pixelShader, nullptr, 0);

	//Set constant buffer data
	MapBuffer(cbPostProcess, &postProcessIntance->postProcessData, sizeof(ShaderPostProcessData));
	context->PSSetConstantBuffers(0, 1, &cbPostProcess);

	context->PSSetShaderResources(0, 1, &postSRV);
	context->PSSetSamplers(0, 1, &RenderUtils::GetDefaultSampler()->data);

	UINT frameIndex = swapchain->GetCurrentBackBufferIndex();
	context->OMSetRenderTargets(1, &rtvs[frameIndex], dsv);

	const float clearColour[4] = { 0.f, 0.f, 0.f, 0.f };
	context->ClearRenderTargetView(rtvs[frameIndex], clearColour);

	//@Todo: there's an idea from the old post processing code that I liked.
	//Basically instead of drawing a stupid quad and rendering a texture onto it, copy to the backbuffer like below:
	//context->CopyResource(backBuffer, postBuffer);
	//There are typecasting errors with this (e.g. 16_FLOAT can't be cast to 8_UNORM) but it felt better.

	context->Draw(6, 0);

	context->PSSetShaderResources(0, 1, &nullSRV);
}

void CreatePostProcessRenderTarget()
{
	if (postBuffer) postBuffer->Release();
	if (postRTV) postRTV->Release();
	if (postSRV) postSRV->Release();

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.SampleDesc.Count = 1;
	desc.Width = viewport.Width;
	desc.Height = viewport.Height;

	HR(device->CreateTexture2D(&desc, nullptr, &postBuffer));
	assert(postBuffer);

	HR(device->CreateRenderTargetView(postBuffer, nullptr, &postRTV));
	HR(device->CreateShaderResourceView(postBuffer, nullptr, &postSRV));
}

RastState* Renderer::GetRastState(std::string rastStateName)
{
	return rastStateMap[rastStateName];
}

BlendState* Renderer::GetBlendState(std::string blendStateName)
{
	return blendStateMap[blendStateName];
}
