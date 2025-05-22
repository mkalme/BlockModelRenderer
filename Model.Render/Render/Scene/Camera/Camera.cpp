#include "Camera.h"
#include <cmath>

Camera::Camera() {
	Location = Vec3f();
	PlaneRotation = Rotation::FromRadians(0);
	SetDirection(Vec3f(0, 0, -1));
}

Camera::Camera(Vec3f location) {
	Location = location;
	PlaneRotation = Rotation::FromRadians(0);
	SetDirection(Vec3f(0, 0, -1));
}

Vec3f Camera::GetDirection() const {
	return m_direction;
}

void Camera::SetDirection(Vec3f direction) {
	m_direction = direction;

	m_hRotation = PlaneRotation.FromRadians(std::atan2f(direction.X, direction.Z));
	m_vRotation = PlaneRotation.FromRadians(std::atan2f(direction.Y, Vec2f(direction.X, direction.Z).GetLength()));
}

void Camera::LookAt(Vec3f point) {
	SetDirection((point - Location).Normalize());
}

void Camera::RotatePoint(Vec3f& point) const {
	Vec2f d = Vec2f(-point.X, point.Y);
	PlaneRotation.Rotate(d);

	point.X = -d.X;
	point.Y = d.Y;
}

void Camera::RotatePointToDirection(Vec3f& point) const {
	Vec2f v = Vec2f(point.Z, point.Y);
	m_vRotation.Rotate(v);

	Vec2f h = Vec2f(v.X, point.X);
    m_hRotation.Rotate(h);

	point.X = h.Y;
	point.Y = v.Y;
	point.Z = h.X;
}