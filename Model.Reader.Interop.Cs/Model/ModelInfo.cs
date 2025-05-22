using System.Runtime.InteropServices;

namespace Model.Reader.Interop.Cs
{
    [StructLayout(LayoutKind.Sequential)]
    public readonly struct ModelInfo
    {
        public Vec3f From { get; init; }
        public Vec3f To { get; init; }
    }
}
