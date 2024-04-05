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
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void SetAudioFilename(std::string_view filename) { audioFilename = filename; }

	void SetLoop(bool value) { loop = value; }

	void SetToFadeIn() { fade = FadeValue::In; }
	void SetToFadeOut() { fade = FadeValue::Out; }

	void Play();
	void Stop();

	void SetVolume(float volume_) { volume = volume_; }
	auto GetVolume() const { return volume; }

	auto GetChannelID() const { return channelID; }

	auto GetVolumeAttenuation() const { return volumeAttenuation; }

protected:
	void SetVolumeToPlayerPositionAgainstRadius();

	std::string audioFilename;

	FadeValue fade = FadeValue::None;

	uint64_t channelID = 0;

	float volume = 1.0f;
	float volumeAttenuation = 5.f;
	float pitch = 1.0f;

	bool playOnStart = true;
	bool loop = true;
};
