using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace Model.Gui
{
    public class LockedBitmap
    {
        public int Width { get; }
        public int Height { get; }
        public byte[] Buffer { get; }

        private WriteableBitmap _source;

        public LockedBitmap(int width, int height) 
        {
            Buffer = new byte[width * height * 4];

            Width = width;
            Height = height;

            _source = new WriteableBitmap(width, height, 96, 96, PixelFormats.Bgra32, null);
            _source.WritePixels(new Int32Rect(0, 0, width, height), Buffer, width * 4, 0);
        }

        public LockedBitmap(WriteableBitmap bitmap)
        {
            Buffer = new byte[bitmap.PixelWidth * bitmap.PixelHeight * 4];
            bitmap.CopyPixels(Buffer, bitmap.PixelWidth * 4, 0);

            Width = bitmap.PixelWidth;
            Height = bitmap.PixelHeight;
        }

        public void Write(Color color, int x, int y)
        {
            int index = (y * Width + x) * 4;

            Buffer[index] = color.B;
            Buffer[index + 1] = color.G;
            Buffer[index + 2] = color.R;
            Buffer[index + 3] = color.A;
        }

        public unsafe BitmapSource GetBitmapSource() 
        {
            _source.Lock();
            IntPtr bitmapPtr = _source.BackBuffer;

            fixed (void* source = Buffer) 
            {
                int length = Width * Height * 4;
                System.Buffer.MemoryCopy(source, bitmapPtr.ToPointer(), length, length);
            }

            _source.AddDirtyRect(new Int32Rect(0, 0, _source.PixelWidth, _source.PixelHeight));
            _source.Unlock();

            return _source;
        }
    }
}
