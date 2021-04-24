#include "TestActor.h"
#include "Input.h"
#include "TypeFactory.h"

TestActorSystem::TestActorSystem()
{
	shaderName = L"shaders.hlsl";
	textureName = L"texture2.jpg";
	modelName = "cube.fbx";
	name = L"testactor";

	sizeofActor = sizeof(TestActor);
}

void TestActorSystem::Tick(float deltaTime)
{
	__super::Tick(deltaTime);


}
