#pragma once

#include "Actor.h"

//Just for testing
class TestActor : public ActorSystem
{
public:
	TestActor()
	{
		shaderName = L"shaders.hlsl";
		textureName = L"texture2.jpg";
		modelName = "cube.fbx";
		name = L"testactor";
	}

	virtual void Tick(float deltaTime) {}
};
