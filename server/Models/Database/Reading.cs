using System;
using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;
using Plantagotchi.Models.Interfaces;

namespace Plantagotchi.Models.Database
{
    public class Reading : IReading
    {
        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string Id { get; set; }
        
        [BsonRepresentation(BsonType.ObjectId)]
        public string InitiatedByUser { get; set; }

        public int Temperature { get; set; }
      
        public int Humidity { get; set; }
        
        public int LightLevel { get; set; }
        
        public int SoilMoistureLevel { get; set; }
        
        public bool WithinTemperatureRange { get; set; }
        
        public bool WithinHumidityRange { get; set; }
        
        public bool WithinLightLevelRange { get; set; }
    }
    
    public static class ReadingExtensions {

        public static bool IsWithinStreakInterval(this Reading reading, StreakInterval streakInterval)
        {
            var startDateTime = DateTime.Parse(streakInterval.Start);
            var endDateTime = DateTime.Parse(streakInterval.End);

            var objectId = ObjectId.Parse(reading.Id);
            var readingCreationTime = TimeSpan.Parse(objectId.CreationTime.Hour + ":" + objectId.CreationTime.Minute);

            return readingCreationTime >= startDateTime.TimeOfDay && readingCreationTime <= endDateTime.TimeOfDay;
        }
    }
}