#pragma once
#include "../../Geometry/Vector/Vec3f.h"
#include "../Paint/Rgba.h"
#include "../Scene/Camera/Camera.h"
#include "Shading.h"

class Environment
{
public:
	Camera* EnvironmentCamera;
	Shading EnvironmentShading;
	Rgba SkyColor;

	Environment();
	Environment(Camera* environmentCamera, Shading environmentShading, Rgba skyColor);
};