  //this header file contains the I2C.c file's interface function and variables.
#if (IC_CARD_MODE==RF_CARD)
	
#ifndef _CV520_H

#define _CV520_H


/********************************************************************************************/
#ifndef _CV520_DEFINE_
#define HEAD_MACRO_CV520	extern
#else
#define HEAD_MACRO_CV520	  
#endif
/********************************************************************************************/

//global varialbes
/******************************************************************************************/

/******************************************************************************************/

//global fuctions
/******************************************************************************************/
HEAD_MACRO_CV520 uchar InitCV520 ( void );
HEAD_MACRO_CV520 uchar GetCV520Version ( void );
HEAD_MACRO_CV520 uchar CV520ProcComm ( uchar Command, uchar *pInData, uchar InLenByte, uchar *pOutData, uint16* pOutLenBit);

HEAD_MACRO_CV520 uchar CV520RequestCard ( uchar reqCode, uchar *pTagType );
HEAD_MACRO_CV520 uchar CV520DetectCard ( void );
HEAD_MACRO_CV520 uchar CV520Anticoll ( uchar *pSnr );
HEAD_MACRO_CV520 uchar CV520SelectCard ( void );
HEAD_MACRO_CV520 uchar CV520MFAuthent ( uchar *pkey, uchar *pSnr, uchar block, uchar modeByte );
HEAD_MACRO_CV520 uchar CV520ReadData ( uchar block );
HEAD_MACRO_CV520 uchar CV520WriteData ( uchar block, uchar *pbuf );
HEAD_MACRO_CV520 uchar CV520Halt ( void );
HEAD_MACRO_CV520 uchar CV520ProcessValue ( uchar mode, uchar block, uchar* pValue );
/******************************************************************************************/


#endif 

#endif
