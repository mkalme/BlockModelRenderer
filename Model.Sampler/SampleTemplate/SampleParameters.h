#pragma once
#include <Render/Scene/Camera/Camera.h>
#include <Render/Scene/Environment.h>
#include <Render/Scene/Object/BoundingBox.h>

struct SampleParameters
{	
	Environment SampleEnvironment;
	BoundingBox CuboidVolume;
	int Width;
	int Height;
};