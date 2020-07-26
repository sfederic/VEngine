#pragma once

#include "RenderSystem.h"
#include <vector>
#include <DirectXMath.h>
#include "Actor.h"

using namespace DirectX;

bool LoadOBJFile(const char* filename, ModelData& data);
