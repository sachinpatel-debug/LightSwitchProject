#include <ServoTimer2.h>

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <string.h>

// #include <Servo.h>
// Servo myServo;
ServoTimer2 myServo;
RH_ASK driver;
int lightStatus = 0;
int photoResistance = 0;
void setup()
{
  // Serial.begin(9600);	
  driver.init();
  // Serial.println("This is the receiver sketch"); 
  myServo.attach(3);
  // pinMode(7, OUTPUT);
  myServo.write(1675);

  
}

void loop()
{
  photoResistance = analogRead(A0);
  Serial.println(photoResistance);
  // randomSeed(analogRead(A0));
  // int randomNumber=random(180);
  // int buttonStatus = digitalRead(11);
  // if(buttonStatus==1){
  //   Serial.print(buttonStatus);
  //   myServo.write(randomNumber);
  //   delay(10000);
  // }
  // int pinStatus = digitalRead(7);
  // if(pinStatus==1){
  //   Serial.print(pinStatus);
  // }
  
    uint8_t buf[2];
    uint8_t buflen = sizeof(buf);
    
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
    
      // Message with a good checksum received, dump it.
      // Serial.print("Message: ");
      // Serial.println((char*)buf);         
    }

    // Serial.print("This is buf 0: ");
    // Serial.println(buf[0]);
    // Serial.print("This is buf 1: ");
    // Serial.println(buf[1]);
    // Serial.print("This is buf 2: ");
    // Serial.println(buf[2]);
    // Serial.print("This is buf 3: ");
    // Serial.println(buf[3]);

    // Serial.print("This is buf 1 characters: ");
    
    // // Serial.print(char(buf[1]));
    // Serial.println((char*)buf[1]);
    // Serial.print("This is buf 2 characters: ");
    // Serial.print(char(buf[2]));
    // Serial.println((char*)buf[2]);
    // Serial.print("This is buf 3 characters: ");
    // Serial.print(char(buf[3]));
    // Serial.println((char*)buf[3]);
    // Serial.print("This is the full message  ");
    // Serial.print((char*)buf);
    // Serial.println("      ");
    // Serial.println((char)buf);         
    String message = (char*)buf;
  message = message.substring(0, 2);    
  Serial.print("This is the message as a string: ");
    Serial.println(message);
    if(message == "Ga"){
      // digitalWrite(7, HIGH);
      buf[0] = 0;
      buf[1] = 0;
      // delay(500);
      // digitalWrite(7, LOW);
      
      if(photoResistance > 705){
        myServo.write(2250);
        delay(1000);
        myServo.write(1685);
        // digitalWrite(7, LOW);
        // lightStatus = 0;
        // Serial.print("The light status is ");
        // Serial.println(lightStatus);
      }
      if(photoResistance < 705){
        myServo.write(1400);
        delay(1000); 
        myServo.write(1675);        // digitalWrite(7,HIGH);
        // lightStatus = 1;
        // Serial.print("The light status is ");
        // Serial.println(lightStatus);
      }
    }

    delay(500);
    
    
  

   
    
}
