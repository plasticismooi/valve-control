//____________________________library_________________________
#include <Servo.h>
Servo myservo1;

// ____________________________Config Settings________________
int ServoStartPosition = 0;
int TimeValveIsOpen = 1000;
int TimeValveIsClosed = 2200;
int MaxAngleServoAutomatisch = 20 ; //Maximal angle of servo (max: 30)
int MaxAngleServoPot = 50;
int ThresholdValveOpen = 5;
bool PrintHatchInfo = true;
bool PrintAngleServoInfo = true;

// ____________________________Variables___________________________
int PositionServo;
int SwitchToManualMode;
int mainSwitchState;

//_____________________________Port declaration_________________
int PortPotentiometer = 1;
int PortLedValveOpen = 2;
int PortLedValveClosed = 3;
int PortLedManualMode = 4;
int PortLedAutomaticMode = 5;
int PortSwitchToManualMode = 7;
int PortMainSwitch = 8;
int PortServo1 = 9;

//_____________________________Function______________________________
int CheckIfValveIsOpen(int PositionServo) {
  if (PositionServo < ThresholdValveOpen) {
    return true;
  }
  else {
    return false;
  }
}

int PrintValvePositionOpen() {
  Serial.println("Valve is Open");
}
int PrintValvePositionClosed() {
  Serial.println("Valve is Closed");
}
int PrintAngleServo(int PositionServo) {
  Serial.println(PositionServo);
}
void SetLedsToPositionClosed() {
  digitalWrite(PortLedValveClosed, HIGH);
  digitalWrite(PortLedValveOpen, LOW);
}
void SetLedsToPositionOpen() {
  digitalWrite(PortLedValveClosed, LOW);
  digitalWrite(PortLedValveOpen, HIGH);
}
void TurnOffAlLeds() {
  digitalWrite(PortLedValveClosed, LOW);
  digitalWrite(PortLedValveOpen, LOW);
  digitalWrite(PortLedAutomaticMode, LOW);
  digitalWrite(PortLedManualMode, LOW);
}
void SetLedsToAutomaticMode() {
  digitalWrite(PortLedManualMode, LOW);
  digitalWrite(PortLedAutomaticMode, HIGH);
}
void SetLedsToManualMode() {
  digitalWrite(PortLedManualMode, HIGH);
  digitalWrite(PortLedAutomaticMode, LOW);
}


// ____________________________Void_Setup_________________
void setup() {
  Serial.begin(9600);
  Serial.println("start systeem");

  myservo1.attach(PortServo1);
  pinMode(PortSwitchToManualMode, INPUT);
  pinMode(PortLedAutomaticMode, OUTPUT);
  pinMode(PortLedManualMode, OUTPUT);
  pinMode(PortLedValveClosed, OUTPUT);
  pinMode(PortLedValveOpen, OUTPUT);
  pinMode(PortMainSwitch, INPUT);
}

// ____________________________Void_Loop_____________
void loop() {
  mainSwitchState = digitalRead(PortMainSwitch);
  if (mainSwitchState == HIGH) {
    SwitchToManualMode = digitalRead(PortSwitchToManualMode);
    if (PrintAngleServoInfo == true) {
      PrintAngleServo(PositionServo);
    }
    if (CheckIfValveIsOpen(PositionServo) == true) {
      SetLedsToPositionOpen();
      PrintValvePositionOpen();
    }
    else {
      SetLedsToPositionClosed();
      PrintValvePositionClosed();
    }
    //Entering ManualMode
    if (SwitchToManualMode == HIGH) {
      SetLedsToManualMode();
      PositionServo = analogRead(PortPotentiometer);
      PositionServo = map(PositionServo, 0, 1023, 0, MaxAngleServoPot);
      myservo1.write(PositionServo);
      delay(150);
    }
    //Entering AutoPilot
    else {
      SetLedsToAutomaticMode();
      for (ServoStartPosition = 0; ServoStartPosition < MaxAngleServoAutomatisch; ServoStartPosition += 1) {
        myservo1.write(ServoStartPosition);
        CheckIfValveIsOpen(ServoStartPosition);
        delay(15);

      }
      delay (TimeValveIsOpen);
      for (ServoStartPosition = MaxAngleServoAutomatisch; ServoStartPosition > 0; ServoStartPosition -= 1) {
        myservo1.write(ServoStartPosition);
        CheckIfValveIsOpen(ServoStartPosition);
        delay(15);
      }
      delay (TimeValveIsClosed);
    }
  }

  //Turn off control valve and leds
  else {
    myservo1.write(0);
    TurnOffAlLeds();
  }
}



