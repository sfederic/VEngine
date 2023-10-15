#include "vpch.h"
#include "Texture2D.h"

Texture2D::~Texture2D()
{
	if (data)
	{
		data->Release();
		data = nullptr;
	}

	if (srv)
	{
		srv->Release();
		srv = nullptr;
	}

	width = 0;
	height = 0;
}
