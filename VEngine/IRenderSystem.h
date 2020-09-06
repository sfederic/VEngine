#pragma once

#include "RenderTypes.h"

class IRenderBuffer
{
public:

};

//Render system Interface
class IRenderSystem
{
public:
	virtual void Tick() = 0;
	virtual void Init() = 0;
	virtual void RenderSetup(float deltaTime) = 0;
	virtual void Render(float deltaTime) = 0;
	virtual void RenderEnd(float deltaTime) = 0;
	virtual void CreateDefaultBuffer() = 0;
	virtual void CreateVertexBuffer(unsigned int size, const void* data, class ActorSystem* actor) = 0;
	virtual void CreateSamplerState(class ActorSystem* actorSystem) = 0;
	virtual void CreateTexture(class ActorSystem* actorSystem) = 0;
	virtual void CreateVertexShader() = 0;
	virtual void CreatePixelShader() = 0;
	virtual void CreateAllShaders() = 0;
	virtual void* GetSwapchain() = 0;
	virtual void Present() = 0;

	Matrices matrices;
};

