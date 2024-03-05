#include "vpch.h"
#include "DebugLightProbe.h"
#include "Components/InstanceMeshComponent.h"

DebugLightProbe::DebugLightProbe(int probeCount)
{
	instanceMesh = new InstanceMeshComponent(probeCount, "ico_sphere.vmesh", "test.png", ShaderItems::LightProbe);
	instanceMesh->SetCollisionLayer(CollisionLayers::Editor);
	instanceMesh->Create();
}
