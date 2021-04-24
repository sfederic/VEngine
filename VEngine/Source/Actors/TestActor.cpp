#include "TestActor.h"
#include "Input.h"
#include "ActorSystemFactory.h"

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

