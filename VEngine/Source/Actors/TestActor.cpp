#include "TestActor.h"
#include "ActorSystemFactory.h"
#include "Input.h"
#include "MathHelpers.h"

TestActorSystem testActorSystem;

TestActorSystem::TestActorSystem()
{
	shaderName = L"shaders.hlsl";
	textureName = L"texture2.jpg";
	modelName = "cube.fbx";
	name = L"TestActorSystem";

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

Properties TestActor::GetProperties()
{
	Properties props;
	props.Register("VertexBufferOffset", &vertexBufferOffset);
	props.Register("Render", &bRender);
	
	return props;
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
