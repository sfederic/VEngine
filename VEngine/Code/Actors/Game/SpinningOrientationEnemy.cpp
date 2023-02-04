#include "vpch.h"
#include "SpinningOrientationEnemy.h"
#include "Core/VMath.h"

void SpinningOrientationEnemy::Start()
{
	nextRot = GetRotationV();
}

void SpinningOrientationEnemy::Tick(float deltaTime)
{
	spinTimer += deltaTime;
	if (spinTimer > 1.0f)
	{
		spinTimer = 0.f;

		std::vector<XMVECTOR> directions{ GetUpVectorV(), GetRightVectorV(), GetUpVectorV(),
				-GetUpVectorV(), -GetRightVectorV(), -GetUpVectorV() };
		int randomDirectionIndex = VMath::RandomRangeInt(0, directions.size() - 1);

		std::vector<float> angles{ 90.f, 180.f, 270.f, -90.f, -180.f, -270.f };
		int randomAngleIndex = VMath::RandomRangeInt(0, angles.size() - 1);

		nextRot = XMQuaternionMultiply(GetRotationV(),
			DirectX::XMQuaternionRotationAxis(directions[randomDirectionIndex], XMConvertToRadians(angles[randomAngleIndex])));
	}

	SetRotation(VMath::QuatConstantLerp(GetRotationV(), nextRot, deltaTime, 20.f));
}
