using System;
using MongoDB.Bson;
using Plantagotchi.Models.Database;
using Plantagotchi.Models.Interfaces;
using Plantagotchi.Services;

namespace Plantagotchi.Models
{
    public class ReadingRequest : IReading
    {
        public string UserId { get; set; }
        
        public int Temperature { get; set; }
      
        public int Humidity { get; set; }
        
        public int LightLevel { get; set; }
        
        public int SoilMoistureLevel { get; set; }
        
        public bool WithinTemperatureRange { get; set; }
        
        public bool WithinHumidityRange { get; set; }
        
        public bool WithinLightLevelRange { get; set; }
    }

    public static class ReadingRequestExtensions
    {

        public static Reading ConvertForDatabase(this ReadingRequest readingRequest)
        {
            var reading = ObjectConverter.Convert<Reading, ReadingRequest>(readingRequest);
            reading.Id = ObjectId.GenerateNewId().ToString();
            if (readingRequest.UserId != null)
                reading.InitiatedByUser = readingRequest.UserId;

            return reading;
        }
    }
}