  //this header file contains the Uart2.c file's interface function and variables.

#ifndef _UART2_H

#define _UART2_H


/********************************************************************************************/
#ifndef _UART2_DEFINE_
#define HEAD_MACRO_UART2	extern
#else
#define HEAD_MACRO_UART2	  
#endif
/********************************************************************************************/

//global fuctions
/******************************************************************************************/
HEAD_MACRO_UART2 void InitUart2 ( void );
HEAD_MACRO_UART2 uchar SPIRead_REG ( uchar reg,uchar *s, uchar regLen );
HEAD_MACRO_UART2 uchar SPIWrite_REG ( uchar reg,uchar *s, uchar regLen );
HEAD_MACRO_UART2 void SoftResetCl2208 ( void );
/******************************************************************************************/

#endif 