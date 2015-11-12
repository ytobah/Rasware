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
uint32_t SendLength = 2;
uint32_t RecLength = 2;
float WaitTime = 100;
tSPI * rSpiPt; //= &rspitest;
uint32_t * rSendPt = &rTestD;
uint32_t * rRecPt = &rTestD2;
uint32_t rSendLength = 1;
uint32_t rRecLength = 1;
tBoolean Check = false;
uint8_t * MiniPt;
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
    SpiPt =  InitializeSPI(PIN_A2, PIN_A5, PIN_A4, 100, 8, false, false); //Clock, mosi, miso, frequency, bitsPerFrame(?), clockPolarity(?), phase (?)
    *SendPt= 0x8220;  //Write Command, Config for Send
    SPIRequestUS(SpiPt, PIN_A3, SendPt, SendLength, RecPt, RecLength, 
WaitTime);

    Printf("Hello\nIs it me you're looking for?");
    rSpiPt =  InitializeSPI(PIN_F2, PIN_F1, PIN_F0, 100, 8, false, false);
    *SendPt=0x8320;  //Write to Config Reg, Config for Recieve
    SPIRequestUS(rSpiPt, PIN_B7, SendPt, 2, RecPt, 2, WaitTime);


SetPin(PIN_A7, true); //Manually set CE pins to receive/transmit
SetPin(PIN_B2, false );


    while (1) {
   // Printf("Hello ");

        // Runtime code can go here
   while (ch == 0){
  //   ch = Getc();
   ch = 0x7A;
    // *(SendPt + 3) = ch;
   Printf("Working\n");
}
   Printf("%c\n", ch);
   *SendPt = 0x7AA0; //8220 Little Endian: Write Command, Config for Send, 
                                        //Send Command, then Data to be sent
  // *(SendPt++)=0x82;
   MiniPt = SendPt;
   ch = *(MiniPt);
   Printf("%c\n", ch);
   ch = *(MiniPt+1);
   Printf("%c\n", ch);
//   ch = *(MiniPt+2);
  // Printf("%c\n", ch);


   Check = SPIRequestUS(SpiPt, PIN_A3, SendPt, 2, RecPt, 
  2, WaitTime);
  SetPin(PIN_B2, true);
   WaitUS(15);
  SetPin(PIN_B2, false);

  // Printf("%c Again!!\n",ch);
   if (Check){
   Printf("True\n");
    }

   SetPin(PIN_A7, false);  //Disable Receiver to Read Data
   *SendPt = 0x33445561; //8320;      //Write Command, Config for Receive  
                                 // ReceiveCommand, and Dummy Variable
   SPIRequestUS(rSpiPt, PIN_B7, SendPt, 4, RecPt, 2, WaitTime);
   SetPin(PIN_A7, true);   //Enable Receiver for next batch
   Printf("%c", ch);
   MiniPt = RecPt;
  //   ch = *MiniPt;
  //   Printf("%c\n", ch);
   ch = *(MiniPt+1);
   Printf("%c\n", ch);
   ch = *(MiniPt+2);
   Printf("%c\n",ch);
   ch = *(MiniPt+3);
   Printf("%c\n\n",ch);
Wait(5);
ch = 0;
}
}
