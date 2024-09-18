#include <Arduino.h>
#include <VirtualWire.h>

uint8_t message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message
int last_idx = 0;

void setup() {
  Serial.begin(9600);

  Serial.println("Device is ready");

  vw_setup(4000);
  vw_set_rx_pin(4);
  vw_rx_start();
}

void loop() {
  if (vw_get_message(message, &messageLength))
    {
      int i;

      Serial.print("Got: ");
      
      for (i = 0; i < messageLength; i++)
      {
          Serial.print(message[i], HEX);
          Serial.print(' ');
      }
      Serial.println();
    }
  }