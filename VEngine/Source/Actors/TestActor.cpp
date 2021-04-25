#include "TestActor.h"
#include "Input.h"
#include "ActorSystemFactory.h"
#include "RenderSystem.h"

TestActorSystem::TestActorSystem()
{
	ActorSystemFactory::Register<TestActorSystem>(this);

	shaderName = L"shaders.hlsl";
	textureName = L"texture2.jpg";
	modelName = "cube.fbx";
	name = L"testactor";

	sizeofActor = sizeof(TestActor);
}

void TestActorSystem::Tick(float deltaTime)
{
}

void TestActorSystem::SpawnActors(int numToSpawn)
{
	CreateActors<TestActor>(&gRenderSystem, numToSpawn);
}

void TestActorSystem::SpawnActor(Transform transform)
{
	AddActor<TestActor>(transform);
}
