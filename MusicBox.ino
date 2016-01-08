/* 
                                      +-----+
         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |         GND/RST2  [ ][ ]            |
         |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   
         |          5V/MISO2 [ ][ ]  A4/SDA[ ] |  
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                    SCK/13[ ] |   
         | [ ]IOREF                 MISO/12[ ] |   
         | [ ]RST                   MOSI/11[ ]~|   Next / v++
         | [ ]3V3    +---+               10[ ]~|   Repeat
         | [ ]5v    -| A |-               9[ ]~|   Play / Pause
         | [ ]GND   -| R |-               8[ ] |   Prev / v--
         | [ ]GND   -| D |-                    |
         | [ ]Vin   -| U |-               7[ ] |   
         |          -| I |-               6[ ]~|   
         | [ ]A0    -| N |-               5[ ]~|   
         | [ ]A1    -| O |-               4[ ] |   
DT       | [ ]A2     +---+           INT1/3[ ]~|   
CLK      | [ ]A3                     INT0/2[ ] |   SW 
         | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   
         | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   
         |            [ ] [ ] [ ]              |
         |  UNO_R3    GND MOSI 5V  ____________/
          \_______________________/

          Written by Bram Rausch
*/

//import libraries
#include <ClickEncoder.h>
#include <TimerOne.h>

ClickEncoder *encoder;
int16_t last, value;

void timerIsr() {
  encoder->service();
}


void setup() {
  encoder = new ClickEncoder(A2, A3, 2); //define A2 as dt pin, A3 as Clk pin and 2 as switch
  for(int i = 8; i < 12; i++){ //configure pin 8,9,10,11 as output
    pinMode(i, OUTPUT);  
  }

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  last = -1;
}

void loop() {  
  ClickEncoder::Button b = encoder->getButton();
  value += encoder->getValue();
  
  if (value != last) { //if encoder rotated
    if(last > value){ //to the left
        digitalWrite(8, HIGH); //Prev / v--
    } else if(last < value){ //to the right
        digitalWrite(11, HIGH); //Next / v++
    }
    delay(100);
    digitalWrite(8, LOW);
    digitalWrite(11, LOW);
    last = value;
  }
  
  if(b != ClickEncoder::Open && ClickEncoder::Clicked){ //if encoder is clicked
    digitalWrite(9, HIGH); //Play / Pause
    delay(100);
    digitalWrite(9, LOW);
  }
}    

