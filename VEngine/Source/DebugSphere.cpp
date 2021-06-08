#include "DebugSphere.h"

DebugSphere debugSphere;

DebugSphere::DebugSphere()
{
	shaderName = "debugDraw.hlsl";
	modelName = "ico_sphere.fbx";
}

void DebugSphere::Tick(float deltaTime)
{

}

void DebugSphere::Start()
{
	Init<Actor>(1);
}
