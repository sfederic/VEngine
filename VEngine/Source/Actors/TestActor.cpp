#include "TestActor.h"
#include "ActorSystemFactory.h"
#include "Input.h"
#include "MathHelpers.h"
#include "Raycast.h"
#include "World.h"

TestActorSystem testActorSystem;

TestActorSystem::TestActorSystem()
{
	shaderName = L"shaders.hlsl";
	textureName = L"texture2.jpg";
	modelName = "cube.fbx";
	name = L"testactor";

	sizeofActor = sizeof(TestActor);

	ActorSystemFactory::Register<TestActorSystem>(this);
}

void TestActorSystem::Tick(float deltaTime)
{
}

void TestActorSystem::SpawnActors(int numToSpawn)
{
	Init<TestActor>(numToSpawn);
}

void TestActorSystem::SpawnActor(Transform transform)
{
	AddActor<TestActor>(transform);
}

TestActor::TestActor()
{
	currentPos = GetPositionVector();
	nextPos = currentPos;
}

void TestActor::Tick(float deltaTime)
{
	if(VecEqual(currentPos, nextPos))
	{
		if (gInputSystem.GetKeyDownState(Keys::Right))
		{
			nextPos.m128_f32[0] += 2.0f;
		}
	}

	if (gInputSystem.GetKeyDownState(Keys::Down))
	{
		Ray ray = {};
		if (RaycastAll(ray, XMVectorSet(0.f, 10.f, 0.f, 0.f), -XMVectorUp(), GetWorld()))
		{
			throw;
		}
	}

	const float moveSpeed = 5.0f;
	currentPos = XMVectorConstantLerp(currentPos, nextPos, (deltaTime * moveSpeed));
	SetPosition(currentPos);
}
