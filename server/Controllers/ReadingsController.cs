using System;
using System.Linq;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Plantagotchi.Models;
using Plantagotchi.Models.Database;
using Plantagotchi.Services;

namespace Plantagotchi.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class ReadingsController : ControllerBase
    {
        private readonly ILogger<ReadingsController> _logger;
        private readonly DeviceService _deviceService;

        public ReadingsController(ILogger<ReadingsController> logger, DeviceService deviceService)
        {
            _logger = logger;
            _deviceService = deviceService;
        }

        [HttpPost("{serialNumber}")]
        public IActionResult AddReading([FromRoute] string serialNumber, [FromBody] ReadingRequest readingRequest)
        {
            var device = _deviceService.FindDevice(serialNumber);

            if (device == null)
                return NotFound();
            
            var reading = readingRequest.ConvertForDatabase();
            
            if (!reading.IsWithinStreakInterval(device.StreakInterval))
            {
                var latestReading = _deviceService.GetLatestReading(device.SerialNumber);

                if (latestReading.IsWithinStreakInterval(device.StreakInterval))
                    CheckForStreaks(device);
            }
            
            var updateSuccessful = _deviceService.AddReadingToDevice(device.SerialNumber, reading);

            if (!updateSuccessful)
                return BadRequest();
                
            return Ok();
        }

        [HttpGet("{serialNumber}")]
        public IActionResult GetReadingsForDate([FromRoute] string serialNumber, [FromQuery] DateTime date)
        {
            var device = _deviceService.FindDevice(serialNumber);

            if (device == null)
                return NotFound();
            
            var response = _deviceService.GetReadingsFromDate(device.SerialNumber, date);

            return Ok(response);
        }

        private void CheckForStreaks(Device device)
        {
            var readings = _deviceService.GetReadingsFromDate(device.SerialNumber, DateTime.Today);

            var readingsInRange = readings.Select(x => x).Where(x => x.WithinHumidityRange && x.WithinTemperatureRange && x.WithinLightLevelRange).ToList();
            var readingsNotInRange = readings.Select(x => x).Where(x => !x.WithinHumidityRange || !x.WithinTemperatureRange || !x.WithinLightLevelRange).ToList();

            if (readingsInRange.Count > readingsNotInRange.Count)
                _deviceService.UpdateStreak(device.SerialNumber);
            else
                _deviceService.ResetStreak(device.SerialNumber);
            
        }
    }
}