#include "BoxTriggerComponent.h"
#include "DebugBox.h"

void BoxTriggerComponent::Tick(float deltaTime)
{

}

void BoxTriggerComponent::Start()
{
	Transform t;
	t.position = XMFLOAT3(0.f, 2.f, 0.f);
	box = debugBox.SpawnActor(t);
}
