#include <LoRa.h>
#include "boards.h"
#define PACKET_HEADER "We are group 6 "

#define SF 8      // ranged from 6-12, default is 7. Check docs.
#define BW 125E3  // Bandwidth: 125 kHz
#define TP 20     // Transmission Power (dBm)
#define CR 5      // Coding Rate

int counter = 0;

void setup()
{
  initBoard();
  // When the power is turned on, a delay is required.
  delay(1500);

  Serial.println("LoRa Sender");
  LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
  if (!LoRa.begin(LoRa_frequency)) {
      Serial.println("LoRa init failed! Check your connections.");
      while (1);
  }

  
  LoRa.setSpreadingFactor(SF);     
  LoRa.setSignalBandwidth(BW); 
  LoRa.setCodingRate4(CR); // Coding Rate: 4/5
  LoRa.setTxPower(TP);   
  Serial.println("LoRa init succeeded.");
}

void loop()
{
  Serial.print("Sending packet (ASH): Can someone lemme know that the sender works? :P (┬─┬ ノ( ゜-゜ノ)");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print(PACKET_HEADER);
  LoRa.print("Sending packet (ASH): Can someone lemme know that the sender works? :P (┬─┬ ノ( ゜-゜ノ)");
  LoRa.print(counter);
  LoRa.endPacket();

#ifdef HAS_DISPLAY
  if (u8g2) {
    char buf[256];
    u8g2->clearBuffer();
    u8g2->drawStr(0, 12, "Transmitting: OK!");
    snprintf(buf, sizeof(buf), "Sending: %d", counter);
    u8g2->drawStr(0, 30, buf);
    u8g2->sendBuffer();
  }
#endif
  counter++;
  delay(5000);
}
