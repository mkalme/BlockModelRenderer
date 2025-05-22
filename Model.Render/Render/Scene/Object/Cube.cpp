#include "Cube.h"
#include "../../../Utilities/Utilities.h"
#include <iostream>

Cube::Cube(Vec3f from, Vec3f to) {
	Min = Vec3f::MinVector(from, to);
	Max = Vec3f::MaxVector(from, to);

    Rotation = AxisRotation(Axis::Y, Rotation::FromDegrees(0), Vec3f(0));
    Shade = true;

    Up = Face(Vec2f(Min.X, Min.Z), Vec2f(Max.X, Max.Z));
    Down = Face(Vec2f(Min.X, Max.Z), Vec2f(Max.X, Min.Z));
    North = Face(Vec2f(Max.X, Max.Y), Vec2f(Min.X, Min.Y));
    South = Face(Vec2f(Min.X, Max.Y), Vec2f(Max.X, Min.Y));
    West = Face(Vec2f(Min.Z, Max.Y), Vec2f(Max.Z, Min.Y));
    East = Face(Vec2f(Max.Z, Max.Y), Vec2f(Min.Z, Min.Y));
}

CastResult Cube::Intersect(const Ray& ray) const {
    Ray rotatedRay = ray;
    Rotation.Rotate.RotateWithAxis(rotatedRay, Rotation.RotationAxis, Rotation.Origin);

    CastResult result = IntersectRotated(rotatedRay);
    result.Shade = Shade;
    if (!result.Intersected()) return result;

    Rotation.Rotate.RotateWithAxis(result.Normal, Rotation.RotationAxis);
    return result;
}

CastResult Cube::IntersectRotated(const Ray& ray) const {
    float tx0 = (Min.X - ray.Origin.X) / ray.Direction.X;
    float tx1 = (Max.X - ray.Origin.X) / ray.Direction.X;
    if (tx0 > tx1) Utilities::Swap(tx0, tx1);

    float ty0 = (Min.Y - ray.Origin.Y) / ray.Direction.Y;
    float ty1 = (Max.Y - ray.Origin.Y) / ray.Direction.Y;
    if (ty0 > ty1) Utilities::Swap(ty0, ty1);

    if (tx0 > ty1 || ty0 > tx1) return EMPTY_CAST;

    float tz0 = (Min.Z - ray.Origin.Z) / ray.Direction.Z;
    float tz1 = (Max.Z - ray.Origin.Z) / ray.Direction.Z;
    if (tz0 > tz1) Utilities::Swap(tz0, tz1);

    float tmin = Utilities::Max(tx0, ty0), tmax = Utilities::Min(tx1, ty1);
    if ((tmin > tz1) || (tz0 > tmax)) return EMPTY_CAST;

    tmin = Utilities::Max(tz0, tmin);
    tmax = Utilities::Min(tz1, tmax);

    if (tx0 == tmin){
        if (ray.Direction.X < 0) {
            if (East.Visible) return IntersectXAxis(ray, tmin, East);
        } else if (West.Visible) return IntersectXAxis(ray, tmin, West);
        return CastResult(tmin, 0, Rgba(0, -1));
    } else if (ty0 == tmin) {
        if (ray.Direction.Y < 0) {
            if (Up.Visible) return IntersectYAxis(ray, tmin, Up);
        } else if (Down.Visible) return IntersectYAxis(ray, tmin, Down);
        return CastResult(tmin, 0, Rgba(0, -1));
    } else if (tz0 == tmin) {
        if (ray.Direction.Z < 0) {
            if (South.Visible) return IntersectZAxis(ray, tmin, South);
        } else if (North.Visible) return IntersectZAxis(ray, tmin, North);
        return CastResult(tmin, 0, Rgba(0, -1));
    }

    //if (tx1 == tmax) {
    //    if (ray.Direction.X < 0 ) {
    //        if (West.Visible) return IntersectXAxis(ray, tmax, West);
    //    } else if (East.Visible) return IntersectXAxis(ray, tmax, East);
    //} else if (ty1 == tmax) {
    //    if (ray.Direction.Y < 0) {
    //        if (Down.Visible) return IntersectYAxis(ray, tmax, Down);
    //    } else if (Up.Visible) return IntersectYAxis(ray, tmax, Up);
    //} else if (tz1 == tmax) {
    //    if (ray.Direction.Z < 0) {
    //        if (North.Visible) return IntersectZAxis(ray, tmax, North);
    //    } else if (South.Visible) return IntersectZAxis(ray, tmax, South);
    //}

    return EMPTY_CAST;
}


CastResult Cube::IntersectXAxis(const Ray& ray, float t, const Face& face) const {
    Vec3f intercept = ray.Origin + ray.Direction * t;

    Vec2f pixel = Vec2f(intercept.Z, intercept.Y) - face.From;
    pixel.X = Utilities::Abs(pixel.X);
    pixel.Y = Utilities::Abs(pixel.Y);
    pixel /= face.Size;

    return CastResult(t, X_NORMAL, face.Surface->ReadPixel(pixel));
}

CastResult Cube::IntersectYAxis(const Ray& ray, float t, const Face& face) const {    
    Vec3f intercept = ray.Origin + ray.Direction * t;

    Vec2f pixel = Vec2f(intercept.X, intercept.Z) - face.From;
    pixel.X = Utilities::Abs(pixel.X);
    pixel.Y = Utilities::Abs(pixel.Y);
    pixel /= face.Size;

    return CastResult(t, Y_NORMAL, face.Surface->ReadPixel(pixel));
}

CastResult Cube::IntersectZAxis(const Ray& ray, float t, const Face& face) const {
    Vec3f intercept = ray.Origin + ray.Direction * t;

    Vec2f pixel = Vec2f(intercept.X, intercept.Y) - face.From;
    pixel.X = Utilities::Abs(pixel.X);
    pixel.Y = Utilities::Abs(pixel.Y);
    pixel /= face.Size;

    return CastResult(t, Z_NORMAL, face.Surface->ReadPixel(pixel));
}