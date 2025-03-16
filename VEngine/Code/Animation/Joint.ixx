#include <DirectXMath.h>

export module Animation.Joint;

import <string>;

export struct Joint
{
	void SetName(const char* name_)
	{
		strcpy_s(name, sizeof(name), name_);
	}

	typedef int JointIndex;

	// -1 is the root joint's index or if the joint doesn't have a parent.
	inline const static int ROOT_JOINT_INDEX = -1;

	// -1 is the lowest index a joint can have, -2 will be invalid.
	inline const static int INVALID_JOINT_INDEX = -2;

	DirectX::XMMATRIX currentPose = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX inverseBindPose = DirectX::XMMatrixIdentity();

	char name[32]{};

	JointIndex parentIndex = ROOT_JOINT_INDEX;
	JointIndex index = INVALID_JOINT_INDEX;
};
