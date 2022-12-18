#pragma once

#include <string>

struct GameplayTags
{
	//For all Destroy()'able meshes on an Enemy. When all are gone, destroy the Enemy.
	inline static const std::string EnemyMeshPiece = "EnemyMeshPiece";
};
