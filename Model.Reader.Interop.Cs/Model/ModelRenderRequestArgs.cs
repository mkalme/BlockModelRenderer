using System.Runtime.InteropServices;

namespace Model.Reader.Interop.Cs
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ModelRenderRequestArgs
    {
        public string Name { get; set; }
        public Environment Environment { get; set; }
        public int Width { get; set; }
        public int Height { get; set; }
    }
}
