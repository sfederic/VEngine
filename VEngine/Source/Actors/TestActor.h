#pragma once

#include "Actor.h"
#include "Input.h"

//Just for testing

class TestActor : public Actor
{
public:
	TestActor() 
	{
	}

	virtual void Tick(float deltaTime)
	{

	}
};

class TestActorSystem : public ActorSystem
{

public:
	TestActorSystem()
	{
		shaderName = L"shaders.hlsl";
		textureName = L"texture2.jpg";
		modelName = "cube.fbx";
		name = L"testactor";

		sizeofActor = sizeof(TestActor);
	}

	virtual void Tick(float deltaTime) override;
};
