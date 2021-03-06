#pragma once

struct ID3D11RasterizerState;

class RasterizerState
{
public:
	virtual void Create();
	virtual void* Get();

	ID3D11RasterizerState* data;
};
