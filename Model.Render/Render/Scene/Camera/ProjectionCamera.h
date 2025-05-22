#pragma once
#include "Camera.h"

class ProjectionCamera : public Camera
{
public:
	float PlaneWidth;

	ProjectionCamera();

	ProjectionCamera(float planeWidth);

	ProjectionCamera(Vec3f location);

	void Cast(Ray* rays, int widthPixels, int heightPixels) override;

	virtual ~ProjectionCamera() {}
};

