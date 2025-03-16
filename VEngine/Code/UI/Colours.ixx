#include <d2d1_1.h>

export module UI.Colours;

export namespace Colours
{
	constexpr D2D1_COLOR_F White = { 1.f, 1.f, 1.f, 1.f };
	constexpr D2D1_COLOR_F Black = { 0.f, 0.f, 0.f, 1.f };
	constexpr D2D1_COLOR_F Grey = { 0.5f, 0.5f, 0.5f, 1.f };
	constexpr D2D1_COLOR_F Green = { 0.f, 1.f, 0.f, 1.f };
	constexpr D2D1_COLOR_F Magneta = { 1.f, 0.f, 1.f, 1.f };
}
