#include <Arduino.h>
#include <VirtualWire.h>

short value = 0;
short axisValue = 0;
short l_sensorX = A1;
short l_sensorY = A0;
short l_button = 10; 

const short L_X = 511;
const short L_Y = 508;
const short R_X = 519;
const short R_Y = 511;

const short TRANSMITTER_PIN = 7;

short r_sensorX = A3;
short r_sensorY = A2;
short r_button = 13; 
short button3 = 11;

short lxval = 0;
short lyval = 0;
short rxval = 0;
short ryval = 0;
short lbval = 0;
short rbval = 0;
short bval = 0;

char buffer[26];

void setup() {
  vw_setup(4000);
  pinMode(l_button, INPUT_PULLUP);
  pinMode(r_button, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  vw_set_tx_pin(TRANSMITTER_PIN);
  Serial.begin(9600);
}

void send (char *message)
{
  Serial.println(message);
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx();
}

void loop() {

  lxval = analogRead(l_sensorX);
  lyval = analogRead(l_sensorY);
  rxval = analogRead(r_sensorX);
  ryval = analogRead(r_sensorY);

  lbval = digitalRead(l_button);
  rbval = digitalRead(r_button);
  bval = digitalRead(button3);

  sprintf(buffer, "%d#%d#%d#%d#%d#%d#%d#",lxval,lyval,rxval,ryval,lbval,rbval,bval);

  send(buffer);
}

