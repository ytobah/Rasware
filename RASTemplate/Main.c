#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/spi.h>

// Blink the LED to show we're on
tBoolean blink_on = true;
tSPI * SpiPt;
void SendReq(void);
void RecReq(void);
uint32_t TestData[4];
uint32_t TestData2[4];
uint32_t rTestD[4];
uint32_t rTestD2[4];
uint32_t * SendPt = &TestData;
uint32_t * RecPt = &TestData2;
uint32_t SendLength = 2;
uint32_t RecLength = 2;
float WaitTime = 1000;
tSPI * rSpiPt;
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

//A function for Sending Data 
void SendReq(void){
    SetPin(PIN_A3, false);
    SPIRequestUS(SpiPt, PIN_B7, SendPt, SendLength, RecPt, RecLength, WaitTime);
    SetPin(PIN_A3, true);
    }

//A function for Receiving Data
void RecReq(void){
    SetPin(PIN_B7, false);
    SPIRequestUS(rSpiPt, PIN_A3, SendPt, 2, RecPt, 2, WaitTime);
    SetPin(PIN_B7, true);
    }


// The 'main' function is the entry point of the program
int main(void) {

    Printf("Hi\n");
    // Initialization code can go here
    InitializeGPIO();
    CallEvery(blink, 0, 0.5);
    SetPin(PIN_B7, true);                  //CSL Pin for Receiver
    SetPin(PIN_A3, true);                  //CSL Pin for Transmit

    SpiPt =  InitializeSPI(PIN_A2, PIN_A5, PIN_A4, 100, 8, false, 
             false);    //Clock, mosi, miso, frequency, bitsPerFrame(?), 
                        //clockPolarity(?), phase (?)

    *SendPt= 0x20;      //Write Command,
    *(SendPt+1) = 0x82; //Config module as Transmitter

    SetPin(PIN_A3, false);
    SPIRequestUS(SpiPt, PIN_B7, SendPt, SendLength, RecPt, RecLength, WaitTime);
    SetPin(PIN_A3, true);

    *SendPt = 0x27;    //Clear FIFO Full flag
    *(SendPt+1) = 0x10;
    SetPin(PIN_A3, false);
    SPIRequestUS(SpiPt, PIN_B7, SendPt, SendLength, RecPt, RecLength, WaitTime);
    SetPin(PIN_A3, true);

    *SendPt = 0xE1;    //Flush Transmit FIFO
    SetPin(PIN_A3, false);
    SPIRequestUS(SpiPt, PIN_B7, SendPt, 1, RecPt, 1, WaitTime);
    SetPin(PIN_A3, true);


    ch = *RecPt;
    Printf("%X\n", ch);
    Wait(3);
    Printf("Hello\nIs it me you're looking for?\n");




    rSpiPt =  InitializeSPI(PIN_F2, PIN_F1, PIN_F0, 100, 8, false, 
    false);

    *SendPt = 0xE2;
    SetPin(PIN_B7, false); //flush Transmit FIFO of Receiver
    SPIRequestUS(rSpiPt, PIN_A3, SendPt, 1, RecPt, 1, WaitTime);
    SetPin(PIN_B7, true);

    ch = 0;
   // while(ch==0){
    *SendPt=0x20;       //Write to Config Reg, 
    *(SendPt+1) = 0x7F; //Config Module as Receiver
    SetPin(PIN_B7, false);
    SPIRequestUS(rSpiPt, PIN_A3, SendPt, 2, RecPt, 2, WaitTime);
    SetPin(PIN_B7, true);

    ch = *RecPt;
    Printf("CHECK: %X\n\n",ch);
   // }

    *SendPt=0x22;       //Write to RX Pipe Enable
    *(SendPt+1) = 0x20; //Enable Receive Pipe 0 Enhanced Shockburst
    SetPin(PIN_B7, false);
    SPIRequestUS(rSpiPt, PIN_A3, SendPt, 2, RecPt, 2, WaitTime);
    SetPin(PIN_B7, true);

    *SendPt=0x36;       //Write to RX Pipe 1 width config
    *(SendPt+1) = 0x01; //Pipe has Payload of 1 Byte
    SetPin(PIN_B7, false);
    SPIRequestUS(rSpiPt, PIN_A3, SendPt, 2, RecPt, 2, WaitTime);
    SetPin(PIN_B7, true);

    ch = *RecPt;
    Printf("CHECK: %X\n\n",ch);




    SetPin(PIN_A7, true); //Manually set CE pins to receive/transmit
    SetPin(PIN_B2, false);


    while (1) {
    Printf("Hi\n");
        // Runtime code can go here
        while (ch == 0){
            ch = 0x7A;
            Printf("Working\n");
        }
        Printf("%c\n\n", ch);
        *SendPt = 0xA0;      //Send Command
        *(SendPt+1) = 0x7A;  //The data to be sent
        SetPin(PIN_A3, false);
        Check = SPIRequestUS(SpiPt, PIN_B7, SendPt, 2, RecPt, 2, WaitTime);
        SetPin(PIN_A3, true);
        Printf("Data has been put into TX FIFO\n");
        Printf("Checking TX FIFO Status.....\n\n");
        Wait(2);

        *SendPt = 0x17;     //Read FIFO Status Register
        *(SendPt +1) = 0x55;
        SetPin(PIN_A3, false);
        SPIRequestUS(SpiPt, PIN_B7, SendPt, 2, RecPt, 2, WaitTime);
        SetPin(PIN_A3, true);


        Printf("*TX FIFO STATUS*\n");
        ch = *(RecPt);
        Printf("Status Register: %X\n",ch);
        ch = *(RecPt + 1);
        Printf("FIFO Status Register: %X\n\n",ch);

        Printf("Attempting to Send Out Data.....\n\n");
        Wait(2);
        SetPin(PIN_B2, true);  //Send off data
        WaitUS(100);
        SetPin(PIN_B2, false);

        *SendPt= 0x17;        //Read FIFO Status Again
        *(SendPt+1) = 0x55;
        SetPin(PIN_A3, false);
        SPIRequestUS(SpiPt, PIN_B7, SendPt, 2, RecPt, 2, WaitTime);
        SetPin(PIN_A3, true);
       
        Printf("*TX FIFO STATUS*\n");
        ch = *(RecPt);
        Printf("Status Register: %X\n",ch);
        ch = *(RecPt + 1);
        Printf("FIFO Status Register: %X\n\n",ch);

        if (Check){
        Printf("True\n");
        }

        SetPin(PIN_A7, false);  //Disable Receiver to Read Data
        *SendPt = 0x17;         //ReceiveCommand
        *(SendPt + 1) = 0x55;   //Dummy Variable
        *(SendPt + 2) = 0x55;

        SetPin(PIN_B7, false);
        SPIRequestUS(rSpiPt, PIN_A3, SendPt, 3, RecPt, 3, WaitTime);
        SetPin(PIN_B7, true);

        SetPin(PIN_A7, true);   //Enable Receiver for next batch
        Printf("%c\n", ch);
        ch = *RecPt;
        Printf("%X This is 1st RecPt Hex Value\n", ch);
        ch = *(RecPt+1);
        Printf("%X This is 2nd RecPt Hex Value\n", ch);
        ch = *(RecPt +2);
        Printf("%X This is 3rd RecPt Hex Value\n",ch);


//   if(*(RecPt+1) == 0x7A){SetPin(PIN_F0, true);
  //                         Wait(.5);
    //                       SetPin(PIN_F0, true);
      //                     Wait(.5);
        //                   SetPin(PIN_F0, false);
          //                 SetPin(PIN_F0, false);
            //              }
Wait(100);
ch = 0;
}
}
