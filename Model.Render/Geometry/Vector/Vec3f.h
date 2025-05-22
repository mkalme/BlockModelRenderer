#pragma once
#include <cmath>
#include <functional>

class Vec3f
{
public:
	float X, Y, Z;
	
	constexpr Vec3f() noexcept : X(0), Y(0), Z(0) {}
	constexpr Vec3f(float value) noexcept : X(value), Y(value), Z(value) {}
	constexpr Vec3f(float x, float y, float z) noexcept : X(x), Y(y), Z(z) {}

	inline float GetLengthSquared() const noexcept {
		return X * X + Y * Y + Z * Z;
	}

	inline float GetLength() const noexcept {
		return std::sqrtf(GetLengthSquared());
	}

	inline constexpr float Dot(const Vec3f& other) const noexcept {
		return X * other.X + Y * other.Y + Z * other.Z;
	}

	inline constexpr Vec3f Cross(const Vec3f& other) const noexcept {
		return Vec3f(Y * other.Z - Z * other.Y, Z * other.X - X * other.Z, X * other.Y - Y * other.X);
	}

	inline Vec3f& Normalize() noexcept {
		float length = GetLength();
		if (length > 0) {
			X /= length;
			Y /= length;
			Z /= length;
		}

		return *this;
	}

	inline Vec3f Clamp(float min, float max) noexcept {
		X = std::fminf(std::fmaxf(min, X), max);
		Y = std::fminf(std::fmaxf(min, Y), max);
		Z = std::fminf(std::fmaxf(min, Z), max);

		return *this;
	}
	
	inline constexpr static Vec3f MinVector(const Vec3f& a, const Vec3f& b) noexcept {
		return Vec3f(std::fminf(a.X, b.X), std::fminf(a.Y, b.Y), std::fminf(a.Z, b.Z));
	}

	inline constexpr static Vec3f MaxVector(const Vec3f& a, const Vec3f& b) noexcept {
		return Vec3f(std::fmaxf(a.X, b.X), std::fmaxf(a.Y, b.Y), std::fmaxf(a.Z, b.Z));
	}

	inline constexpr Vec3f& operator+=(const Vec3f& other) noexcept {
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	}
	inline constexpr Vec3f& operator+=(const float value) noexcept {
		X += value;
		Y += value;
		Z += value;
		return *this;
	}
	inline constexpr Vec3f operator+(const Vec3f& other) const noexcept {
		return Vec3f(X + other.X, Y + other.Y, Z + other.Z);
	}
	inline constexpr Vec3f operator+(float value) const noexcept {
		return Vec3f(X + value, Y + value, Z + value);
	}
	inline constexpr friend Vec3f operator+(float value, const Vec3f& vector) noexcept {
		return vector + value;
	}

	inline constexpr Vec3f& operator-=(const Vec3f& other) noexcept {
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	}
	inline constexpr Vec3f& operator-=(const float value) noexcept {
		X -= value;
		Y -= value;
		Z -= value;
		return *this;
	}
	inline constexpr Vec3f operator-(const Vec3f& other) const noexcept {
		return Vec3f(X - other.X, Y - other.Y, Z - other.Z);
	}
	inline constexpr Vec3f operator-(float value) const noexcept {
		return Vec3f(X - value, Y - value, Z - value);
	}

	inline constexpr Vec3f& operator*=(const Vec3f& other) noexcept {
		X *= other.X;
		Y *= other.Y;
		Z *= other.Z;
		return *this;
	}
	inline constexpr Vec3f& operator*=(const float value) noexcept {
		X *= value;
		Y *= value;
		Z *= value;
		return *this;
	}
	inline constexpr Vec3f operator*(const Vec3f& other) const noexcept {
		return Vec3f(X * other.X, Y * other.Y, Z * other.Z);
	}
	inline constexpr Vec3f operator*(float value) const noexcept {
		return Vec3f(X * value, Y * value, Z * value);
	}
	inline constexpr friend Vec3f operator*(float value, const Vec3f& vector) noexcept {
		return vector * value;
	}

	inline constexpr Vec3f& operator/=(const Vec3f& other) {
		X /= other.X;
		Y /= other.Y;
		Z /= other.Z;
		return *this;
	}
	inline constexpr Vec3f& operator/=(const float value) {
		X /= value;
		Y /= value;
		Z /= value;
		return *this;
	}
	inline constexpr Vec3f operator/(const Vec3f& other) const {
		return Vec3f(X / other.X, Y / other.Y, Z / other.Z);
	}
	inline constexpr Vec3f operator/(float value) const noexcept {
		return Vec3f(X / value, Y / value, Z / value);
	}
	inline constexpr friend Vec3f operator/(float value, const Vec3f& vector) noexcept {
		return Vec3f(value / vector.X, value / vector.Y, value / vector.Z);
	}

	inline constexpr bool operator==(const Vec3f& other) const noexcept {
		return X == X && Y == Y && Z == Z;
	}

	inline constexpr operator Vec3f() const noexcept {
		return Vec3f(X, X, X);
	}
};

namespace std {
	template <>
	struct hash<Vec3f> {
		std::size_t operator()(const Vec3f& vec) const noexcept {
			std::size_t hashX = std::hash<float>{}(vec.X);
			std::size_t hashY = std::hash<float>{}(vec.Y);
			std::size_t hashZ = std::hash<float>{}(vec.Z);
			return hashX ^ (hashY << 1) ^ (hashZ << 2);
		}
	};
}