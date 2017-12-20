//____________________________library_________________________
#include <Servo.h>
Servo myservo1;

// ____________________________Config Settings________________
int ServoStartPosition = 0;
int HatchOpenTime = 100;
int HatchClosingTime = 2200;
int MaxAngleServoAutomatisch = 13 ; //Maximal angle of servo (max: 30)
int MaxAngleServoPot = 50;
int LedIndicatorOpenHatchAngle = 5;
int FlowEngineProcent = 100;
bool PrintHatchInfo = false;
bool PrintAngleServoInfo = false;

// ____________________________Variables___________________________
int PositionServo;
int buttonState;
int mainSwitchState;
int pos;


//_____________________________Port declaration_________________
int PortPotentiometer = 1;
int PortButton = 2;
int AutomaticModeLED = 3;
int ClosedLEDIndicator = 13;
int OpenLEDIndicator = 12;
int portMainSwitch = 23;
int ServoPowerHighOutput = 10;
int PortServo1 = 9;

//Function for turn on/off the Hatchindication LED
int CheckServoPosition(int ServoStartPosition) {
  if (ServoStartPosition < LedIndicatorOpenHatchAngle) {
    return true;
  }
  else {
    return false;
  }
}

/* int PrintValvePositionOpen() {
  Serial.println("Valve is Open");
}
int PrintValvePositionClosed() {
  Serial.println("Valve is Closed");
}
*/
void SetLedsToPositionClosed() {
  digitalWrite(ClosedLEDIndicator, HIGH);
  digitalWrite(OpenLEDIndicator, LOW);
}
void SetLedsToPositionOpen() {
  digitalWrite(ClosedLEDIndicator, LOW);
  digitalWrite(OpenLEDIndicator, HIGH);
}
void SetLedValveOff() {
  digitalWrite (ClosedLEDIndicator, LOW);
  digitalWrite (OpenLEDIndicator, LOW);
  digitalWrite (AutomaticModeLED, LOW);
}


int PrintAngleServo(int PositionServo) { //naam met servo info high toevoegen
  if (PrintAngleServoInfo == HIGH) { 
    Serial.println(PositionServo);
    delay(1000);
  }
}

// ____________________________Void_Setup_________________
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

// ____________________________Void_Loop_____________
void loop() {
  mainSwitchState = digitalRead(portMainSwitch);

//_____________________________Mainswitch____________
  if (mainSwitchState == HIGH) {
    buttonState = digitalRead(PortButton);
    PrintAngleServo(PositionServo);

  bool PositionIndicator = CheckServoPosition(PositionServo);
    if (PositionIndicator == true) {
      SetLedsToPositionOpen();
//      PrintValvePositionOpen();
    }
    else {
      SetLedsToPositionClosed();
  //    PrintValvePositionClosed();
    }

    if (buttonState == HIGH) {
      digitalWrite(AutomaticModeLED, LOW);
      PositionServo = analogRead(PortPotentiometer);
      PositionServo = map(PositionServo, 0, 1023, 0, MaxAngleServoPot);
      myservo1.write(PositionServo);
      delay(150);
    }
    
//_____________________________Goes in the automatic pilot loop
    else {
      digitalWrite(AutomaticModeLED, HIGH);
      Serial.println(pos);
      for (pos = 0; pos < MaxAngleServoAutomatisch; pos += 1) {
        myservo1.write(pos);
        delay(15);
      }
      delay (HatchOpenTime);

      for (pos = MaxAngleServoAutomatisch; pos > 0; pos -= 1) {
        myservo1.write(pos);
        CheckServoPosition(PositionServo);
        delay(15);
        
      }
      delay (HatchClosingTime);
    }
  }

  //Turn Servo to start position.
  else {
    myservo1.write(0);
    SetLedValveOff();
  }
}







