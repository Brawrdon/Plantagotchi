using System.Collections.Generic;
using MongoDB.Driver;
using Plantagotchi.Models.Database;

namespace Plantagotchi.Services
{
    public class DeviceService
    {
        private readonly IMongoCollection<Device> _devices;
        
        public DeviceService(IDatabaseSettings settings)
        {
            var client = new MongoClient(settings.ConnectionString);
            var database = client.GetDatabase(settings.DatabaseName);

            _devices = database.GetCollection<Device>(settings.DeviceCollectionName);
        }

        public Device FindDevice(string serialNumber)
        {
            return _devices.Find(device => device.SerialNumber.Equals(serialNumber)).FirstOrDefault();
        }

        public bool SetDeviceAvailability(string serialNumber, bool availability)
        {
            var builder = new UpdateDefinitionBuilder<Device>();
            var update = builder.Set(nameof(Device.Available), availability);
            var response = _devices.UpdateOne(device => device.SerialNumber.Equals(serialNumber), update);

            return Updated(response);
        }
        
        public bool AddReadingToDevice(string serialNumber, Reading reading)
        {
            var builder = Builders<Device>.Update; 
            var update = builder.Push(nameof(Device.Readings), reading);

            var response = _devices.UpdateOne(device => device.SerialNumber.Equals(serialNumber), update);
            
            return Updated(response);
        }

        public List<Reading> GetReadingsForDevice(string serialNumber)
        {
            return _devices.Find(devices => devices.SerialNumber.Equals(serialNumber)).FirstOrDefault().Readings;
        }

        
        public void PopulateTestData()
        {
            _devices.DeleteMany(device => true);
            _devices.InsertOne(new Device()
            {
                Available = false,
                Readings = new List<Reading>(),
                SerialNumber = "2109698364",
                Streak = 0
            });
        }

        private bool Updated(UpdateResult updateResult)
        {
            return updateResult.IsAcknowledged && updateResult.ModifiedCount == 1;
        } 
    }
}