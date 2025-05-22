#pragma once

class Utilities
{
public:
	inline static void Swap(float& a, float& b) {
		float temp = a;
		a = b;
		b = temp;
	}

	inline static float Max(float a, float b) {
		return a > b ? a : b;
	}

	inline static float Min(float a, float b) {
		return a < b ? a : b;
	}

	inline static float Abs(float a) {
		return a < 0 ? -a : a;
	}

	inline static bool InBounds(float a, float min, float max) {
		return a >= min && a <= max;
	}
};

