#pragma once
#include "Axis.h"
#include "Rotation.h"
#include "../Vector/Vec3f.h"

class AxisRotation
{
public:
	Axis RotationAxis;
	Rotation Rotate;
	Vec3f Origin;

	AxisRotation() noexcept : RotationAxis(Axis::Y), Rotate(Rotation::FromRadians(0)), Origin(Vec3f()) {}
	AxisRotation(Axis axis, Rotation rotation, Vec3f origin) noexcept : RotationAxis(axis), Rotate(rotation), Origin(origin) {}

	inline constexpr bool operator==(const AxisRotation& other) const noexcept {
		return RotationAxis == other.RotationAxis && Rotate == other.Rotate && Origin == other.Origin;
	}
};

