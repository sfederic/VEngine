#include "vpch.h"
#include "MoveableActor.h"
#include "Core/VMath.h"

void MoveableActor::Start()
{
	nextPos = GetPositionV();
	nextRot = GetRotationV();
}

void MoveableActor::Tick(float deltaTime)
{
	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, 5.f));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, 5.f));
}
