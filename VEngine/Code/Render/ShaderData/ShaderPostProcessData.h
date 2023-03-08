#pragma once

struct ShaderPostProcessData
{
	float exposure = 0.67f;
	float linearAngle = 0.001f;
	float linearStrength = 2.2f;
	float shoulderStrength = 0.19f;
	float toeDenominator = 0.33f;
	float toeNumerator = 0.012f;
	float toeStrenth = 2.0f;
	float linearWhitePointValue = 2.0f;
};
