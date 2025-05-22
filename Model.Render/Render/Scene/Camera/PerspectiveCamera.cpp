#include "PerspectiveCamera.h"
#include <execution>

static int m_prevHeight = 0;
static std::vector<int> m_prevRange;

PerspectiveCamera::PerspectiveCamera() : Camera() {
    FocalLength = 1;
}

PerspectiveCamera::PerspectiveCamera(float focalLength) : Camera() {
    FocalLength = focalLength;
}

PerspectiveCamera::PerspectiveCamera(Vec3f location) : Camera(location) {
    FocalLength = 1;
}

void PerspectiveCamera::Cast(Ray* rays, int widthPixels, int heightPixels) {
    float height = heightPixels / (float)widthPixels;
    float dx = 1.0 / widthPixels, dy = height / heightPixels;

    if (m_prevHeight != heightPixels) {
        m_prevHeight = heightPixels;
        m_prevRange = std::vector<int>(m_prevHeight);
        std::iota(m_prevRange.begin(), m_prevRange.end(), 0);
    }

    std::for_each(std::execution::par, m_prevRange.begin(), m_prevRange.end(),
    [&, dx, dy, height](int& yPx) {
        for (int xPx = 0; xPx < widthPixels; xPx++) {
            float x = 0.5 - xPx * dx - dx / 2;
            float y = height / 2 - yPx * dy - dy / 2;

            Vec3f point = Vec3f(x, y, FocalLength);
            RotatePoint(point);
            RotatePointToDirection(point);

            rays[yPx * widthPixels + xPx].Origin = Location;
            rays[yPx * widthPixels + xPx].Direction = point.Normalize();
        }
    });
}