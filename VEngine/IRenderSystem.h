#pragma once

#include "RenderTypes.h"
#include <Windows.h>

class ISampler;

class IRenderBuffer
{
public:

};

class IDXGISwapChain3;

//Render system Interface
class IRenderSystem
{
public:
	virtual void Tick() = 0;
	virtual void Init(HWND window) = 0;
	virtual void RenderSetup(float deltaTime) = 0;
	virtual void Render(float deltaTime) = 0;
	virtual void RenderEnd(float deltaTime) = 0;
	//virtual void CreateDefaultBuffer() = 0;
	virtual void CreateVertexBuffer(unsigned int size, const void* data, class ActorSystem* actor) = 0;
	virtual void CreateSamplerState(ISampler* sampler) = 0;
	virtual void CreateTexture(class ActorSystem* actorSystem) = 0;
	virtual void CreateVertexShader() = 0;
	virtual void CreatePixelShader() = 0;
	virtual void CreateAllShaders() = 0;
	virtual IDXGISwapChain3* GetSwapchain() = 0;
	virtual void Present() = 0;
	virtual void Flush() = 0; //Acting as a stand in for WaitForPreviousFrame()

	Matrices matrices;
	Material material;
};

