#pragma once

#include <string>

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
};
