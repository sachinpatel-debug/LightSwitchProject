/*
  LED

  This example creates a Bluetooth® Low Energy peripheral with service that contains a
  characteristic to control an LED.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic Bluetooth® Low Energy central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/
#define SERVO1PIN 12
#define SERVO2PIN 11
#define SERVO3PIN 10
#define SERVO4PIN 9

#include <ArduinoBLE.h>
#include <Servo.h>

BLEService servoService("19B10000-E8F2-537E-4F6C-D104768A1214"); // Bluetooth® Low Energy LED Service
Servo servo1; 
Servo servo2;
Servo servo3;
Servo servo4;

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic servo1Characteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic servo2Characteristic("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic servo3Characteristic("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEByteCharacteristic servo4Characteristic("19B10004-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
const int ledPin = LED_BUILTIN; // pin to use for the LED
void moveServo(Servo &servoToMove, int);
void checkCharacteristic(BLEByteCharacteristic &servoCharacteristic, Servo&, int);
void setup() {
  Serial.begin(9600);
  // while (!Serial);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);
  

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(servoService);

  // add the characteristic to the service
  servoService.addCharacteristic(servo1Characteristic);
  servoService.addCharacteristic(servo2Characteristic);
  servoService.addCharacteristic(servo3Characteristic);
  servoService.addCharacteristic(servo4Characteristic);

  // add service
  BLE.addService(servoService);

  // set the initial value for the characteristic:
  servo1Characteristic.writeValue(0);
  servo2Characteristic.writeValue(0);
  servo3Characteristic.writeValue(0);
  servo4Characteristic.writeValue(0);  

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      checkCharacteristic(servo1Characteristic, servo1, SERVO1PIN);
      checkCharacteristic(servo2Characteristic, servo2, SERVO2PIN);
      checkCharacteristic(servo3Characteristic, servo3, SERVO3PIN);
      checkCharacteristic(servo4Characteristic, servo4, SERVO4PIN);
    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
void checkCharacteristic(BLEByteCharacteristic &servoCharacteristic, Servo &servoToMove, int servoPin){
    if(servoCharacteristic.written()){
        if (!servoCharacteristic.value()) {   // any value other than 0
          moveServo(servoToMove, servoPin);        
          }

}
}

void moveServo(Servo &servoToMove, int SERVOPIN){
          servoToMove.attach(SERVOPIN);
          delay(100);
          if(&servoToMove == &servo1 || &servoToMove == &servo3){
          servoToMove.write(110);
          delay(700);
          servoToMove.write(70);
          }
          else if(&servoToMove == &servo2 || &servoToMove == &servo4){
            servoToMove.write(0);
            delay(700);
            servoToMove.write(40);
          }
          delay(500);
          servoToMove.detach();
}

