#include <DirectXMath.h>

import Core.Transform;

using namespace DirectX;

Transform::Transform()
{
	scale = XMFLOAT3(1.f, 1.f, 1.f);
	rotation = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	position = XMFLOAT3(0.f, 0.f, 0.f);
}

XMMATRIX Transform::GetAffine() const
{
	return XMMatrixAffineTransformation(
		XMLoadFloat3(&scale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMLoadFloat4(&rotation),
		XMLoadFloat3(&position)
	);
}

//Helps with rotating child actors around their parent using Quaternions
XMMATRIX Transform::GetAffineRotationOrigin(XMVECTOR rotOrigin) const
{
	return XMMatrixAffineTransformation(
		XMLoadFloat3(&scale),
		rotOrigin,
		XMLoadFloat4(&rotation),
		XMLoadFloat3(&position)
	);
}

void Transform::Decompose(XMMATRIX m)
{
	XMVECTOR outScale, outQuat, outTrans;
	XMMatrixDecompose(&outScale, &outQuat, &outTrans, m);

	XMStoreFloat3(&scale, outScale);
	XMStoreFloat4(&rotation, outQuat);
	XMStoreFloat3(&position, outTrans);
}
