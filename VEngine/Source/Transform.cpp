#include "Transform.h"

Transform::Transform()
{
	scale = XMFLOAT3(1.f, 1.f, 1.f);
	euler = XMFLOAT3(0.f, 0.f, 0.f);
	quatRotation = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	position = XMFLOAT3(0.f, 0.f, 0.f);
}