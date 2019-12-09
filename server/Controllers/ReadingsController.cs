using System;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Plantagotchi.Models;
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


    }
}