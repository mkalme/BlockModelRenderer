#include "Face.h"
#include <iostream>

Face::Face() {
	From = Vec2f();
	Size = Vec2f();
}

Face::Face(Vec2f from, Vec2f to) {
	From = from;
	Size = to - from;
	Size.X = fabs(Size.X);
	Size.Y = fabs(Size.Y);
}