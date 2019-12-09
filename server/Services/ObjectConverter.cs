using System.Text.Json;
using Plantagotchi.Models;
using Plantagotchi.Models.Database;

namespace Plantagotchi.Services
{
    public static class ObjectConverter
    {
        public static T1 Convert<T1, T2>(T2 convertFrom)
        {
            var jsonString = JsonSerializer.Serialize(convertFrom);

            return JsonSerializer.Deserialize<T1>(jsonString);
        }
    }
}