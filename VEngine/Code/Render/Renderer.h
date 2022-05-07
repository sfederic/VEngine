#pragma once
#include <string>

struct RastState;
struct BlendState;

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
	void RenderPostProcessSetup();
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

	RastState* GetRastState(std::string rastStateName);
	BlendState* GetBlendState(std::string blendStateName);
};
