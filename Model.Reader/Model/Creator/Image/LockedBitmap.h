#pragma once
#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include "../../External/stb_image.h"
#include "../Model.Render/Render/Paint/Rgba.h"
#include "../Model.Render/Geometry/Vector/Vec2f.h"

class LockedBitmap
{
public:
	LockedBitmap() {
		m_pixelData = 0;
		m_width = 0;
		m_height = 0;
	}
	LockedBitmap(const char* path) {
		int width, height;
		m_pixelData = stbi_load(path, &width, &height, NULL, 4);

		m_width = width;
		m_height = height;
	}

	Rgba ReadPixel(Vec2f pixel) const noexcept {
		int x = (int)(pixel.X * m_width);
		int y = (int)(pixel.Y * m_height);

		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x >= m_width) x = m_width - 1;
		if (y >= m_height) y = m_height - 1;

		int index = (y * m_width + x) * 4;

		float r = m_pixelData[index] / 255.0;
		float g = m_pixelData[index + 1] / 255.0;
		float b = m_pixelData[index + 2] / 255.0;
		float a = m_pixelData[index + 3] / 255.0;

		return Rgba(Vec3f(r, g, b), a);
	}

	inline int GetWidth() const noexcept {
		return m_width;

	}
	inline int GetHeight() const noexcept {
		return m_height;
	}

	~LockedBitmap() {
		delete m_pixelData;
	}
private:
	unsigned char* m_pixelData;
	uint32_t m_width;
	uint32_t m_height;
};

