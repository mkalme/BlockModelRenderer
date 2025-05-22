#pragma once
#include "../../Geometry/Vector/Vec3f.h"

class Rgba
{
public:
	Vec3f Rgb;
	float A;
	float Tint;

	constexpr Rgba() noexcept : Rgb(Vec3f()), A(0), Tint(0) {}
	constexpr Rgba(Vec3f rgb, float a) noexcept : Rgb(rgb), A(a), Tint(0) {}
	constexpr Rgba(Vec3f rgb, float a, float tint) noexcept : Rgb(rgb), A(a), Tint(tint) {}

	inline constexpr void CombineTowardsTop(const Rgba& other) noexcept {
		if (other.A == 0) return;

		Rgb = Rgb * (1 - other.A) + other.Rgb * other.A;
		A += (1 - A) * other.A;
		Tint = Tint * (1 - other.A) * other.Tint * other.A;
	}

	inline constexpr void CombineTowardsBehind(const Rgba& other) noexcept {
		if (A == 1 || other.A == 0) return;
		if (A == 0) {
			Rgb = other.Rgb;
			A = other.A;
			Tint = other.Tint;
		}
		else {
			Rgb = other.Rgb * (1 - A) + Rgb * A;
			A += (1 - A) * other.A;
			Tint = Tint * (1 - other.A) * other.Tint * other.A;
		}
	}

	inline constexpr void Mix(const Rgba& other, float delta) noexcept {
		if (delta == 0) return;
		if (delta == 1) {
			Rgb = other.Rgb;
			A = other.A;
			return;
		}

		Rgb = Rgb * (1 - delta) + other.Rgb * delta;
		A = A * (1 - delta) + other.A * delta;
		Tint = Tint * (1 - delta) * other.Tint * delta;
	}

	inline constexpr bool operator==(const Rgba& other) const noexcept {
		return Rgb == other.Rgb && A == other.A;
	}
};