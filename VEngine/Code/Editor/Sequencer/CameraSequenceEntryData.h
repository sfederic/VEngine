#pragma once

#include "SequenceEntryData.h"
#include "SequenceEntryTypes.h"

class CameraSequenceEntryData : public SequenceEntryData
{
public:
	//The reference to the name of the CameraActor in the world.
	std::string cameraName;

	CameraSequenceEntryData() : SequenceEntryData((int)SequenceEntryTypes::Camera) {}
	void Activate() override {}
	void Deactivate() override {}
	Properties GetProps() override
	{
		auto props = __super::GetProps();
		props.Add("Camera", &cameraName);
		return props;
	}
};
