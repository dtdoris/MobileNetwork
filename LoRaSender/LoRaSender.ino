#include <LoRa.h>
#include <string>
#include <iostream>

#include "boards.h"
#define PACKET_HEADER "4419grp6"
#define PACKET_MSG "[Ash] Do you like my pikachu hoodie?"

int  SF = 8;
long BW = 125E3;
int  CR = 5;
int  TP = 20;

int counter = 0;

void setup()
{

  initBoard();
  // When the power is turned on, a delay is required.
  delay(1500);

  Serial.println("LoRa Sender");
  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);

  LoRa.setSpreadingFactor(SF);      // ranged from 6-12, default is 7. Check docs.
  LoRa.setSignalBandwidth(BW);      // Bandwidth (eg.125E3 which is 125 kHz)
  LoRa.setCodingRate4(CR);          // Coding Rate (eg. 4/5)
  LoRa.setTxPower(TP);              // Transmission Power (dBm)
  
  if (!LoRa.begin(LoRa_frequency)) {
      Serial.println("Starting LoRa failed!");
      while (1);
  }


  Serial.println("LoRa init succeeded.");
}

void loop()
{ 
  // String sending_packet_msg = "Seq_no:" + String(counter) + " Sending packet: " + String(PACKET_MSG);
  
  String sending_packet_msg = String(counter) + "," + SF + "," + BW + "," + CR + "," + TP;

  Serial.print("11111111"); // Fixed prefix
  Serial.println(sending_packet_msg);

  // send packet
  LoRa.beginPacket();
  LoRa.print(PACKET_HEADER);
  LoRa.print("11111111"); // Fixed prefix
  LoRa.print(sending_packet_msg);
  LoRa.endPacket();
  
  counter++;
  delay(5000);
}
