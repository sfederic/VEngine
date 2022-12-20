#include "vpch.h"
#include "CoreEnemy.h"
#include "Gameplay/GameplayTags.h"
#include "Components/MeshComponent.h"

void CoreEnemy::Create()
{
	auto mesh = CreateComponent(MeshComponent("cube.fbx", "water.jpg"), "MeshCore");
	mesh->SetPosition(0.f, 0.f, 0.f);
	mesh->AddTag(GameplayTags::EnemyMeshPiece);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshX+");
	mesh->SetPosition(1.f, 0.f, 0.f);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshX-");
	mesh->SetPosition(-1.f, 0.f, 0.f);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshY+");
	mesh->SetPosition(0.f, 1.f, 0.f);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshY-");
	mesh->SetPosition(0.f, -1.f, 0.f);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshZ+");
	mesh->SetPosition(0.f, 0.f, 1.f);
	rootComponent->AddChild(mesh);

	mesh = CreateComponent(MeshComponent("cube.fbx", "test.png"), "MeshZ-");
	mesh->SetPosition(0.f, 0.f, -1.f);
	rootComponent->AddChild(mesh);
}

Properties CoreEnemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = "CoreEnemy";
	return props;
}
