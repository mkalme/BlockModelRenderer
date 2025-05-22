using System;
using System.Collections.Generic;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using Model.Reader.Interop.Cs;

namespace Model.Gui
{
    public class MainWindowModel : IMainWindowModel
    {
        public IReadOnlyList<string> Models => _models;
        private readonly List<string> _models = new();

        public Instance Instance { get; set; }

        private BitmapSource _source;

        public MainWindowModel(Dispatcher dispatcher) 
        {
            string path = "C:\\Users\\tawle\\Desktop\\assets\\minecraft";

            Instance = new Instance(dispatcher);
            Instance.Initialize();
            Instance.LoadDirectory(path);

            _models.Clear();
            _models.AddRange(Instance.ProvideModelNames());

            BitmapImage bitmap = new BitmapImage();

            // Begin initialization
            bitmap.BeginInit();
            bitmap.UriSource = new Uri("test.png", UriKind.Relative);
            bitmap.EndInit();

            // Set the Image control's source to the BitmapImage
            _source = bitmap;
        }

        public ModelInfo RequestModelInfo(string name) 
        {
            Reader.Interop.Cs.ModelInfo info = Instance.RequestModelInfo(name);
        
            return new ModelInfo() 
            {
                Name = name,
                From = new Vec3f(info.From.X, info.From.Y, info.From.Z),
                To = new Vec3f(info.To.X, info.To.Y, info.To.Z)
            };
        }

        public ImageSource? RequestModelRender(ModelRenderRequestArgs args)
        {
            Reader.Interop.Cs.Camera camera = new() 
            {
                Location = new Reader.Interop.Cs.Vec3f(args.Environment.Camera.Location.X, args.Environment.Camera.Location.Y, args.Environment.Camera.Location.Z),
                Direction = new Reader.Interop.Cs.Vec3f(args.Environment.Camera.Direction.X, args.Environment.Camera.Direction.Y, args.Environment.Camera.Direction.Z),
                Rotation = args.Environment.Camera.Rotation,
                PlaneWidth = args.Environment.Camera.PlaneWidth,
                FocalLength = args.Environment.Camera.FocalLength,
                CameraType = (Reader.Interop.Cs.CameraType) args.Environment.Camera.CameraType
            };

            Reader.Interop.Cs.Shading shading = new() 
            {
                ShadingEnabled = args.Environment.Shading.ShadingEnabled ? (byte)1 : (byte)0,
                AmbientLight = new Reader.Interop.Cs.Vec3f(args.Environment.Shading.AmbientLight.X, args.Environment.Shading.AmbientLight.Y, args.Environment.Shading.AmbientLight.Z),
                DirectionalLightDirection = new Reader.Interop.Cs.Vec3f(args.Environment.Shading.DirectionalLightDirection.X, args.Environment.Shading.DirectionalLightDirection.Y, args.Environment.Shading.DirectionalLightDirection.Z),
                DirectionalLight = new Reader.Interop.Cs.Vec3f(args.Environment.Shading.DirectionalLight.X, args.Environment.Shading.DirectionalLight.Y, args.Environment.Shading.DirectionalLight.Z)

            };

            Reader.Interop.Cs.Environment e = new()
            {
                SkyColor = new Reader.Interop.Cs.Rgba(new Reader.Interop.Cs.Vec3f(args.Environment.SkyColor.Rgb.X, args.Environment.SkyColor.Rgb.Y, args.Environment.SkyColor.Rgb.Z), args.Environment.SkyColor.A),
                Shading = shading,
                Camera = camera,
            };

            Reader.Interop.Cs.ModelRenderRequestArgs a = new() 
            {
                Name = args.Name,
                Environment = e,
                Width = args.Width,
                Height = args.Height
            };

            return Instance.RequestModelRender(a);
        }
    }
}
