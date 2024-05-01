
#include <LoRa.h>
#include "boards.h"




void setup()
{
    initBoard();
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
  static unsigned int receivedPackets = 0;
  String packet_header = "4419grp6";
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String recv = "";
      // read packet
      while (LoRa.available()) {
          recv += (char)LoRa.read();
      }
      if(recv.startsWith(packet_header)) {
        receivedPackets++;
        String packet = recv.substring(packet_header.length());
        // Process the packet for BER and display the message without the prefix
        processPacket(packet, receivedPackets);
      }
    }
}

void processPacket(String received, unsigned int &receivedPackets) {
  String expectedOnesString = "11111111";
  String message = received.substring(expectedOnesString.length());
  int firstCommaIndex = message.indexOf(',');
  String sequenceNumberStr = message.substring(0, firstCommaIndex);
  float sequenceNumber = sequenceNumberStr.toFloat();
  float PRR = ((float)receivedPackets / sequenceNumber);

  // Check if the received message starts with the expected prefix
  // Remove the prefix and print the rest of the message
  String receivedPrefix = received.substring(0, expectedOnesString.length());
  Serial.print(message + ",");
  Serial.print(millis());
  Serial.print(",");
  Serial.print(String(LoRa.packetRssi()) + "," + String(LoRa.packetSnr())+ ",");
  Serial.print(PRR); // PRR
  Serial.print(",");
  // Calculate and display BER
  calculateBER(receivedPrefix, expectedOnesString);
}

void calculateBER(String receivedPrefix, String expectedPrefix) {
  int errorChars = 0;
  int totalChars = expectedPrefix.length();

  for (int i = 0; i < totalChars; i++) {
    char receivedChar = receivedPrefix[i];
    char expectedChar = expectedPrefix[i];
    if(receivedChar != expectedChar) errorChars++;
  }

  float ber = (float)errorChars / (float)totalChars;
  Serial.println(ber, 4); // Print BER with high precision
}
