#include "vpch.h"
#include "Texture2D.h"
#include "RenderUtils.h"

Texture2D::~Texture2D()
{
	width = 0;
	height = 0;
}

void Texture2D::SetBufferNames()
{
	RenderUtils::SetResourceName(data.Get(), "Resource_" + filename);
	RenderUtils::SetResourceName(srv.Get(), "SRV_" + filename);
}
