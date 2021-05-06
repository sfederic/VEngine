#include "TestActor.h"
#include "ActorSystemFactory.h"
#include "Input.h"
#include "MathHelpers.h"
#include "Raycast.h"
#include "World.h"
#include <tuple>

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

	currentRot = XMLoadFloat4(&GetRotationQuat());
	nextRot = currentRot;
}

PropertyMap TestActor::GetProperties()
{
	PropertyMap properties;
	properties[typeid(bRender)] = std::make_pair("Visibility", (void*)&bRender);
	properties[typeid(vertexBufferOffset)] = std::make_pair("Test int", (void*)&vertexBufferOffset);
	
	return properties;
}

void TestActor::Tick(float deltaTime)
{
	if(VecEqual(currentPos, nextPos))
	{
		if (gInputSystem.GetKeyDownState(Keys::D))
		{
			nextPos += (GetRightVector() * 2.f);
		}
		else if (gInputSystem.GetKeyDownState(Keys::A))
		{
			nextPos += (-GetRightVector() * 2.f);
		}
		else if (gInputSystem.GetKeyDownState(Keys::W))
		{
			nextPos += (GetForwardVector() * 2.f);
		}
		else if (gInputSystem.GetKeyDownState(Keys::S))
		{
			nextPos += (-GetForwardVector() * 2.f);
		}
	}

	if (gInputSystem.GetKeyDownState(Keys::Right))
	{
		nextRot.m128_f32[0] = 0.5f;
		nextRot.m128_f32[1] = 1.f;
	}

	const float moveSpeed = 5.0f;
	currentPos = XMVectorConstantLerp(currentPos, nextPos, (deltaTime * moveSpeed));
	SetPosition(currentPos);

	const float rotationSpeed = 15.0f;
	currentRot = XMQuaternionSlerp(currentRot, nextRot, (deltaTime * rotationSpeed));
}
