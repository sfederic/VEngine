#include "vpch.h"
#include "CorkscrewLift.h"
#include "Core/VMath.h"

void CorkscrewLift::Create()
{
	__super::Create();

	canBeMovedInLink = false;
	canBeRotatedPitchXAxis = false;
	canBeRotatedRollZAxis = false;
	canFall = false;
}

void CorkscrewLift::OnLinkRotateRight()
{
	__super::OnLinkRotateRight();

	if (rotationIncrementIndex < maxRotationIncrement)
	{
		rotationIncrementIndex++;

		const auto pos = GetPositionV() + VMath::GlobalUpVector();
		SetNextPos(pos);
	}
	else
	{
		ResetNextRot();
	}
}

void CorkscrewLift::OnLinkRotateLeft()
{
	__super::OnLinkRotateLeft();

	if (rotationIncrementIndex > minRotationIncrement)
	{
		rotationIncrementIndex--;

		const auto pos = GetPositionV() - VMath::GlobalUpVector();
		SetNextPos(pos);
	}
	else
	{
		ResetNextRot();
	}
}

Properties CorkscrewLift::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Min Rotation Count", &minRotationIncrement);
	props.Add("Max Rotation Count", &maxRotationIncrement);
	return props;
}
