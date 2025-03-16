export module Asset.VertexColourData;

import Core.UID;
import <vector>;
import <DirectXMath.h>;

struct VertexColourData
{
	uint64_t numVertices = 0;
	UID meshComponentUID = 0;
	std::vector<DirectX::XMFLOAT4> colours;
};
