export module Core.Transform;

import <DirectXMath.h>;

export struct Transform
{
	DirectX::XMFLOAT4 rotation; //Quaternion
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;

	Transform();
	DirectX::XMMATRIX GetAffine() const;
	DirectX::XMMATRIX GetAffineRotationOrigin(DirectX::XMVECTOR rotOrigin) const;
	void Decompose(DirectX::XMMATRIX m);
};

Transform::Transform()
{
	scale = DirectX::XMFLOAT3(1.f, 1.f, 1.f);
	rotation = DirectX::XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	position = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
}

DirectX::XMMATRIX Transform::GetAffine() const
{
	return DirectX::XMMatrixAffineTransformation(
		DirectX::XMLoadFloat3(&scale),
		DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f),
		DirectX::XMLoadFloat4(&rotation),
		DirectX::XMLoadFloat3(&position)
	);
}

//Helps with rotating child actors around their parent using Quaternions
DirectX::XMMATRIX Transform::GetAffineRotationOrigin(DirectX::XMVECTOR rotOrigin) const
{
	return DirectX::XMMatrixAffineTransformation(
		DirectX::XMLoadFloat3(&scale),
		rotOrigin,
		DirectX::XMLoadFloat4(&rotation),
		DirectX::XMLoadFloat3(&position)
	);
}

void Transform::Decompose(DirectX::XMMATRIX m)
{
	DirectX::XMVECTOR outScale, outQuat, outTrans;
	DirectX::XMMatrixDecompose(&outScale, &outQuat, &outTrans, m);

	DirectX::XMStoreFloat3(&scale, outScale);
	DirectX::XMStoreFloat4(&rotation, outQuat);
	DirectX::XMStoreFloat3(&position, outTrans);
}
