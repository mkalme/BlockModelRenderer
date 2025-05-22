#pragma once
#include "../../../Geometry/Vector/Vec3f.h"
#include "../../../Geometry/Vector/Vec2f.h"
#include "../../../Geometry/Rotation/Rotation.h"
#include "../../../Geometry/Ray.h"

class Camera
{
public:
    Vec3f Location;
    Rotation PlaneRotation;

    Camera();

    Camera(Vec3f location);

    Vec3f GetDirection() const;
    
    void SetDirection(Vec3f direction);

    virtual void Cast(Ray* rays, int widthPixels, int heightPixels) = 0;

    void LookAt(Vec3f point);

    virtual ~Camera() {}
protected:
    Vec3f m_direction;
    Rotation m_hRotation, m_vRotation;

    void RotatePoint(Vec3f& point) const;
    void RotatePointToDirection(Vec3f& point) const;
};

