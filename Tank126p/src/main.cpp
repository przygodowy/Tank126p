#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <AsyncDelay.h>
#include <Wire.h> 
#include <Adafruit_PWMServoDriver.h> 

#define IS_ON 0
#define IS_OFF 1
#define MOTOR_L 0
#define MOTOR_R 1
#define SERVO 14
#define LED 15

RH_ASK driver(4000, D0, D4, D4, false);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

const short blinkersLed_l_out = D5;
const short blinkersLed_r_out = D6;
const short headlights_pwm = D4;
const short rear_lights_led = D3;
const short motor_l_in_1 = D7;
const short motor_l_in_2 = D8;
const short motor_r_in_1 = D9;
const short motor_r_in_2 = D10;
const int DEADZONE = 50;

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

int motor_l_pwm;
int motor_r_pwm;

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

bool isInDeadzone(int value)
{
  return value <= (512 + DEADZONE) && value >= (512 - DEADZONE);
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
      delay_blinkers.restart();
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

void motors(){
  if (isInDeadzone(l_x)){
    digitalWrite(motor_l_in_1, LOW);
    digitalWrite(motor_l_in_2, LOW);
    motor_l_pwm = 0;
  }

  if (l_x < (512 - DEADZONE)){
    digitalWrite(motor_l_in_1, HIGH);
    digitalWrite(motor_l_in_2, LOW);
    motor_l_pwm = 4000;
  }

  if (l_x > (512 + DEADZONE)){
    digitalWrite(motor_l_in_1, LOW);
    digitalWrite(motor_l_in_2, HIGH);
    motor_l_pwm = 4000;
  }

  if (isInDeadzone(l_y)){
    digitalWrite(motor_r_in_1, LOW);
    digitalWrite(motor_r_in_2, LOW);
    motor_r_pwm = 0;
  }

  if (l_y < (512 - DEADZONE)){
    digitalWrite(motor_r_in_1, HIGH);
    digitalWrite(motor_r_in_2, LOW);
    motor_r_pwm = 4000;
  }

  if (l_y > (512 + DEADZONE)){
    digitalWrite(motor_r_in_1, LOW);
    digitalWrite(motor_r_in_2, HIGH);
    motor_r_pwm = 4000;
  }
}

void pwm_s(){
  pwm.setPWM(SERVO, 0, r_x);
  pwm.setPWM(LED, 0, r_y * 4);
  pwm.setPWM(MOTOR_L, 0, motor_l_pwm);
  pwm.setPWM(MOTOR_R, 0, motor_r_pwm);
}

void setup() {
  Serial.begin(9600);
  pinMode(blinkersLed_l_out, OUTPUT);
  pinMode(blinkersLed_r_out, OUTPUT);
  pinMode(headlights_pwm, OUTPUT);
  pinMode(rear_lights_led, OUTPUT);
  pinMode(motor_l_in_1, OUTPUT);
  pinMode(motor_l_in_2, OUTPUT);
  pinMode(motor_r_in_1, OUTPUT);
  pinMode(motor_r_in_2, OUTPUT);
  
  pwm.begin();
  pwm.setPWMFreq(60);

  delay_blinkers.start(500, AsyncDelay::MILLIS);

  while(!Serial){
    delay(1);
  }

  driver.init();
}

void loop() {
  decodeRadioMessage();
  blinkers();
  headlights();
  rear_lights();
  motors();
  pwm_s();
}