using System.Collections.Generic;
using MongoDB.Driver;
using Plantagotchi.Models.Database;

namespace Plantagotchi.Services
{
    public class UserService
    {
        private readonly IMongoCollection<User> _users;
        
        public UserService(IDatabaseSettings settings)
        {
            var client = new MongoClient(settings.ConnectionString);
            var database = client.GetDatabase(settings.DatabaseName);

            _users = database.GetCollection<User>(settings.UserCollectionName);
        }
        
        public void PopulateTestData()
        {
            _users.DeleteMany(user => true);
            _users.InsertOne(new User()
            {
                Id = "5dee87327d441e41fe4d5acb",
                Name = "Brandon"
            });
        }
        
    }
}