namespace Model.Gui
{
    public class Shading
    {
        public bool ShadingEnabled { get; set; }
        public Vec3f AmbientLight { get; set; }
        public Vec3f DirectionalLightDirection { get; set; }
        public Vec3f DirectionalLight { get; set; }

        public Shading Clone() 
        {
            return new Shading() 
            {
                ShadingEnabled = ShadingEnabled,
                AmbientLight = AmbientLight,
                DirectionalLightDirection = DirectionalLightDirection,
                DirectionalLight = DirectionalLight
            };
        }
    }
}
