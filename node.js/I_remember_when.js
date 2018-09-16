
var Twit = require("twit");
var mqtt = require('mqtt')

var T = new Twit({
  consumer_key: 'oPt39CMn6vLQjQSV0TKS4FYXW',
  consumer_secret: 'JaFENSbbTNV2UorQDxD46oxAMJEJyl2eNIR2LPaKFVvL9G2EwN',
  access_token: '843853018453958656-VmGYekpWTSBmfnHMkXptGb2D3DAUA93',
  access_token_secret: 'Dx5V2uMhDhPMEPIXkVL8yJ9WmAcM6d15VCDZjLTFex7Ha',
  //timeout_ms:           60*1000,  // optional HTTP request timeout to apply to all requests.
});

var msg; // holds data string from twitter
console.log(" ... collecting data from Twitter API containing text --> I remember when <-- ... ");

var stream = T.stream('statuses/filter', { track: 'I remember when' })
stream.on('tweet', function (tweet) {
  msg = tweet.text;
  console.log(msg);

  if (msg != null) {
    var num = Math.floor((Math.random() * 6) + 1).toString();
    MQTT(num);
    msg = null;
  }
});

function MQTT(val) {
 // var client = mqtt.connect("10.3.141.1", port: 1883);
  var client = mqtt.connect({ host: '10.3.141.1', port: 1883 });
  
  client.on('connect', function () { // When connected
    // publish a message to a topic
    client.publish("test", val, function () {
      //console.log("Message is published");
      client.end(); // Close the connection when published
    });
  });
};
