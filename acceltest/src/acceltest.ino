#include "AccelStepper.h"

const int redpin = A0;
const int greenpin = A1;
const int bluepin = A2;

const int xbackstop = A3;
const int xfrontstop = A4;
const int ynearstop = A5;
const int yfarstop = A6;

const int testpin = D7;

int xbackval;
int xfrontval;
int ynearval;
int yfarval;

AccelStepper stepper1(AccelStepper::DRIVER, D0, D1);
AccelStepper stepper2(AccelStepper::DRIVER, D2, D3);
AccelStepper stepper3(AccelStepper::DRIVER, D4, D5);

String validLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
String charOne = "";
String charTwo = "";

void setup()
{
  pinMode(redpin,OUTPUT);
  pinMode(greenpin,OUTPUT);
  pinMode(bluepin,OUTPUT);
  pinMode(xbackstop,INPUT);
  pinMode(xfrontstop,INPUT);
  pinMode(ynearstop,INPUT);
  pinMode(yfarstop,INPUT);
  pinMode(testpin,OUTPUT);
  Serial.begin(9600);
  Serial.println("Please enter first character: ");
}

void loop()
{
  byte incoming_byte;
  if (Serial.available() > 0)
  {
    incoming_byte = Serial.read();
    char byteChar = (char)incoming_byte;
    String tempstring = String(byteChar);
    tempstring = tempstring.toUpperCase();
    Serial.println(tempstring);

    int validChar = 0;
    if(validLetters.indexOf(tempstring) != -1)
    {
      validChar = 1;
    }

    if(validChar)
    {
      if(charOne.length() == 0)
      {
        charOne = tempstring;
        Serial.println("Please enter your second character");
        digitalWrite(bluepin,HIGH);
        delay(500);
        digitalWrite(bluepin,LOW);
      }
      else if(charTwo.length() == 0)
      {
        charTwo = tempstring;
        Serial.println("Now Printing");
        digitalWrite(greenpin,HIGH);
        delay(500);
        digitalWrite(greenpin,LOW);
        print_cheesecake(charOne,charTwo);
      }
      else
      {
          Serial.println("WTFFFF");
          digitalWrite(redpin,HIGH);
          delay(100);
          digitalWrite(redpin,LOW);
          delay(100);
          digitalWrite(redpin,HIGH);
          delay(100);
          digitalWrite(redpin,LOW);
          delay(100);
          digitalWrite(redpin,HIGH);
          delay(100);
          digitalWrite(redpin,LOW);
      }
    }
    else
    {
      charOne = "";
      charTwo = "";
      Serial.println("Invalid Character.  Please Start Over.");
      Serial.println("Please enter first character: ");
      digitalWrite(redpin,HIGH);
      delay(500);
      digitalWrite(redpin,LOW);
    }

  }
}

void print_cheesecake(String first, String second)
{
  Serial.write("Begin print of: (");
  Serial.write(first);
  Serial.write(") and (");
  Serial.write(second);
  Serial.write(")");
  Serial.println("");

  goto_zero_zero();
  print_character(first);
  goto_zero_zero();
  goto_zero_one();
  print_character(second);
  goto_deliver();

  charOne = "";
  charTwo = "";
  Serial.println("DONE! :)");
  Serial.println("Please enter your first character.");
}

void goto_zero_zero()
{
  Serial.println("Goto Zero-Zero....");
  xbackval = digitalRead(xbackstop);
  while(xbackval == 1)
  {
    //Serial.println(xbackval);
    stepper2.setMaxSpeed(100.0);
    stepper2.setAcceleration(20.0);
    stepper2.moveTo(-10000);
    stepper2.run();
    xbackval = digitalRead(xbackstop);
  }
  stepper2.stop();
  stepper2.setMaxSpeed(100.0);
  stepper2.setAcceleration(200.0);
  stepper2.move(400);
  stepper2.runToPosition();
  Serial.println("Zero-Zero DONE");
}


void goto_zero_one()
{
  Serial.println("Goto Zero-One");
  stepper2.setMaxSpeed(100.0);
  stepper2.setAcceleration(200.0);
  stepper2.move(-100);
  stepper2.runToPosition();
}

void print_character(String character)
{
  Serial.write("Printing: ");
  Serial.println(character);

  // TODO print character
  Serial.write("Printing DONE");
}

void goto_deliver()
{
  Serial.println("Delivering");
  // TODO send x to far end to deliver to users

  stepper2.setMaxSpeed(100.0);
  stepper2.setAcceleration(200.0);
  stepper2.move(1100);
  stepper2.runToPosition();
  Serial.println("Deliver DONE");
}
