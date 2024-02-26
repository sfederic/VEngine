#pragma once

#include <string>
#include "Render/MeshComponentFramePacket.h"

struct RastState;
struct Sampler;
struct BlendState;
struct Line;
namespace DirectX {
	struct BoundingOrientedBox;
}
struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Renderer
{
	extern float frameTime;

	extern bool drawBoundingBoxes;
	extern bool drawTriggers;
	extern bool drawAllAsWireframe;

	extern unsigned int stride;
	extern unsigned int offset;

	void Init(void* window, int viewportWidth, int viewportHeight);
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
	void DeferSwapchainResize(int newWidth, int newHeight);
	void ResizeSwapchain();

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

	void PassInMeshComponentFramePackets(std::vector<MeshComponentFramePacket>& meshPackets);
};
