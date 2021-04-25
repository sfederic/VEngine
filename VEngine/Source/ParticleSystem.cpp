#include "ParticleSystem.h"
#include "RenderSystem.h"
#include "MathHelpers.h"
#include "Camera.h"

ParticleSystem::ParticleSystem()
{
	modelName = "plane.fbx";
	shaderName = L"shaders.hlsl";
	textureName = L"texture.png";
	name = L"ParticleSystem";
	Init<Particle>(&gRenderSystem, 5);
}

void ParticleSystem::Tick(float deltaTime)
{
	for (int i = 0; i < actors.size(); i++)
	{
		XMMATRIX m = actors[i]->GetTransformationMatrix();
		LookAtRotation(GetActiveCamera()->location, m);
		actors[i]->transform.Decompose(m);
	}
}
