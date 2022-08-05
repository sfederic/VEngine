#pragma once

#include <vector>

struct ID3D11PixelShader;

struct PixelShader
{
	std::vector<uint8_t> code;
	ID3D11PixelShader* shader = nullptr;
};
