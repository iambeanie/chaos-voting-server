using System.Collections.Generic;

namespace ChaosHostingApi.Models.Host
{
    public class HostVoteOptions
    {
        public string RoomCode { get; set; }
        public IEnumerable<HostVoteOption> VoteOptions { get; set; }
    }
}