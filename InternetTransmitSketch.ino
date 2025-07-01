#include <RadioHead.h>

// #include <RadioHead.h>

#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;
void setup()
{
    Serial.begin(9600);	  // Debugging only
    pinMode(7,INPUT);
    driver.init();
}

void loop()
{
    const char *msg = "Ga";
    int buttonStatus = digitalRead(7);
    if(buttonStatus==1){
        // Serial.println(buttonStatus);
        driver.send((uint8_t *)msg, strlen(msg));
        driver.waitPacketSent();
        delay(500);
    }    
    
}
