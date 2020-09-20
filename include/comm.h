  //this header file contains the I2C.c file's interface function and variables.



#ifndef _COMM_H

#define _COMM_H


/********************************************************************************************/
#ifndef _COMM_DEFINE_
#define HEAD_MACRO_COMM	extern
#else
#define HEAD_MACRO_COMM  
#endif
/********************************************************************************************/

//global varialbes
/******************************************************************************************/

/******************************************************************************************/

//global fuctions
/******************************************************************************************/
HEAD_MACRO_COMM void InitIR ( void );
HEAD_MACRO_COMM void InitRS485 ( void );

HEAD_MACRO_COMM void ResetIRComm ( void );
HEAD_MACRO_COMM void ResetRS485Comm ( void );

HEAD_MACRO_COMM void ProcessCommRxdOrTxd ( uchar channel );
HEAD_MACRO_COMM uchar CheckCommMessage ( uchar channel, uchar rxdlen );
HEAD_MACRO_COMM uchar DecodeIRMessage ( uchar channel );
HEAD_MACRO_COMM uchar BuildIRMessage( uchar channel );

#ifdef BONUS_PADI_ENABLE
	HEAD_MACRO_COMM void StoreBonusBuyHistroy ( void );
#endif

HEAD_MACRO_COMM uchar GetChecksum ( uchar* buf, uchar len );

#if(IC_CARD_MODE==PLC_COMM_USED)
	HEAD_MACRO_COMM void InitPLCComm ( void );
	HEAD_MACRO_COMM void ResetPLCComm ( void );
#endif	

/******************************************************************************************/

#endif 

