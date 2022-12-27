#include "vpch.h"
#include "RepairObject.h"
#include "Components/MeshComponent.h"
#include "Particle/ParticleEmitter.h"

RepairObject::RepairObject()
{
	rootComponent = CreateComponent(MeshComponent("cube.fbx", "test.png"), "Mesh");

	auto emitter = CreateComponent(ParticleEmitter(), "Smoke");
	emitter->textureData.filename = "Particle/smoke.png";
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
