#include "vpch.h"
#include "VerticalMoveGridActor.h"
#include "Core/VMath.h"

void VerticalMoveGridActor::Create()
{
	__super::Create();

	canBeRotatedRollZAxis = false;
	canBeRotatedPitchXAxis = false;

	canFall = false;
}

Properties VerticalMoveGridActor::GetProps()
{
	auto props = __super::GetProps();
	props.title = GetTypeName();
	props.Add("Movement Increment", &movementIncrement);
	return props;
}

void VerticalMoveGridActor::OnLinkMoveForward()
{
	__super::OnLinkMoveForward();

	ResetNextPos();
	AddNextPosition(VMath::GlobalUpVector() * movementIncrement);

	RecalcCurrentNodeDontIgnoreThis();
}

void VerticalMoveGridActor::OnLinkMoveBack()
{
	__super::OnLinkMoveBack();

	ResetNextPos();
	AddNextPosition(-VMath::GlobalUpVector() * movementIncrement);

	RecalcCurrentNodeDontIgnoreThis();
}

void VerticalMoveGridActor::OnLinkMoveLeft()
{
	__super::OnLinkMoveLeft();

	ResetNextPos();
}

void VerticalMoveGridActor::OnLinkMoveRight()
{
	__super::OnLinkMoveRight();

	ResetNextPos();
}
