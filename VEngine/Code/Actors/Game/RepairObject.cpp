#include "vpch.h"
#include "RepairObject.h"
#include "Components/MeshComponent.h"
#include "Particle/ParticleEmitter.h"

RepairObject::RepairObject()
{
	rootComponent = CreateComponent(MeshComponent("cube.vmesh", "test.png"), "Mesh");

	auto emitter = CreateComponent(ParticleEmitter("Particle/smoke.png"), "Smoke");
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
