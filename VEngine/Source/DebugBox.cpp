#include "DebugBox.h"

DebugBox debugBox;

DebugBox::DebugBox()
{
	modelName = "cube.fbx";
	shaderName = "debugDraw.hlsl";
}

void DebugBox::Tick(float deltaTime)
{
}

void DebugBox::Start()
{
	Init<Actor>(1);
}
