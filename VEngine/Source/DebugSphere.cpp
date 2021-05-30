#include "DebugSphere.h"

DebugSphere::DebugSphere()
{
	shaderName = "debugDraw.hlsl";
	modelName = "ico_sphere.fbx";
	Init<Actor>(1);
}

void DebugSphere::Tick(float deltaTime)
{
}
