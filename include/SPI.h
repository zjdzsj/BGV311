  //this header file contains the I2C.c file's interface function and variables.

#ifndef _SPI_H

#define _SPI_H


/********************************************************************************************/
#ifndef _SPI_DEFINE_
#define HEAD_MACRO_SPI	extern
#else
#define HEAD_MACRO_SPI	  
#endif
/********************************************************************************************/

//global fuctions
/******************************************************************************************/
HEAD_MACRO_SPI uchar SPIRead_REG ( uchar reg,uchar *s, uchar regLen, uchar bank );
HEAD_MACRO_SPI uchar SPIWrite_REG ( uchar reg,uchar *s, uchar regLen, uchar bank );
HEAD_MACRO_SPI uchar Write_Register_CMD ( uchar reg, uchar cmd );

/******************************************************************************************/

#endif 