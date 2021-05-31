#include "ParticleSystem.h"
#include "MathHelpers.h"
#include "Camera.h"

ParticleSystem::ParticleSystem()
{
	modelName = "plane.fbx";
	shaderName = "shaders.hlsl";
	textureName = "texture.png";
	name = "ParticleSystem";
	Init<Particle>(5);
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

