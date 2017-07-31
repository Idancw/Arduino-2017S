using System;
using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json;

namespace Led_Pong_App
{
    public class PlayerItem
    {
        public string Id { get; set; }

        [JsonProperty(PropertyName = "Score")]
        public int Score { get; set; }

        [JsonProperty(PropertyName = "GamesPlayed")]
        public int GamesPlayed { get; set; }
    }
}
