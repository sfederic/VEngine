#pragma once

//This is for std::numeric_limits clashes with Windows.h
#define NOMINMAX

#include <Windows.h>
#include <d3d11.h>
#include <d2d1_1.h>
#include <dwrite_1.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Actors/ActorSystem.h"
#include "Components/ComponentSystem.h"
#include "Core/Properties.h"
