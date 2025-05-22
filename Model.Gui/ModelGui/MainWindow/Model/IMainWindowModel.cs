using System.Collections.Generic;
using System.Windows.Media;

namespace Model.Gui
{
    public interface IMainWindowModel
    {
        IReadOnlyList<string> Models { get; }

        ModelInfo RequestModelInfo(string name);
        ImageSource? RequestModelRender(ModelRenderRequestArgs args);
    }
}
