#pragma once
#include "FaceToken.h"
#include "Geometry/Vector/Vec3f.h"
#include "RotationToken.h"

class ElementToken
{
public:
	Vec3f From, To;
	RotationToken* Rotation;
	bool Shade = true;
	FaceToken* Up;
	FaceToken* Down;
	FaceToken* North;
	FaceToken* South;
	FaceToken* West;
	FaceToken* East;
};

