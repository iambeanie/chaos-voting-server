using System.Collections.Generic;

namespace ChaosHostingApi.Models.Client
{
    public class PartialGameState
    {
        public IEnumerable<PartialVoteState> PartialVoteStates { get; set; }
    }

    public class PartialVoteState
    {
        public int Id { get; set; }
        public int Count { get; set; }
    }
}