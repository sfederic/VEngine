#include "Renderer.h"
#include "Debug.h"
#include "Core.h"
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
#include "Components/BoxTriggerComponent.h"
#include "Actors/Actor.h"
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

Renderer renderer;

const int cbMatrixRegister = 0;
const int cbMaterialRegister = 1;
const int cbSkinningRegister = 2;
const int cbLightsRegister = 3;
const int instanceSRVRegister = 3;
const int shadowMapTextureResgiter = 1;

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
	CreateMainConstantBuffers();
	CreateQueries();
	CheckSupportedFeatures();

	RenderUtils::defaultSampler = RenderUtils::CreateSampler();

	spriteSystem.Init();
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

	//IDXGIAdapter1* adapter;
	//for (int i = 0; dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_MINIMUM_POWER,
	//	IID_PPV_ARGS(&adapter)) != DXGI_ERROR_NOT_FOUND; i++)
	//{
	//	gpuAdapters.push_back(adapter);
	//	DXGI_ADAPTER_DESC1 desc = {};
	//	adapter->GetDesc1(&desc);
	//	gpuAdaptersDesc.push_back(desc);
	//}

	//TODO: selecting adapters here isn't working.
	HR(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
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
	//Create Render target views
	for (int i = 0; i < swapchainCount; i++)
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		swapchain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		HR(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &rtvs[i]));
		backBuffer.Reset();
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
		RastState* rs = new RastState("solid", rastDesc, rastStateSolid);
		rastStateMap[rs->name] = rs;
	}

	//WIREFRAME
	{
		rastDesc.FillMode = D3D11_FILL_WIREFRAME;
		rastDesc.CullMode = D3D11_CULL_NONE;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateWireframe));
		RastState* rs = new RastState("wireframe", rastDesc, rastStateWireframe);
		rastStateMap[rs->name] = rs;
	}

	//SOLID, NO BACK CULL
	{
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		HR(device->CreateRasterizerState(&rastDesc, &rastStateNoBackCull));
		RastState* rs = new RastState("nobackcull", rastDesc, rastStateNoBackCull);
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
		RastState* rs = new RastState("shadow", rastDesc, rastStateShadow);
		rastStateMap[rs->name] = rs;
	}
}

void Renderer::CreateBlendStates()
{
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

		BlendState* bs = new BlendState("default", alphaToCoverageDesc, blendStateAlphaToCoverage);
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

void Renderer::CreateMainConstantBuffers()
{
	//Shader matrix constant buffer
	shaderMatrices.Create();

	cbMatrices = RenderUtils::CreateDefaultBuffer(sizeof(shaderMatrices), D3D11_BIND_CONSTANT_BUFFER, &shaderMatrices);
	assert(cbMatrices);

	//Material buffer
	MaterialShaderData materialShaderData;
	cbMaterial = RenderUtils::CreateDefaultBuffer(sizeof(MaterialShaderData), 
		D3D11_BIND_CONSTANT_BUFFER, &materialShaderData);
	assert(cbMaterial);

	//Lights buffer
	ShaderLights shaderLights;
	cbLights = RenderUtils::CreateDefaultBuffer(sizeof(ShaderLights),
		D3D11_BIND_CONSTANT_BUFFER, &shaderLights);
	assert(cbLights);

	//Skinning data
	XMMATRIX skinningMatrices[96] = {};
	cbSkinningData = RenderUtils::CreateDefaultBuffer(sizeof(XMMATRIX) * 96,
		D3D11_BIND_CONSTANT_BUFFER, skinningMatrices);
	assert(cbSkinningData);
}

void Renderer::CheckSupportedFeatures()
{
	D3D11_FEATURE_DATA_THREADING threadFeature = {};
	device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadFeature, sizeof(threadFeature));
}

void Renderer::RenderShadowPass()
{
	PROFILE_START

	shadowMap->BindDsvAndSetNullRenderTarget(context);

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.lightMVP = shadowMap->OutputMatrix();
	shaderMatrices.lightViewProj = shadowMap->GetLightViewMatrix() * shadowMap->GetLightPerspectiveMatrix();

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
		shaderMatrices.MakeTextureMatrix(&mesh->material->materialShaderData);

		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);
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

	const float clearColour[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	UINT frameIndex = swapchain->GetCurrentBackBufferIndex();

	context->ClearRenderTargetView(rtvs[frameIndex], clearColour);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	context->OMSetRenderTargets(1, &rtvs[frameIndex], dsv);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::Render()
{
	PROFILE_START

	StartGPUQueries();

	RenderShadowPass();
	RenderMeshComponents();
	RenderInstanceMeshComponents();
	RenderBounds();
	RenderLightMeshes();
	RenderCameraMeshes();

	PROFILE_END
}

void Renderer::RenderMeshComponents()
{
	PROFILE_START

	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.proj = activeCamera->GetProjectionMatrix();

	if (!DirectionalLightComponent::system.components.empty())
	{
		shaderMatrices.lightMVP = shadowMap->OutputMatrix();
		shaderMatrices.lightViewProj = shadowMap->GetLightViewMatrix() * shadowMap->GetLightPerspectiveMatrix();
	}

	RenderSetup();

	UpdateLights();

	const float blendFactor[4] = { 1.f, 1.f, 1.f, 1.f };
	context->OMSetBlendState(nullptr, blendFactor, 0xFFFFFFFF);

	for (auto mesh : MeshComponent::system.components)
	{
		if (!mesh->active) continue;

		SetRenderPipelineStates(mesh);

		//Animation
		AnimateSkeletalMesh(mesh);

		//Set matrices
		shaderMatrices.model = mesh->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		shaderMatrices.MakeTextureMatrix(&mesh->material->materialShaderData);
	
		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);
		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		//Set lights buffer
		context->PSSetConstantBuffers(cbLightsRegister, 1, &cbLights);

		//Set shadow resources
		context->PSSetShaderResources(shadowMapTextureResgiter, 1, &shadowMap->depthMapSRV);
		context->PSSetSamplers(1, 1, &shadowMap->sampler);

		//Draw
		context->DrawIndexed(mesh->meshDataProxy->indices->size(), 0, 0);
		
		//Setting the shadowmap to null here gets rid of a warning. Can also set this outside of the loop.
		ID3D11ShaderResourceView* nullSRV = nullptr;
		context->PSSetShaderResources(shadowMapTextureResgiter, 1, &nullSRV);
	}	

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

	context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);
	context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

	const float factor[4] = { 0.f };
	context->OMSetBlendState(blendStateMap.find("default")->second->data, factor, 0xFFFFFFFF);

	for (InstanceMeshComponent* instanceMesh : InstanceMeshComponent::system.components)
	{
		if (!instanceMesh->active) continue;

		SetRenderPipelineStates(instanceMesh);

		//update texture matrix
		shaderMatrices.MakeTextureMatrix(&instanceMesh->material->materialShaderData);
		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);
		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		//Update instance data and set SRV
		context->UpdateSubresource(instanceMesh->structuredBuffer, 0, nullptr, instanceMesh->instanceData.data(), 0, 0);
		context->VSSetShaderResources(instanceSRVRegister, 1, &instanceMesh->srv);

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

		context->IASetVertexBuffers(0, 1, &debugBox.boxMesh->pso->vertexBuffer->data, &stride, &offset);

		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		shaderMatrices.view = activeCamera->GetViewMatrix();
		shaderMatrices.proj = activeCamera->GetProjectionMatrix();

		//Set debug wireframe material colour
		materialShaderData.ambient = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
		context->UpdateSubresource(cbMaterial, 0, nullptr, &materialShaderData, 0, 0);
		context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);

		for (Actor* actor : world.GetAllActorsInWorld())
		{
			for (SpatialComponent* spatialComponent : actor->GetComponentsOfType<SpatialComponent>())
			{
				//Skip any triggers, otherwise you're rendering two wireframe meshes per component
				if (dynamic_cast<BoxTriggerComponent*>(spatialComponent))
				{
					continue;
				}

				BoundingOrientedBox boundingBox = spatialComponent->boundingBox;
				XMMATRIX boundsMatrix = VMath::GetBoundingBoxMatrix(boundingBox, actor);

				shaderMatrices.model = boundsMatrix;

				//Set bouding box scale just slightly more than the component to avoid overlap
				shaderMatrices.model.r[0].m128_f32[0] += 0.01f;
				shaderMatrices.model.r[1].m128_f32[1] += 0.01f;
				shaderMatrices.model.r[2].m128_f32[2] += 0.01f;

				shaderMatrices.mvp = shaderMatrices.model * shaderMatrices.view * shaderMatrices.proj;
				context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);

				context->Draw(debugBox.boxMesh->meshDataProxy->vertices->size(), 0);
			}
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

		//Set trigger wireframe material colour
		materialShaderData.ambient = XMFLOAT4(0.1f, 0.75f, 0.1f, 1.0f);
		context->UpdateSubresource(cbMaterial, 0, nullptr, &materialShaderData, 0, 0);
		context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);

		for (auto boxTrigger : BoxTriggerComponent::system.components)
		{
			shaderMatrices.model = boxTrigger->GetWorldMatrix();

			shaderMatrices.model.r[0].m128_f32[0] *= boxTrigger->boundingBox.Extents.x;
			shaderMatrices.model.r[1].m128_f32[1] *= boxTrigger->boundingBox.Extents.y;
			shaderMatrices.model.r[2].m128_f32[2] *= boxTrigger->boundingBox.Extents.z;

			shaderMatrices.mvp = shaderMatrices.model * shaderMatrices.view * shaderMatrices.proj;
			context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);

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

	//DRAW CAMERAS
	for (auto camera : CameraComponent::system.components)
	{
		context->RSSetState(rastStateWireframe);

		ShaderItem* shader = shaderSystem.FindShader(L"SolidColour.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		context->IASetVertexBuffers(0, 1, &debugCamera.mesh->pso->vertexBuffer->data, &stride, &offset);

		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		shaderMatrices.view = activeCamera->GetViewMatrix();
		shaderMatrices.proj = activeCamera->GetProjectionMatrix();

		//Make cameras red
		materialShaderData.ambient = XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f);
		context->UpdateSubresource(cbMaterial, 0, nullptr, &materialShaderData, 0, 0);
		context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);

		shaderMatrices.model = camera->GetWorldMatrix();

		shaderMatrices.mvp = shaderMatrices.model * shaderMatrices.view * shaderMatrices.proj;
		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);

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
	context->UpdateSubresource(cbMaterial, 0, nullptr, &materialShaderData, 0, 0);
	context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);


	//DIRECTIONAL LIGHTS
	context->IASetVertexBuffers(0, 1, &debugSphere.sphereMesh->pso->vertexBuffer->data, &stride, &offset);

	for (auto directionalLight : DirectionalLightComponent::system.components)
	{
		shaderMatrices.model = directionalLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);

		context->Draw(debugSphere.sphereMesh->meshDataProxy->vertices->size(), 0);
	}


	//POINT LIGHTS
	context->IASetVertexBuffers(0, 1, &debugIcoSphere.mesh->pso->vertexBuffer->data, &stride, &offset);

	for (auto pointLight : PointLightComponent::system.components)
	{
		shaderMatrices.model = pointLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);

		context->Draw(debugIcoSphere.mesh->meshDataProxy->vertices->size(), 0);
	}


	//SPOT LIGHTS
	context->IASetVertexBuffers(0, 1, &debugCone.mesh->pso->vertexBuffer->data, &stride, &offset);

	for (auto spotLight : SpotLightComponent::system.components)
	{
		shaderMatrices.model = spotLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);

		context->Draw(debugCone.mesh->meshDataProxy->vertices->size(), 0);
	}
}

void Renderer::AnimateSkeletalMesh(MeshComponent* mesh)
{
	PROFILE_START

	Skeleton* skeleton = mesh->meshDataProxy->skeleton;

	if (skeleton && !skeleton->joints.empty())
	{
		std::vector<XMMATRIX> skinningData;

		//push meshes' matrix into vector for base armature bone (Identity matrix works as well somehow)
		skinningData.push_back(mesh->GetWorldMatrix());

		//Move through and animate all joints on skeleton
		for (Joint& joint : skeleton->joints)
		{
			if (!joint.anim.frames.empty())
			{
				if (joint.anim.currentTime >= joint.anim.GetEndTime())
				{
					joint.anim.currentTime = 0.f;
				}

				joint.anim.currentTime += Core::GetDeltaTime();
				joint.anim.Interpolate(joint.anim.currentTime, joint, skeleton);

				skinningData.push_back(joint.transormPose);
			}
		}

		//Update skinning constant buffers
		if (!skinningData.empty())
		{
			context->UpdateSubresource(cbSkinningData, 0, nullptr, skinningData.data(), 0, 0);
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

	for (auto emitter : ParticleEmitter::system.components)
	{
		if (drawAllAsWireframe)
		{
			context->RSSetState(rastStateWireframe);
		}
		else
		{
			//Careful with back culling visibility here for UI
			context->RSSetState(rastStateMap["nobackcull"]->data);
		}

		ShaderItem* shader = shaderSystem.FindShader(L"Unlit.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		context->PSSetSamplers(0, 1, &RenderUtils::GetDefaultSampler()->data);

		//Set texture from emitter for every particle
		context->PSSetShaderResources(0, 1, &textureSystem.FindTexture2D(emitter->textureFilename)->srv);

		spriteSystem.UpdateAndSetSpriteBuffers(context);

		for (auto& particle : emitter->particles)
		{
			VMath::RotateTowardsCamera(particle.transform);

			shaderMatrices.model = particle.transform.GetAffine();
			shaderMatrices.MakeModelViewProjectionMatrix();

			context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);
			context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

			//Draw
			//TODO: do instancing here for particles.
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

		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);
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

	shaderLights.globalAmbience = XMFLOAT4(0.35f, 0.35f, 0.35f, 1.f);
	XMStoreFloat4(&shaderLights.eyePosition, activeCamera->transform.world.r[3]);

	context->UpdateSubresource(cbLights, 0, nullptr, &shaderLights, 0, 0);
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

	HR(swapchain->ResizeBuffers(swapchainCount, newWidth, newHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	viewport.Width = newWidth;
	viewport.Height = newHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);

	CreateRTVAndDSV();

	uiSystem.Init((void*)swapchain);

	shaderMatrices.Create();
}

void Renderer::SetRenderPipelineStates(MeshComponent* mesh)
{
	Material* material = mesh->material;
	PipelineStateObject* pso = mesh->pso;

	if (drawAllAsWireframe)
	{
		context->RSSetState(rastStateWireframe);
	}
	else
	{
		context->RSSetState(material->rastState->data);
	}

	const FLOAT blendState[4] = { 0.f };
	//context->OMSetBlendState(material->blendState->data, blendState, 0xFFFFFFFF);

	context->VSSetShader(material->shader->vertexShader, nullptr, 0);
	context->PSSetShader(material->shader->pixelShader, nullptr, 0);

	context->PSSetSamplers(0, 1, &material->sampler->data);
	context->PSSetShaderResources(0, 1, &material->texture->srv);

	context->IASetVertexBuffers(0, 1, &pso->vertexBuffer->data, &stride, &offset);
	context->IASetIndexBuffer(pso->indexBuffer->data, DXGI_FORMAT_R32_UINT, 0);

	context->UpdateSubresource(cbMaterial, 0, nullptr, &material->materialShaderData, 0, 0);
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
