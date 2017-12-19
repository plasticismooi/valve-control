/*
  Arduino code demo 3, versie 4
  created: 05-12-2017
  by:Jelte en Tom en Ferry, Polytential.

  5-12-2017: Add LEDs - Servo close by Low mainswitch
  6-12-2017: Clean Code - hardwarebuilding
  12-12-2017: FlowEngine
  15-12-2017: Remove FlowEngine
  18-12-2017: cleancode Ferry, (comments, Typefouten, functie checkservoposition verdelen in ziek veel mooie code dingen, wit regels)

  Mainswitch: Turn system on/off
  Servo: 180Angle Motor, Attached to hatch
  Potentiometer: Open and closed the hatch
  Button: Switch from manual to automatic
  FlowEngine:
  light:
    Yellow: Power on arduino
    Green: Power on system
    White: Hatchsystem works Automaticlly
    Blue: Hatchsystem works on Manual
    Groen: Hatch is open
    Rood: Hatch is closed
  Time = miliseconds.
*/

//____________________________library_________________________

#include <Servo.h>


// ____________________________Config Settings________________

int ServoStartPosition = 0;
int HatchOpenTime = 100;
int HatchClosingTime = 2200;
int MaxAngleServoAutomatisch = 13 ; //Maximal angle of servo (max: 30)
int MaxAngleServoPot = 50;
int LedIndicatorOpenHatchAngle = 5;
int FlowEngineProcent = 100;
bool PrintHatchInfo = true;
bool PrintAgleServoInfo = false;


// ____________________________Variables___________________________

int PositionServo;
int buttonState;
int mainSwitchState;

//_____________________________Port declaration_________________

int PortPotentiometer = 1;
int PortButton = 2;
int AutomaticModeLED = 3;
int ClosedLEDIndicator = 13;
int OpenLEDIndicator = 12;
int portMainSwitch = 23;
int ServoPowerHighOutput = 10;
Servo myservo1;

//Function for turn on/off the Hatchindication LED
int CheckServoPosition(int ServoStartPosition) {
  if (ServoStartPosition < LedIndicatorOpenHatchAngle) {
    digitalWrite(ClosedLEDIndicator, HIGH); //in een andere functie
    digitalWrite(OpenLEDIndicator, LOW);

    if (PrintHatchInfo == true) {
      Serial.println("Hatch Closed");
    }
  }
  else {

    digitalWrite(ClosedLEDIndicator, LOW); //in een andere functie
    digitalWrite(OpenLEDIndicator, HIGH);

    if (PrintHatchInfo == true) {
      Serial.println("Hatch Open");
    }
  }
}


int PrintAngleServo(int PositionServo) { //naam met servo info high toevoegen
  if (PrintAgleServoInfo == HIGH) {  //typefoutje
    Serial.println(PositionServo);
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("start systeem");
  myservo1.attach(9);
  pinMode(PortButton, INPUT);
  pinMode(AutomaticModeLED, OUTPUT);
  pinMode(ClosedLEDIndicator, OUTPUT);
  pinMode(OpenLEDIndicator, OUTPUT);
  pinMode(portMainSwitch, INPUT);
  pinMode(ServoPowerHighOutput, OUTPUT);
}

void loop() {

  mainSwitchState = digitalRead(portMainSwitch);
  if (mainSwitchState == HIGH) {
    buttonState = digitalRead(PortButton);
    PrintAngleServo(PositionServo);
    CheckServoPosition(PositionServo);

    if (buttonState == HIGH) {
      digitalWrite(AutomaticModeLED, LOW);
      PositionServo = analogRead(PortPotentiometer);
      PositionServo = map(PositionServo, 0, 1023, 0, MaxAngleServoPot);
      myservo1.write(PositionServo);
      delay(150);
    }

    //Goes in the automatic pilot loop
    else {
      digitalWrite(AutomaticModeLED, HIGH);
      for (ServoStartPosition = 0; ServoStartPosition < MaxAngleServoAutomatisch; ServoStartPosition += 1) {
        myservo1.write(ServoStartPosition);
        delay(15);
      }
      delay (HatchOpenTime);

      CheckServoPosition(ServoStartPosition);

      for (ServoStartPosition = MaxAngleServoAutomatisch; ServoStartPosition > 0; ServoStartPosition -= 1) {
        myservo1.write(ServoStartPosition);
        CheckServoPosition(ServoStartPosition);
        delay(15);
      }
      delay (HatchClosingTime);
    }
  }
  else {
    myservo1.write(0);
  }
}







