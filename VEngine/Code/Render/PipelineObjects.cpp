#include "PipelineObjects.h"
#include "Renderer.h"

PipelineStateObject::PipelineStateObject()
{
	rastState.data = renderer.rastStateSolid;
}