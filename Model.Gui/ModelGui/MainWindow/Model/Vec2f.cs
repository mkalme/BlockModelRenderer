namespace Model.Gui
{
    public readonly struct Vec2f
    {
        public float X { get; init; }
        public float Y { get; init; }

        public Vec2f(float x, float y) 
        {
            X = x;
            Y = y;
        }
    }
}
