#include <ServoTimer2.h>

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#include <string.h>
#include <avr/interrupt.h>
#define wdt_reset() __asm__ __volatile__("wdr");
const int LED = 13;
volatile bool wakeUpTime = false;

ServoTimer2 myServo;
RH_ASK driver;
int lightStatus = 0;
uint8_t buf[2];
uint8_t buflen = sizeof(buf);  
int timeElapsed;
ISR(WDT_vect){
wakeUpTime = true;
}
void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);	
  driver.init();
  // Serial.println("This is the receiver sketch"); 
  // myServo.attach(3);
  // pinMode(7, OUTPUT);
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

  else if(wakeUpTime ==true){
unsigned long timeBeforeLoop = millis();
    do{
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
    
      // Message with a good checksum received, dump it.
      // Serial.print("Message: ");
      // Serial.println((char*)buf);         
    }
    
  String message = (char*)buf;
  message = message.substring(0, 2);    
  //  Serial.println(message);
    if(message == "Ga"){
      buf[0] = 0;
      buf[1] = 0;
      
      if(lightStatus == 1){
        // myServo.write(2250);
        // delay(1000);
        // myServo.write(1685);
        digitalWrite(13, HIGH);
        delay(700);
        digitalWrite(13, LOW);
        lightStatus = 0;
      
      }
      else if(lightStatus == 0){
        // myServo.write(1400);
        // delay(1000); 
        // myServo.write(1675);       
          digitalWrite(13, HIGH);
          delay(700);
          digitalWrite(13, LOW);
          lightStatus = 1;
      }
      wakeUpTime = false;
      delay(250);

    }  
  unsigned long timeAfterLoop = millis();
  timeElapsed = timeAfterLoop - timeBeforeLoop;
  Serial.println(timeElapsed);
  }
  while((timeElapsed <= 300) && (wakeUpTime == true));
  wdt_reset();
  wakeUpTime = false;
  }
  


    
    
  

   
    
}
