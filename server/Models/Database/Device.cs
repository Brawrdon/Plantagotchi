using System.Collections.Generic;
using MongoDB.Bson.Serialization.Attributes;

namespace Plantagotchi.Models.Database
{
    public class Device
    {
        [BsonId]
        public string SerialNumber { get; set; }
        
        public int Streak { get; set; }
        
        public StreakInterval StreakInterval { get; set; }
        
        public List<Reading> Readings { get; set; }
        
        public bool Available { get; set; }

    }

    public class StreakInterval
    {
        public string Start { get; set; }
        
        public string End { get; set; }
    }
}