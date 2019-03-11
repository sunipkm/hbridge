#include <SPI.h>
typedef union
{
  unsigned short cmd;
  struct
  {
    unsigned char ovlo   : 1;
    unsigned char hbcnf1 : 1;
    unsigned char hbcnf2 : 1;
    unsigned char hbcnf3 : 1;
    unsigned char hbcnf4 : 1;
    unsigned char hbcnf5 : 1;
    unsigned char hbcnf6 : 1;
    unsigned char hben1  : 1;
    unsigned char hben2  : 1;
    unsigned char hben3  : 1;
    unsigned char hben4  : 1;
    unsigned char hben5  : 1;
    unsigned char hben6  : 1;
    unsigned char uldsc  : 1;
    unsigned char hbsel  : 1;
    unsigned char srr    : 1;
  };
} hbridge_cmd;
typedef union
{
  unsigned short data;
  struct
  {
    
    unsigned char tw      : 1;
    unsigned char hbcr1   : 1;
    unsigned char hbcr2   : 1;
    unsigned char hbcr3   : 1;
    unsigned char hbcr4   : 1;
    unsigned char hbcr5   : 1;
    unsigned char hbcr6   : 1;
    unsigned char hbst1   : 1;
    unsigned char hbst2   : 1;
    unsigned char hbst3   : 1;
    unsigned char hbst4   : 1;
    unsigned char hbst5   : 1;
    unsigned char hbst6   : 1;
    unsigned char uld     : 1;
    unsigned char psf     : 1;
    unsigned char ocs     : 1;
    //unsigned char tsd     : 1;
    //unsigned short unused : 15;
  };
} hbridge_data;
#define SS 4
hbridge_cmd x;
hbridge_data y;
void setup() 
{
  Serial.begin(9600);
  Serial.println("System up!");
  pinMode(SS,OUTPUT);
  digitalWrite(SS,HIGH);
  
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(5000000);
  SPI.begin();
  delay(100);
  //x.hbsel = 0;
  x.ovlo = 0;
  x.hbcnf1 = 0;
  x.hbcnf2 = 0;
  x.hbcnf3  = 0;
  x.hbcnf4  = 0;
  x.hbcnf5 = 0;
  x.hbcnf6 = 0;
  x.hben1  = 1;
  x.hben2  = 1;
  x.hben3  = 1;
  x.hben4  = 1;
  x.hben5  = 1;
  x.hben6  = 1;
  x.uldsc  = 0;
  x.hbsel  = 0;
  x.srr  = 0;
  
  //x.hben6 = 1;
  //x.hbcnf6 = 1;
  // Serial.println("%c", x.ovlo);
  //Serial.print("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", x.ovlo, x.hbcnf1, x.hbcnf2, x.hbcnf3, x.hbcnf4, x.hbcnf5, x.hbcnf6, x.hben1, x.hben2, x.hben3, x.hben4, x.hben5, x.hben6, x.uldsc, x.hbsel, x.srr);
//  digitalWrite(SS, LOW);
//  
//  y.data = SPI.transfer16(x.cmd);
//
//  //y.data = y.data >> 8;
//  digitalWrite(SS, HIGH);
//  Serial.println(y.data);
//  delay(100);
//  digitalWrite(SS, LOW);
//  x.hbcnf6 = 1;
//  y.data = SPI.transfer16(x.cmd);
//  Serial.println(y.data);
//  digitalWrite(SS, HIGH);
}
void loop() {
  int res ;
    x.hbcnf1 = 1;
    x.hbcnf2 = 1;
    x.hbcnf3 = 1;
    x.hbcnf4 = 1;
    x.hbcnf5 = 1;
    x.hbcnf6 = 1;
    digitalWrite(SS,LOW);
    delay(100);
    res=SPI.transfer16(0b0000011000010000);
    digitalWrite(SS,HIGH);
    Serial.println("On");Serial.println(res);
    delay(1000);
    x.hbcnf1 = 0;
    x.hbcnf2 = 0;
    x.hbcnf3 = 0;
    x.hbcnf4 = 0;
    x.hbcnf5 = 0;
    x.hbcnf6 = 0;
    digitalWrite(SS,LOW);
    delay(100);
    res=SPI.transfer16(0b0000011000001000);
    digitalWrite(SS,HIGH);
    Serial.println("Off");Serial.println(res);
    delay(1000);
    
}
