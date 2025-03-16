
#include "DebugLightProbe.h"
#include "Components/InstanceMeshComponent.h"

DebugLightProbe::DebugLightProbe(int probeCount)
{
	instanceMesh = new InstanceMeshComponent(probeCount, "ico_sphere.vmesh", "test.png", "LightProbe");
	instanceMesh->SetCollisionLayer(CollisionLayers::Editor);
	instanceMesh->Create();
}
