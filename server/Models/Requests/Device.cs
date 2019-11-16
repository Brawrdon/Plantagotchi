using System.Collections.Generic;

namespace Plantagotchi.Models.Requests
{
    public class Device
    {
        public int Serial { get; set; }
        
        public bool Connected { get; set; }
        
        public List<Reading> Readings { get; set; }
    }
}