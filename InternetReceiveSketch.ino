#include <ServoTimer2.h>

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#include <string.h>
#include <avr/interrupt.h>
#define wdt_reset() __asm__ __volatile__("wdr");
const int ServoEn = 8;
volatile bool wakeUpTime = false;
int cyclesSinceReceived = 0; //triggers if the receiver just got the message
ServoTimer2 myServo;
RH_ASK driver;
int lightStatus = 0;
uint8_t buf[2];
uint8_t buflen = sizeof(buf);  
int timeElapsed;
const int RxEn = 8;
ISR(WDT_vect){
wakeUpTime = true;
}
void setup()
{
  // pinMode(RxEn, OUTPUT);
  // digitalWrite(RxEn, LOW);
  // pinMode(13, OUTPUT);
  // digitalWrite(13, LOW);
  // pinMode(ServoEn, OUTPUT);
  // digitalWrite(ServoEn, HIGH);
  pinMode(RxEn, OUTPUT);
  digitalWrite(RxEn, LOW);
  Serial.begin(9600);
  driver.init();
  myServo.attach(3);
  myServo.write(1675);
  // myServo.detach();
  delay(500);
  digitalWrite(ServoEn, LOW);

  // Serial.println("This is the receiver sketch"); 
  // myServo.attach(3);
  
  // myServo.write(1675);
  cli();
  wdt_reset();
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  WDTCSR = (1 << WDIE) | (1<<WDP2) | (1<<WDP1); 
  sei();

}

void loop()
{

  if(wakeUpTime == false){
    //sleep code here
    ADCSRA &= ~(1<<7);
    SMCR |= (1<<2);
    SMCR |= 1; 

    MCUCR |= (3<<5);
    MCUCR |= (MCUCR & ~(1 << 5)) | (1 << 6);

    __asm__ __volatile__("sleep");
    
  }

  else if(wakeUpTime == true){
    digitalWrite(RxEn, HIGH); //turns radio on
    delay(100);
    Serial.println(cyclesSinceReceived);
    buf[0] = 0;
    buf[1] = 0;
unsigned long timeBeforeLoop = millis();
    // digitalWrite(RxEn, HIGH);
    do{
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
    
      // Message with a good checksum received, dump it.
      // Serial.print("Message: ");
      // Serial.println((char*)buf);         
    }
    
  String message = (char*)buf;
  // Serial.println((char*)buf);         

  message = message.substring(0, 2);    
  //  Serial.println(message);
    if((message == "Ga") && (cyclesSinceReceived > 15000)){
      cyclesSinceReceived = 0;
      Serial.println("We are in");
      // digitalWrite(ServoEn, HIGH);
      delay(500);
      myServo.attach(3);
      delay(500);
      if(lightStatus == 1){
        Serial.println("Servo Moves now");
        myServo.write(1400);
        delay(750);
        myServo.write(1675);
        delay(500);
        lightStatus = 0;
      }
      else if(lightStatus == 0){
          Serial.println("Servo Moves now 2");  
          myServo.write(2200);  //this is the off position
          delay(750);
          myServo.write(1675);   
          delay(500);       
          lightStatus = 1;
      }
      myServo.detach();
      delay(500);
      digitalWrite(ServoEn, LOW);
      wakeUpTime = false;
      
      // Serial.println(wakeUpTime);
      delay(250);
    }  
    else{ //if we enter this else loop, the message received was not Ga
      message = " ";
      if(cyclesSinceReceived < 15000){
      cyclesSinceReceived = cyclesSinceReceived + 1;
      }
      else{
        cyclesSinceReceived = 16000;
        }
    }
  unsigned long timeAfterLoop = millis();
  timeElapsed = timeAfterLoop - timeBeforeLoop;
  // Serial.println(timeElapsed);
  }
  while((timeElapsed <= 300) && (wakeUpTime == true));
  // Serial.print("WakeUpTime after while loop is ");
  // Serial.println(wakeUpTime);
  pinMode(RxEn, LOW); //turn the radio off
  wdt_reset(); //reset watchdog timer
  digitalWrite(RxEn, LOW); //turn radio off
  wakeUpTime = false;
  }
   
    
  

   
    
}
