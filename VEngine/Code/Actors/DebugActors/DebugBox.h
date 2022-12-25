#pragma once

//There's only one DebugBox in the engine. Its Mesh's transform becomes each SpatialComponents bounds over a loop.
struct DebugBox
{
	class MeshComponent* boxMesh;

	DebugBox();
};
