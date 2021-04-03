using ChaosHostingApi.Services;
using Microsoft.AspNetCore.SignalR;
using System.Threading.Tasks;

namespace ChaosHostingApi.Hubs
{
    public class VotingHub : Hub
    {
        private readonly ChaosGameManager chaosGameManager;

        public VotingHub(ChaosGameManager chaosGameManager)
        {
            this.chaosGameManager = chaosGameManager ?? throw new System.ArgumentNullException(nameof(chaosGameManager));
        }

        public async Task RegisterAsVoter(string roomCode)
        {
            if (!chaosGameManager.GameExistsWithRoomCode(roomCode)) return;

            await Groups.AddToGroupAsync(Context.ConnectionId, roomCode);
        }
    }
}