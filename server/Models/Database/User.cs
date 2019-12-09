using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace Plantagotchi.Models.Database
{
    public class User
    {
        [BsonId]
        [BsonRepresentation(BsonType.ObjectId)]
        public string Id { get; set; }
        
        public string Name { get; set; }
    }
}