#pragma once
#include <malloc.h>

struct Frame {
	int Position = 0;
	int Duration = 1;

	Frame() {
		Position = 0;
		Duration = 1;
	}
	Frame(int position, int duration) {
		Position = position;
		Duration = duration;
	}
};

class Animation
{
public:
	bool IsEnabled = false;
	bool IsAnimated = false;
	int FrameDuration = 1;
	bool CustomFrameDuration = false;
	bool Interpolate = false;
	int TileCount = 0;
	int FrameCount = 0;
	Frame* Frames = 0;

	Animation() {
		IsEnabled = false;
		IsAnimated = false;
		FrameDuration = 1;
		CustomFrameDuration = false;
		Interpolate = false;
		TileCount = 0;
	}

	Animation(int tileCount) {
		IsEnabled = true;
		IsAnimated = true;
		FrameDuration = 1;
		CustomFrameDuration = false;
		Interpolate = false;
		TileCount = tileCount;
	}

	int GetFramePosition(int currentTick, float& delta, int& nextFramePosition) const {
		if (!IsEnabled) {
			delta = 0;
			nextFramePosition = 0;
			return 0;
		}

		int within = currentTick % (FrameCount * FrameDuration);
		if (!CustomFrameDuration) {
			if (Interpolate) {
				delta = (within % FrameDuration) / (float)FrameDuration;
				nextFramePosition = Mod(Frames[Mod(within / FrameDuration + 1, FrameCount)].Position, TileCount);
			}

			return Mod(Frames[within / FrameDuration].Position, TileCount);
		}

		int frame = 0, tick = 0, beginTick = 0;
		while (tick < within) {
			beginTick = tick;
			tick += Frames[frame++].Duration;
		}

		if (Interpolate) {
			delta = (within - beginTick) / Frames[frame].Duration;
			nextFramePosition = Mod(Frames[Mod(frame + 1, FrameCount)].Position, TileCount);
		}

		return Mod(Frames[frame].Position, TileCount);
	}

private:
	int Mod(int x, int m) const {
		int r = x % m;
		return r < 0 ? r + m : r;
	}
};

