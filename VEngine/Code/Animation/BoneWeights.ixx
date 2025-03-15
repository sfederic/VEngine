export module Animation.BoneWeights;

import <vector>;

export struct BoneWeights
{
	inline static int MAX_WEIGHTS = 3;
	inline static int MAX_BONE_INDICES = 4;

	std::vector<double> weights;
	std::vector<int> boneIndex;
};
