using System;
using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json;

namespace Led_Pong_App
{
    public class NatanTest
    {
        public string Id { get; set; }

        [JsonProperty(PropertyName = "GameName")]
        public string GameName { get; set; }

        [JsonProperty(PropertyName = "complete")]
        public bool Complete { get; set; }
    }
}
