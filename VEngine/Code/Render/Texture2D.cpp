#include "vpch.h"
#include "Texture2D.h"

Texture2D::~Texture2D()
{
	data->Release();
	srv->Release();
}
