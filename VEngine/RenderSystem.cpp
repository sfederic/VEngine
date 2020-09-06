#include "RenderSystem.h"
#include "D3D11RenderSystem.h"
#include "D3D12RenderSystem.h"

#ifdef D3D11_RENDERER
IRenderSystem* gRenderSystem = new D3D11RenderSystem();
#elif D3D12_RENDERER
IRenderSystem* gRenderSystem = new D3D12RenderSystem();
#endif
