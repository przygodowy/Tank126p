#include <Arduino.h>
#include <RH_ASK.h>

short value = 0;
short axisValue = 0;
const short l_sensorX = A0;
const short l_sensorY = A1;
const short r_sensorX = A2;
const short r_sensorY = A3;
const short l_button = 9; 
const short r_button = 10; 
const short red_button = 11;
const short black_button = 8;
const short blinkers_left_switch = 5;
const short blinkers_right_switch = 6;
const short headlights_on_switch = 4;
const short headlights_mode_switch = 3;


const short LOOSE_PIN = 0;

const short L_X = 511;
const short L_Y = 508;
const short R_X = 519;
const short R_Y = 511;

const short TRANSMITTER_PIN = 7;

short lxval = 0;
short lyval = 0;
short rxval = 0;
short ryval = 0;
short lbval = 0;
short rbval = 0;
short red_bval = 0;
short black_bval = 0;
short lights_on_val = 0;
short lights_mode_val = 0; // 0 = short, 1 = long
short blinkers_left_val = 0;
short blinkers_right_val = 0;
short blinkers_val = 0;

char buffer[34];
RH_ASK rfdriver(4000, LOOSE_PIN, TRANSMITTER_PIN, LOOSE_PIN, false);

void setup() {

  if (!rfdriver.init())
  {
    Serial.println(F("RF initialization failed"));
  }
  else
  {
     Serial.println(F("RF initialization succeeded"));
  }

  pinMode(l_button, INPUT_PULLUP);
  pinMode(r_button, INPUT_PULLUP);
  pinMode(red_button, INPUT_PULLUP);
  pinMode(black_button, INPUT_PULLUP);
  pinMode(blinkers_left_switch, INPUT_PULLUP);
  pinMode(blinkers_right_switch, INPUT_PULLUP);
  pinMode(headlights_on_switch, INPUT_PULLUP);
  pinMode(headlights_mode_switch, INPUT_PULLUP);
  Serial.begin(9600);
}

void send (char *message)
{
  Serial.println(message);
  rfdriver.send((uint8_t *)message, strlen(message));
  rfdriver.waitPacketSent(); 
}

void loop() {

  lxval = analogRead(l_sensorX);
  lyval = analogRead(l_sensorY);
  rxval = analogRead(r_sensorX);
  ryval = analogRead(r_sensorY);

  lbval = digitalRead(l_button);
  rbval = digitalRead(r_button);
  red_bval = digitalRead(red_button);
  black_bval = digitalRead(black_button);
  blinkers_left_val = digitalRead(blinkers_left_switch);
  blinkers_right_val = digitalRead(blinkers_right_switch);
  lights_on_val = digitalRead(headlights_on_switch);
  lights_mode_val = digitalRead(headlights_mode_switch);
  blinkers_val = blinkers_left_val == 0 ? 1 : blinkers_right_val == 0 ? 2 : 0; // 1 = left, 2 = right, 0 = off;


  sprintf(buffer, "%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#%d#", lxval, lyval, rxval, ryval, lbval, rbval, red_bval, black_bval, lights_on_val, lights_mode_val, blinkers_val);
  Serial.println((char *) buffer);

  send(buffer);
}

