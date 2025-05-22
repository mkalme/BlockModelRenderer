using System.Runtime.InteropServices;

namespace Model.Reader.Interop.Cs
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Shading
    {
        public byte ShadingEnabled { get; set; }
        public Vec3f AmbientLight { get; set; }
        public Vec3f DirectionalLightDirection { get; set; }
        public Vec3f DirectionalLight { get; set; }
    }
}
