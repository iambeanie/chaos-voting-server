namespace ChaosHostingApi.Models.Client
{
    public class CompleteGameState
    {
        public string PreviousResultDescription { get; set; }
        public int CurrentRound { get; set; }
        public FullVoteOptionState[] VoteOptionsState { get; set; }
    }

    public class FullVoteOptionState
    {
        public int Id { get; set; }
        public int Count { get; set; }
        public string Description { get; set; }
    }
}