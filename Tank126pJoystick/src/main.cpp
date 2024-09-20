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
const short button3 = 11;

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
short bval = 0;

char buffer[26];
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
  pinMode(button3, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  Serial.begin(9600);
}

void send (char *message)
{
  analogWrite(3, 50);
  Serial.println(message);
  rfdriver.send((uint8_t *)message, strlen(message));
  rfdriver.waitPacketSent(); 
  analogWrite(3, 200);
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

