#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/spi.h>

// Blink the LED to show we're on
tBoolean blink_on = true;
//tSPI spitest;
//tSPI rspitest;
tSPI * SpiPt;  //= &spitest;
uint32_t TestData;
uint32_t TestData2;
uint32_t rTestD;
uint32_t rTestD2;
uint32_t * SendPt = &TestData;
uint32_t * RecPt = &TestData2;
uint32_t SendLength = 1;
uint32_t RecLength = 1;
float WaitTime = 0.5;
tSPI * rSpiPt; //= &rspitest;
uint32_t * rSendPt = &rTestD;
uint32_t * rRecPt = &rTestD2;
uint32_t rSendLength = 1;
uint32_t rRecLength = 1;
tBoolean Check = false;
void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}

int ch=0;

// The 'main' function is the entry point of the program
int main(void) {
    // Initialization code can go here
    InitializeGPIO();
    CallEvery(blink, 0, 0.5);
    SpiPt =  InitializeSPI(PIN_A2, PIN_A5, PIN_A4, 100, 16, false, 
false);
    Printf("Hello\n");
    rSpiPt =  InitializeSPI(PIN_F2, PIN_F1, PIN_F0, 100, 16, false, 
false);

SetPin(PIN_A7, true);
SetPin(PIN_B2, false);


    while (1) {
   // Printf("Hello ");

        // Runtime code can go here
   while (ch == 0){
    ch = Getc();
   *SendPt = ch;
   Printf("Working\n");
}
   Printf("%c\n", ch);

  Check = SPIRequestUS(SpiPt, PIN_A3, SendPt, SendLength, RecPt, 
RecLength, WaitTime);
   Printf("%c Again!!\n",ch);
   if (Check){
   Printf("True\n");
    }
   SPIRequestUS(rSpiPt, PIN_B7, rSendPt, rSendLength, rRecPt, 
   rRecLength, WaitTime);

    Printf("%c", ch);
    ch = *rRecPt;
    Printf("%c\n", ch);
//Wait(2);
ch = 0;
}
}
