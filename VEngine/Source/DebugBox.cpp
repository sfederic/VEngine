#include "DebugBox.h"
#include "RenderSystem.h"

DebugBox::DebugBox()
{
	modelName = "cube.fbx";
	shaderName = L"debugDraw.hlsl";
	Init<Actor>(&gRenderSystem, 1);
}

void DebugBox::Tick(float deltaTime)
{
}
