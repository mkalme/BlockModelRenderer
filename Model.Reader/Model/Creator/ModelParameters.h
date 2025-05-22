#pragma once

struct ModelParameters
{
	int DegreesX = 0;
	int DegreesY = 0;
	bool UvLock = false;
	bool AllowSemiTransparency = true;

	ModelParameters(int degreesX, int degreesY, bool uvLock, bool allowSemiTransparency) {
		DegreesX = degreesX;
		DegreesY = degreesY;
		UvLock = uvLock;
		AllowSemiTransparency = allowSemiTransparency;
	}
};

