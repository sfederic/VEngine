#include "RenderTypes.h"
#include "MathHelpers.h"

bool ModelData::CheckForDuplicateVertices(Vertex& vert)
{
	auto pos = XMLoadFloat3(&vert.pos);

	const int size = verts.size();
	for (int i = 0; i < size; i++)
	{
		XMVECTOR p = XMLoadFloat3(&verts[i].pos);
		if (VecEqual(p, pos, 0.0f))
		{
			indices.push_back(i);
			return true;
		}
	}

	return false;
}