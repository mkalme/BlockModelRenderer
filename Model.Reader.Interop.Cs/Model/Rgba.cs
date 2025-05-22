using System.Runtime.InteropServices;

namespace Model.Reader.Interop.Cs
{
    [StructLayout(LayoutKind.Sequential)]
    public readonly struct Rgba
    {
        public Vec3f Rgb { get; init; }
        public float A { get; init; }

        public Rgba(Vec3f rgb, float a) 
        {
            Rgb = rgb;
            A = a;
        }
    }
}
