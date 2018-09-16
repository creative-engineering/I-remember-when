
#include <ESP8266WiFi.h>  //for the wifi connection
#include <PubSubClient.h> //for the MQTT server connection

// Setup wifi and MQTT //
// Update these with values suitable for your network.
const char* ssid = "I_Remember_When";
const char* password = "IRW_2018";
const char* mqtt_server = "10.3.141.1";
const int mqtt_port = 1883;

void callback(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(mqtt_server, mqtt_port, callback, espClient);

#define UP 0
#define DOWN 1

boolean PCBfadeDirection = UP;

boolean data1 = HIGH;
boolean data2 = HIGH;
boolean data3 = HIGH;
boolean data4 = HIGH;
boolean data5 = HIGH;
boolean data6 = HIGH;

unsigned long oldMillis = 0;
unsigned long oldMillis2 = 0;

int increment;            // increment to move for each interval
int updateInterval;       // interval between updates

const int minPWM = 0;
const int maxPWM = 1024; //

byte fadeDirection1 = UP;
byte fadeDirection2 = UP;
byte fadeDirection3 = UP;
byte fadeDirection4 = UP;
byte fadeDirection5 = UP;
byte fadeDirection6 = UP;

int fadeValue1 = 0;
int fadeValue2 = 0;
int fadeValue3 = 0;
int fadeValue4 = 0;
int fadeValue5 = 0;
int fadeValue6 = 0;

int PCBfadeValue = 0;
bool PCBflag = LOW;

bool updateFlag1 = LOW;
bool updateFlag2 = LOW;
bool updateFlag3 = LOW;
bool updateFlag4 = LOW;
bool updateFlag5 = LOW;
bool updateFlag6 = LOW;


void Update1()    // update D0
{
  if (fadeDirection1 == UP ) {
    fadeValue1 = fadeValue1 + 5;
    if (fadeValue1 >= maxPWM) {              // At max, limit and change direction
      fadeValue1 = maxPWM;
      fadeDirection1 = DOWN;
    }
  }
  else {
    fadeValue1 = fadeValue1 - 2;  // if we aren't going up, we're going down
    if (fadeValue1 <= minPWM) {
      fadeValue1 = minPWM;
      fadeDirection1 = UP;
      data1 = LOW;
    }
  }
  analogWrite(D0, fadeValue1);
}



void Update2()     // update D1
{
  if (fadeDirection2 == UP ) {
    fadeValue2 = fadeValue2 + 5;
    if (fadeValue2 >= maxPWM) {
      fadeValue2 = maxPWM;
      fadeDirection2 = DOWN;
    }
  }
  else {
    fadeValue2 = fadeValue2 - 2;
    if (fadeValue2 <= minPWM) {
      fadeValue2 = minPWM;
      fadeDirection2 = UP;
      data2 = LOW;
    }
  }
  analogWrite(D1, fadeValue2);
}


void Update3()   // update D2
{
  if (fadeDirection3 == UP ) {
    fadeValue3 = fadeValue3 + 5;
    if (fadeValue3 >= maxPWM) {
      fadeValue3 = maxPWM;
      fadeDirection3 = DOWN;
    }
  }
  else {
    fadeValue3 = fadeValue3 - 2;
    if (fadeValue3 <= minPWM) {
      fadeValue3 = minPWM;
      fadeDirection3 = UP;
      data3 = LOW;
    }
  }
  analogWrite(D2, fadeValue3);
}

void Update4()          // update D5
{
  if ( fadeDirection4 == UP ) {
    fadeValue4 = fadeValue4 + 5;
    if (fadeValue4 >= maxPWM) {
      fadeValue4 = maxPWM;
      fadeDirection4 = DOWN;
    }
  }
  else {
    fadeValue4 = fadeValue4 - 2;
    if (fadeValue4 <= minPWM) {
      fadeValue4 = minPWM;
      fadeDirection4 = UP;
      data4 = LOW;
    }
  }
  analogWrite(D5, fadeValue4);
}


void Update5()            // update D6
{
  if (fadeDirection5 == UP ) {
    fadeValue5 = fadeValue5 + 5;
    if (fadeValue5 >= maxPWM) {
      fadeValue5 = maxPWM;
      fadeDirection5 = DOWN;
    }
  }
  else {
    fadeValue5 = fadeValue5 - 2;
    if (fadeValue5 <= minPWM) {
      fadeValue5 = minPWM;
      fadeDirection5 = UP;
      data5 = LOW;
    }
  }
  analogWrite(D6, fadeValue5);
}


void Update6()             // update D7
{
  if (fadeDirection6 == UP ) {
    fadeValue6 = fadeValue6 + 5;
    if (fadeValue6 >= maxPWM) {
      fadeValue6 = maxPWM;
      fadeDirection6 = DOWN;
    }
  }
  else {
    fadeValue6 = fadeValue6 - 2;
    if (fadeValue6 <= minPWM) {
      fadeValue6 = minPWM;
      fadeDirection6 = UP;
      data6 = LOW;
    }
  }
  analogWrite(D7, fadeValue6);
}

void PCB () {

  if ( PCBfadeDirection == UP ) {
    PCBfadeValue = PCBfadeValue + 12;
    if ( PCBfadeValue >= 1023 )
    {
      PCBfadeValue = 1023;
      PCBfadeDirection = DOWN;
    }
  }
  else {
    PCBfadeValue = PCBfadeValue - 4;
    if (PCBfadeValue <= 0)
    {
      PCBfadeValue = 0;
      PCBfadeDirection = UP;
      PCBflag = LOW;
    }
  }
  //Serial.println(PCBfadeValue);
  analogWrite(D3, PCBfadeValue);
}


void setup() {

  pinMode(D0, OUTPUT);     // D0 - Initialize the PWM pins as output (set as GPIO)
  pinMode(D1, OUTPUT);     // D1
  pinMode(D2, OUTPUT);     // D2
  pinMode(D5, OUTPUT);     // D5
  pinMode(D6, OUTPUT);     // D6
  pinMode(D7, OUTPUT);     // D7
  pinMode(D3, OUTPUT);     //PCB LED

  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void setup_wifi() {
  // connecting to a WiFi network //
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Reconnects if wifi is lost //
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("MQTT-out", "Connected" );
      // ... and resubscribe
      client.subscribe("twit_data");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);                       // Wait 5 seconds before retrying
    }
  }
}

void callback(char* topic, byte* byteArrayPayload, unsigned int length) {
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  String payload;

  Serial.println();
  // Puts and converts the MQTT payload into a String //
  for (int i = 0; i < length; i++) {
    payload += (char)byteArrayPayload[i];
  }

  // Looks from MQTT input and puts data into data variable //
  if ((char)payload[0] == '1') {
    Serial.print(payload[0]);
    data1 = HIGH;
    payload[0] == '0';
  }
  else if ((char)payload[0] == '2') {
    Serial.print(payload[0]);
    data2 = HIGH;
    payload[0] == '0';
  }
  else if ((char)payload[0] == '3') {
    Serial.print(payload[0]);
    data3 = true;
    payload[0] == '0';
  }
  else if ((char)payload[0] == '4') {
    Serial.print(payload[0]);
    data4 = HIGH;
    payload[0] == '0';
  }
  else if ((char)payload[0] == '5') {
    Serial.print(payload[0]);
    data5 = HIGH;
    payload[0] == '0';
  }
  else if ((char)payload[0] == '6') {
    Serial.print(payload[0]);
    data6 = HIGH;
    payload[0] == '0';
  }
  updateFlag1 = HIGH;
  updateFlag2 = HIGH;
  updateFlag3 = HIGH;
  updateFlag4 = HIGH;
  updateFlag5 = HIGH;
  updateFlag6 = HIGH;
  PCBfadeValue = 0;
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  if (( millis() - oldMillis ) > 2 ) {

    if ( data1 ) {
      Update1();

      if ( updateFlag1 ) {
        PCBflag = HIGH;
        PCBfadeValue = 0;
        PCBfadeDirection = UP;
        updateFlag1 = LOW;
        fadeValue1 = 0;
      }
    }

    if ( data2 ) {
      Update2();
      if ( updateFlag2 ) {
        PCBflag = HIGH;
        PCBfadeValue = 0;
        PCBfadeDirection = UP;
        updateFlag2 = LOW;
        fadeValue2 = 0;
      }
    }

    if ( data3 ) {

      Update3();

      if ( updateFlag3 ) {
        PCBflag = HIGH;
        PCBfadeValue = 0;
        PCBfadeDirection = UP;
        updateFlag3 = LOW;
        fadeValue3 = 0;
      }
    }

    if ( data4 ) {

      Update4();

      if ( updateFlag4 ) {
        PCBflag = HIGH;
        PCBfadeValue = 0;
        PCBfadeDirection = UP;
        updateFlag4 = LOW;
        fadeValue4 = 0;
      }
    }

    if ( data5 ) {

      Update5();

      if ( updateFlag5 ) {
        PCBflag = HIGH;
        PCBfadeValue = 0;
        PCBfadeDirection = UP;
        updateFlag5 = LOW;
        fadeValue5 = 0;
      }
    }

    if ( data6 ) {

      Update6();

      if ( updateFlag6 ) {
        PCBflag = HIGH;
        PCBfadeValue = 0;
        PCBfadeDirection = UP;
        updateFlag6 = LOW;
        fadeValue6 = 0;
      }
    }

    oldMillis = millis();
  }

  if (( millis() - oldMillis2 ) > 1 ) {
    if (/* ( data1 || data2 || data3 || data4 || data5 || data6 )  && */ PCBflag  ) {
      PCB();
    }
    oldMillis2 = millis();
  }

}

