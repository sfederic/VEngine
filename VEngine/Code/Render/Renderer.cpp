#include "vpch.h"
#include "Renderer.h"
#include "Actors/DiffuseProbeMap.h"
#include "Actors/PostProcessVolume.h"
#include "BlendStates.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/DestructibleMeshComponent.h"
#include "Components/InstanceMeshComponent.h"
#include "Components/Lights/DirectionalLightComponent.h"
#include "Render/Texture2D.h"
#include "Components/Lights/PointLightComponent.h"
#include "Components/Lights/SpotLightComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SliceableMeshComponent.h"
#include "Components/SocketMeshComponent.h"
#include "ConstantBuffer.h"
#include "Core/Camera.h"
#include "Core/Core.h"
#include "Core/Debug.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Core/Profile.h"
#include "Core/UID.h"
#include "Core/VMath.h"
#include "Core/VString.h"
#include "Core/World.h"
#include "Core/WorldEditor.h"
#include "Editor/DebugMenu.h"
#include "Material.h"
#include "MeshData.h"
#include "Particle/ParticleEmitter.h"
#include "Particle/Polyboard.h"
#include "Particle/SpriteSheet.h"
#include "Physics/PhysicsSystem.h"
#include "Physics/Raycast.h"
#include "PipelineObjects.h"
#include "RastStates.h"
#include "Render/Line.h"
#include "Render/RenderTarget.h"
#include "Render/ShaderData/InstanceData.h"
#include "Render/ShaderData/ShaderLights.h"
#include "Render/ShaderData/ShaderMatrices.h"
#include "Render/ShaderData/ShaderMeshData.h"
#include "Render/ShaderData/ShaderPostProcessData.h"
#include "Render/ShaderData/ShaderSkinningData.h"
#include "Render/ShaderData/ShaderTimeData.h"
#include "Render/SpriteSystem.h"
#include "Render/VertexShader.h"
#include "RenderUtils.h"
#include "ShaderData/MaterialShaderData.h"
#include "ShaderItem.h"
#include "ShaderSystem.h"
#include "ShadowMap.h"
#include "TextureSystem.h"
#include "UI/UISystem.h"
#include "Vertex.h"
#include <combaseapi.h>
#include <cstdint>
#include <d3d11.h>
#include <d3dcommon.h>
#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <DirectXMathConvert.inl>
#include <DirectXMathMatrix.inl>
#include <DirectXMathVector.inl>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include <dxgicommon.h>
#include <dxgiformat.h>
#include <memory>
#include <minwindef.h>
#include <ScreenGrab.h>
#include <SHMath/DirectXSH.h>
#include <string>
#include <utility>
#include <vector>
#include <WinCodec.h> //For GUID_ContainerFormatJpeg
#include <windef.h>
#include <QProgressDialog>
#include <QApplication>

void CreateFactory();
void CreateDevice();
void CreateSwapchain(HWND window);
void CreateRTVAndDSV();
void CreateInputLayout();
void CreateRasterizerStates();
void CreateBlendStates();
void CreateConstantBuffers();
void CreateLightProbeBuffers();
void CheckSupportedFeatures();
void RenderShadowPass();
void RenderMeshComponents();
void RenderDestructibleMeshes();
void RenderMeshForShadowPass(MeshComponent* mesh);
void RenderInstanceMeshForShadowPass(InstanceMeshComponent& instanceMesh);
void AnimateAndRenderSkeletalMeshes();
void RenderSocketMeshComponents();
void RenderInstanceMeshComponents();
void RenderBounds();
void RenderPhysicsMeshes();
void RenderCharacterControllers();
void RenderCameraMeshes();
void RenderLightMeshes();
void RenderPolyboards();
void RenderSpriteSheets();
void RenderPostProcess();
void RenderWireframeForVertexPaintingAndPickedActor();
void RenderLightProbes();

void PointLightVertexColourMap();

void MapBuffer(ID3D11Resource* resource, const void* src, size_t size);
void DrawMesh(MeshComponent* mesh);
void DrawMeshInstanced(InstanceMeshComponent* mesh);
void DrawBoundingBox(MeshComponent* mesh, MeshComponent* boundsMesh);

void RenderDebugLines();

//Inner render functions to set shader resources
void SetNullRTV();
void SetGeneralShaderResourcesToNull();
void SetShadowData();
void SetLightResources();
void SetShadowResources();
void SetMatricesFromMesh(MeshComponent* mesh);
void SetShaderMeshData(MeshComponent* mesh);
void SetRenderPipelineStates(MeshComponent* mesh);
void SetRenderPipelineStatesForShadows(MeshComponent* mesh);
void SetShaders(ShaderItem* shaderItem);
void SetRastStateByName(std::string rastStateName);
void SetRastState(RastState& rastState);
void SetBlendStateByName(std::string blendStateName);
void SetBlendState(BlendState& blendState);
void SetConstantBufferVertexPixel(uint32_t shaderRegister, ID3D11Buffer* constantBuffer);
void SetConstantBufferVertex(uint32_t shaderRegister, ID3D11Buffer* constantBuffer);
void SetConstantBufferPixel(uint32_t shaderRegister, ID3D11Buffer* constantBuffer);
void SetVertexBuffer(Buffer& vertexBuffer);
void SetIndexBuffer(Buffer* indexBuffer);
void SetSampler(uint32_t shaderRegister, Sampler* sampler);
void SetShaderResourcePixel(uint32_t shaderRegister, std::string textureName);
void SetShaderResourceFromMaterial(uint32_t shaderRegister, Material& material);
void SetLightsConstantBufferData();
void ClearBounds();

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
std::unordered_map<std::string, std::unique_ptr<RastState>> rastStateMap;
std::unordered_map<std::string, std::unique_ptr<BlendState>> blendStateMap;
ID3D11RasterizerState* rastStateSolid;
ID3D11RasterizerState* rastStateWireframe;
ID3D11RasterizerState* rastStateNoBackCull;
ID3D11RasterizerState* rastStateFrontCull;
ID3D11RasterizerState* rastStateShadow;

//Blendstates
ID3D11BlendState* nullBlendState = nullptr;
ID3D11BlendState* blendStateAlphaToCoverage = nullptr;

//DXGI
IDXGISwapChain3* swapchain;
IDXGIFactory6* dxgiFactory;

//Constant buffers and data
ConstantBuffer<ShaderMatrices>* cbMatrices;
ConstantBuffer<MaterialShaderData>* cbMaterial;
ConstantBuffer<ShaderLights>* cbLights;
ConstantBuffer<ShaderTimeData>* cbTime;
ConstantBuffer<ShaderMeshData>* cbMeshData;
ConstantBuffer<ShaderSkinningData>* cbSkinningData;
//ConstantBuffer<ShaderMeshLightMapData>* cbMeshLightMapData;
ConstantBuffer<ShaderPostProcessData>* cbPostProcess;

//Viewport
D3D11_VIEWPORT viewport;

//Shadow maps
ShadowMap* shadowMap;

Sampler* defaultSampler;

//Light probe buffers
ID3D11RenderTargetView* lightProbeRTVs[6]; //Cubemap
ID3D11ShaderResourceView* lightProbeSRV = nullptr;
ID3D11Texture2D* lightProbeTexture = nullptr;

//Post process
RenderTarget postProcessRenderTarget(DXGI_FORMAT_R16G16B16A16_FLOAT);

//Quality = 0 and Count = 1 are the 'default'
DXGI_SAMPLE_DESC sampleDesc;

const int shadowMapTextureResgiter = 1;
const int reflectionTextureResgiter = 2;
const int instanceSRVRegister = 3;
const int environmentMapTextureRegister = 4;
const int normalMapTexureRegister = 5;
const int lightProbeInstanceDataRegister = 6;

const int lightProbeTextureWidth = 64;
const int lightProbeTextureHeight = 64;

ShaderMatrices shaderMatrices;
ShaderLights shaderLights;

struct DebugBoxData
{
	DirectX::BoundingOrientedBox bb;
	float timer = 0.f;
	bool clearWithTimer = false;
};

//Debug object containers
std::vector<DebugBoxData> debugOrientedBoxesOnTimerToRender;
std::vector<Vertex> debugLines;
ID3D11Buffer* debugLinesBuffer;
static const uint64_t debugLinesBufferSize = 32 * 32 * sizeof(Vertex);

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

	ShaderSystem::Init();

	shadowMap = new ShadowMap(device, 2048, 2048);

	CheckSupportedFeatures();

	CreateSwapchain((HWND)window);
	CreateRTVAndDSV();
	CreateInputLayout();
	CreateRasterizerStates();
	CreateBlendStates();
	CreateConstantBuffers();

	postProcessRenderTarget.Create(viewport.Width, viewport.Height);

	defaultSampler = RenderUtils::CreateSampler();

	SpriteSystem::Init();

	//debugLines.emplace_back(Vertex()); //dummy data so DirectX doesn't crash
	////@Todo: this crashes the program a lot (most dynamic buffers do) and not sure why.
	//debugLinesBuffer = RenderUtils::CreateDynamicBuffer(debugLinesBufferSize, D3D11_BIND_VERTEX_BUFFER, debugLines.data());
	//debugLines.clear();
}

void Renderer::Tick()
{
	//BOUNDING BOXES HOTKEY
	if (Input::GetKeyHeld(Keys::Ctrl))
	{
		if (Input::GetKeyDown(Keys::B))
		{
			drawBoundingBoxes = !drawBoundingBoxes;
		}
	}

	//RENDER TRIGGERS HOTKEY
	if (Input::GetKeyHeld(Keys::Ctrl))
	{
		if (Input::GetKeyDown(Keys::Y))
		{
			drawTriggers = !drawTriggers;
		}
	}

	//DRAW ALL AS WIREFRAME HOTKEY
	if (Input::GetKeyUp(Keys::F3))
	{
		drawAllAsWireframe = !drawAllAsWireframe;
	}

	if (Input::GetKeyUp(Keys::F4))
	{
		PointLightVertexColourMap();
		Log("vertex colour map done");
	}

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
	//For now, keep adapter CreateDevice() input as nullptr. Change on release.
	//IDXGIAdapter1* adapter = nullptr;
	//HR(dxgiFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));

	HR(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
		featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &device,
		&selectedFeatureLevel, &context));
}

void CreateSwapchain(HWND window)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc = { (UINT)viewport.Width, (UINT)viewport.Height, {60, 1}, DXGI_FORMAT_R8G8B8A8_UNORM };
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
	//@Todo: Because the game worlds are so simple for the time being, the vertex structure being fat with the animation
	//variables is fine, it's only a few more bytes. Later on if you want to utilise D3D reflection and pretty input layouts
	//up, can use https://takinginitiative.net/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
	//as a starting point.

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"COLOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, colour), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, pos), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, tangent), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, offsetof(Vertex, boneIndices), D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"WEIGHTS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, weights), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	VertexShader* shader = ShaderSystem::FindVertexShader(L"Default_vs.cso");

	HR(device->CreateInputLayout(inputDesc, _countof(inputDesc), shader->GetByteCodeData(), shader->GetByteCodeSize(), &inputLayout));
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

		rastStateMap.emplace(RastStates::solid, std::make_unique<RastState>(RastStates::solid, rastStateSolid));
	}

	//WIREFRAME
	{
		rastDesc.FillMode = D3D11_FILL_WIREFRAME;
		rastDesc.CullMode = D3D11_CULL_NONE;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateWireframe));

		rastStateMap.emplace(RastStates::wireframe, std::make_unique<RastState>(RastStates::wireframe, rastStateWireframe));
	}

	//SOLID, NO BACK CULL
	{
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateNoBackCull));

		rastStateMap.emplace(RastStates::noBackCull, std::make_unique<RastState>(RastStates::noBackCull, rastStateNoBackCull));
	}

	//FRONT CULL
	{
		rastDesc.CullMode = D3D11_CULL_FRONT;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateFrontCull));

		rastStateMap.emplace(RastStates::frontCull, std::make_unique<RastState>(RastStates::frontCull, rastStateFrontCull));
	}

	//SHADOWS
	{
		rastDesc.CullMode = D3D11_CULL_BACK;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.DepthBias = 25000;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.SlopeScaledDepthBias = 1.0f;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateShadow));

		rastStateMap.emplace(RastStates::shadow, std::make_unique<RastState>(RastStates::shadow, rastStateShadow));
	}
}

void CreateBlendStates()
{
	//NULL BLEND STATE
	{
		D3D11_BLEND_DESC nullBlendDesc = {};
		nullBlendDesc.RenderTarget[0].BlendEnable = false;
		HR(device->CreateBlendState(&nullBlendDesc, &blendStateAlphaToCoverage));

		blendStateMap.emplace(BlendStates::null, std::make_unique<BlendState>(BlendStates::null, nullBlendState));
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

		blendStateMap.emplace(BlendStates::Default, std::make_unique<BlendState>(BlendStates::Default, blendStateAlphaToCoverage));
	}
}

void CreateConstantBuffers()
{
	//Registers
	const int cbMatrixRegister = 0;
	const int cbMaterialRegister = 1;
	const int cbSkinningRegister = 2;
	const int cbLightsRegister = 3;
	const int cbTimeRegister = 4;
	const int cbMeshDataRegister = 5;

	//Shader matrix constant buffer
	shaderMatrices.Create();

	cbMatrices = new ConstantBuffer<ShaderMatrices>(&shaderMatrices, cbMatrixRegister);
	assert(cbMatrices);

	//Material buffer
	MaterialShaderData materialShaderData = {};
	cbMaterial = new ConstantBuffer<MaterialShaderData>(&materialShaderData, cbMaterialRegister);
	assert(cbMaterial);

	//Lights buffer
	ShaderLights shaderLights = {};
	cbLights = new ConstantBuffer<ShaderLights>(&shaderLights, cbLightsRegister);
	assert(cbLights);

	//Time buffer
	ShaderTimeData timeData = {};
	cbTime = new ConstantBuffer<ShaderTimeData>(&timeData, cbTimeRegister);
	assert(cbTime);

	//Mesh data buffer
	ShaderMeshData meshData = {};
	cbMeshData = new ConstantBuffer<ShaderMeshData>(&meshData, cbMeshDataRegister);
	assert(cbMeshData);

	//Skinning data
	ShaderSkinningData skinningData = {};
	cbSkinningData = new ConstantBuffer<ShaderSkinningData>(&skinningData, cbSkinningRegister);
	assert(cbSkinningData);

	//Post process data
	ShaderPostProcessData postProcessData = {};
	cbPostProcess = new ConstantBuffer<ShaderPostProcessData>(&postProcessData, 0);
	assert(cbPostProcess);
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

void SetGeneralShaderResourcesToNull()
{
	//Set to null to remove warnings
	ID3D11ShaderResourceView* nullSRV = nullptr;
	context->PSSetShaderResources(shadowMapTextureResgiter, 1, &nullSRV);
	context->PSSetShaderResources(reflectionTextureResgiter, 1, &nullSRV);
}

void SetShadowData()
{
	if (DirectionalLightComponent::system.GetNumComponents() > 0)
	{
		auto dLight = DirectionalLightComponent::system.GetFirstComponent();
		shaderMatrices.lightMVP = shadowMap->DirectionalLightViewProjectionTextureMatrix(dLight);
		shaderMatrices.lightViewProj =
			shadowMap->GetLightViewMatrix(dLight) * shadowMap->GetDirectionalLightOrthoMatrix(dLight);

		shaderLights.shadowsEnabled = true;
	}
	else if (SpotLightComponent::system.GetNumComponents() > 0)
	{
		auto spotLight = SpotLightComponent::system.GetFirstComponent();
		shaderMatrices.lightMVP = shadowMap->SpotLightViewProjectionTextureMatrix(spotLight);
		shaderMatrices.lightViewProj =
			shadowMap->GetLightViewMatrix(spotLight) * shadowMap->GetSpotLightPerspectiveMatrix(spotLight);

		shaderLights.shadowsEnabled = true;
	}
	else
	{
		shaderLights.shadowsEnabled = false;
	}
}

void SetLightResources()
{
	cbLights->SetVSAndPS();
}

void DrawMesh(MeshComponent* mesh)
{
	context->Draw(mesh->meshDataProxy.vertices.size(), 0);
}

void DrawMeshInstanced(InstanceMeshComponent* mesh)
{
	context->DrawInstanced(mesh->meshDataProxy.vertices.size(), mesh->GetInstanceCount(), 0, 0);
}

void DrawBoundingBox(MeshComponent* mesh, MeshComponent* boundsMesh)
{
	const DirectX::BoundingOrientedBox boundingBox = mesh->GetBoundsInWorldSpace();

	const XMVECTOR center = XMLoadFloat3(&boundingBox.Center);
	XMVECTOR scale = XMLoadFloat3(&boundingBox.Extents);
	scale *= 2.f; //Double extents for rendering
	const XMVECTOR orientation = XMLoadFloat4(&boundingBox.Orientation);

	const XMMATRIX boundsMatrix = XMMatrixAffineTransformation(scale,
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		orientation,
		center);

	shaderMatrices.model = boundsMatrix;

	//Set bounding box scale just slightly more than the component to avoid mesh overlap
	shaderMatrices.model.r[0].m128_f32[0] += 0.01f;
	shaderMatrices.model.r[1].m128_f32[1] += 0.01f;
	shaderMatrices.model.r[2].m128_f32[2] += 0.01f;

	shaderMatrices.MakeModelViewProjectionMatrix();
	cbMatrices->Map(&shaderMatrices);
	cbMatrices->SetVS();

	DrawMesh(boundsMesh);
}

//@Todo: can change the input assembly to only take in Line structs to the shader stage. Right now it's taking
//in entire verticies and might be too heavy when visualsing entire lightmap raycast networks.
void RenderDebugLines()
{
	MaterialShaderData materialShaderData{};
	materialShaderData.ambient = XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f);
	cbMaterial->Map(&materialShaderData);
	cbMaterial->SetPS();
	SetShaders(ShaderItems::SolidColour);

	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HR(context->Map(debugLinesBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, debugLines.data(), debugLinesBufferSize);
	context->Unmap(debugLinesBuffer, 0);

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	context->IASetVertexBuffers(0, 1, &debugLinesBuffer, &Renderer::stride, &Renderer::offset);

	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();
	cbMatrices->Map(&shaderMatrices);
	cbMatrices->SetVS();

	context->Draw(debugLines.size(), 0);
}

void CheckSupportedFeatures()
{
	//Threading check
	D3D11_FEATURE_DATA_THREADING threadFeature = {};
	HR(device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadFeature, sizeof(threadFeature)));

	//Swapchain's SwapEffect needs to be DXGI_SWAP_EFFECT_DISCARD for MSAA to work.
	const int sampleCount = 1;

	//MSAA check and set
	UINT msaaQualityLevel;
	HR(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &msaaQualityLevel));
	assert(msaaQualityLevel > 0);

	//Quality has to be one less than what CheckMultisampleQualityLevels() spits out for some reason
	sampleDesc.Quality = msaaQualityLevel - 1;
	sampleDesc.Count = sampleCount;
}

void RenderMeshForShadowPass(MeshComponent* mesh)
{
	if (!mesh->castsShadow || !mesh->IsVisible())
	{
		return;
	}

	Material& mat = mesh->GetMaterial();

	SetRenderPipelineStatesForShadows(mesh);

	//Set matrices
	shaderMatrices.model = mesh->GetWorldMatrix();
	shaderMatrices.MakeModelViewProjectionMatrix();
	shaderMatrices.MakeTextureMatrix(mat);

	cbMatrices->Map(&shaderMatrices);
	cbMatrices->SetVS();

	//Set textures
	context->PSSetSamplers(0, 1, &mat.sampler->data);
	SetShaderResourceFromMaterial(0, mat);

	//Draw
	context->Draw(mesh->meshDataProxy.vertices.size(), 0);
}

void RenderInstanceMeshForShadowPass(InstanceMeshComponent& instanceMesh)
{
	if (!instanceMesh.castsShadow) {
		return;
	}

	Material& mat = instanceMesh.GetMaterial();
	SetRenderPipelineStatesForShadows(&instanceMesh);

	const auto vertexCount = instanceMesh.meshDataProxy.vertices.size();

	for (const auto& instanceData : instanceMesh.GetInstanceData())
	{
		//Set matrices
		shaderMatrices.model = instanceData.world;
		shaderMatrices.MakeModelViewProjectionMatrix();
		shaderMatrices.MakeTextureMatrix(mat);

		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		//Set textures
		context->PSSetSamplers(0, 1, &mat.sampler->data);
		SetShaderResourceFromMaterial(0, mat);

		//Draw
		context->Draw(vertexCount, 0);
	}
}

void RenderShadowPass()
{
	Profile::Start();

	if (!shaderLights.shadowsEnabled)
	{
		return;
	}

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(inputLayout);

	shadowMap->BindDsvAndSetNullRenderTarget(context);

	for (auto& mesh : MeshComponent::system.GetComponents())
	{
		RenderMeshForShadowPass(mesh.get());
	}

	//@Todo: For VagrantTactics, don't need shadows on InstanceMeshes as only the Grid is using them and 
	//the levels are small enough.
	//for (auto& instanceMesh : InstanceMeshComponent::system.GetComponents())
	//{
	//	RenderInstanceMeshForShadowPass(*instanceMesh.get());
	//}

	for (auto& skeletalMesh : SkeletalMeshComponent::system.GetComponents())
	{
		SkeletalMeshComponent* mesh = skeletalMesh.get();

		if (!mesh->castsShadow || !mesh->IsVisible())
		{
			return;
		}

		context->RSSetState(rastStateMap["shadow"]->data);

		PipelineStateObject& pso = mesh->pso;
		context->IASetVertexBuffers(0, 1, &pso.GetVertexBuffer().data, &Renderer::stride, &Renderer::offset);

		ShaderItem* shader = ShaderItems::ShadowAnimation;

		context->VSSetShader(shader->GetVertexShader(), nullptr, 0);
		context->PSSetShader(shader->GetPixelShader(), nullptr, 0);

		//Set matrices
		shaderMatrices.model = mesh->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		shaderMatrices.MakeTextureMatrix(mesh->GetMaterial());

		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		//Set skinning data
		//The constant buffer set here is working off of the data inputted on animating skeletons
		ShaderSkinningData skinningData = skeletalMesh->shaderSkinningData;
		cbSkinningData->Map(&skinningData);
		cbSkinningData->SetVS();

		//Set textures
		Material& mat = mesh->GetMaterial();
		context->PSSetSamplers(0, 1, &mat.sampler->data);
		SetShaderResourceFromMaterial(0, mat);

		//Draw
		context->Draw(mesh->meshDataProxy.vertices.size(), 0);
	}

	SetNullRTV();

	Profile::End();
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

	constexpr float clearColour[4] = { 0.f, 0.f, 0.f, 1.f };
	UINT frameIndex = swapchain->GetCurrentBackBufferIndex();

	context->ClearRenderTargetView(&postProcessRenderTarget.GetRTV(), clearColour);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	context->OMSetRenderTargets(1, postProcessRenderTarget.GetRTVAddress(), dsv);

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
	Profile::Start();

	auto& activeCamera = Camera::GetActiveCamera();
	shaderMatrices.view = activeCamera.GetViewMatrix();
	shaderMatrices.proj = activeCamera.GetProjectionMatrix();

	//Set time constant buffer
	ShaderTimeData timeData = {};
	timeData.deltaTime = Core::GetDeltaTime();
	timeData.timeSinceStartup = Core::timeSinceStartup;

	cbTime->Map(&timeData);
	cbTime->SetVSAndPS();

	SetShadowData();
	RenderShadowPass();

	if (PostProcessVolume::system.GetNumActors() > 0
		&& PostProcessVolume::system.GetFirstActor()->IsActive()
		&& PostProcessVolume::system.GetFirstActor()->IsActiveCameraInsideVolume())
	{
		RenderPostProcessSetup();
	}
	else
	{
		RenderSetup();
	}

	SetLightsConstantBufferData();

	RenderMeshComponents();
	RenderWireframeForVertexPaintingAndPickedActor();
	RenderDestructibleMeshes();
	AnimateAndRenderSkeletalMeshes();
	RenderSocketMeshComponents();
	RenderInstanceMeshComponents();
	RenderLightProbes();
	RenderPolyboards();
	RenderSpriteSheets();
	RenderPhysicsMeshes();
	RenderBounds();
	RenderLightMeshes();
	RenderCameraMeshes();
	RenderCharacterControllers();
	//RenderDebugLines();
	RenderPostProcess();

	ClearBounds();

	Profile::End();
}

void SetMatricesFromMesh(MeshComponent* mesh)
{
	shaderMatrices.model = mesh->GetWorldMatrix();
	shaderMatrices.MakeModelViewProjectionMatrix();
	shaderMatrices.MakeTextureMatrix(mesh->GetMaterial());

	XMMATRIX A = mesh->GetWorldMatrix();
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR det = XMMatrixDeterminant(A);
	shaderMatrices.invTranModel = XMMatrixTranspose(XMMatrixInverse(&det, A));

	cbMatrices->Map(&shaderMatrices);
	cbMatrices->SetVS();
}

void SetShaderMeshData(MeshComponent* mesh)
{
	ShaderMeshData meshData = {};
	meshData.position = mesh->GetWorldPosition();

	//@Todo: light probe data should have its own constant buffer, for now in testing, it's part of ShaderMeshData
	//Set light probe resources
	if (!DiffuseProbeMap::system.GetActors().empty())
	{
		context->PSSetShaderResources(environmentMapTextureRegister, 1, &lightProbeSRV);

		LightProbeInstanceData probeData = DiffuseProbeMap::system.GetFirstActor()->FindClosestProbe(mesh->GetWorldPositionV());
		memcpy(meshData.SH, probeData.SH, sizeof(XMFLOAT4) * 9);

		meshData.isDiffuseProbeMapActive = DiffuseProbeMap::system.GetOnlyActor()->IsActive();
	}
	else
	{
		meshData.isDiffuseProbeMapActive = false;
	}

	cbMeshData->Map(&meshData);
	cbMeshData->SetVSAndPS();

	//Light map data
	//ShaderMeshLightMapData meshLightMapData;
	//meshLightMapData = mesh->lightMapData;
	//meshLightMapData.atlasSize.x = lightMap.GetWidth();
	//meshLightMapData.atlasSize.y = lightMap.GetHeight();
	//meshLightMapData.AssertValues();

	//cbMeshLightMapData->Map(&meshLightMapData);
	//cbMeshLightMapData->SetVS();
}

void RenderMeshComponents()
{
	Profile::Start();

	//Shader Resources
	SetShadowResources();
	SetLightResources();

	auto meshes = MeshComponent::SortMeshComponentsByDistance();
	for (auto mesh : meshes)
	{
		if (!mesh->IsVisible()) { continue; }

		SetRenderPipelineStates(mesh);

		//Constant buffer data
		SetMatricesFromMesh(mesh);
		SetShaderMeshData(mesh);

		DrawMesh(mesh);
	}

	for (auto& mesh : SliceableMeshComponent::system.GetComponents())
	{
		if (!mesh->IsVisible()) { continue; }

		SetRenderPipelineStates(mesh.get());

		//Constant buffer data
		SetMatricesFromMesh(mesh.get());
		SetShaderMeshData(mesh.get());

		DrawMesh(mesh.get());
	}

	SetGeneralShaderResourcesToNull();

	Profile::End();
}

void RenderDestructibleMeshes()
{
	Profile::Start();

	SetShadowResources();
	SetLightResources();

	for (auto& mesh : DestructibleMeshComponent::system.GetComponents())
	{
		for (auto meshCell : mesh->meshCells)
		{
			if (!meshCell->IsVisible()) { continue; }
			SetRenderPipelineStates(meshCell);
			SetMatricesFromMesh(meshCell);
			SetShaderMeshData(meshCell);
			DrawMesh(meshCell);
		}
	}

	SetGeneralShaderResourcesToNull();

	Profile::End();
}

void Renderer::RenderLightProbeViews()
{
	auto startTime = Profile::QuickStart();

	auto diffuseProbeMap = DiffuseProbeMap::system.GetFirstActor();
	if (diffuseProbeMap == nullptr)
	{
		Log("No diffuse probe map in level to render probes from.");
		return;
	}

	Log("Diffuse light probe map bake started...");

	const int previousWiewportWidth = viewport.Width;
	const int previousWiewportHeight = viewport.Height;
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

	XMVECTOR resultantUpVectors[6] =
	{
		XMVectorSet(0.f, 1.f, 0.f, 0.f),
		XMVectorSet(0.f, 1.f, 0.f, 0.f),
		XMVectorSet(1.f, 0.f, 0.f, 0.f),
		XMVectorSet(1.f, 0.f, 0.f, 0.f),
		XMVectorSet(0.f, 1.f, 0.f, 0.f),
		XMVectorSet(0.f, 1.f, 0.f, 0.f),
	};

	CreateLightProbeBuffers();

	int probeIndex = 0;

	for (auto& probeData : diffuseProbeMap->lightProbeData)
	{
		const XMMATRIX probeMatrix = probeData.modelMatrix;

		for (int i = 0; i < 6; i++)
		{
			context->RSSetViewports(1, &viewport);
			constexpr float clearColour[4] = { 1.f, 1.f, 1.f, 0.f };
			context->ClearRenderTargetView(lightProbeRTVs[i], clearColour);
			context->IASetInputLayout(inputLayout);
			context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			context->OMSetRenderTargets(1, &lightProbeRTVs[i], nullptr);

			context->RSSetState(rastStateMap[RastStates::solid]->data);

			SetLightsConstantBufferData();

			//Set lights buffer
			cbLights->SetPS();

			//Set shadow resources (not now for lightprobes)
			//context->PSSetShaderResources(shadowMapTextureResgiter, 1, &shadowMap->depthMapSRV);
			//context->PSSetSamplers(1, 1, &shadowMap->sampler);

			ShaderItem* lightProbeShader = ShaderItems::Default;

			for (auto& mesh : MeshComponent::system.GetComponents())
			{
				if (!mesh->IsVisible()) { continue; }

				Material& material = mesh->GetMaterial();

				const FLOAT blendState[4] = { 0.f };
				context->OMSetBlendState(mesh->GetBlendState().data, blendState, 0xFFFFFFFF);
				context->RSSetState(mesh->GetRastState().data);

				context->VSSetShader(lightProbeShader->GetVertexShader(), nullptr, 0);
				context->PSSetShader(lightProbeShader->GetPixelShader(), nullptr, 0);

				context->PSSetSamplers(0, 1, &material.sampler->data);

				SetShaderResourceFromMaterial(0, material);

				context->IASetVertexBuffers(0, 1, &mesh->pso.GetVertexBuffer().data, &stride, &offset);

				cbMaterial->Map(&material.materialShaderData);
				cbMaterial->SetPS();

				//Set matrices
				const auto probePos = XMLoadFloat3(&probeData.position);
				shaderMatrices.view = XMMatrixLookAtLH(probePos,
					probePos + faces[i],
					resultantUpVectors[i]);
				shaderMatrices.model = mesh->GetWorldMatrix();
				shaderMatrices.MakeModelViewProjectionMatrix();
				shaderMatrices.MakeTextureMatrix(mesh->GetMaterial());

				cbMatrices->Map(&shaderMatrices);
				cbMatrices->SetVS();

				//Set mesh data to shader
				ShaderMeshData meshData;
				XMStoreFloat3(&meshData.position, mesh->GetWorldPositionV());
				cbMeshData->Map(&meshData);
				cbMeshData->SetVSAndPS();

				//Draw
				context->Draw(mesh->meshDataProxy.vertices.size(), 0);
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

		probeData.index = probeIndex;
		memcpy(probeData.SH, coefs, sizeof(XMFLOAT4) * 9);
		XMStoreFloat3(&probeData.position, probeMatrix.r[3]);

		probeIndex++;
	}

	//Remap probe data to structured buffer
	MapBuffer(diffuseProbeMap->GetStructuredBuffer(), diffuseProbeMap->lightProbeData.data(),
		sizeof(LightProbeInstanceData) * diffuseProbeMap->lightProbeData.size());

	//@Todo: Store SH into vertex colour data for static meshes
	/*for (auto& mesh : MeshComponent::system.GetComponents())
	{
		if(!mesh->IsRenderStatic()) continue;

		auto probeData = diffuseProbeMap->FindClosestProbe(mesh->GetWorldPositionV());

		constexpr float PI = 3.14159265f;
		constexpr float SQRT_PI = 1.7724538509f;
		constexpr float SQRT_5 = 2.2360679775f;
		constexpr float SQRT_15 = 3.8729833462f;
		constexpr float SQRT_3 = 1.7320508076f;

		constexpr float AO = 0.075f;

		constexpr float Y[9] =
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

		const float t = acos(sqrt(1 - AO));

		const float a = sin(t);
		const float b = cos(t);

		const float A0 = sqrt(4 * PI) * (sqrt(PI) / 2) * a * a;
		const float A1 = sqrt(4 * PI / 3) * (sqrt(3 * PI) / 3) * (1 - b * b * b);
		const float A2 = sqrt(4 * PI / 5) * (sqrt(5 * PI) / 16) * a * a * (2 + 6 * b * b);

		for (auto& vertex : mesh->GetAllVertices())
		{
			const XMVECTOR normal = XMVector3Normalize(XMLoadFloat3(&vertex.normal));
			XMFLOAT3 n{};
			XMStoreFloat3(&n, normal);

			XMVECTOR irradiance =
				XMLoadFloat4(&probeData.SH[0]) * A0 * Y[0] +
				XMLoadFloat4(&probeData.SH[1]) * A1 * Y[1] * n.y +
				XMLoadFloat4(&probeData.SH[2]) * A1 * Y[2] * n.z +
				XMLoadFloat4(&probeData.SH[3]) * A1 * Y[3] * n.x +
				XMLoadFloat4(&probeData.SH[4]) * A2 * Y[4] * (n.y * n.x) +
				XMLoadFloat4(&probeData.SH[5]) * A2 * Y[5] * (n.y * n.z) +
				XMLoadFloat4(&probeData.SH[6]) * A2 * Y[6] * (3.0 * n.z * n.z - 1.0) +
				XMLoadFloat4(&probeData.SH[7]) * A2 * Y[7] * (n.z * n.x) +
				XMLoadFloat4(&probeData.SH[8]) * A2 * Y[8] * (n.x * n.x - n.y * n.y);

			irradiance = XMVectorMax(irradiance, XMVectorSet(0.f, 0.f, 0.f, 0.f));
			XMVECTOR vertexColour = XMLoadFloat4(&vertex.colour);
			vertexColour += irradiance / PI;
			XMStoreFloat4(&vertex.colour, vertexColour);
		}

		mesh->CreateNewVertexBuffer();
	}*/

	ResizeSwapchain(previousWiewportWidth, previousWiewportHeight);

	//Set main RTV and DSV back on
	RenderSetup();

	diffuseProbeMap->WriteProbeDataToFile();

	double endTime = Profile::QuickEnd(startTime);
	Log("Light probe bake took [%f]sec.", endTime);
}

void RenderInstanceMeshComponents()
{
	//@Todo: shadows for instancemeshes
	//@Todo: animated instance meshes

	Profile::Start();

	//Set matrices (Instance mesh model matrices placed in a structured buffer)
	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();

	cbMatrices->Map(&shaderMatrices);
	cbMatrices->SetVS();

	for (auto& instanceMesh : InstanceMeshComponent::system.GetComponents())
	{
		if (!instanceMesh->IsVisible()) continue;

		//@Todo: instance meshes won't render behind transparent objects unless RenderInstanceMeshes() is called
		//before RenderMeshComponents(). However doing so will render instance meshes seemingly without a 
		//blend state, even though one appears to be set. HOWEVER, certain instance meshes WILL render as if
		//with a transparent blend state because you can see them blended though various instance meshes.
		SetRenderPipelineStates(instanceMesh.get());

		//Update texture matrix
		shaderMatrices.MakeTextureMatrix(instanceMesh->GetMaterial());
		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		//Update instance data and set SRV
		MapBuffer(instanceMesh->structuredBuffer, instanceMesh->GetInstanceData().data(), sizeof(InstanceData) * instanceMesh->GetInstanceData().size());
		context->VSSetShaderResources(instanceSRVRegister, 1, &instanceMesh->srv);
		context->PSSetShaderResources(instanceSRVRegister, 1, &instanceMesh->srv);

		//Set lights buffer
		cbLights->SetPS();

		DrawMeshInstanced(instanceMesh.get());
	}

	Profile::End();
}

//Remember that when rendering bounds, the Extents needs to be doubled.
void RenderBounds()
{
	auto debugBox = MeshComponent::GetDebugMesh("DebugBox");
	MaterialShaderData materialShaderData;

	if (Renderer::drawBoundingBoxes)
	{
		SetRastStateByName(RastStates::wireframe);
		SetShaders(ShaderItems::SolidColour);

		SetVertexBuffer(debugBox->GetVertexBuffer());

		//Set debug wireframe material colour
		materialShaderData.ambient = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
		cbMaterial->Map(&materialShaderData);
		cbMaterial->SetPS();

		for (auto& mesh : MeshComponent::system.GetComponents())
		{
			DrawBoundingBox(mesh.get(), debugBox);
		}

		for (auto& skeletalMesh : SkeletalMeshComponent::system.GetComponents())
		{
			DrawBoundingBox(skeletalMesh.get(), debugBox);
		}

		//@Todo: drawing bounds for instance meshes is hard, but have a think about it.

		for (auto& boxData : debugOrientedBoxesOnTimerToRender)
		{
			const auto& box = boxData.bb;

			XMVECTOR center = XMLoadFloat3(&box.Center);
			XMVECTOR scale = XMLoadFloat3(&box.Extents);
			scale *= 2.f;
			XMVECTOR rotation = XMLoadFloat4(&box.Orientation);

			XMMATRIX boundsMatrix = XMMatrixAffineTransformation(scale,
				XMVectorSet(0.f, 0.f, 0.f, 1.f),
				rotation,
				center);

			shaderMatrices.model = boundsMatrix;

			shaderMatrices.MakeModelViewProjectionMatrix();
			cbMatrices->Map(&shaderMatrices);
			cbMatrices->SetVS();

			DrawMesh(debugBox);
		}
	}

	//DRAW TRIGGER BOUNDS
	if (Renderer::drawTriggers)
	{
		SetRastStateByName(RastStates::wireframe);
		SetShaders(ShaderItems::SolidColour);

		SetVertexBuffer(debugBox->GetVertexBuffer());

		for (auto& boxTrigger : BoxTriggerComponent::system.GetComponents())
		{
			if (!boxTrigger->IsVisible())
			{
				continue;
			}

			auto bounds = boxTrigger->GetBoundsInWorldSpace();
			const XMVECTOR center = XMLoadFloat3(&bounds.Center);
			XMVECTOR scale = XMLoadFloat3(&bounds.Extents);
			scale *= 2.f; //Double extents for rendering
			const XMVECTOR orientation = XMLoadFloat4(&bounds.Orientation);

			shaderMatrices.model = XMMatrixAffineTransformation(scale,
				XMVectorSet(0.f, 0.f, 0.f, 1.f),
				orientation,
				center);

			shaderMatrices.MakeModelViewProjectionMatrix();
			cbMatrices->Map(&shaderMatrices);
			cbMatrices->SetVS();

			//Set trigger wireframe material colour
			materialShaderData.ambient = boxTrigger->renderWireframeColour;
			cbMaterial->Map(&materialShaderData);
			cbMaterial->SetPS();

			DrawMesh(debugBox);
		}
	}
}

void RenderPhysicsMeshes()
{
	static bool showPhysicsMeshes;

	if (Input::GetKeyUp(Keys::F3))
	{
		showPhysicsMeshes = !showPhysicsMeshes;
	}

	if (!showPhysicsMeshes) return;

	auto& physicsMeshes = PhysicsSystem::GetAllPhysicsMeshes();
	for (const auto& [uid, mesh] : physicsMeshes)
	{
		context->RSSetState(rastStateMap[RastStates::solid]->data);

		auto shaderItem = ShaderSystem::FindShaderItem("SolidColour");
		context->VSSetShader(shaderItem->GetVertexShader(), nullptr, 0);
		context->PSSetShader(shaderItem->GetPixelShader(), nullptr, 0);

		SetVertexBuffer(mesh->GetVertexBuffer());

		//@Todo: feels like this GetComponentByUID() call could get stupidly expensive
		auto originalMesh = MeshComponent::system.GetComponentByUID(mesh->GetUID());
		assert(originalMesh);
		shaderMatrices.model = originalMesh->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		MaterialShaderData materialShaderData;
		materialShaderData.ambient = XMFLOAT4(0.1f, 0.9f, 0.1f, 1.f);
		cbMaterial->Map(&materialShaderData);
		cbMaterial->SetPS();

		DrawMesh(mesh.get());
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

void RenderCharacterControllers()
{
	if (Core::gameplayOn) return;

	auto debugCapsule = MeshComponent::GetDebugMesh("DebugCapsule");
	MaterialShaderData materialShaderData;

	SetRastStateByName(RastStates::wireframe);
	SetShaders(ShaderItems::SolidColour);
	SetVertexBuffer(debugCapsule->GetVertexBuffer());

	materialShaderData.ambient = XMFLOAT4(1.0f, 0.8f, 0.7f, 1.0f); //Pink
	cbMaterial->Map(&materialShaderData);
	cbMaterial->SetPS();

	for (auto& characterController : CharacterControllerComponent::system.GetComponents())
	{
		shaderMatrices.model = characterController->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		DrawMesh(debugCapsule);
	}
}

void RenderCameraMeshes()
{
	if (Core::gameplayOn) return;

	auto debugCamera = MeshComponent::GetDebugMesh("DebugCamera");
	MaterialShaderData materialShaderData;

	SetRastStateByName(RastStates::wireframe);
	SetShaders(ShaderItems::SolidColour);
	SetVertexBuffer(debugCamera->GetVertexBuffer());

	materialShaderData.ambient = XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f); //Make cameras red
	cbMaterial->Map(&materialShaderData);
	cbMaterial->SetPS();

	for (auto& camera : CameraComponent::system.GetComponents())
	{
		shaderMatrices.model = camera->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		DrawMesh(debugCamera);
	}
}

void RenderLightMeshes()
{
	if (Core::gameplayOn) return;

	auto debugSphere = MeshComponent::GetDebugMesh("DebugSphere");
	auto debugIcoSphere = MeshComponent::GetDebugMesh("DebugIcoSphere");
	auto debugCone = MeshComponent::GetDebugMesh("DebugCone");

	SetRastStateByName(RastStates::wireframe);
	SetShaders(ShaderItems::SolidColour);

	//Set debug sphere wireframe material colour
	MaterialShaderData materialShaderData = {};
	materialShaderData.ambient = XMFLOAT4(1.f, 1.f, 0.f, 1.0f);
	cbMaterial->Map(&materialShaderData);
	cbMaterial->SetPS();

	//DIRECTIONAL LIGHTS
	SetVertexBuffer(debugSphere->GetVertexBuffer());

	for (auto& directionalLight : DirectionalLightComponent::system.GetComponents())
	{
		shaderMatrices.model = directionalLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		DrawMesh(debugSphere);
	}

	//POINT LIGHTS
	SetVertexBuffer(debugIcoSphere->GetVertexBuffer());

	for (auto& pointLight : PointLightComponent::system.GetComponents())
	{
		shaderMatrices.model = pointLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		DrawMesh(debugIcoSphere);
	}

	//SPOT LIGHTS
	SetVertexBuffer(debugCone->GetVertexBuffer());

	for (auto& spotLight : SpotLightComponent::system.GetComponents())
	{
		shaderMatrices.model = spotLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		DrawMesh(debugCone);
	}
}

void RenderPolyboards()
{
	Profile::Start();

	shaderMatrices.texMatrix = XMMatrixIdentity();
	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();

	cbMatrices->Map(&shaderMatrices);
	cbMatrices->SetVS();

	SetBlendStateByName(BlendStates::Default);
	SetRastStateByName(RastStates::noBackCull);
	SetShaders(ShaderItems::DefaultClip);

	for (auto polyboard : World::GetAllComponentsOfType<Polyboard>())
	{
		if (!polyboard->IsActive() || !polyboard->IsVisible()) continue;

		polyboard->CalcVertices();

		context->PSSetSamplers(0, 1, &Renderer::GetDefaultSampler().data);

		SetShaderResourcePixel(0, polyboard->GetTextureFilename());

		//VERTEX MAP
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			HR(context->Map(polyboard->GetVertexBuffer().data, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
			memcpy(mappedResource.pData, polyboard->GetVertices().data(), sizeof(Vertex) * polyboard->GetVertices().size());
			context->Unmap(polyboard->GetVertexBuffer().data, 0);
		}

		//INDEX MAP
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			HR(context->Map(polyboard->GetIndexBuffer().data, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
			memcpy(mappedResource.pData, polyboard->GetIndices().data(), sizeof(MeshData::indexDataType) * polyboard->GetIndices().size());
			context->Unmap(polyboard->GetIndexBuffer().data, 0);
		}

		SetVertexBuffer(polyboard->GetVertexBuffer());
		SetIndexBuffer(&polyboard->GetIndexBuffer());

		context->DrawIndexed(polyboard->GetIndices().size(), 0, 0);
	}

	Profile::End();
}

void RenderSpriteSheets()
{
	Profile::Start();

	for (auto& spriteSheet : SpriteSheet::system.GetComponents())
	{
		if (!spriteSheet->IsActive() || !spriteSheet->IsVisible())
		{
			continue;
		}

		SetRastStateByName(RastStates::noBackCull);
		SetShaders(ShaderItems::DefaultClip);

		SetSampler(0, &Renderer::GetDefaultSampler());
		SetShaderResourcePixel(0, spriteSheet->GetTextureFilename());

		spriteSheet->UpdateSprite();

		SpriteSystem::BuildSpriteQuadForSpriteSheetRendering(spriteSheet->GetSprite());
		SpriteSystem::UpdateAndSetSpriteBuffers();

		const XMVECTOR lookAtRotation = VMath::LookAtRotation(Camera::GetActiveCamera().GetWorldPositionV(),
			spriteSheet->GetWorldPositionV());
		spriteSheet->SetWorldRotation(lookAtRotation);

		shaderMatrices.model = spriteSheet->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();

		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		context->DrawIndexed(6, 0, 0);
	}

	Profile::End();
}

//@Todo: think about splitting render and animate up
void AnimateAndRenderSkeletalMeshes()
{
	Profile::Start();

	SetShadowResources();
	SetLightResources();

	for (auto& skeletalMesh : SkeletalMeshComponent::system.GetComponents()) //@Todo: figure out transparency sort
	{
		if (!skeletalMesh->IsVisible()) { continue; }

		SetRenderPipelineStates(skeletalMesh.get());

		//Constant buffer data
		SetMatricesFromMesh(skeletalMesh.get());
		SetShaderMeshData(skeletalMesh.get());

		if (!skeletalMesh->GetCurrentAnimatonName().empty())
		{
			if (skeletalMesh->HasJoints())
			{
				//Set shader for skeletal animation
				ShaderItem* shaderItem = ShaderItems::Animation;
				context->VSSetShader(shaderItem->GetVertexShader(), nullptr, 0);
				context->PSSetShader(shaderItem->GetPixelShader(), nullptr, 0);

				if (!skeletalMesh->GetNextAnimationName().empty())
				{
					skeletalMesh->CrossFadeNextAnimation();
				}
				else
				{
					skeletalMesh->InterpolateCurrentAnimation();
				}

				//Update skinning constant buffers
				ShaderSkinningData& skinningData = skeletalMesh->shaderSkinningData;
				cbSkinningData->Map(&skinningData);
				cbSkinningData->SetVS();
			}
		}

		DrawMesh(skeletalMesh.get());
	}

	SetGeneralShaderResourcesToNull();

	Profile::End();
}

//Make sure this is called after SkeletalMesh animations else the skinning data won't be set 
//and the SocketMeshes will have nothing correct to index into.
void RenderSocketMeshComponents()
{
	//Map skinning data to socketmeshcomponents
	for (auto& socketMesh : SocketMeshComponent::system.GetComponents())
	{
		if (!socketMesh->IsVisible()) { continue; }

		SetRenderPipelineStates(socketMesh.get());

		socketMesh->SetTransformFromLinkedSkeletonJoint();

		SetMatricesFromMesh(socketMesh.get());
		SetShaderMeshData(socketMesh.get());

		DrawMesh(socketMesh.get());
	}

	SetGeneralShaderResourcesToNull();
}

void Renderer::RenderParticleEmitters()
{
	Profile::Start();

	//Only need to build sprite quad once for in-world rendering
	SpriteSystem::BuildSpriteQuadForParticleRendering();

	auto& activeCamera = Camera::GetActiveCamera();
	shaderMatrices.view = activeCamera.GetViewMatrix();
	shaderMatrices.proj = activeCamera.GetProjectionMatrix();
	shaderMatrices.texMatrix = XMMatrixIdentity();

	for (auto& emitter : ParticleEmitter::system.GetComponents())
	{
		if (!emitter->IsVisible() || !emitter->IsActive())
		{
			continue;
		}

		if (drawAllAsWireframe)
		{
			context->RSSetState(rastStateWireframe);
		}
		else
		{
			context->RSSetState(rastStateMap["nobackcull"]->data);
		}

		SetBlendStateByName(BlendStates::Default);

		SetShaders(emitter->GetMaterial().shader);

		context->PSSetSamplers(0, 1, &Renderer::GetDefaultSampler().data);

		MaterialShaderData materialShaderData;
		materialShaderData = emitter->GetMaterial().materialShaderData;
		cbMaterial->Map(&materialShaderData);
		cbMaterial->SetPS();

		//Set texture from emitter for every particle
		SetShaderResourcePixel(0, emitter->GetMaterial().textureData.filename);

		SpriteSystem::UpdateAndSetSpriteBuffers();

		for (auto& particle : emitter->particles)
		{
			//Add rotation to particle (keep in mind that rotate speed needs to match angle's +/- value)
			auto particleRotation =
				VMath::LookAtRotation(Camera::GetActiveCamera().GetWorldPositionV(), XMLoadFloat3(&particle.transform.position));
			XMStoreFloat4(&particle.transform.rotation, particleRotation);

			shaderMatrices.model = particle.transform.GetAffine();
			shaderMatrices.MakeModelViewProjectionMatrix();

			cbMatrices->Map(&shaderMatrices);
			cbMatrices->SetVS();

			//Note: Apparently using DrawInstanced() degrades performance when the vertex count it really low
			//and DrawIndexed is actually faster.
			context->DrawIndexed(6, 0, 0);
		}
	}

	Profile::End();
}

void Renderer::RenderSpritesInScreenSpace()
{
	Profile::Start();

	for (const auto& sprite : SpriteSystem::GetScreenSprites())
	{
		SetRastStateByName(RastStates::solid);
		SetShaders(ShaderItems::UI);
		SetSampler(0, &Renderer::GetDefaultSampler());
		SetShaderResourcePixel(0, sprite.textureFilename);

		MaterialShaderData defaultMaterialShaderData;
		cbMaterial->Map(&defaultMaterialShaderData);
		cbMaterial->SetPS();

		SpriteSystem::BuildSpriteQuadForViewportRendering(sprite);
		SpriteSystem::UpdateAndSetSpriteBuffers();

		shaderMatrices.model = XMMatrixIdentity();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices->Map(&shaderMatrices);
		cbMatrices->SetVS();

		context->DrawIndexed(6, 0, 0);
	}

	Profile::End();
}

void SetLightsConstantBufferData()
{
	Profile::Start();

	int shaderLightsIndex = 0;

	for (auto& light : DirectionalLightComponent::system.GetComponents())
	{
		if (!light->IsActive()) continue;

		shaderLights.lights[shaderLightsIndex] = light->GetLightData();
		shaderLightsIndex++;
	}

	for (auto& light : PointLightComponent::system.GetComponents())
	{
		if (!light->IsActive()) continue;

		shaderLights.lights[shaderLightsIndex] = light->GetLightData();
		shaderLightsIndex++;
	}

	for (auto& light : SpotLightComponent::system.GetComponents())
	{
		if (!light->IsActive()) continue;

		shaderLights.lights[shaderLightsIndex] = light->GetLightData();
		shaderLightsIndex++;
	}

	shaderLights.numLights = shaderLightsIndex;
	assert(shaderLights.numLights < ShaderLights::MAX_LIGHTS);

	XMStoreFloat4(&shaderLights.eyePosition, Camera::GetActiveCamera().GetWorldPositionV());

	cbLights->Map(&shaderLights);
	cbLights->SetVSAndPS();

	Profile::End();
}

void Renderer::Present()
{
	HR(swapchain->Present(1, 0));
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

	UISystem::Cleanup();

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

	postProcessRenderTarget.Recycle();
	postProcessRenderTarget.Create(viewport.Width, viewport.Height);

	UISystem::Init((void*)swapchain);

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

void Renderer::MeshIconImageCapture()
{
	Actor* actor = WorldEditor::GetPickedActor();
	std::vector<MeshComponent*> meshComponents = actor->GetComponents<MeshComponent>();

	//@Todo: isn't working with multiple meshes nicely
	if (!meshComponents.empty())
	{
		MeshComponent* mesh = meshComponents.front();

		ID3D11Texture2D* backBuffer = nullptr;
		HR(swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
		assert(backBuffer);

		const std::wstring imageFile = L"Icons/MeshIcons/" +
			VString::stows(mesh->meshComponentData.filename) + L".jpg";
		HR(SaveWICTextureToFile(context, backBuffer, GUID_ContainerFormatJpeg, imageFile.c_str()));
		debugMenu.AddNotification(L"Mesh Icon created.");
	}
}

void Renderer::MapIconImageCapture()
{
	ID3D11Texture2D* backBuffer = nullptr;
	HR(swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	assert(backBuffer);

	const std::wstring imageFile = L"Icons/MapIcons/" + VString::stows(World::worldFilename) + L".jpg";
	HR(SaveWICTextureToFile(context, backBuffer, GUID_ContainerFormatJpeg, imageFile.c_str()));
	debugMenu.AddNotification(L"Map Icon created.");
}

void Renderer::PlayerPhotoCapture(std::wstring outputFilename)
{
	debugMenu.debugNotifications.clear();

	ID3D11Texture2D* backBuffer = nullptr;
	HR(swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
	assert(backBuffer);

	std::wstring imageFile = L"Textures/" + outputFilename;
	HR(SaveWICTextureToFile(context, backBuffer, GUID_ContainerFormatJpeg, imageFile.c_str()));
	Log("Photo taken [%S]", imageFile.c_str());
}

void SetRenderPipelineStates(MeshComponent* mesh)
{
	Material& material = mesh->GetMaterial();
	PipelineStateObject& pso = mesh->pso;

	if (Renderer::drawAllAsWireframe)
	{
		context->RSSetState(rastStateWireframe);
	}
	else if (material.rastState)
	{
		context->RSSetState(material.rastState->data);
	}

	constexpr FLOAT blendState[4] = { 0.f };
	context->OMSetBlendState(material.blendState->data, blendState, 0xFFFFFFFF);

	context->VSSetShader(material.GetVertexShader(), nullptr, 0);
	context->PSSetShader(material.GetPixelShader(), nullptr, 0);

	context->PSSetSamplers(0, 1, &material.sampler->data);
	SetShaderResourceFromMaterial(0, material);

	SetVertexBuffer(pso.GetVertexBuffer());

	cbMaterial->Map(&material.materialShaderData);
	cbMaterial->SetPS();
}

void SetRenderPipelineStatesForShadows(MeshComponent* mesh)
{
	context->RSSetState(rastStateShadow);

	ShaderItem* shader = ShaderItems::Shadow;

	context->VSSetShader(shader->GetVertexShader(), nullptr, 0);
	context->PSSetShader(shader->GetPixelShader(), nullptr, 0);

	PipelineStateObject& pso = mesh->pso;
	context->IASetVertexBuffers(0, 1, &pso.GetVertexBuffer().data, &Renderer::stride, &Renderer::offset);
}

void SetShaders(ShaderItem* shaderItem)
{
	context->VSSetShader(shaderItem->GetVertexShader(), nullptr, 0);
	context->PSSetShader(shaderItem->GetPixelShader(), nullptr, 0);
}

void SetRastStateByName(std::string rastStateName)
{
	if (Renderer::drawAllAsWireframe)
	{
		context->RSSetState(rastStateWireframe);
		return;
	}

	auto& rastState = rastStateMap.find(rastStateName)->second;
	context->RSSetState(rastState->data);
}

void SetRastState(RastState& rastState)
{
	context->RSSetState(rastState.data);
}

void SetBlendStateByName(std::string blendStateName)
{
	auto& blendState = blendStateMap.find(blendStateName)->second;
	const float factor[4] = {};
	context->OMSetBlendState(blendState->data, factor, 0xFFFFFFFF);
}

void SetBlendState(BlendState& blendState)
{
	const float factor[4] = {};
	context->OMSetBlendState(blendState.data, factor, 0xFFFFFFFF);
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

void SetVertexBuffer(Buffer& vertexBuffer)
{
	context->IASetVertexBuffers(0, 1, &vertexBuffer.data, &Renderer::stride, &Renderer::offset);
}

void SetIndexBuffer(Buffer* indexBuffer)
{
	context->IASetIndexBuffer(indexBuffer->data, indexBufferFormat, 0);
}

void SetSampler(uint32_t shaderRegister, Sampler* sampler)
{
	context->PSSetSamplers(shaderRegister, 1, &sampler->data);
}

void SetShaderResourceFromMaterial(uint32_t shaderRegister, Material& material)
{
	//Testing code for normal map SRV set
	/*auto normalMapTexture = textureSystem.FindTexture2D("wall_normal_map.png");
	auto normalMapSRV = normalMapTexture->GetSRV();
	context->PSSetShaderResources(normalMapTexureRegister, 1, &normalMapSRV);*/

	auto textureSRV = material.texture->GetSRV();
	context->PSSetShaderResources(shaderRegister, 1, &textureSRV);
}

void SetShaderResourcePixel(uint32_t shaderRegister, std::string textureName)
{
	auto texture = TextureSystem::FindTexture2D(textureName);
	auto textureSRV = texture->GetSRV();
	context->PSSetShaderResources(shaderRegister, 1, &textureSRV);
}

void RenderPostProcess()
{
	auto postProcessVolume = PostProcessVolume::system.GetFirstActor();
	if (postProcessVolume == nullptr) return;
	if (!postProcessVolume->IsActive()) return;
	if (!postProcessVolume->IsActiveCameraInsideVolume()) return;

	SetNullRTV();

	ID3D11ShaderResourceView* nullSRV = nullptr;
	ID3D11UnorderedAccessView* nullUAV = nullptr;

	context->RSSetState(rastStateMap[RastStates::solid]->data);

	context->IASetInputLayout(nullptr);
	context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	SetShaders(ShaderItems::PostProcess);

	//Set constant buffer data
	ShaderPostProcessData postProcessData = postProcessVolume->GetPostProcessData();
	cbPostProcess->Map(&postProcessData);
	cbPostProcess->SetPS();

	context->PSSetShaderResources(0, 1, postProcessRenderTarget.GetSRVAddress());
	SetSampler(0, &Renderer::GetDefaultSampler());

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

void RenderWireframeForVertexPaintingAndPickedActor()
{
	const auto wireframeRender = [&](MeshComponent* mesh)
		{
			context->RSSetState(rastStateWireframe);

			MaterialShaderData materialShaderData;
			materialShaderData.ambient = XMFLOAT4(1.f, 0.f, 1.f, 1.f);
			cbMaterial->Map(&materialShaderData);
			cbMaterial->SetPS();

			ShaderItem* shaderItem = ShaderSystem::FindShaderItem(ShaderItems::SolidColour->GetName());
			context->VSSetShader(shaderItem->GetVertexShader(), nullptr, 0);
			context->PSSetShader(shaderItem->GetPixelShader(), nullptr, 0);

			SetVertexBuffer(mesh->pso.GetVertexBuffer());

			SetMatricesFromMesh(mesh);
			SetShaderMeshData(mesh);

			DrawMesh(mesh);
		};

	if (!Core::gameplayOn)
	{
		if (WorldEditor::GetPickedActor())
		{
			for (auto mesh : WorldEditor::GetPickedActor()->GetComponents<MeshComponent>())
			{
				wireframeRender(mesh);
			}
		}

		for (auto actor : WorldEditor::GetPickedActors())
		{
			for (auto mesh : actor->GetComponents<MeshComponent>())
			{
				wireframeRender(mesh);
			}
		}

		SetGeneralShaderResourcesToNull();
	}

	if (WorldEditor::vertexPaintActive)
	{
		if (WorldEditor::vertexPaintLockActor != nullptr)
		{
			for (auto mesh : WorldEditor::vertexPaintLockActor->GetComponents<MeshComponent>())
			{
				wireframeRender(mesh);
			}
		}
		else
		{
			for (auto& mesh : MeshComponent::system.GetComponents())
			{
				wireframeRender(mesh.get());
			}
		}

		SetGeneralShaderResourcesToNull();
	}
}

void RenderLightProbes()
{
	auto probeMap = DiffuseProbeMap::system.GetFirstActor();
	if (probeMap == nullptr) return;
	if (!probeMap->IsActive()) return;
	if (!probeMap->IsVisible()) return;

	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();

	cbMatrices->Map(&shaderMatrices);
	cbMatrices->SetVS();

	auto instanceMesh = probeMap->lightProbesDebugInstanceMesh->instanceMesh;
	SetRenderPipelineStates(instanceMesh);

	SetBlendStateByName(BlendStates::null);

	//Careful with the buffers here, they're not part of the instance mesh, instead they're in the diffuse probe map
	MapBuffer(probeMap->GetStructuredBuffer(), probeMap->lightProbeData.data(), sizeof(LightProbeInstanceData) * probeMap->lightProbeData.size());

	//Update texture matrix
	shaderMatrices.MakeTextureMatrix(instanceMesh->GetMaterial());
	cbMatrices->Map(&shaderMatrices);
	cbMatrices->SetVS();

	ID3D11ShaderResourceView* srv = probeMap->GetSRV();
	context->VSSetShaderResources(lightProbeInstanceDataRegister, 1, &srv);
	context->PSSetShaderResources(lightProbeInstanceDataRegister, 1, &srv);

	cbLights->SetPS();

	context->DrawInstanced(instanceMesh->meshDataProxy.vertices.size(), probeMap->GetProbeCount(), 0, 0);
}

void PointLightVertexColourMap()
{
	const auto numPointLights = PointLightComponent::system.GetNumComponents();
	const auto numDirectionalLights = PointLightComponent::system.GetNumComponents();

	QProgressDialog progress("Mapping vertex colours from lights", QString(), 0, numPointLights + numDirectionalLights);
	progress.setWindowModality(Qt::WindowModal);
	progress.show();

	uint32_t lightIndex = 0;

	auto UpdateProgressDialog = [&]()
		{
			QApplication::processEvents();
			progress.setValue(lightIndex);
			lightIndex++;
		};

	for (auto& pointLight : PointLightComponent::system.GetComponents())
	{
		for (auto& mesh : MeshComponent::system.GetComponents())
		{
			if (!mesh->IsRenderStatic())
			{
				continue;
			}

			const auto meshWorldMatrix = mesh->GetWorldMatrix();

			auto& vertices = mesh->GetAllVertices();

			for (auto& vertex : vertices)
			{
				const auto vertexPos = XMLoadFloat3(&vertex.pos);
				const auto worldSpaceVertexPos = XMVector3TransformCoord(vertexPos, meshWorldMatrix);

				HitResult hit;
				hit.actorsToIgnore.emplace_back(pointLight->GetOwner());
				hit.componentsToIgnore.emplace_back(pointLight.get());
				hit.AddAllRenderStaticMeshesToIgnore();
				hit.ignoreBackFaceHits = false;

				auto normal = XMLoadFloat3(&vertex.normal);
				normal = XMVector3TransformNormal(normal, meshWorldMatrix);
				normal = XMVector3Normalize(normal);

				const auto vertexToLightDirection =
					XMVector3Normalize(pointLight->GetWorldPositionV() - worldSpaceVertexPos);
				float dot = XMVector3Dot(normal, vertexToLightDirection).m128_f32[0];
				dot = std::clamp(dot, 0.1f, 1.f);

				//@Todo: needs work if you want to use this. vertex colours aren't being multiplied properly.
				const auto rayOrigin = worldSpaceVertexPos + (normal * 0.1f);
				if (!Raycast(hit, rayOrigin, pointLight->GetWorldPositionV()))
				{
					auto colour = pointLight->GetLightData().colour;
					auto lightColour = XMLoadFloat4(&colour);
					auto vertColour = XMLoadFloat4(&vertex.colour);
					lightColour *= dot / XM_PI;

					float len = XMVector3Length(rayOrigin - pointLight->GetWorldPositionV()).m128_f32[0];
					float falloff = pointLight->GetLightData().intensity / std::max(len, 0.01f);

					vertColour = lightColour * falloff;
					XMStoreFloat4(&vertex.colour, vertColour);
				}
				else
				{
					vertex.colour = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.f);
				}
			}

			mesh->CreateNewVertexBuffer();
		}

		UpdateProgressDialog();
	}

	//This gives like an ambient occlusion effect
	//@Todo: code between lights here can be condensed 
	for (auto& dLight : DirectionalLightComponent::system.GetComponents())
	{
		for (auto& mesh : MeshComponent::system.GetComponents())
		{
			if (!mesh->IsRenderStatic())
			{
				continue;
			}

			const auto meshWorldMatrix = mesh->GetWorldMatrix();

			auto& vertices = mesh->GetAllVertices();

			for (auto& vertex : vertices)
			{
				const auto vertexPos = XMLoadFloat3(&vertex.pos);
				const auto worldSpaceVertexPos = XMVector3TransformCoord(vertexPos, meshWorldMatrix);

				HitResult hit;
				hit.actorsToIgnore.emplace_back(dLight->GetOwner());
				hit.componentsToIgnore.emplace_back(dLight.get());
				hit.ignoreBackFaceHits = false;

				auto normal = XMLoadFloat3(&vertex.normal);
				normal = XMVector3TransformNormal(normal, meshWorldMatrix);
				normal = XMVector3Normalize(normal);

				const auto dLightDirection = dLight->GetForwardVectorV();
				float dot = XMVector3Dot(normal, dLightDirection).m128_f32[0];
				dot = std::clamp(dot, 0.1f, 1.f);

				const auto rayOrigin = worldSpaceVertexPos + (normal * 0.1f);
				if (Raycast(hit, rayOrigin, -dLightDirection, 50.f))
				{
					auto colour = dLight->GetLightData().colour;
					auto lightColour = XMLoadFloat4(&colour);
					auto vertColour = XMLoadFloat4(&vertex.colour);
					lightColour *= dot;
					vertColour = lightColour;
					XMStoreFloat4(&vertex.colour, vertColour);
				}
			}

			mesh->CreateNewVertexBuffer();
		}

		UpdateProgressDialog();
	}
}

RastState* Renderer::GetRastState(std::string rastStateName)
{
	return rastStateMap[rastStateName].get();
}

BlendState* Renderer::GetBlendState(std::string blendStateName)
{
	return blendStateMap[blendStateName].get();
}

void Renderer::AddDebugDrawOrientedBox(DirectX::BoundingOrientedBox& orientedBox, bool clear)
{
	DebugBoxData data;
	data.bb = orientedBox;
	data.clearWithTimer = clear;
	data.timer = 3.f;
	debugOrientedBoxesOnTimerToRender.emplace_back(data);
}

void ClearBounds()
{
	for (int i = 0; i < debugOrientedBoxesOnTimerToRender.size(); i++)
	{
		auto& boxData = debugOrientedBoxesOnTimerToRender[i];
		if (!boxData.clearWithTimer)
		{
			debugOrientedBoxesOnTimerToRender.erase(debugOrientedBoxesOnTimerToRender.begin() + i);
		}
		else
		{
			boxData.timer -= Core::GetDeltaTime();
			if (boxData.timer < 0.f)
			{
				debugOrientedBoxesOnTimerToRender.erase(debugOrientedBoxesOnTimerToRender.begin() + i);
			}
		}
	}
}

void Renderer::AddDebugLine(Line& line)
{
	Vertex v1{}, v2{};
	v1.pos = line.p1;
	v2.pos = line.p2;
	debugLines.emplace_back(v1);
	debugLines.emplace_back(v2);
}

ID3D11Device& Renderer::GetDevice()
{
	return *device;
}

ID3D11DeviceContext& Renderer::GetDeviceContext()
{
	return *context;
}

Sampler& Renderer::GetDefaultSampler()
{
	return *defaultSampler;
}
