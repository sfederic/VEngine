#pragma once

#include <vector>
#include "Render/RenderTypes.h"

class MeshComponent;
struct Transform;

//A part of a sliced mesh
class SplitMesh : public Actor
{
public:
	void Create(std::vector<Vertex>& meshVerts, const Transform& originalMeshTransform);

private:
	void Destroy();

	MeshComponent* mesh = nullptr;
};
