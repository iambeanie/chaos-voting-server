using ChaosHostingApi.Filters;
using ChaosHostingApi.Models.Host;
using ChaosHostingApi.Services;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Threading.Tasks;

namespace ChaosHostingApi.Controllers
{
    [ApiController]
    [Route("[controller]")]
    [ServiceFilter(typeof(AdminFilterAttribute))]
    public class HostingController : ControllerBase
    {
        private readonly ChaosGameManager chaosGameManager;

        public HostingController(ChaosGameManager chaosGameManager)
        {
            this.chaosGameManager = chaosGameManager ?? throw new ArgumentNullException(nameof(chaosGameManager));
        }

        [HttpPost("[action]")]
        public void CreateGame(string roomCode, string roomPassword)
        {
            if (string.IsNullOrWhiteSpace(roomCode) || string.IsNullOrWhiteSpace(roomPassword)) return;

            chaosGameManager.CreateGame(roomPassword, roomCode);
        }

        [HttpPost("[action]")]
        public async Task UpdateVoteOptionsForRoom(HostVoteOptions hostVoteOptions)
        {
            if (hostVoteOptions == null) return;

            await chaosGameManager.SetNewVoteOptions(hostVoteOptions);
        }

        [HttpDelete("[action]")]
        public async Task CloseGame(string roomPassword, string roomCode)
        {
            await chaosGameManager.CloseGame(roomPassword, roomCode);
        }
    }
}