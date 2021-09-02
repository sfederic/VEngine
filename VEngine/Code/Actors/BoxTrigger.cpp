#include "BoxTrigger.h"
#include "Components/MeshComponent.h"
#include "Render/Renderer.h"

BoxTrigger::BoxTrigger()
{
	boxMesh = MeshComponent::system.Add(this, MeshComponent("cube.fbx"));
	rootComponent = boxMesh;

	//set mesh to wireframe for debugging purposes
	boxMesh->pso->rastState = renderer.rastStateMap["wireframe"];
}

void BoxTrigger::Tick(double deltaTime)
{
	__super::Tick(deltaTime);


}

Properties BoxTrigger::GetProps()
{
	return Actor::GetProps();
}
