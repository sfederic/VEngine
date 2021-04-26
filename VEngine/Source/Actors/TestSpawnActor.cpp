#include "TestSpawnActor.h"
#include "Input.h"
#include "ActorSystemFactory.h"

TestSpawnActorSystem testSpawnActorSystem;

TestSpawnActorSystem::TestSpawnActorSystem()
{
	shaderName = L"shaders.hlsl";
	textureName = L"texture2.jpg";
	modelName = "cone.fbx";
	name = L"testspawnactor";

	sizeofActor = sizeof(TestSpawnActor);

	ActorSystemFactory::Register<TestSpawnActorSystem>(this);
}

void TestSpawnActorSystem::Tick(float deltaTime)
{
}

void TestSpawnActorSystem::SpawnActors(int numToSpawn)
{
	Init<TestSpawnActor>(numToSpawn);
}

void TestSpawnActorSystem::SpawnActor(Transform transform)
{
	AddActor<TestSpawnActor>(transform);
}
