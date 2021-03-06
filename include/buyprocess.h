  //this header file contains the buyprocess.c file's interface function and variables.

#ifndef _BUY_PROCESS_H

#define _BUY_PROCESS_H


/********************************************************************************************/
#ifndef _BUY_PROCESS_DEFINE_
#define HEAD_MACRO_BUY_PROCESS	extern
#else
#define HEAD_MACRO_BUY_PROCESS	  
#endif
/********************************************************************************************/

//global fuctions
/******************************************************************************************/
HEAD_MACRO_BUY_PROCESS uchar ProcessUserBuyQuan ( uchar *pbuf, uchar mode, uchar buyMode );
HEAD_MACRO_BUY_PROCESS void Decrypt ( uchar *buf, uchar pos, uchar len, uchar fixedChar );
HEAD_MACRO_BUY_PROCESS void Encrypt ( uchar *buf, uchar pos, uchar len );
HEAD_MACRO_BUY_PROCESS void ClearAllData ( uint32 preBuy, uint16 price );
/******************************************************************************************/

#endif 