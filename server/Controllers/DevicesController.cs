using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Plantagotchi.Models.Database;
using Plantagotchi.Services;

namespace Plantagotchi.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class DevicesController : ControllerBase
    {
        private readonly ILogger<DevicesController> _logger;
        private readonly DeviceService _deviceService;

        public DevicesController(ILogger<DevicesController> logger, DeviceService deviceService)
        {
            _logger = logger;
            _deviceService = deviceService;
        }


        [HttpPut("{serialNumber}")]
        public IActionResult RegisterDevice([FromRoute] string serialNumber)
        {
            var response = _deviceService.SetDeviceAvailability(serialNumber, true);

            if (!response)
                return BadRequest();

            return Ok();
        }
        
        [HttpPost("test")]
        public IActionResult PopulateTestData()
        {
            _deviceService.PopulateTestData();

            return Ok();
        }
    }
}