
#include <LoRa.h>
#include "boards.h"


void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.println("LoRa Receiver");

    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

void loop()
{
  static unsigned int receivedPackets = 0, bitErrors = 0;
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      
      String recv = "";
      // read packet
      while (LoRa.available()) {
          recv += (char)LoRa.read();
      }
      if(recv.startsWith("4419grp6")) {
        receivedPackets++;
        String packet = recv.substring(prefix.length());
        // Process the packet for BER and display the message without the prefix
        processPacket(packet);
      }
    }
}

void processPacket(String received) {
  String expectedOnesString = "11111111";

  // NEEDS TO BE FIXED
  // Periodically display PRR and BER. 
  if (totalPackets % 50 == 0) { // Every 50 packets, display metrics
    Serial.print("PRR: ");
    Serial.print((float)receivedPackets / totalPackets * 100);
    Serial.println("%");
    // Serial.print("BER: ");
    // Serial.println((float)bitErrors / (receivedPackets * expectedMsg.length() * 8));
  }
  // Check if the received message starts with the expected prefix
  // Remove the prefix and print the rest of the message
  String message = received.substring(expectedPrefix.length());
  Serial.println("Received message: " + message);
  Serial.print(message + ",");
  Serial.print(millis() + ",");
  Serial.print(String(LoRa.packetRssi()) + "," + String(LoRa.packetSnr())+ ",");

  // Calculate and display BER
  calculateBER(received.substring(0, expectedOnesString.length()), expectedOnesString);
}

void calculateBER(String receivedPrefix, String expectedPrefix) {
  int errorBits = 0;
  int totalBits = expectedPrefix.length() * 8; // 8 bits per character

  for (int i = 0; i < expectedPrefix.length(); i++) {
    byte receivedByte = receivedPrefix[i];
    byte expectedByte = expectedPrefix[i];
    for (int bit = 0; bit < 8; bit++) {
      if (((receivedByte >> bit) & 0x01) != ((expectedByte >> bit) & 0x01)) {
        errorBits++;
      }
    }
  }

  float ber = (float)errorBits / (float)totalBits;
  Serial.println(ber, 6); // Print BER with high precision
}
