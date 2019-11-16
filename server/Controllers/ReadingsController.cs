using System.Collections.Generic;
using System.Linq;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Plantagotchi.Models.Requests;
using Plantagotchi.Models.Responses;

namespace Plantagotchi.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class ReadingsController : ControllerBase
    {
        private readonly ILogger<ReadingsController> _logger;
        private readonly IList<Device> _devices;
        
        public ReadingsController(ILogger<ReadingsController> logger, IList<Device> devices)
        {
            _logger = logger;
            _devices = devices;
        }

        [HttpPost("{serial}")]
        public IActionResult AddReadings([FromRoute] int serial, [FromBody] Reading reading)
        {
            var device = _devices.FirstOrDefault(x => x.Serial == serial);
            if (device == null)
                return NotFound(new RequestResult() {Result = "Device not found"});
            
            _logger.LogDebug("Device: " + serial);

            device.Readings.Add(reading);
            
            _logger.LogDebug("Temperature: " + reading.Temperature);
            _logger.LogDebug("Humidity: " + reading.Humidity);
            _logger.LogDebug("Light Level: " + reading.LightLevel);

            return new OkObjectResult(new RequestResult {Result = "Readings added"});

        }
        
        [HttpGet("{serial}")]
        public IActionResult GetReadings([FromRoute] int serial)
        {
            var device = _devices.FirstOrDefault(x => x.Serial == serial);
            if (device == null)
                return NotFound(new RequestResult() {Result = "Device not found"});

            return new OkObjectResult(device.Readings);

        }
    }
}