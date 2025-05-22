#pragma once
#include "../Rgba.h"

class WriteonlyImage
{
public:
	virtual void WritePixel(Rgba pixel, uint32_t x, uint32_t y) {}
};