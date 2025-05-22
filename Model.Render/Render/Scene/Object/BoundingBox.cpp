#include "BoundingBox.h"
#include <cmath>
#include <iostream>
#include "../../../Utilities/Utilities.h"

BoundingBox::BoundingBox() {
    m_min = Vec3f();
    m_max = Vec3f();
}

BoundingBox::BoundingBox(Vec3f from, Vec3f to) {
	m_min = Vec3f::MinVector(from, to);
	m_max = Vec3f::MaxVector(from, to);
}

bool BoundingBox::Intersect(const Ray& ray) const {
    float tmin = (m_min.X - ray.Origin.X) / ray.Direction.X;
    float tmax = (m_max.X - ray.Origin.X) / ray.Direction.X;

    if (tmin > tmax) Utilities::Swap(tmin, tmax);

    float tymin = (m_min.Y - ray.Origin.Y) / ray.Direction.Y;
    float tymax = (m_max.Y - ray.Origin.Y) / ray.Direction.Y;

    if (tymin > tymax) Utilities::Swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    float tzmin = (m_min.Z - ray.Origin.Z) / ray.Direction.Z;
    float tzmax = (m_max.Z - ray.Origin.Z) / ray.Direction.Z;

    if (tzmin > tzmax) Utilities::Swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin) tmin = tzmin;
    if (tzmax < tmax) tmax = tzmax;

    return true;
}

void BoundingBox::Rotate(const AxisRotation& axisRotation) {
    Vec3f vertices[8] = {
        m_min,
        Vec3f(m_max.X, m_min.Y, m_min.Z),
        Vec3f(m_min.X, m_min.Y, m_max.Z),
        Vec3f(m_max.X, m_min.Y, m_max.Z),
        Vec3f(m_min.X, m_max.Y, m_min.Z),
        Vec3f(m_max.X, m_max.Y, m_min.Z),
        Vec3f(m_min.X, m_max.Y, m_max.Z),
        m_max,
    };

    for (int i = 0; i < 8; i++) {
        axisRotation.Rotate.RotateWithAxisNegative(vertices[i], axisRotation.RotationAxis, axisRotation.Origin);
    }

    Vec3f min = vertices[0];
    Vec3f max = vertices[0];

    for (int i = 1; i < 8; i++) {
        min = Vec3f::MinVector(min, vertices[i]);
        max = Vec3f::MaxVector(max, vertices[i]);
    }

    SetMin(min);
    SetMax(max);
}

void BoundingBox::SetMin(Vec3f min) {
    m_min = min;
}

Vec3f BoundingBox::GetMin() const {
    return m_min;
}

void BoundingBox::SetMax(Vec3f max) {
    m_max = max;
}

Vec3f BoundingBox::GetMax() const {
    return m_max;
}