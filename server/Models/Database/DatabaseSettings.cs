namespace Plantagotchi.Models.Database
{
    public interface IDatabaseSettings
    {
        string DeviceCollectionName { get; set; }
        string ConnectionString { get; set; }
        string DatabaseName { get; set; }
    }
    
    public class DatabaseSettings : IDatabaseSettings
    {
        public string DeviceCollectionName { get; set; }
        public string ConnectionString { get; set; }
        public string DatabaseName { get; set; }
    }
}