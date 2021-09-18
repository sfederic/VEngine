#include "Renderer.h"
#include "Debug.h"
#include "Core.h"
#include "ShaderSystem.h"
#include "Camera.h"
#include "UI/UISystem.h"
#include "Components/MeshComponent.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/InstanceMeshComponent.h"
#include "Components/Lights/DirectionalLightComponent.h"
#include "Components/Lights/PointLightComponent.h"
#include "Components/Lights/SpotLightComponent.h"
#include "Actors/Actor.h"
#include "Actors/NormalActor.h"
#include "ShaderSystem.h"
#include "DebugActors/DebugBox.h"
#include "DebugActors/DebugSphere.h"
#include "DebugActors/DebugIcoSphere.h"
#include "DebugActors/DebugCone.h"
#include "Input.h"
#include "World.h"
#include "Material.h"
#include "Profile.h"
#include "RenderUtils.h"
#include "Editor/DebugMenu.h"

Renderer renderer;

UINT stride = sizeof(Vertex);
UINT offset = 0;

const int cbMatrixRegister = 0;
const int cbMaterialRegister = 1;
const int cbSkinningRegister = 2;
const int cbLightsRegister = 3;
const int instanceSRVRegister = 3;

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

	CreateSwapchain((HWND)window);
	CreateRTVAndDSV();
	CreateInputLayout();
	CreateRasterizerStates();
	CreateBlendStates();
	CreateMainConstantBuffers();
	CreateQueries();
	CheckSupportedFeatures();
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
}

void Renderer::CreateBlendStates()
{
	{
		//MSAA has to be set for AlphaToCoverage to work.
		D3D11_BLEND_DESC alphaToCoverageDesc = {};
		alphaToCoverageDesc.AlphaToCoverageEnable = true;
		alphaToCoverageDesc.RenderTarget[0].BlendEnable = true;
		alphaToCoverageDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		alphaToCoverageDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		alphaToCoverageDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		alphaToCoverageDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		alphaToCoverageDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		alphaToCoverageDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HR(device->CreateBlendState(&alphaToCoverageDesc, &blendStateAlphaToCoverage));

		BlendState* bs = new BlendState("alphaToCoverage", alphaToCoverageDesc, blendStateAlphaToCoverage);
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
	shaderMatrices.Create(GetAspectRatio());
	editorCamera.proj = shaderMatrices.proj;

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
}

void Renderer::CheckSupportedFeatures()
{
	D3D11_FEATURE_DATA_THREADING threadFeature = {};
	device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadFeature, sizeof(threadFeature));
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

	RenderMeshComponents();
	RenderInstanceMeshComponents();
	RenderBounds();
	RenderLightMeshes();

	PROFILE_END
}

void Renderer::RenderMeshComponents()
{
	PROFILE_START

	shaderMatrices.view = activeCamera->GetViewMatrix();

	UpdateLights();

	for (auto mesh : MeshComponent::system.components)
	{
		SetRenderPipelineStates(mesh);

		//Set matrices
		shaderMatrices.model = mesh->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		shaderMatrices.MakeTextureMatrix(&mesh->material->shaderData);

		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);
		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		//Set lights buffer
		context->PSSetConstantBuffers(cbLightsRegister, 1, &cbLights);

		//Draw
		context->DrawIndexed(mesh->data->indices->size(), 0, 0);
	}

	PROFILE_END
}

void Renderer::RenderInstanceMeshComponents()
{
	PROFILE_START

	//Set matrices
	shaderMatrices.view = activeCamera->GetViewMatrix();
	shaderMatrices.MakeModelViewProjectionMatrix();

	context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);
	context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

	for (InstanceMeshComponent* instanceMesh : InstanceMeshComponent::system.components)
	{
		SetRenderPipelineStates(instanceMesh);

		//update texture matrix
		shaderMatrices.MakeTextureMatrix(&instanceMesh->material->shaderData);
		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);
		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		//Update instance data and set SRV
		context->UpdateSubresource(instanceMesh->structuredBuffer, 0, nullptr, instanceMesh->instanceData.data(), 0, 0);
		context->VSSetShaderResources(instanceSRVRegister, 1, &instanceMesh->srv);

		//Set lights buffer
		context->PSSetConstantBuffers(cbLightsRegister, 1, &cbLights);

		context->DrawIndexedInstanced(instanceMesh->data->indices->size(), instanceMesh->GetInstanceCount(), 0, 0, 0);
	}

	PROFILE_END
}

void Renderer::RenderBounds()
{
	static DebugBox debugBox;

	if (drawBoundingBoxes)
	{
		context->RSSetState(rastStateWireframe);

		ShaderItem* shader = shaderSystem.FindShader(L"SolidColour.hlsl");
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		context->IASetVertexBuffers(0, 1, &debugBox.boxMesh->pso->vertexBuffer->data, &stride, &offset);

		context->VSSetConstantBuffers(cbMatrixRegister, 1, &cbMatrices);

		shaderMatrices.view = activeCamera->GetViewMatrix();

		//Set debug wireframe material colour
		MaterialShaderData materialShaderData;
		materialShaderData.ambient = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
		context->UpdateSubresource(cbMaterial, 0, nullptr, &materialShaderData, 0, 0);
		context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);

		for (Actor* actor : world.GetAllActorsInWorld())
		{
			for (SpatialComponent* spatialComponent : actor->GetComponentsOfType<SpatialComponent>())
			{
				shaderMatrices.model = spatialComponent->GetWorldMatrix();

				//Set bouding box scale just slightly more than the component to avoid overlap
				shaderMatrices.model.r[0].m128_f32[0] *= spatialComponent->boundingBox.Extents.x + 0.01f;
				shaderMatrices.model.r[1].m128_f32[1] *= spatialComponent->boundingBox.Extents.y + 0.01f;
				shaderMatrices.model.r[2].m128_f32[2] *= spatialComponent->boundingBox.Extents.z + 0.01f;

				shaderMatrices.mvp = shaderMatrices.model * shaderMatrices.view * shaderMatrices.proj;
				context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);

				context->Draw(debugBox.boxMesh->data->vertices->size(), 0);
			}
		}
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

		context->Draw(debugSphere.sphereMesh->data->vertices->size(), 0);
	}


	//POINT LIGHTS
	context->IASetVertexBuffers(0, 1, &debugIcoSphere.mesh->pso->vertexBuffer->data, &stride, &offset);

	for (auto pointLight : PointLightComponent::system.components)
	{
		shaderMatrices.model = pointLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);

		context->Draw(debugIcoSphere.mesh->data->vertices->size(), 0);
	}


	//SPOT LIGHTS
	context->IASetVertexBuffers(0, 1, &debugCone.mesh->pso->vertexBuffer->data, &stride, &offset);

	for (auto spotLight : SpotLightComponent::system.components)
	{
		shaderMatrices.model = spotLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		context->UpdateSubresource(cbMatrices, 0, nullptr, &shaderMatrices, 0, 0);

		context->Draw(debugCone.mesh->data->vertices->size(), 0);
	}
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

	shaderMatrices.Create(GetAspectRatio());
	activeCamera->proj = shaderMatrices.proj;
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
	context->OMSetBlendState(material->blendState->data, blendState, 0xFFFFFFFF);

	context->VSSetShader(material->shader->vertexShader, nullptr, 0);
	context->PSSetShader(material->shader->pixelShader, nullptr, 0);

	context->PSSetSamplers(0, 1, &material->sampler->data);
	context->PSSetShaderResources(0, 1, &material->texture->srv);

	context->IASetVertexBuffers(0, 1, &pso->vertexBuffer->data, &stride, &offset);
	context->IASetIndexBuffer(pso->indexBuffer->data, DXGI_FORMAT_R32_UINT, 0);

	context->UpdateSubresource(cbMaterial, 0, nullptr, &material->shaderData, 0, 0);
	//context->VSSetConstantBuffers(cbMaterialRegister, 0, &cbMaterial);
	context->PSSetConstantBuffers(cbMaterialRegister, 1, &cbMaterial);
}
