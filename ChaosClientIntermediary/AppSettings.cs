namespace ChaosClientIntermediary
{
    public class AppSettings
    {
        public string VoteFilesDirectory { get; set; }
        public string HostingApiUrl { get; set; }
        public bool UseHttps { get; set; }
        
        public string ServerAdminPassword { get; set; }
        public string RoomPassword { get; set; }
    }
}