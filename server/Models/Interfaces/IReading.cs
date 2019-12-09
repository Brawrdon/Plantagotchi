namespace Plantagotchi.Models.Interfaces
{
    public interface IReading
    {
        int Temperature { get; set; }
      
        int Humidity { get; set; }
        
        int LightLevel { get; set; }
        
        int SoilMoistureLevel { get; set; }
        
        bool WithinTemperatureRange { get; set; }
        
        bool WithinHumidityRange { get; set; }
        
        bool WithinLightLevelRange { get; set; }
    }
}