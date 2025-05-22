#include "ProjectionCamera.h"
#include <algorithm>
#include <numeric>
#include <algorithm>
#include <execution>
#include <vector>
#include <iostream>

static int m_prevHeight = 0;
static std::vector<int> m_prevRange;


ProjectionCamera::ProjectionCamera() : Camera() {
    PlaneWidth = 1;
}

ProjectionCamera::ProjectionCamera(float planeWidth) : Camera() {
    PlaneWidth = planeWidth;
}

ProjectionCamera::ProjectionCamera(Vec3f location) : Camera(location) {
    PlaneWidth = 1;
}

void ProjectionCamera::Cast(Ray* rays, int widthPixels, int heightPixels) {
    float height = heightPixels / (float)widthPixels * PlaneWidth;
    float dx = PlaneWidth / widthPixels, dy = height / heightPixels;

    if (m_prevHeight != heightPixels) {
        m_prevHeight = heightPixels;
        m_prevRange = std::vector<int>(m_prevHeight);
        std::iota(m_prevRange.begin(), m_prevRange.end(), 0);
    }

    std::for_each(std::execution::par, m_prevRange.begin(), m_prevRange.end(),
    [&, dx, dy, height](int& yPx) {
        for (int xPx = 0; xPx < widthPixels; xPx++) {
            float x = PlaneWidth / 2 - xPx * dx - dx / 2;
            float y = height / 2 - yPx * dy - dy / 2;

            Vec3f origin = Vec3f(x, y, 1);
            RotatePoint(origin);
            RotatePointToDirection(origin);

            rays[yPx * widthPixels + xPx].Origin = origin - m_direction + Location;
            rays[yPx * widthPixels + xPx].Direction = m_direction;
        }
    });
}