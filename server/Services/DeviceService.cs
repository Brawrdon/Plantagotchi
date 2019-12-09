using System;
using System.Collections.Generic;
using System.Linq;
using MongoDB.Bson;
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
            return _devices.Find(device => device.SerialNumber.Equals(serialNumber) && device.Available).FirstOrDefault();
        }

        public bool ConfigureDevice(string serialNumber, StreakInterval streakInterval)
        {
            var builder = new UpdateDefinitionBuilder<Device>();
            var update = builder.Set(nameof(Device.StreakInterval), streakInterval).Set(nameof(Device.Available), true);
            
            return UpdateOne(serialNumber, update);

        }

        public bool AddReadingToDevice(string serialNumber, Reading reading)
        {
            var builder = Builders<Device>.Update; 
            var update = builder.Push(nameof(Device.Readings), reading);
            
            return UpdateOne(serialNumber, update);
        }

        public List<Reading> GetReadingsFromDate(string serialNumber, DateTime dateTime)
        {
            return _devices.Find(devices => devices.SerialNumber.Equals(serialNumber)).FirstOrDefault().Readings.Where(reading =>
            {
                var objectId = ObjectId.Parse(reading.Id);
                return objectId.CreationTime.Date == dateTime.Date;
            }).ToList();
        }

        public List<Reading> GetReadingsFromDevice(string serialNumber)
        {
            return _devices.Find(devices => devices.SerialNumber.Equals(serialNumber)).FirstOrDefault().Readings;
        }


        public void PopulateTestData()
        {
            _devices.DeleteMany(device => true);
            _devices.InsertOne(new Device
            {
                Available = false,
                Readings = new List<Reading>(),
                SerialNumber = "2109698364",
                Streak = 0
            });

            AddReadingToDevice("2109698364", new Reading
            {
                Id = "5dec3d000000000000000000",
                Temperature = 10,
                SoilMoistureLevel = 10,
                Humidity = 10,
                InitiatedByUser = "5dee87327d441e41fe4d5acb",
                LightLevel = 10,
                WithinHumidityRange = true,
                WithinTemperatureRange = false,
                WithinLightLevelRange = false
            });
        }

        private bool UpdateOne(string serialNumber, UpdateDefinition<Device> update)
        {
            var updateResult = _devices.UpdateOne(device => device.SerialNumber.Equals(serialNumber), update);
            return updateResult.IsAcknowledged && updateResult.ModifiedCount == 1;
        }
    }
}