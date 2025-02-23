#pragma once

// This is for std::numeric_limits clashes with Windows.h
#define NOMINMAX

// Standard Library
#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <optional>
#include <functional>
#include <typeindex>
#include <map>
#include <unordered_map>
#include <set>

// Windows Headers
#include <Windows.h>
#include <wrl.h>

// DirectX Headers
#include <d3d11.h>
#include <d2d1_1.h>
#include <dwrite_1.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

// Qt Headers
#include <QWidget.h>
