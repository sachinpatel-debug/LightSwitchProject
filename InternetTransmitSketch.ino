#include <RadioHead.h>
// #include <avr/sleep.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
// #include <avr/power.h>


RH_ASK driver;
const char *msg = "Ga";
volatile bool sendTime = false;
void buttonClicked(){
    //(I'm copying a youtube video)
    sendTime = true;
    
}
void setup()
{
    Serial.begin(9600);	  // Debugging only
    pinMode(2,INPUT);
    pinMode(8,OUTPUT);
    driver.init();
    
    // sleep_enable();
    attachInterrupt(digitalPinToInterrupt(2), buttonClicked, LOW);
    // sleep_bod_disable();
    // set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    // power_all_disable();
}

void loop()
{
if(sendTime == false){
ADCSRA &= ~(1<<7);
SMCR |= (1<<2);
SMCR |= 1; 

MCUCR |= (3<<5);
MCUCR |= (MCUCR & ~(1 << 5)) | (1 << 6);

__asm__ __volatile__("sleep");
}    
    // int buttonStatus = digitalRead(2);
else if(sendTime==true){
    for(int i =0; i<5;i++){
    driver.send((uint8_t *)msg, strlen(msg));
    // Serial.println("hello");
    driver.waitPacketSent();
    Serial.println("Sent");
    delay(10);
    }
    // delay(100);
    // digitalWrite(8, HIGH);
    // delay(1000);
    // digitalWrite(8, LOW);
    sendTime = false;
    }    
    
}
