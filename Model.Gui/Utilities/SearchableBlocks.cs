using System.Collections;
using System.Collections.Generic;
using System.Windows.Data;

namespace Model.Gui
{
    public class SearchableBlocks : IEnumerable
    {
        public IList<ModelHolder> OriginalBlockList { get; set; }
        public string? SearchName { get; private set; }

        private List<ModelHolder> _displayedBlockList;

        public SearchableBlocks(IList<ModelHolder> list)
        {
            _displayedBlockList = new List<ModelHolder>(list);
            OriginalBlockList = new List<ModelHolder>(list);
        }

        public void Search(string searchName)
        {
            SearchName = searchName;
            Display();
        }

        private void Display()
        {
            var collectionView = CollectionViewSource.GetDefaultView(this);
            using (collectionView.DeferRefresh())
            {
                List<ModelHolder> output = new();
                foreach (ModelHolder entry in OriginalBlockList)
                {
                    if (MatchesSearch(entry, SearchName))
                    {
                        output.Add(entry);
                    }
                }

                _displayedBlockList.Clear();
                _displayedBlockList.AddRange(output);
            }


        }
        private static bool MatchesSearch(ModelHolder entry, string? searchName)
        {
            if (string.IsNullOrEmpty(searchName)) return true;
            return entry.Name.Contains(searchName) || searchName.Contains(entry.Name);
        }

        public IEnumerator GetEnumerator()
        {
            return _displayedBlockList.GetEnumerator();
        }
    }
}
