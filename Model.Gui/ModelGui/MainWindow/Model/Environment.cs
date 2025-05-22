namespace Model.Gui
{
    public class Environment
    {
        public Rgba SkyColor { get; set; }
        public Shading Shading { get; set; } = new Shading();
        public Camera Camera { get; set; } = new Camera();

        public Environment Clone() 
        {
            return new Environment() 
            {
                SkyColor = SkyColor,
                Shading = Shading.Clone(),
                Camera = Camera.Clone()
            };
        }
    }
}
