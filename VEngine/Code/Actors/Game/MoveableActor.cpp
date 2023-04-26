#include "vpch.h"
#include "MoveableActor.h"
#include "Core/VMath.h"

void MoveableActor::Start()
{
	__super::Start();

	nextPos = GetPositionV();
	nextRot = GetRotationV();
}

void MoveableActor::Tick(float deltaTime)
{
	__super::Tick(deltaTime);

	SetPosition(VMath::VectorConstantLerp(GetPositionV(), nextPos, deltaTime, 10.f));
	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, 10.f));
}

bool MoveableActor::HaveMovementAndRotationStopped()
{
	return XMVector4Equal(GetPositionV(), nextPos) && XMQuaternionEqual(GetRotationV(), nextRot);
}
