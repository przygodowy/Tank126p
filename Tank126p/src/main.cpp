#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

RH_ASK driver(4000, D0, D4, D4, false);

int DEBUG_LED = D5;
int i = 0;
String str_out;
String str_l_x;
String str_l_y;

void setup() {
  Serial.begin(9600);
  pinMode(DEBUG_LED, OUTPUT);

  while(!Serial){
    delay(1);
  }

  if(!driver.init())
  {
    Serial.println("RF init failed");
  }
  else
  {
    Serial.println("RF init succeeded");
  }
}

void loop() {
    uint8_t buf[26];
    uint8_t buflen = sizeof(buf);
    analogWrite(DEBUG_LED, 0);
    if (driver.recv(buf, &buflen))
    {
      driver.printBuffer("Got: ", buf, buflen);
      analogWrite(DEBUG_LED, 0);
      Serial.println((char *)buf);
      analogWrite(DEBUG_LED, 255);
    }
  }