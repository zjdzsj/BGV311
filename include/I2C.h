  //this header file contains the I2C.c file's interface function and variables.

#ifndef _I2C_H

#define _I2C_H


/********************************************************************************************/
#ifndef _I2C_DEFINE_
#define HEAD_MACRO_I2C	extern
#else
#define HEAD_MACRO_I2C	  
#endif
/********************************************************************************************/

//global fuctions
/******************************************************************************************/
HEAD_MACRO_I2C void InitI2C ( void );
HEAD_MACRO_I2C uchar I2CWrite ( uchar sla, uint16 suba, const uchar *s, uchar len, uchar sublen );
HEAD_MACRO_I2C uchar I2CRead ( uchar sla, uint16 suba, uchar *s, uchar len, uchar sublen );
/******************************************************************************************/

#endif 