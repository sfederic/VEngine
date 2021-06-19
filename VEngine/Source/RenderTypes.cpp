#include "RenderTypes.h"
#include "MathHelpers.h"

bool ModelData::CheckDuplicateVertices(Vertex& vert)
{
	auto pos = XMLoadFloat3(&vert.pos);

	const int size = verts.size();
	for (int i = 0; i < size; i++)
	{
		XMVECTOR p = XMLoadFloat3(&verts[i].pos);
		if (VecEqual(p, pos, 0.0f))
		{
			return true;
		}
	}

	return false;
}

//Duplicate checks for indices only return true if the index is present in the array
//more than once. Eg. For {2, 1, 0}, {3, 1, 2}, 2 and 1 are the duplicates.
bool ModelData::CheckDuplicateIndices(uint16_t index)
{
	int duplicateCounter = 0;

	for (int i = 0; i < indices.size(); i++)
	{
		if (index == indices[i])
		{
			duplicateCounter++;
			if(duplicateCounter >= 2)
			{
				return true;
			}
		}
	}

	return false;
}
