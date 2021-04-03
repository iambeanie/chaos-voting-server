using System;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR.Client;
using Newtonsoft.Json;

namespace ChaosClientIntermediary
{
    class Program
    {
        private const int _apiAttempts = 5;
        private static AppSettings _settings;

        private static object _lock = false;
        private static DateTime _lastRead = DateTime.Now.AddMinutes(-1);

        private static string _roomCode;

        private static FileSystemWatcher _fileSystemWatcher = null;
        private static HubConnection _hubConnection = null;

        private static ChaosApiClient chaosApiClient = null;

        static async Task Main()
        {
            try
            {
                if (!await ValidateSettings()) return;

                await SetupGame();

                Console.WriteLine("Game is setup on API.");

                SetupFileSystemWatcher();

                Console.WriteLine("Intermediary is watching for file changes.");

                await OpenGameConnection();

                Console.WriteLine("Game Connection is open.");

                while (true)
                {
                    await Task.Delay(100);
                }
            }
            catch (Exception)
            {
                Console.WriteLine("Exiting");
            }
        }

        private static async Task SetupGame()
        {
            var httpClient = new HttpClient();

            httpClient.DefaultRequestHeaders.Add("AdminPassword", _settings.ServerAdminPassword);

            var httpType = _settings.UseHttps ? "https" : "http";

            var chaosClient = new ChaosApiClient($"{httpType}://{_settings.HostingApiUrl}", httpClient);

            _roomCode = CreateRoomCode();

            int attempts = 0;

            while (attempts < _apiAttempts)
            {
                try
                {
                    await chaosClient.CreateGameAsync(_roomCode, _settings.RoomPassword);
                    break;
                }
                catch (Exception)
                {
                    attempts++;

                    if (attempts == _apiAttempts) throw;

                    Console.WriteLine($"An error occurred while creating the game on the API, attempt number: {attempts} of {_apiAttempts}, retrying in 5s.");
                    await Task.Delay(5000);
                }
            }

            
            await File.WriteAllTextAsync(@"C:\chaos\roomCode.txt", _roomCode);
        }

        private static void SetupFileSystemWatcher()
        {
            _fileSystemWatcher = new FileSystemWatcher(_settings.VoteFilesDirectory, "*.txt");

            _fileSystemWatcher.Changed += OnFileInChaosDirectoryChanged;

            _fileSystemWatcher.EnableRaisingEvents = true;
        }

        private static string CreateRoomCode()
        {
            var random = new Random();

            var builder = new StringBuilder();

            for (var i = 0; i < 6; i++)
            {
                var randomChar = random.Next(0, 26 + 10);

                if (randomChar < 10)
                {
                    builder.Append(randomChar.ToString());
                }
                else
                {
                    var charValue = (randomChar - 10) + 97;

                    builder.Append((char)charValue);
                }
            }

            return builder.ToString();
        }

        private static async Task<bool> ValidateSettings()
        {
            if (!File.Exists("./appsettings.json"))
            {
                Console.WriteLine("No appsettings.json file found with executable.");
                return false;
            }

            try
            {
                var settingsText = await File.ReadAllTextAsync("./appsettings.json");

                _settings = JsonConvert.DeserializeObject<AppSettings>(settingsText);
            }
            catch (Exception exc)
            {
                Console.WriteLine($"ERROR: {exc.Message}. An error occurred while loading the settings file.");
                return false;
            }

            if (string.IsNullOrWhiteSpace(_settings.VoteFilesDirectory))
            {
                Console.WriteLine($"Provided vote files directory '{_settings.VoteFilesDirectory}' is not valid. Please provide a directory to use for file storage.");
                return false;
            }

            if (string.IsNullOrWhiteSpace(_settings.HostingApiUrl))
            {
                Console.WriteLine($"Provided hosting api url '{_settings.HostingApiUrl}' is not valid. Please provide a valid url to the hosting API.");
                return false;
            }

            if (!Directory.Exists(_settings.VoteFilesDirectory))
            {
                try
                {
                    Directory.CreateDirectory(_settings.VoteFilesDirectory);
                }
                catch (Exception exc)
                {
                    Console.WriteLine($"ERROR: {exc.Message}. An error occurred while trying to create the following directory: '{_settings.VoteFilesDirectory}'");
                    return false;
                }
            }

            if (!File.Exists(Path.Combine(_settings.VoteFilesDirectory, "vote.txt")))
            {
                try
                {
                    await File.WriteAllTextAsync(Path.Combine(_settings.VoteFilesDirectory, "vote.txt"), "nothing");
                }
                catch (Exception exc)
                {
                    Console.WriteLine($"ERROR: {exc.Message}. An error occurred while trying to create the following file: '{Path.Combine(_settings.VoteFilesDirectory, "vote.txt")}'");
                    return false;
                }
            }

            if (!File.Exists(Path.Combine(_settings.VoteFilesDirectory, "voteoptions.txt")))
            {
                try
                {
                    await File.WriteAllTextAsync(Path.Combine(_settings.VoteFilesDirectory, "voteoptions.txt"), "");
                }
                catch (Exception exc)
                {
                    Console.WriteLine($"ERROR: {exc.Message}. An error occurred while trying to create the following file: '{Path.Combine(_settings.VoteFilesDirectory, "voteoptions.txt")}'");
                    return false;
                }
            }

            return true;
        }

        private static void OnFileInChaosDirectoryChanged(object source, FileSystemEventArgs eventArgs)
        {
            if (!eventArgs.Name.Equals("voteoptions.txt")) return;

            try
            {
                var now = DateTime.Now;

                var timeSinceLastEventFired = (now - _lastRead).TotalMilliseconds;

                if (timeSinceLastEventFired < 500)
                {
                    return;
                }

                Console.WriteLine("Vote options found by filewatcher, updating on voting server.");

                _lastRead = now;

                string voteOptionsText = "";

                try
                {
                    voteOptionsText = File.ReadAllText(eventArgs.FullPath);
                }
                catch (Exception)
                {
                    Thread.Sleep(200);
                    voteOptionsText = File.ReadAllText(eventArgs.FullPath);
                }

                var voteOptionsLines = voteOptionsText.Split(Environment.NewLine);

                var hostVoteOptions = new HostVoteOptions
                {
                    RoomCode = _roomCode,
                    VoteOptions = voteOptionsLines
                        .Where(x => !string.IsNullOrWhiteSpace(x))
                        .Select(x =>
                        {
                            var split = x.Split(',');

                            return new HostVoteOption
                            {
                                Description = string.Join(",", split.Skip(1)),
                                ChaosId = split[0]
                            };
                        })
                        .ToList()
                };

                if (!hostVoteOptions.VoteOptions.Any())
                {
                    Console.WriteLine("Options are empty in file.");
                    return;
                }

                chaosApiClient.UpdateVoteOptionsForRoomAsync(hostVoteOptions).Wait();
            }
            catch (Exception exc)
            {
                Console.WriteLine($"ERROR in filewatch: {exc}");
            }
        }

        private static async Task OpenGameConnection()
        {
            var httpType = _settings.UseHttps ? "https" : "http";

            var httpClient = new HttpClient();
            httpClient.DefaultRequestHeaders.Add("AdminPassword", _settings.ServerAdminPassword);

            chaosApiClient = new ChaosApiClient($"{httpType}://{_settings.HostingApiUrl}", httpClient);

            var hostingHubUrl = $"{httpType}://{_settings.HostingApiUrl}/hostinghub";

            _hubConnection = new HubConnectionBuilder()
                .WithUrl(hostingHubUrl)
                .WithAutomaticReconnect()
                .Build();

            int attempts = 0;

            while (attempts < _apiAttempts) {
                try
                {
                    await _hubConnection.StartAsync();
                    break;
                }
                catch (Exception)
                {
                    attempts++;

                    if (attempts == _apiAttempts) throw;

                    Console.WriteLine($"An error occurred while connecting to the hosting servers message hub, attempt number: {attempts} of {_apiAttempts}, retrying in 5s.");
                    await Task.Delay(5000);
                }
            }

            _hubConnection.On("newVoteResult", (string chaosId) =>
            {
                Console.WriteLine("New vote result received");

                lock (_lock)
                {
                    File.WriteAllText(Path.Combine(_settings.VoteFilesDirectory, "vote.txt"), chaosId);
                }
            });

            await _hubConnection.SendAsync("RegisterAsHost", _roomCode, _settings.RoomPassword, _settings.ServerAdminPassword);
        }
    }
}