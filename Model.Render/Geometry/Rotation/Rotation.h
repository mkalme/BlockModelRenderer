#pragma once
#include <cmath>
#include "../Vector/Vec2f.h"
#include "../Vector/Vec3f.h"
#include "../Ray.h"
#include "Axis.h"

#define PI 3.141592653589793238462643383279502884

static float SIN_ZERO = std::sinf(0);
static float COS_ZERO = std::cosf(0);

class Rotation
{
public:
	float Sin, Cos;

	constexpr Rotation() noexcept : Sin(0), Cos(0) {}
	constexpr Rotation(float sin, float cos) noexcept : Sin(sin), Cos(cos) {}

	inline void Rotate(Vec2f& vector) const noexcept {
		if (!HasRotation()) return;

		float x = vector.X, y = vector.Y;

		vector.X = x * Cos - y * Sin;
		vector.Y = x * Sin + y * Cos;
	}

	inline void RotateNegative(Vec2f& vector) const noexcept {
		if (!HasRotation()) return;
		
		float x = vector.X, y = vector.Y;

		vector.X = x * Cos + y * Sin;
		vector.Y = x * -Sin + y * Cos;
	}

	inline void RotateWithAxis(Vec3f& vector, Axis axis) const noexcept {
		if (!HasRotation()) return;
		
		switch (axis) {
		case Axis::X: {
			Vec2f point(vector.Z, vector.Y);
			Rotate(point);

			vector.Y = point.Y;
			vector.Z = point.X;
		}
		break;
		case Axis::Y: {
			Vec2f point(vector.X, vector.Z);
			Rotate(point);

			vector.X = point.X;
			vector.Z = point.Y;
		}
		break;
		default: {
			Vec2f point(vector.Y, vector.X);
			Rotate(point);

			vector.X = point.Y;
			vector.Y = point.X;
		}
		break;
		}
	}

	inline void RotateWithAxis(Vec3f& vector, Axis axis, const Vec3f& origin) const noexcept {
		if (!HasRotation()) return;

		vector -= origin;
		RotateWithAxis(vector, axis);
		vector += origin;
	}

	inline void RotateWithAxis(Ray& ray, Axis axis, const Vec3f& origin) const noexcept {
		if (!HasRotation()) return;

		RotateWithAxis(ray.Origin, axis, origin);
		RotateWithAxis(ray.Direction, axis);
	}

	inline void RotateWithAxisNegative(Vec3f& vector, Axis axis) const noexcept {
		if (!HasRotation()) return;

		switch (axis) {
		case Axis::X: {
			Vec2f point(vector.Z, vector.Y);
			RotateNegative(point);

			vector.Y = point.Y;
			vector.Z = point.X;
		}
		break;
		case Axis::Y: {
			Vec2f point(vector.X, vector.Z);
			RotateNegative(point);

			vector.X = point.X;
			vector.Z = point.Y;
		}
		break;
		default: {
			Vec2f point(vector.Y, vector.X);
			RotateNegative(point);

			vector.X = point.Y;
			vector.Y = point.X;
		}
		break;
		}
	}

	inline void RotateWithAxisNegative(Vec3f& vector, Axis axis, const Vec3f& origin) const noexcept {
		if (!HasRotation()) return;

		vector -= origin;
		RotateWithAxisNegative(vector, axis);
		vector += origin;
	}

	inline void RotateWithAxisNegative(Ray& ray, Axis axis, const Vec3f& origin) const noexcept {
		if (!HasRotation()) return;

		RotateWithAxisNegative(ray.Origin, axis, origin);
		RotateWithAxisNegative(ray.Direction, axis);
	}

	inline bool HasRotation() const noexcept {
		return Sin != SIN_ZERO || Cos != COS_ZERO;
	}

	inline constexpr static Rotation FromRadians(float radians) noexcept {
		return Rotation(std::sinf(radians), std::cosf(radians));
	}

	inline constexpr static Rotation FromDegrees(float degrees) noexcept {
		return FromRadians(degrees / 180 * PI);
	}

	inline constexpr bool operator==(const Rotation& other) const noexcept {
		return Sin == other.Sin && Cos == other.Cos;
	}
};