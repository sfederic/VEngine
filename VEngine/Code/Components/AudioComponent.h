#pragma once

#include "SpatialComponent.h"
#include "ComponentSystem.h"

class AudioComponent : public SpatialComponent
{
public:
	COMPONENT_SYSTEM(AudioComponent);

	enum class FadeValue
	{
		None,
		In,
		Out
	};

	AudioComponent();
	void Tick(float deltaTime) override;
	void Start() override;
	void Create() override;
	Properties GetProps() override;

	void SetToFadeIn() { fade = FadeValue::In; }
	void SetToFadeOut() { fade = FadeValue::Out; }

	void Play();
	void Stop();

	void SetVolume(float volume_) { volume = volume_; }
	auto GetVolume() { return volume; }

	auto GetChannelID() { return channelID; }

private:
	std::string audioFilename;

	FadeValue fade = FadeValue::None;

	uint64_t channelID = 0;

	float volume = 1.0f;
	float pitch = 1.0f;

	bool playOnStart = true;
	bool loop = true;
};
