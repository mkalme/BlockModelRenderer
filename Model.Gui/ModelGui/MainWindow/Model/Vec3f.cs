using System;

namespace Model.Gui
{
    public readonly struct Vec3f
    {
        public float X { get; }
        public float Y { get; }
        public float Z { get; }

        public Vec3f(float value)
        {
            X = value;
            Y = value;
            Z = value;
        }

        public Vec3f(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vec3f Normalize() 
        {
            float length = (float)Math.Sqrt(X * X + Y * Y + Z * Z);
            return new Vec3f(X / length, Y / length, Z / length);
        }
    }
}
