#pragma once
#include "WriteonlyImage.h"

class Bitmap_BGRA32 : public WriteonlyImage
{
public:
	Bitmap_BGRA32();
	Bitmap_BGRA32(uint32_t width, uint32_t height);

	virtual void WritePixel(Rgba pixel, uint32_t x, uint32_t y) override;

	int GetWidth() const;
	int GetHeight() const;

	uint8_t* GetBuffer() const;

	~Bitmap_BGRA32();
private:
	uint8_t* m_pixelData;
	uint32_t m_width;
	uint32_t m_height;
};