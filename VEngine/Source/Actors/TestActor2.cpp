#include "TestActor2.h"
#include "ActorSystemFactory.h"

TestActor2System testActor2System;

TestActor2System::TestActor2System()
{
	name = "TestActor2System";
	modelName = "monkey.fbx";
	shaderName = L"shaders.hlsl";

	ActorSystemFactory::Register<TestActor2System>(this);
}

void TestActor2System::Tick(float deltaTime)
{
}

void TestActor2System::SpawnActors(int numToSpawn)
{
	Init<TestActor2>(numToSpawn);
}

Actor* TestActor2System::SpawnActor(Transform transform)
{
	return AddActor<TestActor2>(transform);
}

void TestActor2::Tick(float deltaTime)
{
}
