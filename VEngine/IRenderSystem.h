#pragma once

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
	virtual void CreateBuffer() = 0;
	virtual void CreateVertexBuffer() = 0;
	virtual void CreateSampler() = 0;
};

