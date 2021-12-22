#include "RenderTypes.h"
#include "Material.h"
#include "Camera.h"

void ShaderMatrices::Create()
{
	model = XMMatrixIdentity();
	view = XMMatrixIdentity();
	texMatrix = XMMatrixIdentity();
	proj = activeCamera->GetProjectionMatrix();

	MakeModelViewProjectionMatrix();
}

void ShaderMatrices::MakeTextureMatrix(MaterialShaderData* shaderData)
{
	XMVECTOR offset = XMLoadFloat2(&shaderData->uvOffset);
	XMVECTOR scale = XMLoadFloat2(&shaderData->uvScale);

	texMatrix = XMMatrixAffineTransformation2D(scale, XMVectorSet(0.f, 0.f, 0.f, 1.f),
		shaderData->uvRotation, offset);
}

void ShaderMatrices::MakeModelViewProjectionMatrix()
{
	mvp = model * view * proj;
}

Skeleton MeshData::CreateSkeletonFromExistingData()
{
	Skeleton newSkeleton = {};
	newSkeleton.joints = skeleton.joints;
	newSkeleton.animationNames = skeleton.animationNames;
	newSkeleton.currentAnimation = skeleton.currentAnimation;

	//reset some joint data
	for (Joint& joint : newSkeleton.joints)
	{
		joint.currentPose = joint.inverseBindPose;
		
		for (auto& anim : joint.anim)
		{
			anim.second.currentTime = 0.f;
		}
	}

	return newSkeleton;
}
