#include <LoRa.h>
#include "boards.h"
#define PACKET_HEADER "We are group 6 "

int counter = 0;

void setup()
{
    initBoard();
    // When the power is turned on, a delay is required.
    delay(1500);

    Serial.println("LoRa Sender");
    LoRa.setPins(RADIO_CS_PIN, RADIO_RST_PIN, RADIO_DIO0_PIN);
    LoRa.setSpreadingFactor(12);           // ranges from 6-12,default 7 see API docs
    if (!LoRa.begin(LoRa_frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
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
