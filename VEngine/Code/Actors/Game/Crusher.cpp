#include "vpch.h"
#include "Crusher.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"

Crusher::Crusher()
{
	mesh = CreateComponent<MeshComponent>("Mesh");
	SetRootComponent(mesh);
}

void Crusher::Create()
{
	__super::Create();

	mesh->SetMeshFilename("cube.vmesh");
}

void Crusher::Start()
{
	__super::Start();

	originalPos = GetPositionV();
	nextPos = originalPos + GetForwardVectorV();
}

void Crusher::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	if (!retracted)
	{
		retractTimer += deltaTime;

		if (retractTimer > 3.0f)
		{
			retractTimer = 0.0f;
			retracted = true;
		}

		SetPosition(VMath::VectorConstantLerp(GetPositionV(), originalPos, deltaTime, 2.5f));
	}
	else
	{
		moveTimer += deltaTime;

		if (moveTimer > 3.0f)
		{
			moveTimer = 0.0f;
			retracted = false;
		}

		SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, 5.0f));
	}
}

Properties Crusher::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
