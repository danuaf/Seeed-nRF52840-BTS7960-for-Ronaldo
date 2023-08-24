/*
Author  : Danu AF
Title   : Seed XIAO nRF52840 with BTS7960 for Ronaldo Remote Control (BLE)

Parameter for Function direction :
  direction(command, speed, turning speed) 
    command : 1. Move Forward
              2. Move Backward
              3. Turn Right
              4. Turn Left
              5. Turn Up Right
              6. Turn Up Left
              7. Turn Down Right
              8. Turn Down Left
              null. Stop
    speed   : PWM Value 0-255

*/


#include <ArduinoBLE.h>

#define R_RPWM A0
#define R_LPWM A1
#define L_RPWM A2
#define L_LPWM A3

int speed = 0;

BLEService ronaldoService("2E36A1D0-3FBD-11EE-BE56-0242AC120002");
BLEIntCharacteristic controlCharacteristic("2E36A1D2-3FBD-11EE-BE56-0242AC120002", BLERead | BLEWrite);
BLEIntCharacteristic speedCharacteristic("2E36A1D1-3FBD-11EE-BE56-0242AC120002", BLERead | BLEWrite);


void setup() {
  delay(1000);

  Serial.begin(9600);
  Serial.print("Starting System");
  for (int i = 0; i < 10; i++) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(".");
  Serial.println("Complete");

  pinMode(R_RPWM, OUTPUT);
  pinMode(R_LPWM, OUTPUT);
  pinMode(L_RPWM, OUTPUT);
  pinMode(L_LPWM, OUTPUT);

  Serial.println("Initialize Bluetooth Module");

  if (!BLE.begin()) {
    while (true) {
      Serial.println("Failed To Initialize Bluetooth Module");
      delay(500);
    }
  }

  BLE.setLocalName("Ronaldo-01");
  BLE.setAdvertisedService(ronaldoService);
  ronaldoService.addCharacteristic(controlCharacteristic);
  ronaldoService.addCharacteristic(speedCharacteristic);
  BLE.addService(ronaldoService);
  BLE.advertise();

  speed = speedCharacteristic.value();

  Serial.println("Ready to Connect");
}

void direction(int cmd, int spd, int turnSpd) {
  switch (cmd) {
    //up
    case 1:
      analogWrite(R_RPWM, spd);
      analogWrite(R_LPWM, 0);
      analogWrite(L_RPWM, spd);
      analogWrite(L_LPWM, 0);
      Serial.print("Direction : Up  | Speed : ");
      Serial.println(spd);
      break;
    //down
    case 2:
      analogWrite(R_RPWM, 0);
      analogWrite(R_LPWM, spd);
      analogWrite(L_RPWM, 0);
      analogWrite(L_LPWM, spd);
      Serial.print("Direction : Down  | Speed : ");
      Serial.println(spd);
      break;
    //right
    case 3:
      analogWrite(R_RPWM, 0);
      analogWrite(R_LPWM, turnSpd);
      analogWrite(L_RPWM, turnSpd);
      analogWrite(L_LPWM, 0);
      Serial.print("Direction : Right  | Turning Speed : ");
      Serial.println(turnSpd);
      break;
    //left
    case 4:
      analogWrite(R_RPWM, turnSpd);
      analogWrite(R_LPWM, 0);
      analogWrite(L_RPWM, 0);
      analogWrite(L_LPWM, turnSpd);
      Serial.print("Direction : Left  | Turning Speed : ");
      Serial.println(turnSpd);
      break;
    //up right
    case 5:
      analogWrite(R_RPWM, 0);
      analogWrite(R_LPWM, 0);
      analogWrite(L_RPWM, turnSpd);
      analogWrite(L_LPWM, 0);
      Serial.print("Direction : Up Right  | Turning Speed : ");
      Serial.println(turnSpd);
      break;
    //up left
    case 6:
      analogWrite(R_RPWM, turnSpd);
      analogWrite(R_LPWM, 0);
      analogWrite(L_RPWM, 0);
      analogWrite(L_LPWM, 0);
      Serial.print("Direction : Up Left  | Turning Speed : ");
      Serial.println(turnSpd);
      break;
    //down right
    case 7:
      analogWrite(R_RPWM, 0);
      analogWrite(R_LPWM, turnSpd);
      analogWrite(L_RPWM, 0);
      analogWrite(L_LPWM, 0);
      Serial.print("Direction : Down Right  | Turning Speed : " );
      Serial.println(turnSpd);
      break;
    //down left
    case 8:
      analogWrite(R_RPWM, 0);
      analogWrite(R_LPWM, 0);
      analogWrite(L_RPWM, 0);
      analogWrite(L_LPWM, turnSpd);
      Serial.print("Direction : Down Left  | Turning Speed : ");
      Serial.println(turnSpd);
      break;
    default:
      analogWrite(R_RPWM, 0);
      analogWrite(R_LPWM, 0);
      analogWrite(L_RPWM, 0);
      analogWrite(L_LPWM, 0);
      // Serial.println("STOP");
      break;
  }
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.println("Connected");

    while (central.connected()) {
      speed = speedCharacteristic.value();
      direction(controlCharacteristic.value(), speed, 100);
    }

    Serial.println("Disconnected");
  }
}
