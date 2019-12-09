namespace Plantagotchi.Models.Interfaces
{
    public interface IReading
    {
        int Temperature { get; set; }
      
        int Humidity { get; set; }
        
        int LightLevel { get; set; }
        
        int SoilMoistureLevel { get; set; }
    }
}