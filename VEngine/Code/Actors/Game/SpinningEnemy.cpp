#include "vpch.h"
#include "SpinningEnemy.h"
#include "Gameplay/GameplayTags.h"
#include "Components/MeshComponent.h"
#include "VMath.h"

void SpinningEnemy::Create()
{
	auto mesh = CreateComponent(MeshComponent("cube.fbx", "water.jpg"), "MeshCore");
	mesh->SetPosition(0.f, 0.f, 0.f);
	mesh->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshX+");
	mesh->SetPosition(1.f, 0.f, 0.f);
	mesh->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshX-");
	mesh->SetPosition(-1.f, 0.f, 0.f);
	mesh->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshY+");
	mesh->SetPosition(0.f, 1.f, 0.f);
	mesh->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshY-");
	mesh->SetPosition(0.f, -1.f, 0.f);
	mesh->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshZ+");
	mesh->SetPosition(0.f, 0.f, 1.f);
	mesh->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshZ-");
	mesh->SetPosition(0.f, 0.f, -1.f);
	mesh->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(mesh);
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
