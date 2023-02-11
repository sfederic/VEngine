#include "vpch.h"
#include "RepairObject.h"
#include "Components/MeshComponent.h"
#include "Particle/ParticleEmitter.h"

RepairObject::RepairObject()
{
	rootComponent = CreateComponent("Mesh", MeshComponent("cube.vmesh", "test.png"));

	auto emitter = CreateComponent("Smoke", ParticleEmitter("Particle/smoke.png"));
	rootComponent->AddChild(emitter);
}

Properties RepairObject::GetProps()
{
	auto props = __super::GetProps();
	props.title = "RepairObject";
	return props;
}

void RepairObject::Interact()
{
	auto emitter = GetComponent<ParticleEmitter>("Smoke");
	emitter->SetActive(false);
}
