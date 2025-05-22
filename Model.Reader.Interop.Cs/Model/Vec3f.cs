using System.Runtime.InteropServices;
using System.Windows.Media;

namespace Model.Reader.Interop.Cs
{
    [StructLayout(LayoutKind.Sequential)]
    public readonly struct Vec3f
    {
        public float X { get; }
        public float Y { get; }
        public float Z { get; }

        public Vec3f(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vec3f(Color color)
        {
            X = color.R / 255f;
            Y = color.G / 255f;
            Z = color.B / 255f;
        }
    }
}
