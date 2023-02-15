#pragma once

// -1 is the root joint's index or if the bone doesn't have a parent.
constexpr int ROOT_JOINT_INDEX = -1;

// -1 is the lowest index a joint can have, -2 will be invalid.
constexpr int INVALID_JOINT_INDEX = -2;

typedef int JointIndex;
