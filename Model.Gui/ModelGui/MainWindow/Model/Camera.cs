namespace Model.Gui
{
    public class Camera
    {
        public Vec3f Location { get; set; }
        public Vec3f Direction { get; set; }
        public float Rotation { get; set; }
        public float PlaneWidth { get; set; }
        public float FocalLength { get; set; }

        public CameraType CameraType { get; set; }

        public void LookAt(Vec3f point) 
        {
            float x = point.X - Location.X;
            float y = point.Y - Location.Y;
            float z = point.Z - Location.Z;
        
            Direction = new(x, y, z);
            Direction = Direction.Normalize();
        }

        public Camera Clone() 
        {
            return new Camera() 
            {
                Location = Location,
                Direction = Direction,
                Rotation = Rotation,
                PlaneWidth = PlaneWidth,
                FocalLength = FocalLength,
                CameraType = CameraType
            };
        }
    }
}
