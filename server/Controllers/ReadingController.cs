using System;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Plantagotchi.Models;

namespace Plantagotchi.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class ReadingController : ControllerBase
    {
        private readonly ILogger<ReadingController> _logger;

        public ReadingController(ILogger<ReadingController> logger)
        {
            _logger = logger;
        }

        [HttpPost]
        public String ReceiveTemperature(Reading reading) 
        {
            _logger.LogDebug("Temperature: " + reading.Temperature);
            _logger.LogDebug("Humidity: " + reading.Humidity);
            _logger.LogDebug("Light Level: " + reading.LightLevel);

            return "Acknowledged";
        }
    }
}