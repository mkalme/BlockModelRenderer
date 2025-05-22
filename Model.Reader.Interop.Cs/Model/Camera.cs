using System.Runtime.InteropServices;

namespace Model.Reader.Interop.Cs
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Camera
    {
        public Vec3f Location { get; set; }
        public Vec3f Direction { get; set; }
        public float Rotation { get; set; }
        public float PlaneWidth { get; set; }
        public float FocalLength { get; set; }

        public CameraType CameraType { get; set; }
    }
}
