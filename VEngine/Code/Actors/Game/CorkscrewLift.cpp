#include "vpch.h"
#include "CorkscrewLift.h"
#include "Core/VMath.h"

void CorkscrewLift::OnLinkRotateRight()
{
	__super::OnLinkRotateRight();

	const auto pos = GetPositionV() + VMath::GlobalUpVector();
	SetNextPos(pos);
}

void CorkscrewLift::OnLinkRotateLeft()
{
	__super::OnLinkRotateLeft();

	const auto pos = GetPositionV() - VMath::GlobalUpVector();
	SetNextPos(pos);
}
