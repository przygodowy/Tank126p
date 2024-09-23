#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <AsyncDelay.h>

#define IS_ON 0
#define IS_OFF 1
RH_ASK driver(4000, D0, D4, D4, false);

const short blinkersLed_l_out = D5;
const short blinkersLed_r_out = D6;
const short headlights_pwm = D2;
const short rear_lights_led = D1;


int l_x;
int l_y;
int r_x;
int r_y;
int l_b;
int r_b;
int red_b;
int black_b;
int lights_on_switch;
int lights_mode_switch;
int blinkers_mode_switch;

char *decodedStringValues[11];
char *ptr = NULL;
byte decodingIndex = 0;

AsyncDelay delay_blinkers;
bool blinkersState = false;

void decodeRadioMessage(){
    uint8_t buf[34];
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
      red_b = atoi(decodedStringValues[6]);
      black_b = atoi(decodedStringValues[7]);
      lights_on_switch = atoi(decodedStringValues[8]);
      lights_mode_switch = atoi(decodedStringValues[9]);
      blinkers_mode_switch = atoi(decodedStringValues[10]);

    }
}

void debug(){
  if (black_b == IS_ON){
      Serial.printf("Left X: %d, Left Y: %d, Left button: %d, Right X: %d, Right Y: %d, Right button: %d, Red button: %d, Black button: %d, Lights: %d, Lights_Mode: %d, BlinkersMode: %d" , l_x, l_y, l_b, r_x, r_y, r_b, red_b, black_b, lights_on_switch, lights_mode_switch, blinkers_mode_switch);
      Serial.println();
  }
}

void blinkers(){
  if(delay_blinkers.isExpired())
    {
      blinkersState = !blinkersState;
      delay_blinkers.repeat();
    }
      
    if (blinkers_mode_switch == 0)
    {
      digitalWrite(blinkersLed_l_out, LOW); 
      digitalWrite(blinkersLed_r_out, LOW);
    }
    else if (blinkers_mode_switch == 1)
    {
      digitalWrite(blinkersLed_l_out, blinkersState);
      digitalWrite(blinkersLed_r_out, LOW);
    }
    else if (blinkers_mode_switch == 2)
    {
      digitalWrite(blinkersLed_r_out, blinkersState);
      digitalWrite(blinkersLed_l_out, LOW);
    }
}

void headlights(){
  if (lights_on_switch == IS_ON){
    analogWrite(headlights_pwm, lights_mode_switch == IS_ON ? 255 : 45);
  }
  else{
    analogWrite(headlights_pwm, 0);
  }
}

void rear_lights(){
  if (lights_on_switch == IS_ON){
    digitalWrite(rear_lights_led, HIGH);
  }
  else{
    digitalWrite(rear_lights_led, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(blinkersLed_l_out, OUTPUT);
  pinMode(blinkersLed_r_out, OUTPUT);
  pinMode(headlights_pwm, OUTPUT);
  pinMode(rear_lights_led, OUTPUT);
  
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
  headlights();
  rear_lights();
  debug();
}