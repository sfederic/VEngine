#include "TestSpawnActor.h"
#include "Input.h"
#include "ActorSystemFactory.h"
#include "RenderSystem.h"

TestSpawnActorSystem::TestSpawnActorSystem()
{
	ActorSystemFactory::Register<TestSpawnActorSystem>(this);

	shaderName = L"shaders.hlsl";
	textureName = L"texture2.jpg";
	modelName = "cube.fbx";
	name = L"testactor";

	sizeofActor = sizeof(TestSpawnActor);
}

void TestSpawnActorSystem::Tick(float deltaTime)
{
}

void TestSpawnActorSystem::SpawnActors(int numToSpawn)
{
	CreateActors<TestSpawnActor>(&gRenderSystem, numToSpawn);
}

void TestSpawnActorSystem::SpawnActor(Transform transform)
{
	AddActor<TestSpawnActor>(transform);
}
