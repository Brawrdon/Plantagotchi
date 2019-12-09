using System;
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
    }

    public static class ReadingRequestExtensions
    {

        public static Reading ConvertForDatabase(this ReadingRequest readingRequest)
        {
            var reading = ObjectConverter.Convert<Reading, ReadingRequest>(readingRequest);
            reading.Date = DateTime.Now;
            if (readingRequest.UserId != null)
                reading.InitiatedByUser = readingRequest.UserId;

            return reading;
        }

    }
}