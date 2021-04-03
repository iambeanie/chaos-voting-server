using ChaosHostingApi.Models.Host;
using ChaosHostingApi.Services;
using Microsoft.AspNetCore.SignalR;
using System.Linq;
using System.Threading.Tasks;

namespace ChaosHostingApi.Hubs
{
    public class HostingHub : Hub
    {
        private readonly ChaosGameManager chaosGameManager;
        private readonly AppSettings appSettings;

        public HostingHub(ChaosGameManager chaosGameManager, AppSettings appSettings)
        {
            this.chaosGameManager = chaosGameManager ?? throw new System.ArgumentNullException(nameof(chaosGameManager));
            this.appSettings = appSettings ?? throw new System.ArgumentNullException(nameof(appSettings));
        }

        public async Task RegisterAsHost(string roomCode, string roomPassword, string adminPassword)
        {
            if (!adminPassword.Equals(appSettings.AdminPassword)) return;

            if (!chaosGameManager.GameExistsWithRoomCodeAndPassword(roomCode, roomPassword)) return;

            await Clients.Group(roomCode).SendAsync("gameClosed");

            await Groups.AddToGroupAsync(Context.ConnectionId, roomCode);
        }
    }
}