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
        
        [BsonDateTimeOptions(Kind = DateTimeKind.Utc)]
        public DateTime Date { get; set; }
        
        public int Temperature { get; set; }
      
        public int Humidity { get; set; }
        
        public int LightLevel { get; set; }
        
        public int SoilMoistureLevel { get; set; }
    }
}