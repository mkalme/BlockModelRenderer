#pragma once
#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
	float FocalLength;

	PerspectiveCamera();

	PerspectiveCamera(float focalLength);

	PerspectiveCamera(Vec3f location);

	void Cast(Ray* rays, int widthPixels, int heightPixels) override;

	virtual ~PerspectiveCamera() {}
};

