using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Model.Gui
{
    /// <summary>
    /// Interaction logic for ImageWithBackground.xaml
    /// </summary>
    public partial class ImageWithBackground : UserControl
    {
        private ImageDrawing? _mainImage = null;

        private RectangleGeometry? _backgroundSize;

        public ImageWithBackground()
        {
            InitializeComponent();
            RenderOptions.SetBitmapScalingMode(this, BitmapScalingMode.NearestNeighbor);

            CreateBackgroundImage();
        }

        private void Canvas_SizeChanged(object sender, SizeChangedEventArgs e) 
        {
            if (_backgroundSize is not null) _backgroundSize.Rect = new Rect(0, 0, Canvas.ActualWidth, Canvas.ActualHeight);
            CenterImage();
        }

        private void CreateBackgroundImage() 
        {
            Color a = Color.FromArgb(255, 17, 17, 17), b = Color.FromArgb(255, 23, 23, 23);

            int tile = 14;
            LockedBitmap bitmap = new(tile * 2, tile * 2);
            for (int y = 0; y < bitmap.Height; y++)
            {
                for (int x = 0; x < bitmap.Width; x++)
                {
                    bool qX = x / tile % 2 == 0, qY = y / tile % 2 == 0;
                    bitmap.Write(qX ^ qY ? a : b, x, y);
                }
            }

            ImageBrush imageBrush = new ImageBrush();
            imageBrush.ImageSource = bitmap.GetBitmapSource();
            imageBrush.TileMode = TileMode.Tile;
            imageBrush.Viewport = new Rect(0, 0, tile * 2, tile * 2);
            imageBrush.ViewportUnits = BrushMappingMode.Absolute;

            _backgroundSize = new RectangleGeometry(new Rect(0, 0, Canvas.ActualWidth, Canvas.ActualHeight));

            GeometryDrawing background = new GeometryDrawing();
            background.Brush = imageBrush;
            background.Geometry = _backgroundSize;

            Canvas.DrawingGroup.Children.Add(background);
        }

        public void DisplayImage(ImageSource source) 
        {
            Rect rectangle = new(0, 0, source.Width, source.Height);

            if (_mainImage == null)
            {
                _mainImage = new ImageDrawing(source, rectangle);
                Canvas.DrawingGroup.Children.Add(_mainImage);
            }
            else if (_mainImage.ImageSource != source) 
            {
                Canvas.DrawingGroup.Children.Remove(_mainImage);

                _mainImage = new ImageDrawing(source, rectangle);
                Canvas.DrawingGroup.Children.Add(_mainImage);
            }
            else
            {
                _mainImage.Rect = rectangle;
            }

            CenterImage();
        }

        private void CenterImage() 
        {
            if (_mainImage is null) return;

            double imageRatio = _mainImage.ImageSource.Width / _mainImage.ImageSource.Height;
            double controlRatio = Canvas.ActualWidth / Canvas.ActualHeight;

            double width, height;

            if (imageRatio >= controlRatio)
            {
                width = Canvas.ActualWidth;
                height = width / imageRatio;
            }
            else 
            {
                height = Canvas.ActualHeight;
                width = height * imageRatio;
            }

            _mainImage.Rect = new Rect((Canvas.ActualWidth - width) / 2, (Canvas.ActualHeight - height) / 2, width, height);
        }
    }
}
