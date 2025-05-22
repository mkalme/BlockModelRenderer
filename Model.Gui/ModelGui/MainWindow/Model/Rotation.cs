using System;

namespace Model.Gui
{
    public readonly struct Rotation
    {
        public readonly float Sin { get; init; }
        public readonly float Cos { get; init; }

        public Rotation()
        {
            Sin = 0;
            Cos = 0;
        }
        public Rotation(float sin, float cos)
        {
            Sin = sin;
            Cos = cos;
        }

        public Vec2f Rotate(Vec2f a)
        {
            return new Vec2f()
            {
                X = a.X * Cos - a.Y * Sin,
                Y = a.X * Sin + a.Y * Cos
            };
        }

        public static Rotation FromRadians(float radians)
        {
            return new Rotation((float)Math.Sin(radians), (float)Math.Cos(radians));
        }
        public static Rotation FromDegrees(float degrees)
        {
            return FromRadians((float)(degrees / 180 * Math.PI));
        }
    }
}
