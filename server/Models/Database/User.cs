using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace Plantagotchi.Models.Database
{
    public class User
    {
        [BsonId]
        public string Id { get; set; }
        
        public string Name { get; set; }
    }
}