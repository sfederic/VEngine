#include "vpch.h"
#include "Renderer.h"
#include "Actors/DiffuseProbeMap.h"
#include "Actors/PostProcessVolume.h"
#include "BlendStates.h"
#include "Device.h"
#include "Components/BoxTriggerComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/DestructibleMeshComponent.h"
#include "Components/InstanceMeshComponent.h"
#include "Components/Lights/DirectionalLightComponent.h"
#include "Render/Texture2D.h"
#include "Render/DebugMeshMananger.h"
#include "Components/AudioComponent.h"
#include "Components/Lights/PointLightComponent.h"
#include "Components/Lights/SpotLightComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SliceableMeshComponent.h"
#include "Components/SocketMeshComponent.h"
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
#include "RastStates.h"
#include "Render/Line.h"
#include "Render/Sampler.h"
#include "Render/RastState.h"
#include "Render/BlendState.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/RenderTarget.h"
#include "Render/ShaderData/InstanceData.h"
#include "Render/ShaderData/ShaderLights.h"
#include "Render/ShaderData/ShaderMatrices.h"
#include "Render/SpriteSystem.h"
#include "Render/DepthStencil.h"
#include "RenderUtils.h"
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

struct DebugBoxData
{
	DirectX::BoundingOrientedBox bb;
	float timer = 0.f;
	bool clearWithTimer = false;
};

//Debug object containers
std::vector<DebugBoxData> debugOrientedBoxesOnTimerToRender;
std::vector<Vertex> debugLines;
Microsoft::WRL::ComPtr<ID3D11Buffer> debugLinesBuffer;
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
	device.Create();

	ShaderSystem::Init();

	shadowMap.Create(2048, 2048);

	CheckSupportedFeatures();

	swapchain.Create(viewport.Width, viewport.Height, (HWND)window, sampleDesc, device.Get(), dxgiFactory.Get());
	CreateRTVAndDSV();
	CreateRasterizerStates();
	CreateBlendStates();
	CreateConstantBuffers();

	postProcessRenderTarget.Create(viewport.Width, viewport.Height, DXGI_FORMAT_R16G16B16A16_FLOAT);

	defaultSampler.Create();

	SpriteSystem::Init();

	//debugLines.emplace_back(Vertex()); //dummy data so DirectX doesn't crash
	////@Todo: this crashes the program a lot (most dynamic buffers do) and not sure why.
	//debugLinesBuffer = RenderUtils::CreateDynamicBuffer(debugLinesBufferSize, D3D11_BIND_VERTEX_BUFFER, debugLines.data());
	//debugLines.clear();
}

void Renderer::Cleanup()
{
	for (int i = 0; i < Swapchain::SWAPCHAIN_COUNT; i++)
	{
		rtvs[i].Recycle();
	}

	depthStencil.Reset();

	rastStateMap.clear();
	blendStateMap.clear();

	swapchain.Reset();
	dxgiFactory.Reset();

	for (int i = 0; i < 6; i++)
	{
		lightProbeRTVs[i].Reset();
	}
	lightProbeSRV.Reset();
	lightProbeTexture.Reset();

	debugLinesBuffer.Reset();

	ReportLiveObjectsVerbose();
}

void Renderer::Tick()
{
	//BOUNDING BOXES HOTKEY
	if (Input::GetSystemKeyHeld(Keys::Ctrl))
	{
		if (Input::GetSystemKeyDown(Keys::B))
		{
			drawBoundingBoxes = !drawBoundingBoxes;
		}
	}

	//RENDER TRIGGERS HOTKEY
	if (Input::GetSystemKeyHeld(Keys::Ctrl))
	{
		if (Input::GetSystemKeyDown(Keys::Y))
		{
			drawTriggers = !drawTriggers;
		}
	}

	//DRAW ALL AS WIREFRAME HOTKEY
	if (Input::GetSystemKeyUp(Keys::F3))
	{
		drawAllAsWireframe = !drawAllAsWireframe;
	}

	if (Input::GetSystemKeyUp(Keys::F4))
	{
		VertexColourLightBake();
	}

	ScreenshotCapture();
}

void Renderer::CreateFactory()
{
	IDXGIFactory* tempDxgiFactory = nullptr;
	HR(CreateDXGIFactory(IID_PPV_ARGS(&tempDxgiFactory)));
	HR(tempDxgiFactory->QueryInterface(dxgiFactory.GetAddressOf()));
	tempDxgiFactory->Release();
}

void Renderer::CreateRTVAndDSV()
{
	ID3D11Texture2D* backBuffer = nullptr;
	swapchain.GetBackBuffer(&backBuffer);

	for (int i = 0; i < Swapchain::SWAPCHAIN_COUNT; i++)
	{
		rtvs[i].CreateFromSwapchainBackBuffer(backBuffer);
	}

	depthStencil.Create(viewport.Width, viewport.Height, sampleDesc);

	backBuffer->Release();
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
		rastStateMap.emplace(RastStates::solid, std::make_unique<RastState>(RastStates::solid, rastDesc));
	}

	//WIREFRAME
	{
		rastDesc.FillMode = D3D11_FILL_WIREFRAME;
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastStateMap.emplace(RastStates::wireframe, std::make_unique<RastState>(RastStates::wireframe, rastDesc));
	}

	//SOLID, NO BACK CULL
	{
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastStateMap.emplace(RastStates::noBackCull, std::make_unique<RastState>(RastStates::noBackCull, rastDesc));
	}

	//FRONT CULL
	{
		rastDesc.CullMode = D3D11_CULL_FRONT;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastStateMap.emplace(RastStates::frontCull, std::make_unique<RastState>(RastStates::frontCull, rastDesc));
	}

	//SHADOWS
	{
		rastDesc.CullMode = D3D11_CULL_BACK;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.DepthBias = 25000;
		rastDesc.DepthBiasClamp = 0.0f;
		rastDesc.SlopeScaledDepthBias = 1.0f;
		rastStateMap.emplace(RastStates::shadow, std::make_unique<RastState>(RastStates::shadow, rastDesc));
	}
}

void Renderer::CreateBlendStates()
{
	//DEFAULT NULL BLEND STATE
	{
		blendStateMap.emplace(BlendStates::Default, std::make_unique<NullBlendState>(BlendStates::Default));
	}

	//TRANSPARENT BLEND STATE
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

		blendStateMap.emplace(BlendStates::Transparent, std::make_unique<BlendState>(BlendStates::Transparent, alphaToCoverageDesc));
	}
}

void Renderer::CreateConstantBuffers()
{
	constexpr int cbMatrixRegister = 0;
	constexpr int cbMaterialRegister = 1;
	constexpr int cbSkinningRegister = 2;
	constexpr int cbLightsRegister = 3;
	constexpr int cbTimeRegister = 4;
	constexpr int cbMeshDataRegister = 5;
	constexpr int cbCameraDataRegister = 6;
	constexpr int cbLightProbeDataRegister = 7;

	shaderMatrices.Create();

	cbMatrices.Create(&shaderMatrices, cbMatrixRegister);

	MaterialShaderData materialShaderData = {};
	cbMaterial.Create(&materialShaderData, cbMaterialRegister);

	cbLights.Create(&shaderLights, cbLightsRegister);

	ShaderTimeData timeData = {};
	cbTime.Create(&timeData, cbTimeRegister);

	ShaderMeshData meshData = {};
	cbMeshData.Create(&meshData, cbMeshDataRegister);

	ShaderSkinningData skinningData = {};
	cbSkinningData.Create(&skinningData, cbSkinningRegister);

	ShaderPostProcessData postProcessData = {};
	cbPostProcess.Create(&postProcessData, 0);

	ShaderCameraData cameraData;
	cbCameraData.Create(&cameraData, cbCameraDataRegister);

	ShaderLightProbeData lightProbeData;
	cbLightProbeData.Create(&lightProbeData, cbLightProbeDataRegister);
}

void Renderer::MapBuffer(ID3D11Resource* resource, const void* src, size_t size)
{
	D3D11_MAPPED_SUBRESOURCE mapped = {};
	HR(device.GetContext()->Map(resource, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
	std::memcpy(mapped.pData, src, size);
	device.GetContext()->Unmap(resource, 0);
}

void Renderer::SetNullRTV()
{
	ID3D11RenderTargetView* nullRTV = nullptr;
	device.GetContext()->OMSetRenderTargets(1, &nullRTV, nullptr);
}

void Renderer::SetGeneralShaderResourcesToNull()
{
	//Set to null to remove warnings
	ID3D11ShaderResourceView* nullSRV = nullptr;
	device.GetContext()->PSSetShaderResources(shadowMapTextureRegister, 1, &nullSRV);
	device.GetContext()->PSSetShaderResources(reflectionTextureRegister, 1, &nullSRV);
}

void Renderer::SetShadowData()
{
	if (DirectionalLightComponent::system.GetNumComponents() > 0)
	{
		auto dLight = DirectionalLightComponent::system.GetFirstComponent();
		if (!dLight->IsActive())
		{
			shaderLights.shadowsEnabled = false;
		}
		else
		{
			shaderLights.shadowsEnabled = dLight->IsShadowsEnabled();
		}

		shaderMatrices.lightMVP = shadowMap.DirectionalLightViewProjectionTextureMatrix(dLight);
		shaderMatrices.lightViewProj =
			shadowMap.GetLightViewMatrix(dLight) * shadowMap.GetDirectionalLightOrthoMatrix();
	}
	else if (SpotLightComponent::system.GetNumComponents() > 0)
	{
		auto spotLight = SpotLightComponent::system.GetFirstComponent();
		if (!spotLight->IsActive())
		{
			shaderLights.shadowsEnabled = false;
		}
		else
		{
			shaderLights.shadowsEnabled = spotLight->IsShadowsEnabled();
		}

		shaderMatrices.lightMVP = shadowMap.SpotLightViewProjectionTextureMatrix(spotLight);
		shaderMatrices.lightViewProj =
			shadowMap.GetLightViewMatrix(spotLight) * shadowMap.GetSpotLightPerspectiveMatrix(spotLight);
	}
	else
	{
		shaderLights.shadowsEnabled = false;
	}
}

void Renderer::SetLightResources()
{
	cbLights.SetVSAndPS();
}

void Renderer::DrawMesh(MeshComponent* mesh)
{
	device.GetContext()->Draw(static_cast<UINT>(mesh->meshDataProxy.vertices.size()), 0);
}

void Renderer::DrawMeshInstanced(InstanceMeshComponent* mesh)
{
	device.GetContext()->DrawInstanced(static_cast<UINT>(mesh->meshDataProxy.vertices.size()), mesh->GetInstanceCount(), 0, 0);
}

void Renderer::DrawBoundingBox(MeshComponent* mesh, MeshComponent* boundsMesh)
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
	cbMatrices.Map(&shaderMatrices);
	cbMatrices.SetVS();

	DrawMesh(boundsMesh);
}

void Renderer::RenderDebugLines()
{
	MaterialShaderData materialShaderData{};
	materialShaderData.ambient = XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f);
	cbMaterial.Map(&materialShaderData);
	cbMaterial.SetPS();
	SetShaders("SolidColour");

	D3D11_MAPPED_SUBRESOURCE mappedResource{};
	HR(device.GetContext()->Map(debugLinesBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, debugLines.data(), debugLinesBufferSize);
	device.GetContext()->Unmap(debugLinesBuffer.Get(), 0);

	device.GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	device.GetContext()->IASetVertexBuffers(0, 1, debugLinesBuffer.GetAddressOf(), &stride, &offset);

	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();
	cbMatrices.Map(&shaderMatrices);
	cbMatrices.SetVS();

	device.GetContext()->Draw(static_cast<UINT>(debugLines.size()), 0);
}

void Renderer::CheckSupportedFeatures()
{
	//Threading check
	D3D11_FEATURE_DATA_THREADING threadFeature = {};
	HR(device.Get()->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threadFeature, sizeof(threadFeature)));

	//Swapchain's SwapEffect needs to be DXGI_SWAP_EFFECT_DISCARD for MSAA to work.
	const int sampleCount = 1;

	//MSAA check and set
	UINT msaaQualityLevel;
	HR(device.Get()->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &msaaQualityLevel));
	assert(msaaQualityLevel > 0);

	//Quality has to be one less than what CheckMultisampleQualityLevels() spits out for some reason
	sampleDesc.Quality = msaaQualityLevel - 1;
	sampleDesc.Count = sampleCount;
}

void Renderer::RenderMeshForShadowPass(MeshComponent* mesh)
{
	if (!mesh->castsShadow || !mesh->IsVisible() || !mesh->IsActive())
	{
		return;
	}

	Material& mat = mesh->GetMaterial();

	SetRenderPipelineStatesForShadows(mesh);

	//Set matrices
	shaderMatrices.model = mesh->GetWorldMatrix();
	shaderMatrices.MakeModelViewProjectionMatrix();
	shaderMatrices.MakeTextureMatrix(mat);

	cbMatrices.Map(&shaderMatrices);
	cbMatrices.SetVS();

	//Set textures
	device.GetContext()->PSSetSamplers(0, 1, mat.GetSampler().GetDataAddress());
	SetShaderResourceFromMaterial(mat);

	//Draw
	device.GetContext()->Draw(static_cast<UINT>(mesh->meshDataProxy.vertices.size()), 0);
}

void Renderer::RenderInstanceMeshForShadowPass(InstanceMeshComponent& instanceMesh)
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

		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		//Set textures
		device.GetContext()->PSSetSamplers(0, 1, mat.GetSampler().GetDataAddress());
		SetShaderResourceFromMaterial(mat);

		//Draw
		device.GetContext()->Draw(static_cast<UINT>(vertexCount), 0);
	}
}

void Renderer::RenderShadowPass()
{
	Profile::Start();

	if (!shaderLights.shadowsEnabled)
	{
		return;
	}

	device.GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shadowMap.BindDsvAndSetNullRenderTarget(device.GetContext());

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

		if (!mesh->castsShadow || !mesh->IsVisible() || !mesh->IsActive())
		{
			return;
		}

		device.GetContext()->RSSetState(rastStateMap.find(RastStates::shadow)->second->GetData());

		device.GetContext()->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetDataAddress(), &stride, &offset);

		ShaderItem* shader = ShaderSystem::FindShaderItem("ShadowAnimation");

		device.GetContext()->VSSetShader(shader->GetVertexShader(), nullptr, 0);
		device.GetContext()->IASetInputLayout(shader->GetInputLayout());
		device.GetContext()->PSSetShader(shader->GetPixelShader(), nullptr, 0);

		//Set matrices
		shaderMatrices.model = mesh->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		shaderMatrices.MakeTextureMatrix(mesh->GetMaterial());

		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		//Set skinning data
		//The constant buffer set here is working off of the data inputted on animating skeletons
		ShaderSkinningData skinningData = skeletalMesh->shaderSkinningData;
		cbSkinningData.Map(&skinningData);
		cbSkinningData.SetVS();

		//Set textures
		Material& mat = mesh->GetMaterial();
		device.GetContext()->PSSetSamplers(0, 1, mat.GetSampler().GetDataAddress());
		SetShaderResourceFromMaterial(mat);

		//Draw
		device.GetContext()->Draw(static_cast<UINT>(mesh->meshDataProxy.vertices.size()), 0);
	}

	SetNullRTV();

	Profile::End();
}

void Renderer::RenderSetup()
{
	device.GetContext()->RSSetViewports(1, &viewport);

	const float clearColour[4] = { 0.f, 0.f, 0.f, 1.f };
	const UINT frameIndex = swapchain.GetCurrentBackBufferIndex();

	device.GetContext()->ClearRenderTargetView(&rtvs[frameIndex].GetRTV(), clearColour);

	depthStencil.ClearView();
	device.GetContext()->OMSetRenderTargets(1, rtvs[frameIndex].GetRTVAddress(), depthStencil.GetView());

	device.GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::RenderPostProcessSetup()
{
	device.GetContext()->RSSetViewports(1, &viewport);

	constexpr float clearColour[4] = { 0.f, 0.f, 0.f, 1.f };
	device.GetContext()->ClearRenderTargetView(&postProcessRenderTarget.GetRTV(), clearColour);

	depthStencil.ClearView();

	device.GetContext()->OMSetRenderTargets(1, postProcessRenderTarget.GetRTVAddress(), depthStencil.GetView());

	device.GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::SetShadowResources()
{
	device.GetContext()->PSSetShaderResources(shadowMapTextureRegister, 1, shadowMap.GetDepthMapSRVAddress());
	device.GetContext()->PSSetSamplers(1, 1, shadowMap.GetSamplerAddress());
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

	cbTime.Map(&timeData);
	cbTime.SetVSAndPS();

	if (captureMeshIconOnCurrentFrame)
	{
		RenderMeshToCaptureMeshIcon();
		return;
	}

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
	SetCameraConstantBufferData();

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
	RenderAudioComponents();
	RenderCameraMeshes();
	RenderCharacterControllers();
	//RenderDebugLines();
	RenderPostProcess();

	ClearBounds();

	Profile::End();
}

void Renderer::SetMatricesFromMesh(MeshComponent* mesh)
{
	shaderMatrices.model = mesh->GetWorldMatrix();
	shaderMatrices.MakeModelViewProjectionMatrix();
	shaderMatrices.MakeTextureMatrix(mesh->GetMaterial());

	XMMATRIX A = mesh->GetWorldMatrix();
	A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR det = XMMatrixDeterminant(A);
	shaderMatrices.invTranModel = XMMatrixTranspose(XMMatrixInverse(&det, A));

	cbMatrices.Map(&shaderMatrices);
	cbMatrices.SetVS();
}

void Renderer::SetShaderMeshData(MeshComponent* mesh)
{
	ShaderMeshData meshData = {};
	meshData.position = mesh->GetWorldPosition();
	cbMeshData.Map(&meshData);
	cbMeshData.SetVSAndPS();
}

void Renderer::SetLightProbeData(MeshComponent* mesh)
{
	ShaderLightProbeData lightProbeData;

	if (!DiffuseProbeMap::system.GetActors().empty())
	{
		device.GetContext()->PSSetShaderResources(environmentMapTextureRegister, 1, lightProbeSRV.GetAddressOf());

		const auto lightProbeMap = DiffuseProbeMap::system.GetFirstActor();
		LightProbeInstanceData probeData;
		if (mesh->IsRenderStatic())
		{
			probeData = lightProbeMap->GetProbeByIndex(mesh->cachedLightProbeMapIndex);
		}
		else
		{
			probeData = lightProbeMap->FindClosestProbe(mesh->GetWorldPositionV());
		}

		memcpy(lightProbeData.SH, probeData.SH, sizeof(XMFLOAT4) * 9);

		lightProbeData.isDiffuseProbeMapActive = DiffuseProbeMap::system.GetOnlyActor()->IsActive();
	}
	else
	{
		lightProbeData.isDiffuseProbeMapActive = false;
	}

	cbLightProbeData.Map(&lightProbeData);
	cbLightProbeData.SetVSAndPS();
}

void Renderer::RenderMeshComponents()
{
	Profile::Start();

	//Shader Resources
	SetShadowResources();
	SetLightResources();

	auto meshes = RenderUtils::SortMeshesByDistanceToCamera<MeshComponent>();
	for (auto mesh : meshes)
	{
		if (!mesh->IsVisible() || !mesh->IsActive())
		{
			continue;
		}

		SetRenderPipelineStates(mesh);

		//Constant buffer data
		SetMatricesFromMesh(mesh);
		SetShaderMeshData(mesh);
		SetLightProbeData(mesh);

		DrawMesh(mesh);
	}

	for (auto& mesh : SliceableMeshComponent::system.GetComponents())
	{
		if (!mesh->IsVisible() || !mesh->IsActive())
		{
			continue;
		}

		SetRenderPipelineStates(mesh.get());

		//Constant buffer data
		SetMatricesFromMesh(mesh.get());
		SetShaderMeshData(mesh.get());
		SetLightProbeData(mesh.get());

		DrawMesh(mesh.get());
	}

	SetGeneralShaderResourcesToNull();

	Profile::End();
}

void Renderer::RenderDestructibleMeshes()
{
	Profile::Start();

	SetShadowResources();
	SetLightResources();

	for (auto& mesh : DestructibleMeshComponent::system.GetComponents())
	{
		for (auto meshCell : mesh->meshCells)
		{
			if (!meshCell->IsVisible() || !meshCell->IsActive())
			{
				continue;
			}

			SetRenderPipelineStates(meshCell);
			SetMatricesFromMesh(meshCell);
			SetShaderMeshData(meshCell);
			SetLightProbeData(meshCell);

			DrawMesh(meshCell);
		}
	}

	SetGeneralShaderResourcesToNull();

	Profile::End();
}

void Renderer::RenderLightProbeViews()
{
	const auto startTime = Profile::QuickStart();

	const auto diffuseProbeMap = DiffuseProbeMap::system.GetFirstActor();
	if (diffuseProbeMap == nullptr)
	{
		Log("No diffuse probe map in level to render probes from.");
		return;
	}

	Log("Diffuse light probe map bake started...");

	const int previousViewportWidth = viewport.Width;
	const int previousViewportHeight = viewport.Height;
	ResizeSwapchain(lightProbeTextureWidth, lightProbeTextureHeight);

	constexpr int textureCubeFaces = 6;

	//Directions match with D3D11_TEXTURECUBE_FACE
	const XMVECTOR faces[textureCubeFaces] =
	{
		XMVectorSet(1.f, 0.f, 0.f, 0.f), //+X
		XMVectorSet(-1.f, 0.f, 0.f, 0.f), //-X
		XMVectorSet(0.f, 1.f, 0.f, 0.f), //+Y
		XMVectorSet(0.f, -1.f, 0.f, 0.f), //-Y
		XMVectorSet(0.f, 0.f, 1.f, 0.f), //+Z
		XMVectorSet(0.f, 0.f, -1.f, 0.f), //-Z
	};

	const XMVECTOR resultantUpVectors[textureCubeFaces] =
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

	for (auto& probeData : diffuseProbeMap->GetLightProbeData())
	{
		const XMMATRIX probeMatrix = probeData.modelMatrix;

		for (int i = 0; i < textureCubeFaces; i++)
		{
			device.GetContext()->RSSetViewports(1, &viewport);
			constexpr float clearColour[4] = { 0.f, 0.f, 0.f, 0.f };
			device.GetContext()->ClearRenderTargetView(lightProbeRTVs[i].Get(), clearColour);

			device.GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			device.GetContext()->OMSetRenderTargets(1, lightProbeRTVs[i].GetAddressOf(), nullptr);

			device.GetContext()->RSSetState(rastStateMap.find(RastStates::solid)->second->GetData());

			SetLightsConstantBufferData();

			//Set lights buffer
			cbLights.SetPS();

			//Set shadow resources (not now for lightprobes)
			//device.GetContext()->PSSetShaderResources(shadowMapTextureResgiter, 1, shadowMap->depthMapSRV.GetAddressOf());
			//device.GetContext()->PSSetSamplers(1, 1, shadowMap->sampler.GetaddressOf());

			ShaderItem* lightProbeShader = ShaderSystem::FindShaderItem("Default");

			for (auto& mesh : MeshComponent::system.GetComponents())
			{
				if (!mesh->IsVisible() || !mesh->IsActive())
				{
					continue;
				}

				Material& material = mesh->GetMaterial();

				const FLOAT blendState[4] = { 0.f };
				device.GetContext()->OMSetBlendState(mesh->GetBlendState().GetData(), blendState, 0xFFFFFFFF);
				device.GetContext()->RSSetState(mesh->GetRastState().GetData());

				device.GetContext()->VSSetShader(lightProbeShader->GetVertexShader(), nullptr, 0);
				device.GetContext()->IASetInputLayout(lightProbeShader->GetInputLayout());

				device.GetContext()->PSSetShader(lightProbeShader->GetPixelShader(), nullptr, 0);

				device.GetContext()->PSSetSamplers(0, 1, material.GetSampler().GetDataAddress());

				SetShaderResourceFromMaterial(material);

				device.GetContext()->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetDataAddress(), &stride, &offset);

				cbMaterial.Map(&material.GetMaterialShaderData());
				cbMaterial.SetPS();

				//Set matrices
				const auto probePos = XMLoadFloat3(&probeData.position);
				shaderMatrices.view = XMMatrixLookAtLH(probePos,
					probePos + faces[i],
					resultantUpVectors[i]);
				shaderMatrices.model = mesh->GetWorldMatrix();
				shaderMatrices.MakeModelViewProjectionMatrix();
				shaderMatrices.MakeTextureMatrix(mesh->GetMaterial());

				cbMatrices.Map(&shaderMatrices);
				cbMatrices.SetVS();

				//Set mesh data to shader
				ShaderMeshData meshData;
				XMStoreFloat3(&meshData.position, mesh->GetWorldPositionV());
				cbMeshData.Map(&meshData);
				cbMeshData.SetVSAndPS();

				//Draw
				device.GetContext()->Draw(static_cast<UINT>(mesh->meshDataProxy.vertices.size()), 0);
			}

			//Remove lightprobe RTV
			SetNullRTV();
		}

		//Remember that there are 9 coefficients with 3rd order SH per channel
		float SH_R[9] = {}, SH_G[9] = {}, SH_B[9] = {};
		HR(DirectX::SHProjectCubeMap(device.GetContext(), 3, lightProbeTexture.Get(), SH_R, SH_G, SH_B));

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
	MapBuffer(diffuseProbeMap->GetStructuredBuffer(), diffuseProbeMap->GetLightProbeData().data(),
		sizeof(LightProbeInstanceData) * diffuseProbeMap->GetLightProbeData().size());

	ResizeSwapchain(previousViewportWidth, previousViewportHeight);

	//Set main RTV and DSV back on
	RenderSetup();

	diffuseProbeMap->WriteProbeDataToFile();

	const auto endTime = Profile::QuickEnd(startTime);
	Log("Light probe bake took [%f] seconds.", endTime);
}

void Renderer::RenderInstanceMeshComponents()
{
	Profile::Start();

	//Set matrices (Instance mesh model matrices placed in a structured buffer)
	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();

	cbMatrices.Map(&shaderMatrices);
	cbMatrices.SetVS();

	//Note: Sorting instance meshes for transparency won't work, mesh instances could be anywhere in the world.
	//Correct transparency isn't supported right now, and only gets by by calling this function before
	//mesh components are rendered.
	for (auto& instanceMesh : InstanceMeshComponent::system.GetComponents())
	{
		if (!instanceMesh->IsVisible() || !instanceMesh->IsActive())
		{
			continue;
		}

		SetRenderPipelineStates(instanceMesh.get());

		//Update texture matrix
		shaderMatrices.MakeTextureMatrix(instanceMesh->GetMaterial());
		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		//Update instance data and set SRV
		MapBuffer(instanceMesh->structuredBuffer.Get(), instanceMesh->GetInstanceData().data(), sizeof(InstanceData) * instanceMesh->GetInstanceData().size());
		device.GetContext()->VSSetShaderResources(instanceSRVRegister, 1, instanceMesh->srv.GetAddressOf());
		device.GetContext()->PSSetShaderResources(instanceSRVRegister, 1, instanceMesh->srv.GetAddressOf());

		//Set lights buffer
		cbLights.SetPS();

		DrawMeshInstanced(instanceMesh.get());
	}

	Profile::End();
}

//Remember that when rendering bounds, the Extents needs to be doubled.
void Renderer::RenderBounds()
{
	auto debugBox = DebugMeshManager::GetDebugMesh("DebugBox");
	MaterialShaderData materialShaderData;

	if (Renderer::drawBoundingBoxes)
	{
		SetRastStateByName(RastStates::wireframe);
		SetShaders("SolidColour");

		SetVertexBuffer(debugBox->GetVertexBuffer());

		//Set debug wireframe material colour
		materialShaderData.ambient = XMFLOAT4(0.75f, 0.75f, 0.75f, 1.0f);
		cbMaterial.Map(&materialShaderData);
		cbMaterial.SetPS();

		for (auto& mesh : MeshComponent::system.GetComponents())
		{
			if (mesh->IsActive())
			{
				DrawBoundingBox(mesh.get(), debugBox);
			}
		}

		for (auto& skeletalMesh : SkeletalMeshComponent::system.GetComponents())
		{
			if (skeletalMesh->IsActive())
			{
				DrawBoundingBox(skeletalMesh.get(), debugBox);
			}
		}

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
			cbMatrices.Map(&shaderMatrices);
			cbMatrices.SetVS();

			DrawMesh(debugBox);
		}
	}

	//DRAW TRIGGER BOUNDS
	if (Renderer::drawTriggers)
	{
		SetRastStateByName(RastStates::wireframe);
		SetShaders("SolidColour");

		SetVertexBuffer(debugBox->GetVertexBuffer());

		for (auto& boxTrigger : BoxTriggerComponent::system.GetComponents())
		{
			if (!boxTrigger->IsVisible() || !boxTrigger->IsActive())
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
			cbMatrices.Map(&shaderMatrices);
			cbMatrices.SetVS();

			//Set trigger wireframe material colour
			materialShaderData.ambient = boxTrigger->renderWireframeColour;
			cbMaterial.Map(&materialShaderData);
			cbMaterial.SetPS();

			DrawMesh(debugBox);
		}
	}
}

void Renderer::RenderPhysicsMeshes()
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
		device.GetContext()->RSSetState(rastStateMap.find(RastStates::solid)->second->GetData());

		auto shaderItem = ShaderSystem::FindShaderItem("SolidColour");
		device.GetContext()->VSSetShader(shaderItem->GetVertexShader(), nullptr, 0);
		device.GetContext()->IASetInputLayout(shaderItem->GetInputLayout());
		device.GetContext()->PSSetShader(shaderItem->GetPixelShader(), nullptr, 0);

		SetVertexBuffer(mesh->GetVertexBuffer());

		auto originalMesh = MeshComponent::system.GetComponentByUID(mesh->GetUID());
		assert(originalMesh);
		shaderMatrices.model = originalMesh->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		MaterialShaderData materialShaderData;
		materialShaderData.ambient = XMFLOAT4(0.1f, 0.9f, 0.1f, 1.f);
		cbMaterial.Map(&materialShaderData);
		cbMaterial.SetPS();

		DrawMesh(mesh.get());
	}
}

void Renderer::CreateLightProbeBuffers()
{
	//Texture

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

	HR(device.Get()->CreateTexture2D(&texDesc, 0, lightProbeTexture.GetAddressOf()));
	assert(lightProbeTexture);

	//SRV

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = 1;
	HR(device.Get()->CreateShaderResourceView(lightProbeTexture.Get(), &srvDesc, lightProbeSRV.GetAddressOf()));

	//RTVs
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtvDesc.Texture2DArray.ArraySize = 1;

	for (int i = 0; i < 6; i++)
	{
		rtvDesc.Texture2DArray.FirstArraySlice = i;
		HR(device.Get()->CreateRenderTargetView(lightProbeTexture.Get(), &rtvDesc, lightProbeRTVs[i].GetAddressOf()));
	}
}

void Renderer::RenderCharacterControllers()
{
	if (Core::gameplayOn) return;

	auto debugCapsule = DebugMeshManager::GetDebugMesh("DebugCapsule");
	MaterialShaderData materialShaderData;

	SetRastStateByName(RastStates::wireframe);
	SetShaders("SolidColour");
	SetVertexBuffer(debugCapsule->GetVertexBuffer());

	materialShaderData.ambient = XMFLOAT4(1.0f, 0.8f, 0.7f, 1.0f); //Pink
	cbMaterial.Map(&materialShaderData);
	cbMaterial.SetPS();

	for (auto& characterController : CharacterControllerComponent::system.GetComponents())
	{
		shaderMatrices.model = characterController->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		DrawMesh(debugCapsule);
	}
}

void Renderer::RenderCameraMeshes()
{
	if (Core::gameplayOn) return;

	auto debugCamera = DebugMeshManager::GetDebugMesh("DebugCamera");
	MaterialShaderData materialShaderData;

	SetRastStateByName(RastStates::wireframe);
	SetShaders("SolidColour");
	SetVertexBuffer(debugCamera->GetVertexBuffer());

	materialShaderData.ambient = XMFLOAT4(1.0f, 0.0f, 0.f, 1.0f); //Make cameras red
	cbMaterial.Map(&materialShaderData);
	cbMaterial.SetPS();

	for (auto& camera : CameraComponent::system.GetComponents())
	{
		shaderMatrices.model = camera->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		DrawMesh(debugCamera);
	}
}

void Renderer::RenderLightMeshes()
{
	if (Core::gameplayOn) return;

	auto debugSphere = DebugMeshManager::GetDebugMesh("DebugSphere");
	auto debugIcoSphere = DebugMeshManager::GetDebugMesh("DebugIcoSphere");
	auto debugCone = DebugMeshManager::GetDebugMesh("DebugCone");

	SetRastStateByName(RastStates::wireframe);
	SetShaders("SolidColour");

	//Set debug sphere wireframe material colour
	MaterialShaderData materialShaderData = {};
	materialShaderData.ambient = XMFLOAT4(1.f, 1.f, 0.f, 1.0f);
	cbMaterial.Map(&materialShaderData);
	cbMaterial.SetPS();

	//DIRECTIONAL LIGHTS
	SetVertexBuffer(debugSphere->GetVertexBuffer());

	for (auto& directionalLight : DirectionalLightComponent::system.GetComponents())
	{
		if (!directionalLight->IsActive())
		{
			continue;
		}

		shaderMatrices.model = directionalLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		DrawMesh(debugSphere);
	}

	//POINT LIGHTS
	SetVertexBuffer(debugIcoSphere->GetVertexBuffer());

	for (auto& pointLight : PointLightComponent::system.GetComponents())
	{
		if (!pointLight->IsActive())
		{
			continue;
		}

		shaderMatrices.model = pointLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		DrawMesh(debugIcoSphere);
	}

	//SPOT LIGHTS
	SetVertexBuffer(debugCone->GetVertexBuffer());

	for (auto& spotLight : SpotLightComponent::system.GetComponents())
	{
		if (!spotLight->IsActive())
		{
			continue;
		}

		shaderMatrices.model = spotLight->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		DrawMesh(debugCone);
	}
}

void Renderer::RenderAudioComponents()
{
	SetRastStateByName(RastStates::wireframe);
	SetShaders("SolidColour");

	//Set debug sphere wireframe material colour
	MaterialShaderData materialShaderData;
	materialShaderData.ambient = XMFLOAT4(0.3f, 0.4f, 0.55f, 1.0f);
	cbMaterial.Map(&materialShaderData);
	cbMaterial.SetPS();

	const auto debugIcoSphere = DebugMeshManager::GetDebugMesh("DebugIcoSphere");
	SetVertexBuffer(debugIcoSphere->GetVertexBuffer());

	for (const auto& audioComponent : AudioComponent::system.GetComponents())
	{
		//Don't render the debug mesh for attenuation visualisation if spatial audio isn't enabled.
		if (!audioComponent->IsSpatialAudioEnabled())
		{
			return;
		}

		shaderMatrices.model = XMMatrixIdentity();

		//Only want the world position of the AudioComponent.
		shaderMatrices.model.r[3] = audioComponent->GetWorldPositionV();

		//Up scale by volume "radius"
		const float volumeRadius = audioComponent->GetVolumeAttenuation();
		shaderMatrices.model.r[0].m128_f32[0] *= volumeRadius;
		shaderMatrices.model.r[1].m128_f32[1] *= volumeRadius;
		shaderMatrices.model.r[2].m128_f32[2] *= volumeRadius;

		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		DrawMesh(debugIcoSphere);
	}
}

void Renderer::RenderPolyboards()
{
	Profile::Start();

	shaderMatrices.texMatrix = XMMatrixIdentity();
	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();

	cbMatrices.Map(&shaderMatrices);
	cbMatrices.SetVS();

	SetBlendStateByName(BlendStates::Transparent);
	SetRastStateByName(RastStates::noBackCull);
	SetShaders("DefaultClip");

	for (auto polyboard : World::Get().GetAllComponentsOfType<Polyboard>())
	{
		if (!polyboard->IsActive() || !polyboard->IsVisible()) continue;

		polyboard->CalcVertices();

		device.GetContext()->PSSetSamplers(0, 1, Renderer::GetDefaultSampler().GetDataAddress());

		SetShaderResourcePixel(0, polyboard->GetTextureFilename());

		//VERTEX MAP
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			HR(device.GetContext()->Map(polyboard->GetVertexBuffer().GetData(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
			memcpy(mappedResource.pData, polyboard->GetVertices().data(), sizeof(Vertex) * polyboard->GetVertices().size());
			device.GetContext()->Unmap(polyboard->GetVertexBuffer().GetData(), 0);
		}

		//INDEX MAP
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource = {};
			HR(device.GetContext()->Map(polyboard->GetIndexBuffer().GetData(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
			memcpy(mappedResource.pData, polyboard->GetIndices().data(), sizeof(MeshData::indexDataType) * polyboard->GetIndices().size());
			device.GetContext()->Unmap(polyboard->GetIndexBuffer().GetData(), 0);
		}

		SetVertexBuffer(polyboard->GetVertexBuffer());
		SetIndexBuffer(polyboard->GetIndexBuffer());

		device.GetContext()->DrawIndexed(static_cast<UINT>(polyboard->GetIndices().size()), 0, 0);
	}

	Profile::End();
}

void Renderer::RenderSpriteSheets()
{
	Profile::Start();

	for (auto& spriteSheet : SpriteSheet::system.GetComponents())
	{
		if (!spriteSheet->IsActive() || !spriteSheet->IsVisible())
		{
			continue;
		}

		SetRastStateByName(RastStates::noBackCull);
		SetShaders("DefaultClip");

		SetSampler(0, Renderer::GetDefaultSampler());
		SetShaderResourcePixel(0, spriteSheet->GetTextureFilename());

		spriteSheet->UpdateSprite();

		SpriteSystem::BuildSpriteQuadForSpriteSheetRendering(spriteSheet->GetSprite());
		SpriteSystem::UpdateAndSetSpriteBuffers();

		if (!spriteSheet->IsUsingOwnRotation())
		{
			const XMVECTOR lookAtRotation = VMath::LookAtRotation(Camera::GetActiveCamera().GetWorldPositionV(),
				spriteSheet->GetWorldPositionV());
			spriteSheet->SetWorldRotation(lookAtRotation);
		}

		shaderMatrices.model = spriteSheet->GetWorldMatrix();
		shaderMatrices.MakeModelViewProjectionMatrix();

		MaterialShaderData defaultShaderMaterial = {};
		cbMaterial.Map(&defaultShaderMaterial);
		cbMaterial.SetPS();

		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		device.GetContext()->DrawIndexed(6, 0, 0);
	}

	Profile::End();
}

void Renderer::AnimateAndRenderSkeletalMeshes()
{
	Profile::Start();

	SetShadowResources();
	SetLightResources();

	for (auto skeletalMesh : RenderUtils::SortMeshesByDistanceToCamera<SkeletalMeshComponent>())
	{
		if (!skeletalMesh->IsVisible() || !skeletalMesh->IsActive())
		{
			continue;
		}

		SetRenderPipelineStates(skeletalMesh);

		//Constant buffer data
		SetMatricesFromMesh(skeletalMesh);
		SetShaderMeshData(skeletalMesh);

		if (!skeletalMesh->GetCurrentAnimationName().empty())
		{
			if (skeletalMesh->HasJoints())
			{
				//Set shader for skeletal animation
				ShaderItem* shaderItem = ShaderSystem::FindShaderItem("Animation");
				device.GetContext()->VSSetShader(shaderItem->GetVertexShader(), nullptr, 0);
				device.GetContext()->IASetInputLayout(shaderItem->GetInputLayout());
				device.GetContext()->PSSetShader(shaderItem->GetPixelShader(), nullptr, 0);

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
				cbSkinningData.Map(&skinningData);
				cbSkinningData.SetVS();
			}
		}

		DrawMesh(skeletalMesh);
	}

	SetGeneralShaderResourcesToNull();

	Profile::End();
}

//Make sure this is called after SkeletalMesh animations else the skinning data won't be set 
//and the SocketMeshes will have nothing correct to index into.
void Renderer::RenderSocketMeshComponents()
{
	//Map skinning data to socketmeshcomponents
	for (auto& socketMesh : SocketMeshComponent::system.GetComponents())
	{
		if (!socketMesh->IsVisible()) { continue; }

		SetRenderPipelineStates(socketMesh.get());

		socketMesh->SetTransformFromLinkedSkeletonJoint();

		SetMatricesFromMesh(socketMesh.get());
		SetShaderMeshData(socketMesh.get());
		SetLightProbeData(socketMesh.get());

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
			device.GetContext()->RSSetState(rastStateMap.find(RastStates::wireframe)->second->GetData());
		}
		else
		{
			device.GetContext()->RSSetState(rastStateMap.find(RastStates::noBackCull)->second->GetData());
		}

		SetBlendStateByName(BlendStates::Transparent);

		SetShaders(emitter->GetMaterial().GetShaderItem().GetName());

		device.GetContext()->PSSetSamplers(0, 1, Renderer::GetDefaultSampler().GetDataAddress());

		//Set texture from emitter for every particle
		SetShaderResourcePixel(0, emitter->GetMaterial().defaultTextureData.filename);

		SpriteSystem::UpdateAndSetSpriteBuffers();

		for (auto& particle : emitter->particles)
		{
			//Add rotation to particle (keep in mind that rotate speed needs to match angle's +/- value)
			auto particleRotation =
				VMath::LookAtRotation(Camera::GetActiveCamera().GetWorldPositionV(), XMLoadFloat3(&particle.transform.position));
			XMStoreFloat4(&particle.transform.rotation, particleRotation);

			shaderMatrices.model = particle.transform.GetAffine();
			shaderMatrices.MakeModelViewProjectionMatrix();

			cbMatrices.Map(&shaderMatrices);
			cbMatrices.SetVS();

			MaterialShaderData materialShaderData;
			materialShaderData = emitter->GetMaterial().GetMaterialShaderData();
			materialShaderData.ambient.w = particle.alpha;
			cbMaterial.Map(&materialShaderData);
			cbMaterial.SetPS();

			//Note: Apparently using DrawInstanced() degrades performance when the vertex count it really low
			//and DrawIndexed is actually faster.
			device.GetContext()->DrawIndexed(6, 0, 0);
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
		SetShaders("UI");
		SetSampler(0, Renderer::GetDefaultSampler());
		SetShaderResourcePixel(0, sprite.textureFilename);

		MaterialShaderData materialShaderData;
		materialShaderData.ambient = sprite.colour;
		cbMaterial.Map(&materialShaderData);
		cbMaterial.SetPS();

		SpriteSystem::BuildSpriteQuadForViewportRendering(sprite);
		SpriteSystem::UpdateAndSetSpriteBuffers();

		shaderMatrices.model = XMMatrixIdentity();
		shaderMatrices.MakeModelViewProjectionMatrix();
		cbMatrices.Map(&shaderMatrices);
		cbMatrices.SetVS();

		device.GetContext()->DrawIndexed(6, 0, 0);
	}

	Profile::End();
}

void Renderer::SetLightsConstantBufferData()
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

	if (!DirectionalLightComponent::system.Empty())
	{
		//Code is only working with singular directional lights for now because of shadows, so this sort
		//of setting of global ambient should be fine.
		shaderLights.globalAmbient = DirectionalLightComponent::system.GetFirstComponent()->GetGlobalAmbient();
	}

	shaderLights.numLights = shaderLightsIndex;
	assert(shaderLights.numLights < ShaderLights::MAX_LIGHTS);

	cbLights.Map(&shaderLights);
	cbLights.SetVSAndPS();

	Profile::End();
}

void Renderer::SetCameraConstantBufferData()
{
	ShaderCameraData cameraData;
	XMStoreFloat4(&cameraData.cameraWorldPos, Camera::GetActiveCamera().GetWorldPositionV());
	XMStoreFloat4(&cameraData.cameraForwardVector, Camera::GetActiveCamera().GetForwardVectorV());

	cbCameraData.Map(&cameraData);
	cbCameraData.SetVSAndPS();
}

void Renderer::Present()
{
	if (captureMeshIconOnCurrentFrame)
	{
		debugMenu.debugNotifications.clear();

		Microsoft::WRL::ComPtr<ID3D11Texture2D> meshIconBackBuffer;
		swapchain.GetBackBuffer(meshIconBackBuffer.GetAddressOf());

		const std::wstring imageFile = L"Icons/MeshIcons/" +
			VString::stows(captureMeshIconMeshFilename) + L".jpg";
		HR(SaveWICTextureToFile(device.GetContext(), meshIconBackBuffer.Get(), GUID_ContainerFormatJpeg, imageFile.c_str()));

		Log("Mesh Icon created from [%s] mesh.", captureMeshIconMeshFilename.c_str());

		captureMeshIconOnCurrentFrame = false;
		captureMeshIconMeshFilename.clear();

		return;
	}

	swapchain.Present();
}

void* Renderer::GetSwapchain()
{
	return swapchain.Get();
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
	if (swapchain.Get() == nullptr) return;

	device.GetContext()->OMSetRenderTargets(0, 0, 0);

	// Release all outstanding references to the swap chain's buffers.
	for (int rtvIndex = 0; rtvIndex < Swapchain::SWAPCHAIN_COUNT; rtvIndex++)
	{
		rtvs[rtvIndex].Recycle();
	}

	depthStencil.Reset();

	UISystem::Cleanup();

	swapchain.ResizeBuffers(newWidth, newHeight);

	viewport.Width = newWidth;
	viewport.Height = newHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	device.GetContext()->RSSetViewports(1, &viewport);

	CreateRTVAndDSV();

	postProcessRenderTarget.Recycle();
	postProcessRenderTarget.Create(viewport.Width, viewport.Height, DXGI_FORMAT_R16G16B16A16_FLOAT);

	UISystem::Init((void*)swapchain.Get());

	shaderMatrices.Create();
}

void Renderer::ScreenshotCapture()
{
	if (Input::GetKeyUp(Keys::F8))
	{
		//Clear previous notification so nothing appears in the screenshot.
		debugMenu.debugNotifications.clear();

		Microsoft::WRL::ComPtr<ID3D11Texture2D> screenshotBackBuffer;
		swapchain.GetBackBuffer(screenshotBackBuffer.GetAddressOf());
		assert(screenshotBackBuffer);

		//Use a generated UID so that filenames are unique
		UID imageFileID = GenerateUID();
		std::wstring imageFile = L"Screenshots/" + std::to_wstring(imageFileID) + L".jpg";
		HR(SaveWICTextureToFile(device.GetContext(), screenshotBackBuffer.Get(), GUID_ContainerFormatJpeg, imageFile.c_str()));
		debugMenu.AddNotification(L"Screen shot taken.");
	}
}

void Renderer::MeshIconImageCapture()
{
	Actor* actor = WorldEditor::GetPickedActor();
	std::vector<MeshComponent*> meshComponents = actor->GetComponents<MeshComponent>();

	if (!meshComponents.empty())
	{
		MeshComponent* mesh = meshComponents.front();

		Microsoft::WRL::ComPtr<ID3D11Texture2D> meshIconBackBuffer;
		swapchain.GetBackBuffer(meshIconBackBuffer.GetAddressOf());
		assert(meshIconBackBuffer);

		const std::wstring imageFile = L"Icons/MeshIcons/" +
			VString::stows(mesh->meshComponentData.filename) + L".jpg";
		HR(SaveWICTextureToFile(device.GetContext(), meshIconBackBuffer.Get(), GUID_ContainerFormatJpeg, imageFile.c_str()));
		debugMenu.AddNotification(L"Mesh Icon created.");
	}
}

void Renderer::MapIconImageCapture()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> iconImageBackBuffer;
	swapchain.GetBackBuffer(iconImageBackBuffer.GetAddressOf());
	assert(iconImageBackBuffer);

	const std::wstring imageFile = L"Icons/MapIcons/" + VString::stows(World::Get().worldFilename) + L".jpg";
	HR(SaveWICTextureToFile(device.GetContext(), iconImageBackBuffer.Get(), GUID_ContainerFormatJpeg, imageFile.c_str()));
	debugMenu.AddNotification(L"Map Icon created.");
}

void Renderer::PlayerPhotoCapture(std::wstring outputFilename)
{
	debugMenu.debugNotifications.clear();

	Microsoft::WRL::ComPtr<ID3D11Texture2D> photoCaptureBackBuffer;
	swapchain.GetBackBuffer(photoCaptureBackBuffer.GetAddressOf());
	assert(photoCaptureBackBuffer);

	std::wstring imageFile = L"Textures/" + outputFilename + L".jpg";
	HR(SaveWICTextureToFile(device.GetContext(), photoCaptureBackBuffer.Get(), GUID_ContainerFormatJpeg, imageFile.c_str()));
	Log("Photo taken [%S]", imageFile.c_str());
}

void Renderer::SetRenderPipelineStates(MeshComponent* mesh)
{
	Material& material = mesh->GetMaterial();

	if (Renderer::drawAllAsWireframe)
	{
		device.GetContext()->RSSetState(rastStateMap.find(RastStates::wireframe)->second->GetData());
	}
	else
	{
		device.GetContext()->RSSetState(material.GetRastState().GetData());
	}

	constexpr FLOAT blendState[4] = { 0.f };
	device.GetContext()->OMSetBlendState(material.GetBlendState().GetData(), blendState, 0xFFFFFFFF);

	device.GetContext()->VSSetShader(material.GetVertexShader(), nullptr, 0);
	device.GetContext()->IASetInputLayout(material.GetInputLayout());

	device.GetContext()->PSSetShader(material.GetPixelShader(), nullptr, 0);

	device.GetContext()->PSSetSamplers(0, 1, material.GetSampler().GetDataAddress());
	SetShaderResourceFromMaterial(material);

	SetVertexBuffer(mesh->GetVertexBuffer());

	cbMaterial.Map(&material.GetMaterialShaderData());
	cbMaterial.SetPS();
}

void Renderer::SetRenderPipelineStatesForShadows(MeshComponent* mesh)
{
	device.GetContext()->RSSetState(rastStateMap.find(RastStates::shadow)->second->GetData());

	ShaderItem* shader = ShaderSystem::FindShaderItem("Shadow");

	device.GetContext()->VSSetShader(shader->GetVertexShader(), nullptr, 0);
	device.GetContext()->IASetInputLayout(shader->GetInputLayout());

	device.GetContext()->PSSetShader(shader->GetPixelShader(), nullptr, 0);

	device.GetContext()->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer().GetDataAddress(), &stride, &offset);
}

void Renderer::SetShaders(ShaderItem* shaderItem)
{
	device.GetContext()->VSSetShader(shaderItem->GetVertexShader(), nullptr, 0);
	device.GetContext()->IASetInputLayout(shaderItem->GetInputLayout());
	device.GetContext()->PSSetShader(shaderItem->GetPixelShader(), nullptr, 0);
}

void Renderer::SetShaders(const std::string shaderItemName)
{
	ShaderItem* shaderItem = ShaderSystem::FindShaderItem(shaderItemName);
	device.GetContext()->VSSetShader(shaderItem->GetVertexShader(), nullptr, 0);
	device.GetContext()->IASetInputLayout(shaderItem->GetInputLayout());
	device.GetContext()->PSSetShader(shaderItem->GetPixelShader(), nullptr, 0);
}

void Renderer::SetRastStateByName(std::string rastStateName)
{
	if (Renderer::drawAllAsWireframe)
	{
		device.GetContext()->RSSetState(rastStateMap.find(RastStates::wireframe)->second->GetData());
		return;
	}

	auto& rastState = rastStateMap.find(rastStateName)->second;
	device.GetContext()->RSSetState(rastState->GetData());
}

void Renderer::SetRastState(RastState& rastState)
{
	device.GetContext()->RSSetState(rastState.GetData());
}

void Renderer::SetBlendStateByName(std::string blendStateName)
{
	auto& blendState = blendStateMap.find(blendStateName)->second;
	const float factor[4] = {};
	device.GetContext()->OMSetBlendState(blendState->GetData(), factor, 0xFFFFFFFF);
}

void Renderer::SetBlendState(BlendState& blendState)
{
	const float factor[4] = {};
	device.GetContext()->OMSetBlendState(blendState.GetData(), factor, 0xFFFFFFFF);
}

void Renderer::SetConstantBufferVertexPixel(uint32_t shaderRegister, ID3D11Buffer* constantBuffer)
{
	device.GetContext()->VSSetConstantBuffers(shaderRegister, 1, &constantBuffer);
	device.GetContext()->PSSetConstantBuffers(shaderRegister, 1, &constantBuffer);
}

void Renderer::SetConstantBufferVertex(uint32_t shaderRegister, ID3D11Buffer* constantBuffer)
{
	device.GetContext()->VSSetConstantBuffers(shaderRegister, 1, &constantBuffer);
}

void Renderer::SetConstantBufferPixel(uint32_t shaderRegister, ID3D11Buffer* constantBuffer)
{
	device.GetContext()->PSSetConstantBuffers(shaderRegister, 1, &constantBuffer);
}

void Renderer::SetVertexBuffer(VertexBuffer& vertexBuffer)
{
	device.GetContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetDataAddress(), &stride, &offset);
}

void Renderer::SetIndexBuffer(IndexBuffer& indexBuffer)
{
	const DXGI_FORMAT indexBufferFormat = DXGI_FORMAT_R32_UINT;
	device.GetContext()->IASetIndexBuffer(indexBuffer.GetData(), indexBufferFormat, 0);
}

void Renderer::SetSampler(uint32_t shaderRegister, Sampler& sampler)
{
	device.GetContext()->PSSetSamplers(shaderRegister, 1, sampler.GetDataAddress());
}

void Renderer::SetShaderResourceFromMaterial(Material& material)
{
	//Testing code for normal map SRV set
	/*auto normalMapTexture = textureSystem.FindTexture2D("wall_normal_map.png");
	auto normalMapSRV = normalMapTexture->GetSRV();
	device.GetContext()->PSSetShaderResources(normalMapTexureRegister, 1, normalMapSRV.GetAddressOf());*/

	auto defaultTextureSRV = material.GetDefaultTexture().GetSRV();
	device.GetContext()->PSSetShaderResources(defaultTextureRegister, 1, &defaultTextureSRV);

	auto secondaryTextureSRV = material.GetSecondaryTexture().GetSRV();
	device.GetContext()->PSSetShaderResources(secondaryTextureRegister, 1, &secondaryTextureSRV);
}

void Renderer::SetShaderResourcePixel(uint32_t shaderRegister, std::string textureName)
{
	auto texture = TextureSystem::FindTexture2D(textureName);
	auto textureSRV = texture->GetSRV();
	device.GetContext()->PSSetShaderResources(shaderRegister, 1, &textureSRV);
}

void Renderer::RenderPostProcess()
{
	auto postProcessVolume = PostProcessVolume::system.GetFirstActor();
	if (postProcessVolume == nullptr) return;
	if (!postProcessVolume->IsActive()) return;
	if (!postProcessVolume->IsActiveCameraInsideVolume()) return;

	SetNullRTV();

	ID3D11ShaderResourceView* nullSRV = nullptr;

	device.GetContext()->RSSetState(rastStateMap.find(RastStates::solid)->second->GetData());

	device.GetContext()->IASetInputLayout(nullptr);
	device.GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	device.GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	SetShaders("PostProcess");

	//Set constant buffer data
	ShaderPostProcessData postProcessData = postProcessVolume->GetPostProcessData();
	cbPostProcess.Map(&postProcessData);
	cbPostProcess.SetPS();

	device.GetContext()->PSSetShaderResources(0, 1, postProcessRenderTarget.GetSRVAddress());
	SetSampler(0, Renderer::GetDefaultSampler());

	const UINT frameIndex = swapchain.GetCurrentBackBufferIndex();
	device.GetContext()->OMSetRenderTargets(1, rtvs[frameIndex].GetRTVAddress(), depthStencil.GetView());

	const float clearColour[4] = { 0.f, 0.f, 0.f, 0.f };
	device.GetContext()->ClearRenderTargetView(&rtvs[frameIndex].GetRTV(), clearColour);

	device.GetContext()->Draw(6, 0);

	device.GetContext()->PSSetShaderResources(0, 1, &nullSRV);
}

void Renderer::RenderWireframeForVertexPaintingAndPickedActor()
{
	const auto wireframeRender = [&](MeshComponent* mesh)
	{
		device.GetContext()->RSSetState(rastStateMap.find(RastStates::wireframe)->second->GetData());

		MaterialShaderData materialShaderData;
		materialShaderData.ambient = XMFLOAT4(1.f, 0.f, 1.f, 1.f);
		cbMaterial.Map(&materialShaderData);
		cbMaterial.SetPS();

		ShaderItem* shaderItem = ShaderSystem::FindShaderItem("SolidColour");
		device.GetContext()->VSSetShader(shaderItem->GetVertexShader(), nullptr, 0);
		device.GetContext()->IASetInputLayout(shaderItem->GetInputLayout());
		device.GetContext()->PSSetShader(shaderItem->GetPixelShader(), nullptr, 0);

		SetVertexBuffer(mesh->GetVertexBuffer());

		SetMatricesFromMesh(mesh);
		SetShaderMeshData(mesh);
		SetLightProbeData(mesh);

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

void Renderer::RenderLightProbes()
{
	auto probeMap = DiffuseProbeMap::system.GetFirstActor();
	if (probeMap == nullptr) return;
	if (!probeMap->IsActive()) return;
	if (!probeMap->IsVisible()) return;

	shaderMatrices.model = XMMatrixIdentity();
	shaderMatrices.MakeModelViewProjectionMatrix();

	cbMatrices.Map(&shaderMatrices);
	cbMatrices.SetVS();

	auto instanceMesh = probeMap->GetInstanceMesh();
	SetRenderPipelineStates(instanceMesh);

	SetBlendStateByName(BlendStates::Default);

	//Careful with the buffers here, they're not part of the instance mesh, instead they're in the diffuse probe map
	MapBuffer(probeMap->GetStructuredBuffer(), probeMap->GetLightProbeData().data(), sizeof(LightProbeInstanceData) * probeMap->GetLightProbeData().size());

	//Update texture matrix
	shaderMatrices.MakeTextureMatrix(instanceMesh->GetMaterial());
	cbMatrices.Map(&shaderMatrices);
	cbMatrices.SetVS();

	ID3D11ShaderResourceView* srv = probeMap->GetSRV();
	device.GetContext()->VSSetShaderResources(lightProbeInstanceDataRegister, 1, &srv);
	device.GetContext()->PSSetShaderResources(lightProbeInstanceDataRegister, 1, &srv);

	cbLights.SetPS();

	device.GetContext()->DrawInstanced(static_cast<UINT>(instanceMesh->meshDataProxy.vertices.size()), probeMap->GetProbeCount(), 0, 0);
}

void Renderer::RenderMeshToCaptureMeshIcon()
{
	RenderSetup();

	auto mesh = MeshComponent::system.Add("MeshCaptureIcon");
	mesh->SetMeshFilename(captureMeshIconMeshFilename);
	mesh->Create();

	auto& previousActiveCamera = Camera::GetActiveCamera();

	auto camera = CameraComponent::system.Add("MeshCaptureCamera");
	Camera::SetActiveCamera(camera);
	camera->SetWorldPosition(XMFLOAT3(0.f, 0.f, -2.f));
	camera->SetTargetComponent(mesh);

	shaderMatrices.view = camera->GetViewMatrix();

	SetRenderPipelineStates(mesh);
	SetMatricesFromMesh(mesh);
	SetShaderMeshData(mesh);
	SetLightProbeData(mesh);

	DrawMesh(mesh);

	mesh->Remove();
	camera->Remove();

	Camera::SetActiveCamera(&previousActiveCamera);
}

//Don't actually want to use usual light calculations with vertex colour baking.
//Because the pixel shaders are already adding colour to meshes, instead, vertex colour baking can be used
//like a more coarse ambient occlusion, only considering raycast hits from the light source.
void Renderer::VertexColourLightBake()
{
	const auto startTime = Profile::QuickStart();

	for (const auto& mesh : MeshComponent::system.GetComponents())
	{
		//Skipping baking vertex colours to non-static meshes
		if (!mesh->IsRenderStatic())
		{
			continue;
		}

		const auto meshWorldMatrix = mesh->GetWorldMatrix();
		auto& vertices = mesh->GetAllVertices();

		const auto defaultGrey = XMVectorSet(0.3f, 0.3f, 0.3f, 1.f);

		for (const auto& dLight : DirectionalLightComponent::system.GetComponents())
		{
			for (auto& vertex : vertices)
			{
				const auto vertexPos = XMLoadFloat3(&vertex.pos);
				const auto worldSpaceVertexPos = XMVector3TransformCoord(vertexPos, meshWorldMatrix);

				auto normal = XMLoadFloat3(&vertex.normal);
				normal = XMVector3TransformNormal(normal, meshWorldMatrix);
				normal = XMVector3Normalize(normal);

				HitResult vertexRayHit;
				vertexRayHit.AddAllRenderStaticMeshesToIgnore();
				vertexRayHit.ignoreBackFaceHits = false;
				vertexRayHit.ignoreLayer = CollisionLayers::Editor;

				const auto rayOrigin = worldSpaceVertexPos + (normal * 0.1f);
				const auto dLightDirection = dLight->GetForwardVectorV();
				if (Physics::Raycast(vertexRayHit, rayOrigin, -dLightDirection, 50.f))
				{
					const auto result = XMLoadFloat4(&vertex.colour) * defaultGrey;
					XMStoreFloat4(&vertex.colour, result);
				}
			}
		}

		for (const auto& pointLight : PointLightComponent::system.GetComponents())
		{
			for (auto& vertex : vertices)
			{
				const auto vertexPos = XMLoadFloat3(&vertex.pos);
				const auto worldSpaceVertexPos = XMVector3TransformCoord(vertexPos, meshWorldMatrix);

				auto normal = XMLoadFloat3(&vertex.normal);
				normal = XMVector3TransformNormal(normal, meshWorldMatrix);
				normal = XMVector3Normalize(normal);

				HitResult vertexRayHit;
				vertexRayHit.AddAllRenderStaticMeshesToIgnore();
				vertexRayHit.ignoreBackFaceHits = false;
				vertexRayHit.ignoreLayer = CollisionLayers::Editor;

				const auto vertexToLightDirection =
					XMVector3Normalize(pointLight->GetWorldPositionV() - worldSpaceVertexPos);
				const auto rayOrigin = worldSpaceVertexPos + (normal * 0.1f);
				if (Physics::Raycast(vertexRayHit, rayOrigin, pointLight->GetWorldPositionV()))
				{
					const auto result = XMLoadFloat4(&vertex.colour) * defaultGrey;
					XMStoreFloat4(&vertex.colour, result);
				}
			}
		}

		for (const auto& spotLight : SpotLightComponent::system.GetComponents())
		{
			for (auto& vertex : vertices)
			{
				const auto vertexPos = XMLoadFloat3(&vertex.pos);
				const auto worldSpaceVertexPos = XMVector3TransformCoord(vertexPos, meshWorldMatrix);

				auto normal = XMLoadFloat3(&vertex.normal);
				normal = XMVector3TransformNormal(normal, meshWorldMatrix);
				normal = XMVector3Normalize(normal);

				HitResult vertexRayHit;
				vertexRayHit.AddAllRenderStaticMeshesToIgnore();
				vertexRayHit.ignoreBackFaceHits = false;
				vertexRayHit.ignoreLayer = CollisionLayers::Editor;

				const auto spotLightWorldPos = spotLight->GetWorldPositionV();
				const auto rayOrigin = worldSpaceVertexPos + (normal * 0.1f);

				const float angleBetweenSpotLightForwardAndRaycastDirection =
					XMConvertToDegrees(XMVector3AngleBetweenNormals(
						spotLight->GetForwardVectorV(),
						XMVector3Normalize(rayOrigin - spotLightWorldPos)).m128_f32[0]);
				if (angleBetweenSpotLightForwardAndRaycastDirection <= spotLight->GetLightData().spotAngle)
				{
					if (Physics::Raycast(vertexRayHit, rayOrigin, spotLight->GetWorldPositionV()))
					{
						const auto result = XMLoadFloat4(&vertex.colour) * defaultGrey;
						XMStoreFloat4(&vertex.colour, result);
					}
				}
			}
		}

		mesh->CreateNewVertexBuffer();
	}

	const auto endTime = Profile::QuickEnd(startTime);
	Log("Vertex colour baking completed in [%f] seconds.", endTime);
}

RastState* Renderer::GetRastState(std::string rastStateName)
{
	return rastStateMap.find(rastStateName)->second.get();
}

BlendState* Renderer::GetBlendState(std::string blendStateName)
{
	return blendStateMap.find(blendStateName)->second.get();
}

void Renderer::AddDebugDrawOrientedBox(DirectX::BoundingOrientedBox& orientedBox, bool clear)
{
	DebugBoxData data;
	data.bb = orientedBox;
	data.clearWithTimer = clear;
	data.timer = 3.f;
	debugOrientedBoxesOnTimerToRender.emplace_back(data);
}

void Renderer::ClearBounds()
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
	return *device.Get();
}

ID3D11DeviceContext& Renderer::GetDeviceContext()
{
	return *device.GetContext();
}

Sampler& Renderer::GetDefaultSampler()
{
	return defaultSampler;
}

void Renderer::SetRendererToCaptureMeshIcon(std::string meshFilename)
{
	//Remember that it needs to be the .vmesh filename, not .fbx
	captureMeshIconMeshFilename = VString::ReplaceFileExtesnion(meshFilename, ".vmesh");
	captureMeshIconOnCurrentFrame = true;
}

bool Renderer::IsRendererSetToCaptureMeshIcon()
{
	return captureMeshIconOnCurrentFrame;
}

void Renderer::ReportLiveObjectsVerbose()
{
	device.GetDebugDevice()->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}
