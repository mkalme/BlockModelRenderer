#pragma once
#include <cmath>
#include <functional>

class Vec2f
{
public:
	float X, Y;

	constexpr Vec2f() noexcept : X(0), Y(0) {}
	constexpr Vec2f(float value) noexcept : X(value), Y(value) {}
	constexpr Vec2f(float x, float y) noexcept : X(x), Y(y) {}

	inline float GetLengthSquared() const noexcept {
		return X * X + Y * Y;
	}

	inline float GetLength() const noexcept {
		return std::sqrt(GetLengthSquared());
	}

	inline constexpr float Dot(const Vec2f& other) const noexcept {
		return X * other.X + Y * other.Y;
	}

	inline Vec2f& Normalize() noexcept {
		float length = GetLength();
		if (length > 0) {
			X /= length;
			Y /= length;
		}

		return *this;
	}

	inline Vec2f Clamp(float min, float max) noexcept {
		X = std::fminf(std::fmaxf(min, X), max);
		Y = std::fminf(std::fmaxf(min, Y), max);

		return *this;
	}

	inline constexpr static Vec2f MinVector(const Vec2f& a, const Vec2f& b) noexcept {
		return Vec2f(std::fminf(a.X, b.X), std::fminf(a.Y, b.Y));
	}

	inline constexpr static Vec2f MaxVector(const Vec2f& a, const Vec2f& b) noexcept {
		return Vec2f(std::fmaxf(a.X, b.X), std::fmaxf(a.Y, b.Y));
	}

	inline constexpr Vec2f& operator+=(const Vec2f& other) noexcept {
		X += other.X;
		Y += other.Y;
		return *this;
	}
	inline constexpr Vec2f& operator+=(const float value) noexcept {
		X += value;
		Y += value;
		return *this;
	}
	inline constexpr Vec2f operator+(const Vec2f& other) const noexcept {
		return Vec2f(X + other.X, Y + other.Y);
	}
	inline constexpr Vec2f operator+(float value) const noexcept {
		return Vec2f(X + value, Y + value);
	}
	inline constexpr friend Vec2f operator+(float value, const Vec2f& vector) noexcept {
		return vector + value;
	}

	inline constexpr Vec2f& operator-=(const Vec2f& other) noexcept {
		X -= other.X;
		Y -= other.Y;
		return *this;
	}
	inline constexpr Vec2f& operator-=(const float value) noexcept {
		X -= value;
		Y -= value;
		return *this;
	}
	inline constexpr Vec2f operator-(const Vec2f& other) const noexcept {
		return Vec2f(X - other.X, Y - other.Y);
	}
	inline constexpr Vec2f operator-(float value) const noexcept {
		return Vec2f(X - value, Y - value);
	}

	inline constexpr Vec2f& operator*=(const Vec2f& other) noexcept {
		X *= other.X;
		Y *= other.Y;
		return *this;
	}
	inline constexpr Vec2f& operator*=(const float value) noexcept {
		X *= value;
		Y *= value;
		return *this;
	}
	inline constexpr Vec2f operator*(const Vec2f& other) const noexcept {
		return Vec2f(X * other.X, Y * other.Y);
	}
	inline constexpr Vec2f operator*(float value) const noexcept {
		return Vec2f(X * value, Y * value);
	}
	inline constexpr friend Vec2f operator*(float value, const Vec2f& vector) noexcept {
		return vector * value;
	}

	inline constexpr Vec2f& operator/=(const Vec2f& other) {
		X /= other.X;
		Y /= other.Y;
		return *this;
	}
	inline constexpr Vec2f& operator/=(const float value) {
		X /= value;
		Y /= value;
		return *this;
	}
	inline constexpr Vec2f operator/(const Vec2f& other) const {
		return Vec2f(X / other.X, Y / other.Y);
	}
	inline constexpr Vec2f operator/(float value) const noexcept {
		return Vec2f(X / value, Y / value);
	}
	inline constexpr friend Vec2f operator/(float value, const Vec2f& vector) noexcept {
		return Vec2f(value / vector.X, value / vector.Y);
	}

	inline constexpr bool operator==(const Vec2f& other) const noexcept {
		return X == X && Y == Y;
	}

	inline constexpr operator Vec2f() const noexcept {
		return Vec2f(X, X);
	}
};

namespace std {
	template <>
	struct hash<Vec2f> {
		std::size_t operator()(const Vec2f& vec) const noexcept {
			std::size_t hashX = std::hash<float>{}(vec.X);
			std::size_t hashY = std::hash<float>{}(vec.Y);
			return hashX ^ (hashY << 1);
		}
	};
}