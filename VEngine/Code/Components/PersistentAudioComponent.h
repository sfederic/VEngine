#pragma once

#include "AudioComponent.h"

//"persistent audio" is a singular track that plays continuously until overriden.
class PersistentAudioComponent : public AudioComponent
{
public:
	COMPONENT_SYSTEM(PersistentAudioComponent);

	void Start() override;
	Properties GetProps() override;
};
