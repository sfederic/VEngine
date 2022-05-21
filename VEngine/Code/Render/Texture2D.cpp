#include "Texture2D.h"
#include <d3d11.h>

Texture2D::Texture2D(std::string filename_)
{
	filename = filename_;
}

Texture2D::~Texture2D()
{
	data->Release();
	srv->Release();
}
