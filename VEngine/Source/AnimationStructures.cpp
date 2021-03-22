#include "AnimationStructures.h"

using namespace DirectX;

void BoneAnimation::Interpolate(float t, DirectX::XMFLOAT4X4& m)
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
			XMStoreFloat4x4(&m, XMMatrixAffineTransformation(lerpedScale, zero, lerpedRot, lerpedPos));

			return;
		}
	}
}

float AnimationClip::GetStartClipTime()
{
	float startTime = 0.f;
	float tempStartTime = 0.f;
	for (int i = 0; i < boneAnimations.size(); i++)
	{
		tempStartTime = boneAnimations[i].GetStartTime();
		if (tempStartTime >= startTime)
		{
			startTime = tempStartTime;
		}
	}

	return startTime;
}

float AnimationClip::GetEndClipTime()
{
	float endTime = 0.f;
	float tempEndTime = 0.f;
	for (int i = 0; i < boneAnimations.size(); i++)
	{
		tempEndTime = boneAnimations[i].GetEndTime();
		if (tempEndTime >= endTime)
		{
			endTime = tempEndTime;
		}
	}

	return endTime;
}

void AnimationClip::Interpolate(float t, std::vector<DirectX::XMFLOAT4X4>& boneTransforms)
{
	assert(boneTransforms.size() < 96); //TODO: clean this magic number up

	for (int i = 0; i < boneAnimations.size(); i++)
	{
		boneAnimations[i].Interpolate(t, boneTransforms[i]);
	}
}

void SkinnedData::GetFinalTransforms(const std::string& clipName, float timePos, std::vector<XMFLOAT4X4>& finalTransforms)
{
	uint32_t numBones = boneOffsets.size();
	std::vector<XMFLOAT4X4> toParentTransforms(numBones);

	auto clip = animationClips.find(clipName)->second;
	clip.Interpolate(timePos, toParentTransforms);

	std::vector<XMFLOAT4X4> toRootTransforms(numBones);
	toRootTransforms[0] = toParentTransforms[0];

	for (int i = 1; i < numBones; i++)
	{
		XMMATRIX toParent = XMLoadFloat4x4(&toParentTransforms[i]);
		int parentIndex = boneHierarchy[i];
		XMMATRIX parentToRoot = XMLoadFloat4x4(&toRootTransforms[parentIndex]);
		XMMATRIX toRoot = XMMatrixMultiply(toParent, parentToRoot);
		XMStoreFloat4x4(&toRootTransforms[i], toRoot);
	}

	for (int i = 0; i < numBones; i++) 
	{
		XMMATRIX offset = XMLoadFloat4x4(&boneOffsets[i]);
		XMMATRIX toRoot = XMLoadFloat4x4(&toRootTransforms[i]);
		XMStoreFloat4x4(&finalTransforms[i], XMMatrixMultiply(offset, toRoot));
	}
}
