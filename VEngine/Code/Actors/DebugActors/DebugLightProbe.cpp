#include "vpch.h"
#include "DebugLightProbe.h"
#include "Components/InstanceMeshComponent.h"

DebugLightProbe::DebugLightProbe(int probeCount)
{
	instanceMesh = new InstanceMeshComponent(probeCount, "sphere.vmesh", "test.png", ShaderItems::LightProbe);
	instanceMesh->SetCollisionLayer(CollisionLayers::Editor);
	instanceMesh->Create();

	//Don't need these, DiffuseProbeMap has its own D3D objects
	instanceMesh->srv->Release();
	instanceMesh->structuredBuffer->Release();
}
