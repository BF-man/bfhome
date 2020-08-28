/*
   -Arduino 433MHz RF Sniffer
   -Based on an example from the RCSwitch Library
   -https://github.com/sui77/rc-switch/
   -Hardware: Arduino Nano & Generic 433MHz RF Receiver
   -T.K.Hareendran/2018
*/

#include <Arduino.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void listen();
bool listenModeEnabled = false;

void setup()
{
  Serial.begin(9600);
  mySwitch.enableTransmit(D0);    // RF Transmitter is connected to Pin D0
  // mySwitch.setRepeatTransmit(20); // increase transmit repeat to avoid lost of rf sendings
  mySwitch.enableReceive(D5);     // Receiver on pin D5

  pinMode(13, OUTPUT); // D13 as output- Optional
  Serial.println("Press 999 to toggle listen mode");
  Serial.println("Press 1-6 to toggle");
  Serial.println("Press x0 to turn x off");
  Serial.println("Press x1 to turn x on");
}

void loop()
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
