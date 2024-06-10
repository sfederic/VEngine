#pragma once

#include <DirectXMath.h>

struct ShaderLightProbeData
{
	DirectX::XMFLOAT4 SH[9]{};
	int isDiffuseProbeMapActive = false;

private:
	float padding[3];
};
