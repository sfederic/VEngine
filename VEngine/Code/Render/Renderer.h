#pragma once

#include <string>
#include <d3d11.h>
#include <DirectXCollision.h>
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "Swapchain.h"
#include "Device.h"
#include "ShadowMap.h"
#include "Sampler.h"
#include "ShaderData/ShaderMatrices.h"
#include "ShaderData/ShaderLights.h"
#include "Vertex.h"
#include "RastState.h"
#include "BlendState.h"
#include "ConstantBuffer.h"
#include "ShaderData/MaterialShaderData.h"
#include "ShaderData/ShaderTimeData.h"
#include "ShaderData/ShaderMeshData.h"
#include "ShaderData/ShaderSkinningData.h"
#include "ShaderData/ShaderPostProcessData.h"
#include "ShaderData/ShaderCameraData.h"
#include "ShaderData/ShaderLightProbeData.h"

class Line;
class VertexBuffer;
class IndexBuffer;
class MeshComponent;
class InstanceMeshComponent;
class ShaderItem;

class Renderer
{
public:
	static Renderer& Get()
	{
		static Renderer instance;
		return instance;
	}

	void Init(void* window, int viewportWidth, int viewportHeight);
	void Cleanup();
	void Tick();

	void Present();
	void Render();

	//Renders sprites on the viewport as UI elements
	void RenderSpritesInScreenSpace();

	//Called as a 'baking' function once-off through editor
	void RenderLightProbeViews();

	void RenderParticleEmitters();
	void* GetSwapchain();
	float GetAspectRatio();
	float GetViewportWidth();
	float GetViewportHeight();
	void SetViewportWidthHeight(float width, float height);
	void ResizeSwapchain(int newWidth, int newHeight);

	//Write out viewport as image and save to file as an image.
	//Ref:https://github.com/Microsoft/DirectXTK/wiki/ScreenGrab
	void ScreenshotCapture();
	void MeshIconImageCapture();
	void MapIconImageCapture();
	void PlayerPhotoCapture(std::wstring outputFilename);

	RastState* GetRastState(std::string rastStateName);
	BlendState* GetBlendState(std::string blendStateName);

	void AddDebugDrawOrientedBox(DirectX::BoundingOrientedBox& orientedBox, bool clear);
	void AddDebugLine(Line& line);

	ID3D11Device& GetDevice();
	ID3D11DeviceContext& GetDeviceContext();
	Sampler& GetDefaultSampler();

	//@Todo: As nice as automatic mesh icon generation is, I feel like it makes the main rendering code 
	//a bit branchy. It's a big task, but if you could render the mesh to take an icon screenshot of to
	//a different viewport, it might settle the issue of being too branchy.
	void SetRendererToCaptureMeshIcon(std::string meshFilename);
	bool IsRendererSetToCaptureMeshIcon();

	void ReportLiveObjectsVerbose();

	inline static constexpr unsigned int stride = sizeof(Vertex);
	inline static constexpr unsigned int offset = 0;

	auto GetFrameTime() const { return frameTime; }

private:
	void CreateFactory();
	void CreateRTVAndDSV();
	void CreateRasterizerStates();
	void CreateBlendStates();
	void CreateConstantBuffers();
	void CreateLightProbeBuffers();
	void CheckSupportedFeatures();
	void RenderShadowPass();
	void RenderSetup();
	void RenderPostProcessSetup();
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
	void RenderAudioComponents();
	void RenderPolyboards();
	void RenderSpriteSheets();
	void RenderPostProcess();
	void RenderWireframeForVertexPaintingAndPickedActor();
	void RenderLightProbes();
	void RenderMeshToCaptureMeshIcon();

	void VertexColourLightBake();

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
	void SetLightProbeData(MeshComponent* mesh);
	void SetRenderPipelineStates(MeshComponent* mesh);
	void SetRenderPipelineStatesForShadows(MeshComponent* mesh);
	void SetShaders(std::string shaderItemName);
	void SetShaders(ShaderItem* shaderItem);
	void SetRastStateByName(std::string rastStateName);
	void SetRastState(RastState& rastState);
	void SetBlendStateByName(std::string blendStateName);
	void SetBlendState(BlendState& blendState);
	void SetConstantBufferVertexPixel(uint32_t shaderRegister, ID3D11Buffer* constantBuffer);
	void SetConstantBufferVertex(uint32_t shaderRegister, ID3D11Buffer* constantBuffer);
	void SetConstantBufferPixel(uint32_t shaderRegister, ID3D11Buffer* constantBuffer);
	void SetVertexBuffer(VertexBuffer& vertexBuffer);
	void SetIndexBuffer(IndexBuffer& indexBuffer);
	void SetSampler(uint32_t shaderRegister, Sampler& sampler);
	void SetShaderResourcePixel(uint32_t shaderRegister, std::string textureName);
	void SetShaderResourceFromMaterial(Material& material);
	void SetLightsConstantBufferData();
	void SetCameraConstantBufferData();
	void ClearBounds();

	DepthStencil depthStencil;

	RenderTarget rtvs[Swapchain::SWAPCHAIN_COUNT];

	std::unordered_map<std::string, std::unique_ptr<RastState>> rastStateMap;
	std::unordered_map<std::string, std::unique_ptr<BlendState>> blendStateMap;

	Swapchain swapchain;

	Microsoft::WRL::ComPtr<IDXGIFactory6> dxgiFactory;

	Device device;

	D3D11_VIEWPORT viewport = {};

	ShadowMap shadowMap;

	Sampler defaultSampler;

	ShaderMatrices shaderMatrices;
	ShaderLights shaderLights;

	//Light probe buffers
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> lightProbeRTVs[6]; //Cubemap
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> lightProbeSRV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> lightProbeTexture;

	RenderTarget postProcessRenderTarget;

	//Quality = 0 and Count = 1 are the 'default'
	DXGI_SAMPLE_DESC sampleDesc = {};

	float frameTime = 0.f;

	bool drawBoundingBoxes = false;
	bool drawTriggers = true;
	bool drawAllAsWireframe = false;

	//Make sure these match up with the register definitions in Commmon.hlsli
	inline static constexpr int defaultTextureRegister = 0;
	inline static constexpr int secondaryTextureRegister = 1;
	inline static constexpr int shadowMapTextureRegister = 2;
	inline static constexpr int reflectionTextureRegister = 3;
	inline static constexpr int instanceSRVRegister = 4;
	inline static constexpr int environmentMapTextureRegister = 5;
	inline static constexpr int normalMapTextureRegister = 6;
	inline static constexpr int lightProbeInstanceDataRegister = 7;

	inline static constexpr int lightProbeTextureWidth = 64;
	inline static constexpr int lightProbeTextureHeight = 64;

	ConstantBuffer<ShaderMatrices> cbMatrices;
	ConstantBuffer<MaterialShaderData> cbMaterial;
	ConstantBuffer<ShaderLights> cbLights;
	ConstantBuffer<ShaderTimeData> cbTime;
	ConstantBuffer<ShaderMeshData> cbMeshData;
	ConstantBuffer<ShaderSkinningData> cbSkinningData;
	ConstantBuffer<ShaderPostProcessData> cbPostProcess;
	ConstantBuffer<ShaderCameraData> cbCameraData;
	ConstantBuffer<ShaderLightProbeData> cbLightProbeData;

	bool captureMeshIconOnCurrentFrame = false;
	std::string captureMeshIconMeshFilename;
};
