export module Core.Transform;

#include <DirectXMath.h>

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
