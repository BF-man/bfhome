/*
   -Arduino 433MHz RF Sniffer
   -Based on an example from the RCSwitch Library
   -https://github.com/sui77/rc-switch/
   -Hardware: Arduino Nano & Generic 433MHz RF Receiver
   -T.K.Hareendran/2018
*/

// homie approach https://github.com/mhaack/mqtt-433mhz-gateway-homie

#include <Arduino.h>
#include <RCSwitch.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "credentials.h"
// #include <ArduinoOTA.h>

RCSwitch mySwitch = RCSwitch();

void listen();
bool listenModeEnabled = false;

void callback(char *topic, byte *payload, unsigned int length);
void manualLoop();
void reconnect();
String macToStr(const uint8_t *mac);

const char *ssid = WIFI_SSID;
const char *password = WIFI_PW;

char const *switchTopic1 = "/house/switch1/";
char const *switchTopic2 = "/house/switch2/";
char const *switchTopic3 = "/house/switch3/";
char const *switchTopic4 = "/house/switch4/";
char const *switchTopic5 = "/house/switch5/";
char const *switchTopic6 = "/house/switch6/";

WiFiClient wifiClient;
PubSubClient client(MQTT_MQTT_SERVER_IP, MQTT_MQTT_SERVER_PORT, callback, wifiClient);

void setup()
{
  // ArduinoOTA.setHostname("My Arduino WEMO"); // A name given to your ESP8266 module when discovering it as a port in ARDUINO IDE
  // ArduinoOTA.begin(); // OTA initialization

  Serial.begin(9600);
  mySwitch.enableTransmit(D0); // RF Transmitter is connected to Pin D0
  // mySwitch.setRepeatTransmit(20); // increase transmit repeat to avoid lost of rf sendings
  mySwitch.enableReceive(D5); // Receiver on pin D5

  pinMode(13, OUTPUT); // D13 as output- Optional
  Serial.println("Press 999 to toggle listen mode");
  Serial.println("Press 1-6 to toggle");
  Serial.println("Press x0 to turn x off");
  Serial.println("Press x1 to turn x on");

  //start the serial line for debugging
  delay(100);

  //start wifi subsystem
  WiFi.begin(ssid, password);
  //attempt to connect to the WIFI network and then connect to the MQTT server
  reconnect();

  //wait a bit before starting the main loop
  delay(2000);
}

void loop()
{

  //reconnect if connection is lost
  if (!client.connected() && WiFi.status() == 3)
  {
    reconnect();
  }

  //maintain MQTT connection
  client.loop();
  manualLoop();

  //MUST delay to allow ESP8266 WIFI functions to run
  delay(10);
  // ArduinoOTA.handle();
}

void callback(char *topic, byte *payload, unsigned int length)
{

  //convert topic to string to make it easier to work with
  String topicStr = topic;
  //EJ: Note:  the "topic" value gets overwritten everytime it receives confirmation (callback) message from MQTT

  //Print out some debugging info
  Serial.println("Callback update.");
  Serial.print("Topic: ");
  Serial.println(topicStr);

  if (topicStr == "/house/switch1/")
  {
    if (payload[0] == '1')
    {
      mySwitch.send(1000016, 24); // on
      client.publish("/house/switchConfirm1/", "1");
    }
    else if (payload[0] == '0')
    {
      mySwitch.send(1000020, 24); // off
      client.publish("/house/switchConfirm1/", "0");
    }
  }

  else if (topicStr == "/house/switch2/")
  {
    if (payload[0] == '1')
    {
      mySwitch.send(1000013, 24); // on
      client.publish("/house/switchConfirm2/", "1");
    }
    else if (payload[0] == '0')
    {
      mySwitch.send(1000014, 24); // off
      client.publish("/house/switchConfirm2/", "0");
    }
  }
  else if (topicStr == "/house/switch3/")
  {
    if (payload[0] == '1')
    {
      mySwitch.send(11562514, 24); // toggle
      client.publish("/house/switchConfirm3/", "1");
    }
    else if (payload[0] == '0')
    {
      mySwitch.send(11562514, 24); // toggle
      client.publish("/house/switchConfirm3/", "0");
    }
  }
  else if (topicStr == "/house/switch4/")
  {
    if (payload[0] == '1')
    {
      mySwitch.send(11562513, 24); // toggle
      client.publish("/house/switchConfirm4/", "1");
    }
    else if (payload[0] == '0')
    {
      mySwitch.send(11562513, 24); // toggle
      client.publish("/house/switchConfirm4/", "0");
    }
  }
  else if (topicStr == "/house/switch5/")
  {
    if (payload[0] == '1')
    {
      mySwitch.send(9668120, 24); // toggle
      client.publish("/house/switchConfirm4/", "1");
    }
    else if (payload[0] == '0')
    {
      mySwitch.send(9668120, 24); // toggle
      client.publish("/house/switchConfirm4/", "0");
    }
  }
  else if (topicStr == "/house/switch6/")
  {
    if (payload[0] == '1')
    {
      mySwitch.send(9668116, 24); // toggle
      client.publish("/house/switchConfirm4/", "1");
    }
    else if (payload[0] == '0')
    {
      mySwitch.send(9668116, 24); // toggle
      client.publish("/house/switchConfirm4/", "0");
    }
  }
}

void reconnect()
{

  //attempt to connect to the wifi if connection is lost
  if (WiFi.status() != WL_CONNECTED)
  {
    //debug printing
    Serial.print("Connecting to ");
    Serial.println(ssid);

    //loop while we wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }

    //print out some more debug once connected
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  //make sure we are connected to WIFI before attemping to reconnect to MQTT
  if (WiFi.status() == WL_CONNECTED)
  {
    // Loop until we're reconnected to the MQTT server
    while (!client.connected())
    {
      Serial.print("Attempting MQTT connection...");

      // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);

      //if connected, subscribe to the topic(s) we want to be notified about
      //EJ: Delete "mqtt_username", and "mqtt_password" here if you are not using any
      if (client.connect((char *)clientName.c_str(), MQTT_USER_NAME, MQTT_PW))
      { //EJ: Update accordingly with your MQTT account
        Serial.print("\tMQTT Connected");
        client.subscribe(switchTopic1);
        client.subscribe(switchTopic2);
        client.subscribe(switchTopic3);
        client.subscribe(switchTopic4);
        //EJ: Do not forget to replicate the above line if you will have more than the above number of relay switches
      }

      //otherwise print failed for debugging
      else
      {
        Serial.println("\tFailed.");
        abort();
      }
    }
  }
}

//generate unique name from MAC addr
String macToStr(const uint8_t *mac)
{

  String result;

  for (int i = 0; i < 6; ++i)
  {
    result += String(mac[i], 16);

    if (i < 5)
    {
      result += ':';
    }
  }

  return result;
}

void manualLoop()
{
  if (Serial.available() > 0)
  {
    int data = Serial.parseInt();
    Serial.print("Input: ");
    Serial.println(data);
    switch (data)
    {
    case 999:
      listenModeEnabled = !listenModeEnabled;
      if (listenModeEnabled)
      {
        Serial.println("Listen mode on");
      }
      else
      {
        Serial.println("Listen mode off");
      }
      break;
    case 1:
      mySwitch.send(11562520, 24); // toggle
      break;
    case 10:
      mySwitch.send(1000020, 24); // off
      break;
    case 11:
      mySwitch.send(1000016, 24); // on
      break;
    case 2:
      mySwitch.send(11562516, 24); // toggle
      break;
    case 20:
      mySwitch.send(1000014, 24); // off
      break;
    case 21:
      mySwitch.send(1000013, 24); // on
      break;
    case 3:
      mySwitch.send(11562514, 24); // toggle
      break;
    case 4:
      mySwitch.send(11562513, 24); // toggle
      break;
    case 5:
      mySwitch.send(9668120, 24); // toggle
      break;
    case 6:
      mySwitch.send(9668116, 24); // toggle
      break;
    default:
      break;
    }
  }

  if (listenModeEnabled)
    listen();
}

void listen()
{
  if (mySwitch.available())
  {
    Serial.print("Received ");
    Serial.print(mySwitch.getReceivedValue());
    Serial.print(" / ");
    Serial.print(mySwitch.getReceivedBitlength());
    Serial.print("Bit ");
    Serial.print(" // ");
    Serial.print("Protocol: ");
    Serial.println(mySwitch.getReceivedProtocol());

    delay(1);
    mySwitch.resetAvailable();
  }
}
