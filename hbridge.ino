#include <SPI.h>
typedef union
{
  unsigned short cmd; //Combined bits
  struct //separated bits
  {
    unsigned char ovlo   : 1; //over voltage lockout
    unsigned char hbcnf1 : 1; //half bridge 1 configuration (1 -> LS1 off and HS1 on, 0 -> LS1 on and HS1 off)
    unsigned char hbcnf2 : 1;
    unsigned char hbcnf3 : 1;
    unsigned char hbcnf4 : 1;
    unsigned char hbcnf5 : 1;
    unsigned char hbcnf6 : 1;
    unsigned char hben1  : 1; //half bridge 1 enable (1 -> bridge in use, 0 -> bridge not in use)
    unsigned char hben2  : 1;
    unsigned char hben3  : 1;
    unsigned char hben4  : 1;
    unsigned char hben5  : 1;
    unsigned char hben6  : 1;
    unsigned char uldsc  : 1; //under load detection shutdown
    unsigned char hbsel  : 1; //half bridge selection (needs to be set to 0)
    unsigned char srr    : 1; //status reset register: 1 -> clear all faults and reset
  };
} hbridge_cmd;
typedef union
{
  unsigned short data;
  struct
  {
    
    unsigned char tw      : 1; //thermal warning
    unsigned char hbcr1   : 1; //half bridge 1 configuration reporting (mirrors command)
    unsigned char hbcr2   : 1;
    unsigned char hbcr3   : 1;
    unsigned char hbcr4   : 1;
    unsigned char hbcr5   : 1;
    unsigned char hbcr6   : 1;
    unsigned char hbst1   : 1; //half bridge 1 enable status (mirrors command)
    unsigned char hbst2   : 1;
    unsigned char hbst3   : 1;
    unsigned char hbst4   : 1;
    unsigned char hbst5   : 1;
    unsigned char hbst6   : 1;
    unsigned char uld     : 1; //under load detection (1 -> fault)
    unsigned char psf     : 1; //power supply failure
    unsigned char ocs     : 1; //over current shutdown
    //unsigned char tsd     : 1;
    //unsigned short unused : 15;
  };
/*
 * SPI works in the following way: status[n-1] = SPI.transfer16(cmd[n]), meaning
 * at the time a command is sent, the status of the previous command is returned. 
 * If there are no faults, the status corresponding to a command should be identical
 * to the command itself.
 */
} hbridge_data;
#define SS 4 //Enable pin, not in use. The device is enabled by default. Enable requires HIGH on the EN pin.
hbridge_cmd x; //global unions for command and data
hbridge_data y;
void setup() //set up SPI and serial comm
{
  Serial.begin(9600); //Serial comm at baud rate of 9600 bps
  Serial.println("System up!"); //first message
  pinMode(SS,OUTPUT); //set SS pin to output for Arduino
  digitalWrite(SS,HIGH); //set SS to HIGH to wake up the H bridge IC
  /* Setting up SPI */
  SPI.setBitOrder(MSBFIRST); //the most significant bit is sent first
  SPI.setDataMode(SPI_MODE1); //set SPI operation mode
  /*
   * SPI_MODE1 -> SPI mode 1 indicates that communication will start with CLK at LOW 
   * and the bits are read by the slave at the FALLING EDGE.
   */
  SPI.setClockDivider(5000000); //SPI frequency set to 5MHz
  SPI.begin(); //Initialize SPI communication
  delay(100); //Wait 100ms
  /* Enable Half-bridges */
  x.srr = 1 ;
  Serial.print("Cmd: ");Serial.println(x.cmd); //Print the 16 bit command to serial port
  digitalWrite(SS,LOW); //set the chip select to low for SPI comm to start
  y.data = SPI.transfer16(x.cmd); //Transfer the command to the slave (output of this command is garbage because there is no command before this one)
  digitalWrite(SS,HIGH); //set the chip select to high for HBridge to work
  Serial.print("Init reset: "); Serial.println(y.data);
  delay(1000);
  x.srr = 0 ;
  x.hben3 = 1 ; 
  x.hben4 = 1 ;
  x.hben1 = 1 ;
  x.hben2 = 1 ;
  x.hbsel = 0 ;
  Serial.print("Cmd: ");Serial.println(x.cmd); //Print the 16 bit command to serial port
  digitalWrite(SS,LOW);
  y.data = SPI.transfer16(x.cmd); //Transfer the command to the slave (output of this command is garbage because there is no command before this one)
  digitalWrite(SS,HIGH);
  Serial.print("Init: "); Serial.println(y.data); //Should return garbage value
  delay(1000); //Wait 1s
}
void loop() { //Entering loop, everything in here will be repeated indefinitely by Arduino
    x.hbcnf3 = 0; //HB3 on
    x.hbcnf4 = 1; //HB4 off
    x.hbcnf1 = 1; //HB1 on
    x.hbcnf2 = 0; //HB2 off
    digitalWrite(SS,LOW);
    y.data=SPI.transfer16(x.cmd); //Transfer the command, and recover the status of the previous command
    digitalWrite(SS,HIGH);
    Serial.print("On: ");Serial.println(y.data); //print status of previous command to serial
    delay(1000); //Wait 1s
    x.hbcnf3 = 1; //HB3 off
    x.hbcnf4 = 0; //HB4 on
    x.hbcnf1 = 0; //HB1 off
    x.hbcnf2 = 1; //HB2 off
    digitalWrite(SS,LOW);
    y.data=SPI.transfer16(x.cmd); //Transfer the command, and recover the status of the previous command
    digitalWrite(SS,HIGH);
    Serial.print("Off: ");Serial.println(y.data); //print status of previous command to serial
    delay(1000); //Wait 1s
    //The use of the words "On" and "Off" is purely circumstantial.
} //repeat
