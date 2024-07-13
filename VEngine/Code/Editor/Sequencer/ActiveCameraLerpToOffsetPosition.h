#pragma once

#include "SequenceEntryData.h"
#include <DirectXMath.h>

class ActiveCameraLerpToOffsetPosition : public SequenceEntryData
{
private:
	DirectX::XMFLOAT3 originalCameraPosition = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
	DirectX::XMFLOAT3 positionToLerpTo = DirectX::XMFLOAT3(0.f, 0.f, 0.f);

public:
	ActiveCameraLerpToOffsetPosition() :
		SequenceEntryData((int)SequenceEntryTypes::ActiveCameraLerpToOffsetPosition) {}
	void Tick(float deltaTime) override;
	void Activate() override;
	void Deactivate() override;
	Properties GetProps() override
	{
		auto props = __super::GetProps();
		props.Add("PositionToLerpTo", &positionToLerpTo);
		return props;
	}

	void SetPositionToLerpTo();
};
