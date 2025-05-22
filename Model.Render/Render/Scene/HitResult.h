#pragma once
#include "../Paint/Rgba.h"

struct HitResult
{
	bool Hit = false;
	Rgba Color = Rgba();

	constexpr HitResult(Rgba color) noexcept : Hit(true), Color(color) {}
	constexpr HitResult(bool hit, Rgba color) noexcept : Hit(hit), Color(color) {}
};