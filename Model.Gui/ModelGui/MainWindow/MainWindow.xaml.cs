using System;
using System.Collections.Generic;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Model.Gui
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public IMainWindowModel MainWindowModel { get; set; }

        public Environment Environment { get; }

        private SearchableBlocks _modelHolder;

        public string? CurrentModel { get; set; }

        private ModelInfo _modelInfo;
        private bool appIsRunning = true;

        public MainWindow(IMainWindowModel mainWindowModel)
        {
            InitializeComponent();

            MainWindowModel = mainWindowModel;

            List<ModelHolder> models = new();
            foreach (var file in MainWindowModel.Models) 
            {
                models.Add(new ModelHolder(file));
            }

            _modelHolder = new(models);
            SolidBlockView.ItemsSource = _modelHolder;

            Environment = new Environment() 
            {
                SkyColor = new Rgba(new Vec3f(0), 0),
                Shading = new Shading() 
                {
                    ShadingEnabled = true,
                    AmbientLight = new Vec3f(0.05f * 4),
                    DirectionalLightDirection = new Vec3f(30, 50, 10).Normalize(),
                    DirectionalLight = new Vec3f(1)
                },
                Camera = new()
                {
                    PlaneWidth = 16 * 2,
                    FocalLength = 1,
                    CameraType = CameraType.Projection,
                    Location = new Vec3f(8 - 16 * 4, 16 * 3.5f, 8 + 16 * 4),
                },
            };

            Environment.Camera.LookAt(new Vec3f(8, 8, 8));
        }

        private void DataGrid_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (sender is DataGrid dataGrid && dataGrid.SelectedItem is ModelHolder selectedItem)
            {
                CurrentModel = selectedItem.Name;
            }
        }

        private void SolidBlockSearch_TextChanged(object sender, TextChangedEventArgs e)
        {
            string text = SolidBlockSearch.Text.ToLower();

            _modelHolder.Search(text); 
            SolidBlockView.Items.Refresh();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            CustomFrameWindowInitializer customFrame = new(this, Template);

            Vec3f center = new(8, 9, 8);
            //float distanceFromCenter = 2 * 16;
            float distanceFromCenter = 4 * 16;
            float rpm = 10;
            
            DateTime last = DateTime.MinValue;
            DateTime lastFps = DateTime.Now;
            float radians = 0;

            int fps = 0;

            //float sizeX = 512, sizeY = 512;

            //float fromX = 448, fromY = 96;
            //float toX = 24, toY = 24;
            //Console.WriteLine($"{(fromX / sizeX * 16).ToString().Replace(',', '.')}, {(fromY / sizeY * 16).ToString().Replace(',', '.')}, {((fromX + toX) / sizeX * 16).ToString().Replace(',', '.')}, {((fromY + toY) / sizeY * 16).ToString().Replace(',', '.')}");

            new Thread(() =>
            {
                while (appIsRunning) 
                {
                    DateTime now = DateTime.Now;
                    //if ((now - last).TotalMilliseconds < 10) continue;

                    if (last != DateTime.MinValue)
                    {
                        //radians += (float)((now - last).TotalSeconds / 60 * rpm * 2 * Math.PI);
                        radians = (float)((now - new DateTime(2024, 1, 1)).TotalSeconds % 60) / 60 * rpm * 2 * (float)Math.PI;
                    }

                    if ((now - lastFps).TotalMilliseconds >= 1000) 
                    {
                        int countedFps = (int)(1000 / (now - lastFps).TotalMilliseconds * fps);

                        if (!appIsRunning) return;
                        Dispatcher.Invoke(() =>
                        {
                            customFrame.SetSecondaryTitle($"|  FPS: {countedFps}");
                        });

                        fps = 0;
                        lastFps = now;
                    }

                    last = now;

                    if (_modelInfo.Name != CurrentModel && CurrentModel is not null)
                    {
                        _modelInfo = MainWindowModel.RequestModelInfo(CurrentModel);
                    }

                    //float offset = 9 - (_modelInfo.To.Y + _modelInfo.From.Y) / 2;

                    //Rotation rotation = Rotation.FromRadians(radians);
                    //Vec2f point = rotation.Rotate(new Vec2f(0, distanceFromCenter));

                    //Environment.Camera.Location = new Vec3f(point.X + center.X, distanceFromCenter + center.Y - 15, point.Y + center.Z);
                    //Environment.Camera.LookAt(center);
                    //Environment.Camera.Location = new Vec3f(Environment.Camera.Location.X, Environment.Camera.Location.Y - offset, Environment.Camera.Location.Z);

                    ImageSource? source = CallToUpdateImageSource();
                    fps++;

                    if (!appIsRunning) return;
                    Dispatcher.Invoke(() =>
                    {
                        if(source is not null) ImageControl.DisplayImage(source);
                    });
                }
            }).Start();
        }

        private ImageSource? CallToUpdateImageSource()
        {
            if (CurrentModel is null) return null;

            int size = 128;

            ModelRenderRequestArgs args = new()
            {
                Name = CurrentModel,
                Environment = Environment.Clone()
            };

            if (ImageControl.ActualWidth >= ImageControl.ActualHeight)
            {
                args.Width = (int)(size * (ImageControl.ActualWidth / ImageControl.ActualHeight));
                args.Height = size;
            }
            else
            {
                args.Width = size;
                args.Height = (int)(size * (ImageControl.ActualHeight / ImageControl.ActualWidth));
            }

            args.Environment.Camera.PlaneWidth = Environment.Camera.PlaneWidth * args.Width / args.Height;

            return MainWindowModel.RequestModelRender(args);
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            appIsRunning = false;
        }

        private void ImageControl_MouseWheel(object sender, System.Windows.Input.MouseWheelEventArgs e)
        {
            float multiply = 1.2f;
            if (e.Delta > 0) multiply = 1 / multiply;

            Environment.Camera.PlaneWidth *= multiply;            
        }
    }
}
