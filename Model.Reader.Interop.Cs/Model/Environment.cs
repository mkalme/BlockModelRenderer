using System.Runtime.InteropServices;

namespace Model.Reader.Interop.Cs
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Environment
    {
        public Rgba SkyColor { get; set; }
        public Shading Shading { get; set; }
        public Camera Camera { get; set; }
    }
}
