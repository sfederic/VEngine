#include "vpch.h"
#include "Texture2D.h"

Texture2D::~Texture2D()
{
	data->Release();
	data = nullptr;

	srv->Release();
	srv = nullptr;

	width = 0;
	height = 0;
}
