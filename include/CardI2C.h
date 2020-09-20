  //this header file contains the I2C.c file's interface function and variables.

#if(IC_CARD_MODE==AT24_CARD)

#ifndef _CARDI2C_H

#define _CARDI2C_H


/********************************************************************************************/
#ifndef _CARDI2C_DEFINE_
#define HEAD_MACRO_CARDI2C	extern
#else
#define HEAD_MACRO_CARDI2C	  
#endif
/********************************************************************************************/

//global fuctions
/******************************************************************************************/
HEAD_MACRO_CARDI2C uchar CardI2CWrite( uchar sla, uint16 suba, const uchar *s, uchar len );
HEAD_MACRO_CARDI2C uchar CardI2CRead ( uchar sla, uint16 suba, uchar *s, uchar len );
/******************************************************************************************/

#endif 

#endif
