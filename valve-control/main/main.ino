#include <Servo.h>
Servo ServoValveControl;

int StartPositionServo = 0;
int TimeValveIsOpen = 100;
int TimeValveIsClosed = 3000;
int StandbyLedDelay = 80;
int MaxServoPositionAuto = 30;
int MaxAngleServoPot = 20;
int ThresholdValveOpen = 5;

int PositionServo;
int OnSwitchState;
int SwitchToAutoMode;
int SwitchToManualMode;

int PortPotentiometer = 1;
int PortSwitchAutoMode = 2;
int PortSwitchManualMode = 3;
int PortOnSwitch = 4;
int PortLedStandby = 5;
int PortLedOn = 6;
int PortLedAuto = 8;
int PortLedManual = 7;
int PortLedValveOpen = 9;
int PortLedValveClosed = 10;
int PortServo = 11;

int CheckIfValveIsOpen(int PositionServo) {
  if (PositionServo < ThresholdValveOpen) {
    return true;
  }
  else {
    return false;
  }
}

void SetLedsToPositionClosed() {
  digitalWrite(PortLedValveClosed, HIGH);
  digitalWrite(PortLedValveOpen, LOW);
}
void SetLedsToPositionOpen() {
  digitalWrite(PortLedValveClosed, LOW);
  digitalWrite(PortLedValveOpen, HIGH);
}


int SetLedToStandbyMode() {
  digitalWrite(PortLedOn, LOW);
  digitalWrite(PortLedAuto, LOW);
  digitalWrite(PortLedManual, LOW);
}
int TurnSystemOn() {
  digitalWrite(PortLedStandby, HIGH);
}
int SetLedsToManualMode() {
  digitalWrite(PortLedManual, HIGH);
  digitalWrite(PortLedAuto, LOW);
}
int SetLedsToAutoMode() {
  digitalWrite(PortLedManual, LOW);
  digitalWrite(PortLedAuto, HIGH);
}

int  ValveFillStand() {
  ServoValveControl.write(StartPositionServo);
}
int ValveFillStandLed() {
  digitalWrite(PortLedAuto, HIGH);
  delay(500);
  digitalWrite(PortLedAuto, LOW);
  digitalWrite(PortLedManual, HIGH);
  delay(500);
  digitalWrite(PortLedManual, LOW);
}

void setup() {
  ServoValveControl.attach(PortServo);
  pinMode(PortSwitchAutoMode, INPUT);
  pinMode(PortSwitchManualMode, INPUT);
  pinMode(PortOnSwitch, INPUT);
  pinMode(PortLedStandby, OUTPUT);
  pinMode(PortLedOn, OUTPUT);
  pinMode(PortLedAuto, OUTPUT);
  pinMode(PortLedManual, OUTPUT);
  pinMode(PortLedValveOpen, OUTPUT);
  pinMode(PortLedValveClosed, OUTPUT);
  Serial.begin(9600);
  TurnSystemOn();
}

void loop() {

  OnSwitchState = digitalRead(PortOnSwitch);
  if (OnSwitchState == HIGH) {
    digitalWrite (PortLedOn, HIGH);
    Serial.println("OnSwitchState is high");
    SwitchToManualMode = digitalRead(PortSwitchManualMode);
    SwitchToAutoMode = digitalRead(PortSwitchAutoMode);

    if (CheckIfValveIsOpen(PositionServo) == true) {
      SetLedsToPositionOpen();
    }
    else {
      SetLedsToPositionClosed();
    }

    if (SwitchToManualMode == HIGH) {
      SetLedsToManualMode();
      Serial.println("SwitchToManualMode is high");
      PositionServo = analogRead(PortPotentiometer);
      PositionServo = map(PositionServo, 0, 1023, 0, MaxAngleServoPot);
      ServoValveControl.write(PositionServo);
      delay(15);
    }

    else if (SwitchToAutoMode == HIGH) {

      SetLedsToAutoMode();
      Serial.println("SwitchToAutoMode is high");
      for (PositionServo; PositionServo <= MaxServoPositionAuto; PositionServo += 1) {
        ServoValveControl.write(PositionServo);
        delay(15);
        if (CheckIfValveIsOpen(PositionServo) == true) {
          SetLedsToPositionOpen();
        }
        else {
          SetLedsToPositionClosed();
        }
      }
      delay (TimeValveIsOpen);
      for (PositionServo; PositionServo > 0; PositionServo -= 1) {
        ServoValveControl.write(PositionServo);
        delay(15);
        if (CheckIfValveIsOpen(PositionServo) == true) {
          SetLedsToPositionOpen();
        }
        else {
          SetLedsToPositionClosed();
        }
      }
      delay (TimeValveIsClosed);
    }

    else {
      Serial.println("FillStand is HIGH");
      ValveFillStand();
      ValveFillStandLed();
    }
  }


  else {
    Serial.println("OnSwitchState is low" );
    ServoValveControl.write(StartPositionServo);
    SetLedToStandbyMode();
  }
}


