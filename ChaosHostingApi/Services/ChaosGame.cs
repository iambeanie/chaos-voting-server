using ChaosHostingApi.Domain.Voting;
using ChaosHostingApi.Hubs;
using ChaosHostingApi.Models.Client;
using Microsoft.AspNetCore.SignalR;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Net.WebSockets;
using System.Threading.Tasks;

namespace ChaosHostingApi.Services
{
    public class ChaosGame
    {
        private readonly ILogger<ChaosGame> logger;
        private readonly IHubContext<VotingHub> votingHub;
        private readonly IHubContext<HostingHub> hostingHub;
        private readonly Random random;

        #region voting members

        public VoteOption[] CurrentVotingOptions { get; private set; }
        public string PreviousVoteDescription { get; private set; }
        public ConcurrentDictionary<string, int> CurrentVotes { get; private set; }

        private int _currentVotingOption;
        private int _currentVotingRound;

        private string roomPassword;
        public string RoomCode { get; set; }

        #endregion

        public ChaosGame(ILogger<ChaosGame> logger, IHubContext<VotingHub> votingHub, IHubContext<HostingHub> hostingHub, string roomCode, string roomPassword)
        {
            this.logger = logger ?? throw new ArgumentNullException(nameof(logger));
            this.votingHub = votingHub ?? throw new ArgumentNullException(nameof(votingHub));
            this.hostingHub = hostingHub ?? throw new ArgumentNullException(nameof(hostingHub));
            RoomCode = roomCode ?? throw new ArgumentNullException(nameof(roomCode));
            this.roomPassword = roomPassword ?? throw new ArgumentNullException(nameof(roomPassword));

            this.logger.LogInformation("Chaos Game Manager Initializing");

            random = new Random();

            CurrentVotingOptions = Array.Empty<VoteOption>();
            PreviousVoteDescription = null;
            CurrentVotes = new ConcurrentDictionary<string, int>();

            _currentVotingOption = -1;
            _currentVotingRound = 1;
        }

        private VoteOption GetCurrentVoteOptionFromVotes()
        {
            var options = CurrentVotingOptions.Clone() as VoteOption[];
            var votes = CurrentVotes.Values.ToArray();

            if (!votes.Any())
            {
                var randomIndex = random.Next(0, options.Length - 1);

                var randomVote = options[randomIndex];

                return randomVote;
            }
            else
            {
                var groupings = votes.Where(x => options.Any(y => y.Id == x))
                    .GroupBy(x => x)
                    .OrderByDescending(x => x.Count());

                var maxCount = groupings.First().Count();

                var relevantGroupings = groupings.Where(x => x.Count() == maxCount);

                if (relevantGroupings.Count() == 1)
                {
                    return options.First(x => x.Id == relevantGroupings.First().Key);
                }
                else
                {
                    var randomizedGroupingIndex = random.Next(0, relevantGroupings.Count());

                    var randomizedGrouping = relevantGroupings.ElementAt(randomizedGroupingIndex);

                    return options.First(x => x.Id == randomizedGrouping.Key);
                }
            }
        }

        #region Client

        public async Task CastClientVote(int votedOptionId, string voterIPAddress, int round)
        {
            if (round != _currentVotingRound) return;

            if (!CurrentVotingOptions.Any(x => x.Id == votedOptionId)) return;

            logger.LogDebug($"Casting vote from {voterIPAddress} for {votedOptionId} during round {round}");

            var hasVoterAlreadyVoted = CurrentVotes.TryGetValue(voterIPAddress, out var existingValue);

            if (hasVoterAlreadyVoted && existingValue.Equals(votedOptionId)) return;

            if (hasVoterAlreadyVoted)
            {
                CurrentVotes.TryUpdate(voterIPAddress, votedOptionId, existingValue);
            }
            else
            {
                CurrentVotes.TryAdd(voterIPAddress, votedOptionId);
            }

            var currentVote = GetCurrentVoteOptionFromVotes();

            if (currentVote.Id != _currentVotingOption)
            {
                await PushNewVoteResultToHost(currentVote.ChaosId);
            }

            _currentVotingOption = currentVote.Id;

            await PushVoteStateToClients();
        }

        private async Task PushNewVoteOptionsToClients()
        {
            logger.LogDebug("Pushing new vote options to Clients");

            var completeGameState = GetCompleteGameState();

            await votingHub.Clients.Group(RoomCode).SendAsync("newVoteOptions", completeGameState);
        }

        private async Task PushVoteStateToClients()
        {
            logger.LogDebug("Pushing vote state to clients");

            var partialVoteState = GetPartialGameState();

            await votingHub.Clients.Group(RoomCode).SendAsync("voteUpdate", partialVoteState);
        }

        #endregion

        #region Host 

        public async Task SetNewVoteOptions(VoteOption[] newVotingOptions)
        {
            logger.LogInformation("Setting new vote options");

            PreviousVoteDescription = CurrentVotingOptions?.FirstOrDefault(x => x.Id == _currentVotingOption)?.Description
                ?? "No Voting rounds completed yet";

            CurrentVotingOptions = newVotingOptions;

            CurrentVotes.Clear();

            _currentVotingRound++;

            logger.LogInformation("Updating Game State");

            var currentVote = GetCurrentVoteOptionFromVotes();

            _currentVotingOption = currentVote.Id;

            await PushNewVoteResultToHost(currentVote.ChaosId);
            await PushNewVoteOptionsToClients();
        }

        private async Task PushNewVoteResultToHost(string chaosId)
        {
            logger.LogInformation($"Attempting to update host with chaos id: {chaosId}");

            await hostingHub.Clients.Group(RoomCode).SendAsync("newVoteResult", chaosId);
        }

        #endregion

        #region Models

        public CompleteGameState GetCompleteGameState()
        {
            return new CompleteGameState
            {
                PreviousResultDescription = PreviousVoteDescription,
                CurrentRound = _currentVotingRound,
                VoteOptionsState = CurrentVotingOptions.Select(x =>
                {
                    var votesForOption = CurrentVotes.Values.Count(y => y == x.Id);

                    return new FullVoteOptionState
                    {
                        Description = x.Description,
                        Id = x.Id,
                        Count = votesForOption
                    };
                }).ToArray()
            };
        }

        public PartialGameState GetPartialGameState()
        {
            return new PartialGameState
            {
                PartialVoteStates = CurrentVotingOptions.Select(x =>
                {
                    var votesForOption = CurrentVotes.Values.Count(y => y == x.Id);

                    return new PartialVoteState
                    {
                        Id = x.Id,
                        Count = votesForOption
                    };
                })
            };
        }

        #endregion

        public async Task<bool> CloseGame(string roomPassword)
        {
            if (!roomPassword.Equals(this.roomPassword)) return false;

            await hostingHub.Clients.Group(RoomCode).SendAsync("gameClosed");
            await votingHub.Clients.Group(RoomCode).SendAsync("gameClosed");

            return true;
        }

        public bool UserHasAccess(string roomCode, string roomPassword)
        {
            return this.roomPassword.Equals(roomPassword) && RoomCode.Equals(roomCode);
        }
    }
}
