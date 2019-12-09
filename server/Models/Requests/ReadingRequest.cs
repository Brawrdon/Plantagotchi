using Plantagotchi.Models.Interfaces;

namespace Plantagotchi.Models.Requests
{
    public class ReadingRequest : IReading
    {
        public int Temperature { get; set; }
      
        public int Humidity { get; set; }
        
        public int LightLevel { get; set; }
        
        public int SoilMoistureLevel { get; set; }
    }
}