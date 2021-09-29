#include "RenderTypes.h"
#include "Material.h"
#include "Camera.h"

void ShaderMatrices::Create(float aspectRatio)
{
	model = XMMatrixIdentity();
	view = XMMatrixIdentity();
	texMatrix = XMMatrixIdentity();
	proj = activeCamera->proj;

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
