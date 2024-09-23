#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <AsyncDelay.h>

RH_ASK driver(4000, D0, D4, D4, false);

const short blinkersLed_l_out = D5;
const short blinkersLed_r_out = D6;

int l_x;
int l_y;
int r_x;
int r_y;
int l_b;
int r_b;

char *decodedStringValues[7];
char *ptr = NULL;
byte decodingIndex = 0;

AsyncDelay delay_blinkers;
bool blinkersState = false;
int isLeftBlinkerOff = 0;
int isRightBlinkerOff = 0;

void decodeRadioMessage(){
    uint8_t buf[26];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen))
    {
      decodingIndex = 0;
      ptr = strtok((char *)buf, "#");
      while(ptr != NULL)
      {
          decodedStringValues[decodingIndex] = ptr;
          decodingIndex++;
          ptr = strtok(NULL, "#"); 
      }

      l_x = atoi(decodedStringValues[0]);
      l_y = atoi(decodedStringValues[1]);
      r_x = atoi(decodedStringValues[2]);
      r_y = atoi(decodedStringValues[3]);
      l_b = atoi(decodedStringValues[4]);
      r_b = atoi(decodedStringValues[5]);

      Serial.printf("Left X: %d, Left Y: %d, Left button: %d, Right X: %d, Right Y: %d, Right button: %d", l_x, l_y, l_b, r_x, r_y, r_b);
      Serial.println();
    }
}

void blinkers(){
  if(delay_blinkers.isExpired())
    {
      blinkersState = !blinkersState;
      delay_blinkers.repeat();
    }
      
    isLeftBlinkerOff = l_b == 1;
    isRightBlinkerOff = r_b == 1;

    if (isLeftBlinkerOff && isRightBlinkerOff)
    {
      digitalWrite(blinkersLed_l_out, LOW); 
      digitalWrite(blinkersLed_r_out, LOW);
    }
    else if (isLeftBlinkerOff)
    {
      digitalWrite(blinkersLed_r_out, blinkersState);
      digitalWrite(blinkersLed_l_out, LOW);
    }
    else if (isRightBlinkerOff)
    {
      digitalWrite(blinkersLed_l_out, blinkersState);
      digitalWrite(blinkersLed_r_out, LOW);
    }
}

void setup() {
  Serial.begin(9600);
  pinMode(blinkersLed_l_out, OUTPUT);
  pinMode(blinkersLed_r_out, OUTPUT);
  delay_blinkers.start(500, AsyncDelay::MILLIS);

  while(!Serial){
    delay(1);
  }

  if(!driver.init())
  {
    Serial.println("RF init failed");
  }
  else
  {
    Serial.println("RF receiver init succeeded");
  }
}

void loop() {
  decodeRadioMessage();
  blinkers();
}