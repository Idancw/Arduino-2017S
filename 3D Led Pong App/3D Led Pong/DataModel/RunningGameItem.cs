using System;
using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json;

namespace Led_Pong_App
{
    public class RunningGameItem
    {
        public string Id { get; set; }

        [JsonProperty(PropertyName = "GameName")]
        public string GameName { get; set; }

        [JsonProperty(PropertyName = "Complete")]
        public bool Complete { get; set; }

        [JsonProperty(PropertyName = "PlayersNum")]
        public int PlayersNum { get; set; }

        [JsonProperty(PropertyName = "Player1")]
        public string Player1 { get; set; }

        [JsonProperty(PropertyName = "Player2")]
        public string Player2 { get; set; }

        [JsonProperty(PropertyName = "Player3")]
        public string Player3 { get; set; }

        [JsonProperty(PropertyName = "Player4")]
        public string Player4 { get; set; }

        [JsonProperty(PropertyName = "PointsPlayer1")]
        public int PointsPlayer1 { get; set; }

        [JsonProperty(PropertyName = "PointsPlayer2")]
        public int PointsPlayer2 { get; set; }

        [JsonProperty(PropertyName = "PointsPlayer3")]
        public int PointsPlayer3 { get; set; }

        [JsonProperty(PropertyName = "PointsPlayer4")]
        public int PointsPlayer4 { get; set; }

        [JsonProperty(PropertyName = "PlayerQuit1")]
        public bool PlayerQuit1 { get; set; }

        [JsonProperty(PropertyName = "PlayerQuit2")]
        public bool PlayerQuit2 { get; set; }

        [JsonProperty(PropertyName = "PlayerQuit3")]
        public bool PlayerQuit3 { get; set; }

        [JsonProperty(PropertyName = "PlayerQuit4")]
        public bool PlayerQuit4 { get; set; }

        [JsonProperty(PropertyName = "GameStarted")]
        public bool GameStarted { get; set; }
    }
}
