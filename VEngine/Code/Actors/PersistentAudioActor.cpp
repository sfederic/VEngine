
#include "PersistentAudioActor.h"
#include "Components/PersistentAudioComponent.h"

PersistentAudioActor::PersistentAudioActor()
{
	persistentAudio = CreateComponent<PersistentAudioComponent>("PeristentAudio");
	SetRootComponent(persistentAudio);
}

Properties PersistentAudioActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
