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
        

        [HttpPatch("{serialNumber}")]
        public IActionResult ConfigureDevice([FromRoute] string serialNumber, [FromBody] StreakInterval streakInterval)
        {
            var updateSuccessful = _deviceService.ConfigureDevice(serialNumber, streakInterval);

            if (!updateSuccessful)
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