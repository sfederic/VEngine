#include "vpch.h"
#include "SpinningEnemy.h"
#include "Gameplay/GameplayTags.h"
#include "Components/MeshComponent.h"
#include "Core/VMath.h"

void SpinningEnemy::Create()
{
	__super::Create();

	CreateMesh("MeshCore", XMFLOAT3(0.f, 0.f, 0.f));
	CreateMesh("MeshX+", XMFLOAT3(1.f, 0.f, 0.f));
	CreateMesh("MeshX-", XMFLOAT3(-1.f, 0.f, 0.f));
	CreateMesh("MeshY+", XMFLOAT3(0.f, 1.f, 0.f));
	CreateMesh("MeshY-", XMFLOAT3(0.f, -1.f, 0.f));
	CreateMesh("MeshZ+", XMFLOAT3(0.f, 0.f, 1.f));
	CreateMesh("MeshZ-", XMFLOAT3(0.f, 0.f, -1.f));
}

void SpinningEnemy::Start()
{
	nextRot = GetRotationV();
}

void SpinningEnemy::Tick(float deltaTime)
{
	spinTimer += deltaTime;
	if (spinTimer > 0.25f)
	{
		std::vector<XMVECTOR> directions { GetUpVectorV(), GetRightVectorV(), GetUpVectorV(),
			-GetUpVectorV(), -GetRightVectorV(), -GetUpVectorV() };
		int randomDirectionIndex = VMath::RandomRangeInt(0, directions.size() - 1);

		std::vector<float> angles { 90.f, 180.f, 270.f, -90.f, -180.f, -270.f };
		int randomAngleIndex = VMath::RandomRangeInt(0, angles.size() - 1);

		nextRot = XMQuaternionMultiply(GetRotationV(),
			DirectX::XMQuaternionRotationAxis(directions[randomDirectionIndex], XMConvertToRadians(angles[randomAngleIndex])));

		spinTimer = 0.f;
	}

	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, 20.f));
}

Properties SpinningEnemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = "SpinningEnemy";
	return props;
}

void SpinningEnemy::CreateMesh(const std::string name, XMFLOAT3 position)
{
	auto mesh = CreateComponent(name, MeshComponent("cube.vmesh", "test.png"));
	mesh->SetLocalPosition(position);
	mesh->SetLocalScale(0.75f, 0.75f, 0.75f);
	mesh->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(mesh);
}
