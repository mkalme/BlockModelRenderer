using System;
using System.Buffers;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;

namespace Model.Reader.Interop.Cs
{
    public partial class Instance : IDisposable
    {
        [DllImport("Model.Reader.Interop.Cpp.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int initialize();

        [DllImport("Model.Reader.Interop.Cpp.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void loadDirectory(string directory);

        [DllImport("Model.Reader.Interop.Cpp.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void provideModelNames(out IntPtr arr, out int size);

        [DllImport("Model.Reader.Interop.Cpp.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern ModelInfo requestModelInfo(string name);

        [DllImport("Model.Reader.Interop.Cpp.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr requestModelRender(ModelRenderRequestArgs args);

        [DllImport("Model.Reader.Interop.Cpp.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void dispose();

        private Dispatcher _dispatcher;

        private bool _disposed = false;
        private WriteableBitmap? _bitmap;

        public Instance(Dispatcher dispatcher) 
        {
            _dispatcher = dispatcher;
        }

        public void Initialize() 
        {
            initialize();
        }

        public void LoadDirectory(string directory) 
        {
            loadDirectory(directory);
        }

        public IList<string> ProvideModelNames()
        {
            provideModelNames(out IntPtr arrPtr, out int size);
            if(size == 0) return new List<string>();

            IntPtr[] stringPtrs = new IntPtr[size];
            Marshal.Copy(arrPtr, stringPtrs, 0, size);

            List<string> output = new(size);
            for (int i = 0; i < size; i++)
            {
                string? name = Marshal.PtrToStringAnsi(stringPtrs[i]);
                if (name is null) continue;

                output.Add(name);
            }

            return output;
        }

        public unsafe ImageSource? RequestModelRender(ModelRenderRequestArgs args)
        {
            IntPtr buffer = requestModelRender(args);

            _dispatcher.Invoke(new Action(() =>
            {
                if (_bitmap is null || _bitmap.PixelWidth != args.Width || _bitmap.PixelHeight != args.Height)
                {
                    _bitmap = CreateBitmap(args.Width, args.Height);
                }

                _bitmap.Lock();
                IntPtr bitmapPtr = _bitmap.BackBuffer;

                int length = args.Width * args.Height * 4;
                Buffer.MemoryCopy(buffer.ToPointer(), bitmapPtr.ToPointer(), length, length);

                _bitmap.AddDirtyRect(new Int32Rect(0, 0, _bitmap.PixelWidth, _bitmap.PixelHeight));
                _bitmap.Unlock();
            }));

            return _bitmap;
        }

        private static WriteableBitmap CreateBitmap(int width, int height)
        {
            byte[] buffer = ArrayPool<byte>.Shared.Rent(width * height * 4);

            WriteableBitmap bitmap = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgra32, null);
            bitmap.WritePixels(new Int32Rect(0, 0, width, height), buffer, width * 4, 0);

            ArrayPool<byte>.Shared.Return(buffer);

            return bitmap;
        }

        public ModelInfo RequestModelInfo(string name) 
        {
            return requestModelInfo(name);
        }

        public void Dispose()
        {
            dispose();
        }
    }
}
