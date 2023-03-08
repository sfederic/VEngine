#pragma once

struct ShaderTimeData
{
	float deltaTime;
	float timeSinceStartup;
	float pad[2]; //D3D11 ConstantBuffers have to be a multiple of 16 bytes
};
