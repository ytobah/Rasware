#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/spi.h>

// Blink the LED to show we're on
tBoolean blink_on = true;
tSPI * SpiPt;
uint32_t * SendPt;
uint32_t * RecPt;
uint32_t SendLength;
uint32_t RecLength;
float WaitTime;
tSPI * rSpiPt;
uint32_t * rSendPt;
uint32_t * rRecPt;
uint32_t rSendLength;
uint32_t rRecLength;

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}

char ch=0;

// The 'main' function is the entry point of the program
int main(void) {
    // Initialization code can go here
    CallEvery(blink, 0, 0.5);
    SpiPt =  InitializeSPI(PIN_A2, PIN_A5, PIN_A4, 100, 16, true, 
                            false);
    Printf("Hello");
    rSpiPt =  InitializeSPI(PIN_F2, PIN_F1, PIN_F0, 100, 16, true, 
                           false);

    while (1) {
   // Printf("Hello ");

        // Runtime code can go here
   while (ch == 0){
    ch = Getc();
   *SendPt = (int) ch;
  }
   // SPIRequest(SpiPt, PIN_A3, SendPt, SendLength, RecPt, RecLength, 
//WaitTime);
   
  //  SPIRequest(rSpiPt, PIN_A6, rSendPt, rSendLength, rRecPt, 
//rRecLengt, WaitTime);

    Printf("%c\n", ch);
//Wait(2);
ch = 0;
}
}
