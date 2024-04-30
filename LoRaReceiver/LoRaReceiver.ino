
#include <LoRa.h>
#include "boards.h"


void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.println("LoRa Receiver");s

    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

void loop()
{
  static unsigned int totalPackets = 0, receivedPackets = 0, bitErrors = 0;
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      // received a packet
      receivedPackets++;
      Serial.print("Received packet '");

      String recv = "";
      // read packet
      while (LoRa.available()) {
          recv += (char)LoRa.read();
      }
      totalPackets++;
      Serial.println(recv);
      Serial.println("RSSI: " + String(LoRa.packetRssi()) + "\tSNR: " + String(LoRa.packetSnr()));
      // Periodically display PRR and BER
      if (totalPackets % 50 == 0) { // Every 50 packets, display metrics
        Serial.print("PRR: ");
        Serial.print((float)receivedPackets / totalPackets * 100);
        Serial.println("%");
        // Serial.print("BER: ");
        // Serial.println((float)bitErrors / (receivedPackets * expectedMsg.length() * 8));
      }
    }
}
