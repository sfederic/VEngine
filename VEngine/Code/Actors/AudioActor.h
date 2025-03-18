#include "Actors/ActorSystemDefine.h"

export module Actors.AudioActor;

import Actors.Actor;
import Actors.ActorSystem;
import Components.AudioComponent;
import Core.Properties;

export class AudioActor : public Actor
{
public:
	ACTOR_SYSTEM(AudioActor);

	AudioActor();
	Properties GetProps() override;

private:
	AudioComponent* audioComponent = nullptr;
};

AudioActor::AudioActor()
{
	audioComponent = AudioComponent::system.Add("Audio", this);
	SetRootComponent(audioComponent);
}

Properties AudioActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	return props;
}
