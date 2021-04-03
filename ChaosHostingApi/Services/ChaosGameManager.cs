using ChaosHostingApi.Domain.Voting;
using ChaosHostingApi.Hubs;
using ChaosHostingApi.Models.Client;
using ChaosHostingApi.Models.Host;
using Microsoft.AspNetCore.SignalR;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Threading.Tasks;

namespace ChaosHostingApi.Services
{
    public class ChaosGameManager
    {
        private readonly ILoggerFactory loggerFactory;
        private readonly IHubContext<VotingHub> votingHub;
        private readonly IHubContext<HostingHub> hostingHub;
        private ConcurrentDictionary<string, ChaosGame> _games;

        public ChaosGameManager(ILoggerFactory loggerFactory, IHubContext<VotingHub> votingHub, IHubContext<HostingHub> hostingHub)
        {
            _games = new ConcurrentDictionary<string, ChaosGame>();
            this.loggerFactory = loggerFactory ?? throw new ArgumentNullException(nameof(loggerFactory));
            this.votingHub = votingHub ?? throw new ArgumentNullException(nameof(votingHub));
            this.hostingHub = hostingHub ?? throw new ArgumentNullException(nameof(hostingHub));
        }

        public void CreateGame(string roomPassword, string roomCode)
        {
            if (_games.ContainsKey(roomCode))
                return;

            var game = new ChaosGame(loggerFactory.CreateLogger<ChaosGame>(), votingHub, hostingHub, roomCode, roomPassword);

            _games.TryAdd(roomCode, game);
        }

        public async Task CloseGame(string roomPassword, string roomCode)
        {
            if (!_games.ContainsKey(roomCode) 
                || !_games.TryGetValue(roomCode, out var game)) 
                return;

            var gameClosed = await game.CloseGame(roomPassword);

            if (!gameClosed) return;

            _games.TryRemove(game.RoomCode, out _);
        }

        public bool GameExistsWithRoomCodeAndPassword(string roomCode, string roomPassword)
        {
            if (!_games.TryGetValue(roomCode, out var game)) return false;

           return game.UserHasAccess(roomCode, roomPassword);
        }

        public bool GameExistsWithRoomCode(string roomCode)
        {
            return _games.ContainsKey(roomCode);
        }

        public async Task CastVote(string roomCode, int round, int optionId, string ipAddress)
        {
            if (!_games.TryGetValue(roomCode, out var game)) return;

            await game.CastClientVote(optionId, ipAddress, round);
        }

        public CompleteGameState GetCompleteGameState(string roomCode)
        {
            if (!_games.TryGetValue(roomCode, out var game)) return null;

            return game.GetCompleteGameState();
        }

        public PartialGameState GetPartialGameState(string roomCode)
        {
            if (!_games.TryGetValue(roomCode, out var game)) return null;

            return game.GetPartialGameState();
        }

        public async Task SetNewVoteOptions(HostVoteOptions hostVoteOptions)
        {
            if (hostVoteOptions == null || !hostVoteOptions.VoteOptions.Any()) return;

            if (!_games.TryGetValue(hostVoteOptions.RoomCode, out var game)) return;

            await game.SetNewVoteOptions(hostVoteOptions.VoteOptions.Select((vo, i) => new VoteOption
            {
                ChaosId = vo.ChaosId,
                Description = vo.Description,
                Id = i
            }).ToArray());
        }
    }
}