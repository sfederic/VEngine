#include "AnimationStructures.h"

void Animation::Interpolate(float t, DirectX::XMMATRIX& m)
{
	for (int i = 0; i < (frames.size() - 1); i++)
	{
		if (t >= frames[i].time && t <= frames[i + 1].time)
		{
			//Get the interpolation difference between input time and frame time.
			float lerpPercent = (t - frames[i].time) / (frames[i + 1].time - frames[i].time);

			XMVECTOR pos1 = XMLoadFloat3(&frames[i].pos);
			XMVECTOR pos2 = XMLoadFloat3(&frames[i + 1].pos);

			XMVECTOR scale1 = XMLoadFloat3(&frames[i].scale);
			XMVECTOR scale2 = XMLoadFloat3(&frames[i + 1].scale);

			XMVECTOR rot1 = XMLoadFloat4(&frames[i].rot);
			XMVECTOR rot2 = XMLoadFloat4(&frames[i + 1].rot);

			XMVECTOR lerpedPos = XMVectorLerp(pos1, pos2, lerpPercent);
			XMVECTOR lerpedScale = XMVectorLerp(scale1, scale2, lerpPercent);
			XMVECTOR lerpedRot = XMQuaternionSlerp(rot1, rot2, lerpPercent);
			XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
			m = XMMatrixAffineTransformation(lerpedScale, zero, lerpedRot, lerpedPos);

			return;
		}
	}
}

Joint* Skeleton::FindJoint(std::string name)
{
	for (int i = 0; i < joints.size(); i++)
	{
		if (joints[i].name == name)
		{
			return &joints[i];
		}
	}

	return nullptr;
}
