  //this header file contains the I2C.c file's interface function and variables.
#if (IC_CARD_MODE==RF_CARD)
	
#ifndef _RFSPI_H

#define _RFSPI_H


/********************************************************************************************/
#ifndef _RF_SPI_DEFINE_
#define HEAD_MACRO_RFSPI	extern
#else
#define HEAD_MACRO_RFSPI	  
#endif
/********************************************************************************************/

//global fuctions
/******************************************************************************************/
HEAD_MACRO_RFSPI uchar RF_SPIRead_REG  ( uchar reg );
HEAD_MACRO_RFSPI uchar RF_SPIWrite_REG ( uchar reg, uchar sendData );
HEAD_MACRO_RFSPI void HardwareReset ( void );
HEAD_MACRO_RFSPI void HardwareHalt ( void );
HEAD_MACRO_RFSPI void SetBitMask (uchar reg, uchar mask );
HEAD_MACRO_RFSPI void ClearBitMask (uchar reg, uchar mask );
HEAD_MACRO_RFSPI void CalulateCRC ( uchar *pIndata, uchar len, uchar *pOutData );
HEAD_MACRO_RFSPI void memcpy ( uchar* pdest, uchar *psource, uchar len );
/******************************************************************************************/

#endif 

#endif
