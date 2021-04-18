#include "ParticleSystem.h"
#include "RenderSystem.h"
#include "MathHelpers.h"
#include "Camera.h"

ParticleSystem::ParticleSystem()
{
	modelName = "plane.fbx";
	shaderName = L"shaders.hlsl";
	textureName = L"texture.jpg";
	name = L"ParticleSystem";
	CreateActors<Particle>(&gRenderSystem, 5);
}

void ParticleSystem::Tick()
{
	for (int i = 0; i < actors.size(); i++)
	{
		LookAtRotation(GetActiveCamera()->location, actors[i]->GetTransformationMatrix());
	}
}
