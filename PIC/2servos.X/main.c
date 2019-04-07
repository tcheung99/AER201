//#define _XTAL_FREQ 8000000

#include <xc.h>
#include "configBits.h"



void servoRotate90() //90 Degree
{
  unsigned int i;
  for(i=0;i<50;i++)
  {
    RC1 = 1;
//    RC2 = 1;
    __delay_us(1500);
    RC1 = 0;
//    RC2 = 0;    
    __delay_us(18500);
  }
}
void servoRotate0(int servo) //0 Degree
{
    unsigned int i;
    if (servo==1){
      for(i=0;i<15;i++)
//        for(i=0;i<20;i++)

          {
            RC2 = 1;
            RC1 = 1;
//            RC2 = 1;       
//            __delay_us(1400);
            __delay_us(1100); //left one 
            RC2 = 0;
//            RC1 = 0;
//            __delay_us(1150);
            __delay_us(300); //right 
            RC1 = 0;   
//            RC2 = 0;   
            __delay_us(18600);
          }
    }
    if (servo==2){
      for(i=0;i<40;i++)
        {
          RC2 = 1;
          __delay_us(449); //goes down to go lower 
          RC2 = 0;
          __delay_us(27300); //goes up to go lower 
        }
    }
}
void servoRotate180(int servo) //180 Degree
{
  unsigned int i;
    if (servo==1){
    for(i=0;i<15;i++)
//    for(i=0;i<20;i++)
        {
          RC2 = 1;
          RC1 = 1;
//                    __delay_us(450); //goes down to go lower 
          __delay_us(475); //goes down to go lower 
          RC2 =0;
          __delay_us(2100); //was 2100
          RC1 = 0;
          __delay_us(13525);
        }
  }
  if (servo==2){
    for(i=0;i<40;i++)
          {
            RC2 = 1;
            __delay_us(2200);
            RC2 = 0;
            __delay_us(17800);
          }
  }
}
void main()
{
  TRISC = 0; // PORTB as Ouput Port
  {
//    servoRotate0(1); //0 Degree
    servoRotate0(1); //0 Degree
    __delay_ms(2500);
//    servoRotate90(); //90 Degree
//    servoRotate180(1); //180 Degree
    servoRotate180(1); //180 Degree
        __delay_ms(500);

    servoRotate0(1); //0 Degree
    
//    __delay_ms(500);
  }
}
//
//void servoRotate0() //0 Degree
//{
//  unsigned int i;
//  for(i=0;i<50;i++)
//  {
//    RC2 = 1;
//    __delay_us(449); //goes down to go lower 
//    RC2 = 0;
//    __delay_us(27300); //goes up to go lower 
//  }
//}
//
//void servoRotate90() //90 Degree
//{
//  unsigned int i;
//  for(i=0;i<50;i++)
//  {
//    RC2 = 1;
//    __delay_us(1500);
//    RC2 = 0;
//    __delay_us(18500);
//  }
//}
//
//void servoRotate180() //180 Degree
//{
//  unsigned int i;
//  for(i=0;i<50;i++)
//  {
//    RC2 = 1;
//    __delay_us(2200);
//    RC2 = 0;
//    __delay_us(17800);
//  }
//}
//
//void main()
//{
//  TRISC = 0; // PORTB as Ouput Port
//  do
//  {
//    servoRotate0(); //0 Degree
////    servoRotate180(); //180 Degree
//    __delay_ms(2000);
//  }while(1);
//}