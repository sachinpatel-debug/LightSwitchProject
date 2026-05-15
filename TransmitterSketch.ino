/*
  LED Control

  This example scans for Bluetooth® Low Energy peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the Bluetooth® Low Energy peripheral's LED, when the button is pressed or released.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
  - Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button with pull-up resistor connected to pin 2.

  You can use it with another board that is compatible with this library and the
  Peripherals -> LED example.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

// variables for buttons
const int button1Pin = 2;
const int button2Pin = 3;
const int button3Pin = 4;
const int button4Pin = 5;
int oldButton1State = LOW;
int oldButton2State = LOW;
int oldButton3State = LOW;
int oldButton4State = LOW;

BLECharacteristic servo1Characteristic;
BLECharacteristic servo2Characteristic;
BLECharacteristic servo3Characteristic;
BLECharacteristic servo4Characteristic;
void checkButtonState(int buttonPin, int &oldButtonState, BLECharacteristic &servoCharacteristic);
void setup() {
  Serial.begin(9600);
  // while (!Serial);

  // configure the button pin as input
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);  
  pinMode(button4Pin, INPUT);  

  // initialize the Bluetooth® Low Energy hardware
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  Serial.println("Bluetooth® Low Energy Central - LED control");

  // start scanning for peripherals
  BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1214");
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "LED") { //if the discovered peripheral's name isn't LED, go back and search again
      return; 
    }
  //if discovered peripheral does have the name "LED," stop the scan and connect to it.
    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("19B10000-E8F2-537E-4F6C-D104768A1214");
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  servo1Characteristic = peripheral.characteristic("19b10001-e8f2-537e-4f6c-d104768a1214");
  servo2Characteristic = peripheral.characteristic("19b10002-e8f2-537e-4f6c-d104768a1214");
  servo3Characteristic = peripheral.characteristic("19b10003-e8f2-537e-4f6c-d104768a1214");
  servo4Characteristic = peripheral.characteristic("19b10004-e8f2-537e-4f6c-d104768a1214");

  while (peripheral.connected()) {
    // while the peripheral is connected

    checkButtonState(button1Pin, oldButton1State, servo1Characteristic); 
    checkButtonState(button2Pin, oldButton2State, servo2Characteristic); 
    checkButtonState(button3Pin, oldButton3State, servo3Characteristic); 
    checkButtonState(button4Pin, oldButton4State, servo4Characteristic); 


  }

  Serial.println("Peripheral disconnected");
}

void checkButtonState(int buttonPin, int &oldButtonState, BLECharacteristic &servoCharacteristic){
  int buttonState = digitalRead(buttonPin); //read the button pin
  if (oldButtonState != buttonState) { 
      // button changed
      oldButtonState = buttonState;
      if (buttonState) {
        Serial.println("button pressed");
        // button is pressed, write 0x01 to toggle the characteristic value 
        servoCharacteristic.writeValue((byte)0x01);
      } else {
        Serial.println("button released");
        // button is released, write 0x00 to toggle the characteristic value
        servoCharacteristic.writeValue((byte)0x00); //the toggling of the characteristic value will be used to activate the servo for light flipping
      }
    }  


}
