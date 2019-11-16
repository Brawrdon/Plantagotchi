using System;
using System.Collections.Generic;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Plantagotchi.Models.Requests;
using Plantagotchi.Models.Responses;

namespace Plantagotchi.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class DevicesController : ControllerBase
    {
        private readonly ILogger<ReadingsController> _logger;
        private readonly IList<Device> _devices;
        
        public DevicesController(ILogger<ReadingsController> logger, IList<Device> devices)
        {
            _devices = devices;
            _logger = logger;
        }

        [HttpPost]
        public IActionResult RegisterDevice(Device device)
        {
            if (device.Readings == null)
                device.Readings = new List<Reading>();

            _devices.Add(device);

            _logger.LogDebug("Device: " + device.Serial);

            return new OkObjectResult(new RequestResult {Result = "Device registered"});
        }
    }
}