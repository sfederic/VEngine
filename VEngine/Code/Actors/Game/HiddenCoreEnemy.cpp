#include "vpch.h"
#include "HiddenCoreEnemy.h"
#include "Components/SliceableMeshComponent.h"
#include "Components/MeshComponent.h"

void HiddenCoreEnemy::Create()
{
	health = 2;

	hull = CreateComponent(SliceableMeshComponent(), "Hull");
	hull->SetMeshFilename("cube.vmesh");
	rootComponent->AddChild(hull);

	core = CreateComponent(MeshComponent(), "Core");
	core->SetMeshFilename("sphere.vmesh");
	core->SetLocalScale(0.33f, 0.33f, 0.33f);
	core->skipPhysicsCreation = true;
	rootComponent->AddChild(core);
}

void HiddenCoreEnemy::Tick(float deltaTime)
{
	core->AddLocalRotation(core->GetUpVectorV(), 15.f * deltaTime);
}

Properties HiddenCoreEnemy::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}