#include "Transform.h"

Transform::Transform()
{
	scale = XMFLOAT3(1.f, 1.f, 1.f);
	euler = XMFLOAT3(0.f, 0.f, 0.f);
	quatRotation = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	position = XMFLOAT3(0.f, 0.f, 0.f);
}

XMMATRIX Transform::GetAffine()
{
	return XMMatrixAffineTransformation(
		XMLoadFloat3(&scale),
		XMVectorSet(0.f, 0.f, 0.f, 1.f),
		XMLoadFloat4(&quatRotation),
		XMLoadFloat3(&position)
	);
}

void Transform::Decompose(XMMATRIX m)
{
	XMVECTOR outScale, outQuat, outTrans;
	XMMatrixDecompose(&outScale, &outQuat, &outTrans, m);

	XMStoreFloat3(&scale, outScale);
	XMStoreFloat4(&quatRotation, outQuat);
	XMStoreFloat3(&position, outTrans);
}
