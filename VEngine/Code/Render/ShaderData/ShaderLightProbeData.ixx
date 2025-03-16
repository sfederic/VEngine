#include <DirectXMath.h>

export module Render.Shaderdata.ShaderLightProbeData;

export struct ShaderLightProbeData
{
	DirectX::XMFLOAT4 SH[9]{};
	int isDiffuseProbeMapActive = false;

private:
	float padding[3];
};
