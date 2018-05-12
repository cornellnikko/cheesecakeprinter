#include "AccelStepper.h"
#include "MultiStepper.h"

#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL); 
#endif

const long XZZOFFSET = 440.0;
const long YZZOFFSET = 420.0;
const long EXRATE = -20.0;

const int POSRANGE = 1475;


const int redpin = A0;
const int greenpin = A1;
const int bluepin = A2;

const int xbackstop = A3;
const int ynearstop = A4;
const int xfrontstop = A5;

const int yfarstop = A6;

const int testpin = D7;

int xbackval;
int xfrontval;
int ynearval;
int yfarval;

int xCurrentPosition;
int yCurrentPosition;

AccelStepper stepper1(AccelStepper::DRIVER, D0, D1);
AccelStepper stepper2(AccelStepper::DRIVER, D2, D3);
AccelStepper stepper3(AccelStepper::DRIVER, D4, D5);

MultiStepper steppers;
MultiStepper extruder;

String validLetters = "SH";
String charOne = "";
String charTwo = "";

long thePositions[4];
long extruderPositions[4];

int exOn = 0;

void setup()
{
  extruder.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);

  pinMode(redpin,OUTPUT);
  pinMode(greenpin,OUTPUT);
  pinMode(bluepin,OUTPUT);
  pinMode(xbackstop,INPUT);
  pinMode(xfrontstop,INPUT);
  pinMode(ynearstop,INPUT);
  pinMode(yfarstop,INPUT);
  pinMode(testpin,OUTPUT);
  Serial.begin(9600);
  Serial.println("Please enter your selection: ");

  stepper1.setAcceleration(1000);
  stepper1.setMaxSpeed(1000);
  stepper1.setSpeed(500);
  stepper1.setCurrentPosition(0);

  stepper2.setAcceleration(1000);
  stepper2.setMaxSpeed(1000);
  stepper2.setSpeed(500);
  stepper2.setCurrentPosition(0);

  
  
}

float in, out;

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
        print_cheesecake(charOne);
      }
      else
      {
          Serial.println("WTFFFF");
      }
    }
    else
    {
      charOne = "";
      //charTwo = "";
      Serial.println("Invalid Character.  Please Start Over.");
      Serial.println("Please enter first character: ");
    }

  }
  
}

void print_cheesecake(String first)//, String second)
{
  Serial.write("Begin print of: (");
  Serial.write(first);
  Serial.write(")");
  Serial.println("");

  goto_zero_zero();
  print_character(first);
  goto_deliver();

  charOne = "";
  Serial.println("DONE! :)");
  Serial.println("Please enter your selection.");
}


void goto_zero_zero()
{
  Serial.println("Goto Zero-Zero");

  Serial.println("z21");
  xbackval = digitalRead(xbackstop);
  stepper2.setMaxSpeed(400.0);
  stepper2.setAcceleration(1000.0);
  stepper2.move(-10000);

  // Begin Z
  ynearval = digitalRead(ynearstop);
  stepper3.setMaxSpeed(400.0);
  stepper3.setAcceleration(1000.0);
  stepper3.move(10000);

  // Go to backstop
  while(xbackval == 1 || ynearval == 1)
  {
    if(xbackval)
    {
      stepper2.run();
      xbackval = digitalRead(xbackstop);
    }
    else
    {
      stepper2.stop();
    }
    

    if(ynearval)
    {
      stepper3.run();
      ynearval = digitalRead(ynearstop);
    }
    else
    {
      stepper3.stop();
    }
  }
  // At backstop, stop
  stepper2.stop();
  stepper2.setCurrentPosition(0);
  stepper2.move(1);
  stepper2.runToPosition();

  // At backstop, stop
  stepper3.stop();
  stepper3.setCurrentPosition(0);
  stepper3.move(-1);
  stepper3.runToPosition();

  // Back to zero
  stepper2.setCurrentPosition(0);
  stepper3.setCurrentPosition(0);

  // X offset
  stepper2.setMaxSpeed(400.0);
  stepper2.setAcceleration(1000.0);
  stepper2.move(XZZOFFSET);

  // Y offset
  stepper3.setMaxSpeed(400.0);
  stepper3.setAcceleration(1000.0);
  stepper3.move(-YZZOFFSET);

  while(abs(stepper2.currentPosition()) < abs(XZZOFFSET) || abs(stepper3.currentPosition()) < abs(YZZOFFSET))
  {
    if(abs(stepper2.currentPosition()) < abs(XZZOFFSET) )
    {
      stepper2.run();
    }
    else
    {
      stepper2.stop();
    }
    

    if(abs(stepper3.currentPosition()) < abs(YZZOFFSET) )
    {
      stepper3.run();
    }
    else
    {
      stepper3.stop();
    }
  }


  stepper2.setCurrentPosition(0);
  stepper3.setCurrentPosition(0);

  Serial.println("Zero-Zero DONE");
}

void print_character(String character)
{
  Serial.write("Printing (circle): ");
  Serial.println(character);


  stepper2.setSpeed(1000.0);
  stepper2.setAcceleration(1000.0);
  stepper3.setSpeed(1000.0);
  stepper3.setAcceleration(1000.0);

  if(character == "S")
  {
    draw_spiral();
  }
  else if(character == "H")
  {
    draw_heart();
  }
  delay(500);
  Serial.println("AYY");
  //draw_circle_multi(50);
  Serial.println("LMAO");

  Serial.println("Printing: DONE");
}



void draw_spiral()
{
  stepper2.setSpeed(1000);
  stepper2.setAcceleration(1000);
  stepper2.setCurrentPosition(0);
  
  stepper3.setSpeed(1000);
  stepper3.setAcceleration(1000);  
  stepper3.setCurrentPosition(0);

  //start_extrude();
  int  nextDirection = 1;
  long baseDistance = 20.0;
  long nextDistance;

  stepper1.setCurrentPosition(0);
  
  stepper1.setSpeed(5);
  stepper1.setMaxSpeed(5);
  stepper1.move(-5);
  stepper1.runToPosition();
  stepper1.move(-100000);
  

  for(int i=0; i<6; i++)
  {
    nextDirection = (i%2) ? 1 : -1;
    nextDistance = baseDistance * 2 * i * nextDirection;
    stepper2.setCurrentPosition(0);
    stepper2.move(nextDistance);
    stepper3.setCurrentPosition(0);
    stepper3.move(nextDistance);

    stepper1.runSpeed();

    while(abs(stepper2.currentPosition()) < abs(nextDistance))
    {
      stepper1.run();
      stepper2.run();
    }

    stepper2.stop();


    while(abs(stepper3.currentPosition()) < abs(nextDistance))
    {
      stepper1.run();
      stepper3.run();
    }

    stepper3.stop();

  }

  stepper1.stop();
  stepper1.setCurrentPosition(0);
  stepper1.move(5);
  stepper1.runToPosition();

}

void draw_heart()
{
  int sectorDist = 40;
  int stepBit = 20;

  stepper1.setSpeed(5);
  stepper1.setMaxSpeed(5);
  stepper1.move(-5);
  stepper1.runToPosition();
  stepper1.move(-100000);
  
  stepper2.setSpeed(200);
  stepper2.setAcceleration(1000);
  stepper2.setCurrentPosition(0);
  
  stepper3.setSpeed(200);
  stepper3.setAcceleration(1000);  
  stepper3.setCurrentPosition(0);

  long halfDist = sqrt((sectorDist*sectorDist)+(sectorDist*sectorDist));
  long fullDist = 2*halfDist;
  //long fullDist = sqrt((2*(sectorDist*sectorDist))+(2*(sectorDist*sectorDist)));

  long distancePerStep = halfDist/stepBit;

  for(int n=0;n<6;n++)
  {
    stepper2.setCurrentPosition(0);
    stepper3.setCurrentPosition(0);
    
    if(n == 0 || n == 1 || n == 4 || n == 5)
    {
      distancePerStep = halfDist;
    }
    else
    {
      distancePerStep = fullDist;
    }

    
    if(n == 0 || n == 1 || n == 4 || n == 5)
    {
      stepper2.move(distancePerStep);
    }
    else
    {
      stepper2.move(-distancePerStep);
    }
    
    
    if(n == 0 || n == 3 || n == 4)
    {
      stepper3.move(distancePerStep);
    }
    else
    {
      stepper3.move(-distancePerStep);
    }
          
    stepper1.runSpeed();


    while(abs(stepper2.currentPosition()) < abs(distancePerStep) || abs(stepper3.currentPosition()) < abs(distancePerStep))
    {
      stepper1.run();
      stepper2.run();
      
      stepper3.run();
    }
  }

  stepper2.stop();
  stepper3.stop();
  
  stepper1.stop();
  stepper1.setCurrentPosition(0);
  stepper1.move(5);
  stepper1.runToPosition();
}

void goto_deliver()
{
  stepper2.setMaxSpeed(400.0);
  stepper2.setAcceleration(400.0);

  stepper2.move(10000);
  xfrontval = digitalRead(xfrontstop);
  while(xfrontval)
  {
    stepper2.run();
    xfrontval = digitalRead(xfrontstop);
  }
  stepper2.stop();
  stepper2.setCurrentPosition(0);
  stepper2.move(-1);
  stepper2.runToPosition();
  Serial.println("Deliver DONE");
}

