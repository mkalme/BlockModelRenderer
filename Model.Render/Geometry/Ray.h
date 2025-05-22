#pragma once
#include "Vector/Vec3f.h"

class Ray
{
public:
	Vec3f Origin, Direction;

	constexpr Ray() noexcept : Origin(Vec3f()), Direction(Vec3f()) {}
	constexpr Ray(Vec3f origin, Vec3f direction) noexcept : Origin(origin), Direction(direction) {}
};