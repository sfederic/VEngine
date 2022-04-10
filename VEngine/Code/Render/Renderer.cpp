#include "Renderer.h"
#include <ScreenGrab.h>
#include <WinCodec.h> //For GUID_ContainerFormatJpeg
#include <filesystem>
#include "Debug.h"
#include "Core.h"
#include "VMath.h"
#include "ShaderSystem.h"
#include "Camera.h"
#include "UI/UISystem.h"
#include "Components/MeshComponent.h"
#include "Components/DestructibleMeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/InstanceMeshComponent.h"
#include "Components/Lights/DirectionalLightComponent.h"
#include "Components/Lights/PointLightComponent.h"
#include "Components/Lights/SpotLightComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Actors/Actor.h"
#include "Actors/ReflectionPlane.h"
#include "ShaderSystem.h"
#include "DebugActors/DebugBox.h"
#include "DebugActors/DebugSphere.h"
#include "DebugActors/DebugIcoSphere.h"
#include "DebugActors/DebugCamera.h"
#include "DebugActors/DebugCone.h"
#include "Input.h"
#include "World.h"
#include "Material.h"
#include "Profile.h"
#include "RenderUtils.h"
#include "Editor/DebugMenu.h"
#include "ShadowMap.h"
#include "TextureSystem.h"
#include "Render/SpriteSystem.h"
#include "Particle/ParticleSystem.h"
#include "Particle/ParticleEmitter.h"
#include "Particle/SpriteSheet.h"
#include "Particle/Polyboard.h"
#include "Gameplay/GameInstance.h"

Renderer renderer;

const int cbMatrixRegister = 0;
const int cbMaterialRegister = 1;
const int cbSkinningRegister = 2;
const int cbLightsRegister = 3;
const int cbTimeRegister = 4;
const int cbMeshDataRegister = 5;
const int instanceSRVRegister = 3;
const int shadowMapTextureResgiter = 1;
const int reflectionTextureResgiter = 2;

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

	CreateSwapchain((HWND)window);
	CreateRTVAndDSV();
	CreateInputLayout();
	CreateRasterizerStates();
	CreateBlendStates();
	CreateConstantBuffers();
	CreateQueries();
	CheckSupportedFeatures();

	CreatePostProcessRenderTarget();

	RenderUtils::defaultSampler = RenderUtils::CreateSampler();

	Line lines[1024] = {};
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

void Renderer::CreateFactory()
{
	ComPtr<IDXGIFactory> tempDxgiFactory;
	HR(CreateDXGIFactory(IID_PPV_ARGS(tempDxgiFactory.GetAddressOf())));
	HR(tempDxgiFactory->QueryInterface(&dxgiFactory));
}

void Renderer::CreateDevice()
{
	//BGRA support needed for DirectWrite and Direct2D
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1 };
	D3D_FEATURE_LEVEL selectedFeatureLevel;

	IDXGIAdapter1* adapter = nullptr;
	HR(dxgiFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)));
	DXGI_ADAPTER_DESC1 desc = {};
	adapter->GetDesc1(&desc);
	gpuAdaptersDesc.push_back(desc);
	/*for (int i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_MINIMUM_POWER,
		IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND; i++)
	{
		DXGI_ADAPTER_DESC1 desc = {};
		adapter->GetDesc1(&desc);
		gpuAdaptersDesc.push_back(desc);
	}*/

	HR(D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, createDeviceFlags,
		featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &device,
		&selectedFeatureLevel, &context));

	RenderUtils::device = device;
}

void Renderer::CreateSwapchain(HWND window)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc = { (UINT)viewport.Width, (UINT)viewport.Height, {60, 1}, DXGI_FORMAT_R8G8B8A8_UNORM };
	sd.Windowed = TRUE;
	sd.SampleDesc.Count = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.BufferCount = swapchainCount;

	ComPtr<IDXGISwapChain> tempSwapchain;
	HR(dxgiFactory->CreateSwapChain(device, &sd, tempSwapchain.GetAddressOf()));
	HR(tempSwapchain->QueryInterface(&swapchain));

	dxgiFactory->MakeWindowAssociation(window, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
}

void Renderer::CreateRTVAndDSV()
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
	dsDesc.SampleDesc.Count = 1;
	dsDesc.Width = viewport.Width;
	dsDesc.Height = viewport.Height;

	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	HR(device->CreateTexture2D(&dsDesc, nullptr, depthStencilBuffer.GetAddressOf()));
	HR(device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, &dsv));
	depthStencilBuffer.Reset();
}

void Renderer::CreateInputLayout()
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

void Renderer::CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FrontCounterClockwise = FALSE;

	//SOLID
	{
		HR(device->CreateRasterizerState(&rastDesc, &rastStateSolid));
		RastState* rs = new RastState(RastStates::solid, rastDesc, rastStateSolid);
		rastStateMap[rs->name] = rs;
	}

	//WIREFRAME
	{
		rastDesc.FillMode = D3D11_FILL_WIREFRAME;
		rastDesc.CullMode = D3D11_CULL_NONE;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateWireframe));
		RastState* rs = new RastState(RastStates::wireframe, rastDesc, rastStateWireframe);
		rastStateMap[rs->name] = rs;
	}

	//SOLID, NO BACK CULL
	{
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateNoBackCull));
		RastState* rs = new RastState(RastStates::noBackCull, rastDesc, rastStateNoBackCull);
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
		RastState* rs = new RastState(RastStates::shadow, rastDesc, rastStateShadow);
		rastStateMap[rs->name] = rs;
	}
}

void Renderer::CreateBlendStates()
{
	//NULL BLEND STATE
	{
		D3D11_BLEND_DESC nullBlendDesc = {};
		nullBlendDesc.RenderTarget[0].BlendEnable = false;
		HR(device->CreateBlendState(&nullBlendDesc, &blendStateAlphaToCoverage));

		BlendState* bs = new BlendState(BlendStates::null, nullBlendDesc, nullBlendState);
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

		BlendState* bs = new BlendState(BlendStates::Default, alphaToCoverageDesc, blendStateAlphaToCoverage);
		blendStateMap[bs->name] = bs;
	}
}

void Renderer::CreateQueries()
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

void Renderer::CreateConstantBuffers()
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
}

void Renderer::CreatePlanarReflectionBuffers()
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

void Renderer::MapBuffer(ID3D11Resource* resource, const void* src, size_t size)
{
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	HR(context->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
	std::memcpy(mapped.pData, src, size);
	context->Unmap(resource, 0);
}

void Renderer::CheckSupportedFeatures()
{
	//Threading check
	D3D11_FEATURE_DATA_THREADING threadFeature = {};
	device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadFeature, sizeof(threadFeature));

	//MSAA check
	UINT m4xMsaaQuality;
	HR(device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
}

void Renderer::RenderShadowPass()
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

	ID3D11RenderTargetView* nullRTV = nullptr;
	context->OMSetRenderTargets(1, &nullRTV, nullptr);
	context->RSSetState(0);

	PROFILE_END
}

void Renderer::RenderSetup()
{
	context->RSSetViewports(1, &viewport);

	const float clearColour[4] = { 0.f, 0.f, 0.f, 1.f };
	UINT frameIndex = swapchain->GetCurrentBackBufferIndex();

	context->ClearRenderTargetView(rtvs[frameIndex], clearColour);
	//context->ClearRenderTargetView(postRTV, clearColour);

	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	context->OMSetRenderTargets(1, &rtvs[frameIndex], dsv);
	//context->OMSetRenderTargets(1, &postRTV, dsv);

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	//PostProcessRender();

	PROFILE_END
}

void Renderer::RenderMeshComponents()
{
	PROFILE_START

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

	RenderSetup();
	UpdateLights();

	shaderMatrices.view = activeCamera->GetViewMatrix();

	for (auto mesh : MeshComponent::system.components)
	{
		if (!mesh->active || mesh->cullMesh) continue;

		SetRenderPipelineStates(mesh);

		//Set lights buffer
		context->PSSetConstantBuffers(cbLightsRegister, 1, &cbLights);

		//Set shadow resources
		context->PSSetShaderResources(shadowMapTextureResgiter, 1, &shadowMap->depthMapSRV);
		context->PSSetSamplers(1, 1, &shadowMap->sampler);

		//Set reflection resources
		context->PSSetShaderResources(reflectionTextureResgiter, 1, &reflectionSRV);

		Material* material = mesh->material;

		//Set matrices
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

	//Set to null to remove warnings
	ID3D11ShaderResourceView* nullSRV = nullptr;
	context->PSSetShaderResources(shadowMapTextureResgiter, 1, &nullSRV);

	context->PSSetShaderResources(reflectionTextureResgiter, 1, &nullSRV);

	PROFILE_END
}

void Renderer::RenderPlanarReflections()
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

		context->IASetVertexBuffers(0, 1, &mesh->pso->vertexBuffer->data, &stride, &offset);
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
	ID3D11RenderTargetView* nullRTV = nullptr;
	context->OMSetRenderTargets(1, &nullRTV, nullptr);
	context->RSSetState(0);

	PROFILE_END
}

void Renderer::RenderInstanceMeshComponents()
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

void Renderer::RenderBounds()
{
	static DebugBox debugBox;

	MaterialShaderData materialShaderData;

	if (drawBoundingBoxes)
	{
		context->RSSetState(rastStateWireframe);

		ShaderItem* shader = shaderSystem.FindShader(L"SolidColour.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		//@Todo: there's a weird error here where if you create enough lights in the map (about 40),
		//the debug mesh actors will crash here. Tried putting the Debug Actors as global pointers
		//instead of being static, but then Direct2D swapchain/rendertarget errors would happen.
		//Feels like it might be the GPU doing some funny memory thing.
		context->IASetVertexBuffers(0, 1, &debugBox.boxMesh->pso->vertexBuffer->data, &stride, &offset);

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
	if(drawTriggers)
	{
		context->RSSetState(rastStateWireframe);

		ShaderItem* shader = shaderSystem.FindShader(L"SolidColour.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		context->IASetVertexBuffers(0, 1, &debugBox.boxMesh->pso->vertexBuffer->data, &stride, &offset);

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

void Renderer::RenderCameraMeshes()
{
	if (Core::gameplayOn)
	{
		return;
	}

	static DebugCamera debugCamera;;

	MaterialShaderData materialShaderData;

	context->RSSetState(rastStateWireframe);

	ShaderItem* shader = shaderSystem.FindShader(L"SolidColour.hlsl");
	context->VSSetShader(shader->vertexShader, nullptr, 0);
	context->PSSetShader(shader->pixelShader, nullptr, 0);

	context->IASetVertexBuffers(0, 1, &debugCamera.mesh->pso->vertexBuffer->data, &stride, &offset);

	context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

	//Make cameras red
	materialShaderData.ambient = XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f);
	MapBuffer(cbMaterial, &materialShaderData, sizeof(MaterialShaderData));
	context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);

	//DRAW CAMERAS
	for (auto camera : CameraComponent::system.components)
	{
		shaderMatrices.model = camera->GetWorldMatrix();

		shaderMatrices.mvp = shaderMatrices.model * shaderMatrices.view * shaderMatrices.proj;
		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));

		context->Draw(debugCamera.mesh->meshDataProxy->vertices->size(), 0);
	}
}

void Renderer::RenderLightMeshes()
{
	static DebugSphere debugSphere;
	static DebugIcoSphere debugIcoSphere;
	static DebugCone debugCone;

	if (Core::gameplayOn)
	{
		return;
	}

	context->RSSetState(rastStateWireframe);

	ShaderItem* shader = shaderSystem.FindShader(L"SolidColour.hlsl");
	context->VSSetShader(shader->vertexShader, nullptr, 0);
	context->PSSetShader(shader->pixelShader, nullptr, 0);

	context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.proj = activeCamera->GetProjectionMatrix();

	//Set debug sphere wireframe material colour
	MaterialShaderData materialShaderData;
	materialShaderData.ambient = XMFLOAT4(1.f, 1.f, 0.f, 1.0f);
	MapBuffer(cbMaterial, &materialShaderData, sizeof(MaterialShaderData));
	context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);


	//DIRECTIONAL LIGHTS
	context->IASetVertexBuffers(0, 1, &debugSphere.sphereMesh->pso->vertexBuffer->data, &stride, &offset);

	for (auto directionalLight : DirectionalLightComponent::system.components)
	{
		shaderMatrices.model = directionalLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));

		context->Draw(debugSphere.sphereMesh->meshDataProxy->vertices->size(), 0);
	}


	//POINT LIGHTS
	context->IASetVertexBuffers(0, 1, &debugIcoSphere.mesh->pso->vertexBuffer->data, &stride, &offset);

	for (auto pointLight : PointLightComponent::system.components)
	{
		shaderMatrices.model = pointLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));

		context->Draw(debugIcoSphere.mesh->meshDataProxy->vertices->size(), 0);
	}


	//SPOT LIGHTS
	context->IASetVertexBuffers(0, 1, &debugCone.mesh->pso->vertexBuffer->data, &stride, &offset);

	for (auto spotLight : SpotLightComponent::system.components)
	{
		shaderMatrices.model = spotLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));

		context->Draw(debugCone.mesh->meshDataProxy->vertices->size(), 0);
	}
}

void Renderer::RenderSkeletonBones()
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

	context->IASetVertexBuffers(0, 1, &linesBuffer, &stride, &offset);

	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.proj = activeCamera->GetProjectionMatrix();
	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();

	MapBuffer(cbMatrices, &shaderMatrices, sizeof(ShaderMatrices));
	context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

	context->Draw(lines.size() * 2, 0);
}

void Renderer::RenderPolyboards()
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

void Renderer::RenderSpriteSheets()
{
	PROFILE_START

	for (auto spriteSheet : SpriteSheet::system.components)
	{
		if (drawAllAsWireframe)
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

void Renderer::AnimateSkeletalMesh(MeshComponent* mesh)
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
void Renderer::UpdateLights()
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
void Renderer::StartGPUQueries()
{
	if (debugMenu.fpsMenuOpen)
	{
		context->Begin(timeDisjointQuery[frameQueryIndex]);
		context->End(frameStartQuery[frameQueryIndex]);

		GetGPUQueryData();
	}
}

void Renderer::EndGPUQueries()
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
void Renderer::GetGPUQueryData()
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

		frameTime = (float)(timeStampEndFrame - timeStampStartFrame) / (float)timeStampDisjoint.Frequency * 1000.f;
	}

	PROFILE_END
}

void Renderer::Present()
{
	HR(swapchain->Present(1, 0));

	EndGPUQueries();
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

void Renderer::SetRenderPipelineStates(MeshComponent* mesh)
{
	Material* material = mesh->material;
	PipelineStateObject* pso = mesh->pso;

	if (drawAllAsWireframe)
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

	context->IASetVertexBuffers(0, 1, &pso->vertexBuffer->data, &stride, &offset);
	context->IASetIndexBuffer(pso->indexBuffer->data, DXGI_FORMAT_R32_UINT, 0);

	MapBuffer(cbMaterial, &material->materialShaderData, sizeof(MaterialShaderData));
	//context->VSSetConstantBuffers(cbMaterialRegister, 0, &cbMaterial);
	context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);
}

void Renderer::SetRenderPipelineStatesForShadows(MeshComponent* mesh)
{
	Material* material = mesh->material;
	PipelineStateObject* pso = mesh->pso;

	context->RSSetState(rastStateMap["shadow"]->data);

	ShaderItem* shader = shaderSystem.FindShader(L"Shadows.hlsl");

	context->VSSetShader(shader->vertexShader, nullptr, 0);
	context->PSSetShader(shader->pixelShader, nullptr, 0);

	context->IASetVertexBuffers(0, 1, &pso->vertexBuffer->data, &stride, &offset);
	context->IASetIndexBuffer(pso->indexBuffer->data, DXGI_FORMAT_R32_UINT, 0);
}

XMFLOAT4 Renderer::CalcGlobalAmbientBasedOnGameTime()
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

void Renderer::CreatePostProcessRenderTarget()
{
	if (postBuffer) postBuffer->Release();
	if (postRTV) postRTV->Release();
	if (postSRV) postSRV->Release();

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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

//@Todo: this is using the spritesystem to draw a fullscreen quad. Change it.
void Renderer::PostProcessRender()
{
	PROFILE_START

	spriteSystem.BuildSpriteQuadForParticleRendering();

	if (drawAllAsWireframe)
	{
		context->RSSetState(rastStateWireframe);
	}
	else
	{
		context->RSSetState(rastStateMap["nobackcull"]->data);
	}

	context->PSSetSamplers(0, 1, &RenderUtils::GetDefaultSampler()->data);

	spriteSystem.UpdateAndSetSpriteBuffers(context);

	const float blendFactor[4] = {};
	context->OMSetBlendState(blendStateMap.find(BlendStates::Default)->second->data, blendFactor, 0xFFFFFFFF);

	context->VSSetShader(shaderSystem.FindShader(L"PostProcess.hlsl")->vertexShader, nullptr, 0);
	context->PSSetShader(shaderSystem.FindShader(L"PostProcess.hlsl")->pixelShader, nullptr, 0);

	//The post processing RTV needs to be unset here and then the SRV to avoid D3D11 warnings
	ID3D11RenderTargetView* nullRTV = nullptr;
	context->OMSetRenderTargets(1, &nullRTV, nullptr);
	context->PSSetShaderResources(0, 1, &postSRV);
	ID3D11ShaderResourceView* nullSRV = nullptr;
	context->PSSetShaderResources(0, 1, &nullSRV);
	context->OMSetRenderTargets(1, &postRTV, dsv);

	context->DrawIndexed(6, 0, 0);

	context->CopyResource(backBuffer, postBuffer);

	UINT frameIndex = swapchain->GetCurrentBackBufferIndex();
	context->OMSetRenderTargets(1, &rtvs[frameIndex], dsv);

	PROFILE_END
}
