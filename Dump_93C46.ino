//  Read data from the Atmel 93C46 eeprom (DIP8) by using the arduino uno and dump them on serial port monitor.

#include<SPI.h>
#define CS        10  //connect to pin 1 of 93C46
#define CLK       13  //connect to pin 2 of 93C46
#define DATAOUT   11  //connect to pin 3 of 93C46 
#define DATAIN    12  //connect to pin 4 of 93C46



void setup()
{
  pinMode(CS,OUTPUT);
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(CLK,OUTPUT);
  digitalWrite(CS,LOW);
  
  Serial.begin(9600);

  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  dump();
}

void loop()
{
}

void dump(){
  Serial.println("Dump 93C46");
  Serial.println("address:\tdata:");
  
  byte data1;
  byte data2;
  
  for (int address = 0; address < 128; address++) {
    digitalWrite(CS,HIGH);
    SPI.transfer(0b11);       // start bit and first bit of read opcode
    SPI.transfer(address);    // second bit of read opcode is 0 so the address is automatically preceded by 0
    data1 = SPI.transfer(0);  // data1 contains a 0 and the first 7 bits at the address
    data2 = SPI.transfer(0);  // contains the 8th bit (LSB)
    digitalWrite(CS,LOW);

    data1 = data1 << 1; // bitwise shift to get rid of the preceding 0
    if (bitRead(data2, 7) == 1){
      bitWrite(data1, 0, 1); // change LSB if needed
    }
    
    printByte(address, data1);
  }
}

void printByte(byte address, byte data){
  Serial.print(address, HEX);
  Serial.print("\t\t");
  Serial.print(data, HEX);
  Serial.print("\n");
}
