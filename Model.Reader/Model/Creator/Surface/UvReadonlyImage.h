#pragma once
#include "../Model.Render/Render/Paint/Image/ReadonlyImage.h"
#include "../../Reader/Tokenized/Uv.h"
#include "UvImageParameters.h"

class UvReadonlyImage : public ReadonlyImage
{
public:
	LockedBitmap* Bitmap;
	UvImageParameters Parameters;
	Animation* ImageAnimation;
	TickProvider* TickProv = 0;

	UvReadonlyImage(LockedBitmap* bitmap, UvImageParameters parameters, Animation* imageAnimation) {
		Bitmap = bitmap;
		Parameters = parameters;
		ImageAnimation = imageAnimation;

		m_from = Parameters.TextureUv.From / 16;
		m_to = Parameters.TextureUv.To / 16;
	}

	Rgba ReadPixel(Vec2f pixel) override {
		Rotate(pixel, Parameters.TextureRotation);
		pixel = (m_from + pixel * (m_to - m_from)).Clamp(0, 1);
		Rotate(pixel, Parameters.UvRotation);

		Rgba output;

		if (ImageAnimation->IsAnimated && TickProv != NULL) {
			float tileCount = ImageAnimation->TileCount;

			float delta;
			int nextFrame;
			int currentFrame = ImageAnimation->GetFramePosition(TickProv->GetCurrentFrame(), delta, nextFrame);

			float y = pixel.Y;
			pixel.Y = y / tileCount + (1.0 / tileCount * currentFrame);
			output = Bitmap->ReadPixel(pixel);

			if (ImageAnimation->Interpolate) {
				pixel.Y = y / tileCount + (1.0 / tileCount * nextFrame);
				Rgba next = Bitmap->ReadPixel(pixel);
				
				output.Mix(next, delta);
			}
		}
		else {
			output = Bitmap->ReadPixel(pixel);
		}

		if (Parameters.TintIndex >= 0) output.Tint = 1;
		if (Parameters.AllowSemiTransparency) return output;

		if (output.A > 0) output.A = 1;
		return output;
	}

	void Rotate(Vec2f& pixel, int rotation) const {
		switch (rotation)
		{
		case 0:
			return;
		case 90:
			pixel = Vec2f(pixel.Y, 1 - pixel.X);
			break;
		case 180:
			pixel = Vec2f(1 - pixel.X, 1 - pixel.Y);
			break;
		case 270:
			pixel = Vec2f(1 - pixel.Y, pixel.X);
			break;
		}
	}
private:
	Vec2f m_from;
	Vec2f m_to;
};