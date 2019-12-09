using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Plantagotchi.Services;

namespace Plantagotchi.Controllers
{    
    [ApiController]
    [Route("[controller]")]
    public class UsersController : ControllerBase
    {
        private readonly ILogger<UsersController> _logger;
        private readonly UserService _userService;

        public UsersController(ILogger<UsersController> logger, UserService userService)
        {
            _logger = logger;
            _userService = userService;
        }
        
        [HttpPost("test")]
        public IActionResult PopulateTestData()
        {
            _userService.PopulateTestData();
            return Ok();
        }
    }
}