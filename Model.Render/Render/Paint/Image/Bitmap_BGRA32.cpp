#include "Bitmap_BGRA32.h"

Bitmap_BGRA32::Bitmap_BGRA32() {
	m_pixelData = 0;
	m_width = 0;
	m_height = 0;
}

Bitmap_BGRA32::Bitmap_BGRA32(uint32_t width, uint32_t height) {
	m_pixelData = new uint8_t[width * height * 4];
	m_width = width;
	m_height = height;
}

void Bitmap_BGRA32::WritePixel(Rgba pixel, uint32_t x, uint32_t y) {
	int index = (y * m_width + x) * 4;

	m_pixelData[index] = (uint8_t)(pixel.Rgb.Z * 255);
	m_pixelData[index + 1] = (uint8_t)(pixel.Rgb.Y * 255);
	m_pixelData[index + 2] = (uint8_t)(pixel.Rgb.X * 255);
	m_pixelData[index + 3] = (uint8_t)(pixel.A * 255);
}

int Bitmap_BGRA32::GetWidth() const {
	return m_width;
}

int Bitmap_BGRA32::GetHeight() const {
	return m_height;
}

uint8_t* Bitmap_BGRA32::GetBuffer() const {
	return m_pixelData;
}

Bitmap_BGRA32::~Bitmap_BGRA32() {
	delete [] m_pixelData;
}