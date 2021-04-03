using ChaosHostingApi.Models.Client;
using ChaosHostingApi.Services;
using Microsoft.AspNetCore.Mvc;
using System;
using System.Threading.Tasks;

namespace ChaosHostingApi.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class VotingController : ControllerBase
    {
        private readonly ChaosGameManager chaosGameManager;

        public VotingController(ChaosGameManager chaosGameManager)
        {
            this.chaosGameManager = chaosGameManager ?? throw new ArgumentNullException(nameof(chaosGameManager));
        }

        [HttpPost("[action]")]
        public async Task Vote(string roomCode, int round, int optionId)
        {
            var ipAddress = Request.HttpContext.Connection.RemoteIpAddress.ToString();

            await chaosGameManager.CastVote(roomCode, round, optionId, ipAddress);
        }

        [HttpGet("[action]")]
        public CompleteGameState GetCompleteGameState(string roomCode)
        {
            return chaosGameManager.GetCompleteGameState(roomCode);
        }

        [HttpGet("[action]")]
        public PartialGameState GetPartialGameState(string roomCode)
        {
            return chaosGameManager.GetPartialGameState(roomCode);
        }
    }
}