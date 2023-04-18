#include "vpch.h"
#include "DebugLightProbe.h"
#include "Components/InstanceMeshComponent.h"

DebugLightProbe::DebugLightProbe(int probeCount)
{
	instanceMesh = new InstanceMeshComponent(probeCount, "sphere.vmesh", "test.png", ShaderItems::Instance);
	instanceMesh->SetCollisionLayer(CollisionLayers::Editor);
}
