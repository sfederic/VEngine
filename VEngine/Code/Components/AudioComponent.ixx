#include "Components/ComponentSystemDefine.h"

export module Components.AudioComponent;

import Components.SpatialComponent;
import Components.ComponentSystem;
import Core.Properties;
import Core.UID;
import <string>;

export class AudioComponent : public SpatialComponent
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

	float GetVolumeAttenuation() const { return volumeAttenuation; }

	bool IsSpatialAudioEnabled() const { return spatialVolumeEnabled; }
	void SetSpatialAudioEnabled(bool value) { spatialVolumeEnabled = value; }

protected:
	void SetVolumeToPlayerPositionAgainstRadius();

	std::string audioFilename;

	FadeValue fade = FadeValue::None;

	UID channelID = 0;

	float volume = 1.0f;
	float pitch = 1.0f;

	float volumeAttenuation = 5.f;
	bool spatialVolumeEnabled = false;

	bool playOnStart = true;
	bool loop = true;
};
