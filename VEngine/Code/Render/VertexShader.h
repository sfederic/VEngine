#pragma once

#include <vector>

struct ID3D11VertexShader;

struct VertexShader
{
	std::vector<uint8_t> code;
	ID3D11VertexShader* shader = nullptr;
};
