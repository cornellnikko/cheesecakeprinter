#include "AccelStepper.h"

const int stickypin = A4;
const int stickxpin = A5;
const int stickbuttonpin = D6;

const int testpin = D7;

int stickxvalue;
int stickyvalue;
int buttonvalue;

AccelStepper stepper1(AccelStepper::DRIVER, D0, D1);
AccelStepper stepper2(AccelStepper::DRIVER, D2, D3);
AccelStepper stepper3(AccelStepper::DRIVER, D4, D5);

void setup() {
  pinMode(stickxpin,INPUT);
  pinMode(stickypin,INPUT);
  pinMode(stickbuttonpin,INPUT);
  pinMode(testpin,OUTPUT);

  //1c0040001951363330333534

  Serial.begin(9600);

  stepper1.setMaxSpeed(50.0);
  stepper1.setAcceleration(50.0);
  stepper1.moveTo(1000000);

  stepper2.setMaxSpeed(200.0);
  stepper2.setAcceleration(100.0);
  stepper2.moveTo(1000000);

  stepper3.setMaxSpeed(200.0);
  stepper3.setAcceleration(100.0);
  stepper3.moveTo(1000000);
}

void loop()
{
  buttonvalue = digitalRead(stickbuttonpin);
  stickxvalue = analogRead(stickxpin);
  stickyvalue = analogRead(stickypin);

  Serial.print("Button: ");
  Serial.print(buttonvalue);

  Serial.println();

  Serial.print("X: ");
  Serial.print(stickxvalue);

  Serial.println();

  Serial.print("Y: ");
  Serial.print(stickyvalue);

  Serial.println("Ayylmao");

  stepper1.run();
  stepper2.run();
  stepper3.run();

}
