#pragma once

struct ID3D11ShaderResourceView;

class ShaderResourceView
{
public:
	void Create();

	ID3D11ShaderResourceView* data;
};
