#include "RenderSystem.h"
#include "D3D11RenderSystem.h"

IRenderSystem* gRenderSystem = new D3D11RenderSystem();