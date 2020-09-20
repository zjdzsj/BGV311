  //this header file contains the RN8302.c file's interface function and variables.

#ifndef _RN8302_H

#define _RN8302_H


/********************************************************************************************/
#ifndef _RN8302_DEFINE_
#define HEAD_MACRO_RN8302	extern
#else
#define HEAD_MACRO_RN8302	  
#endif
/********************************************************************************************/

//global fuctions
/******************************************************************************************/

HEAD_MACRO_RN8302 uchar SetRN8302_Register ( void );

HEAD_MACRO_RN8302 uchar CaliGainPara ( void );
HEAD_MACRO_RN8302 uchar CaliPhasePara ( void );

HEAD_MACRO_RN8302 uchar CaliOffsetPara ( void );

HEAD_MACRO_RN8302 uchar StoreCaliPara ( void );

HEAD_MACRO_RN8302 uchar GetRegLen ( uchar reg, uchar bank );
HEAD_MACRO_RN8302 uchar CheckRN8302Reg ( void );
HEAD_MACRO_RN8302 void ReadFromCL2208 ( void );

HEAD_MACRO_RN8302 void ReadRmsValue ( void );
HEAD_MACRO_RN8302 uint16 GetRN8302ParaAddr ( uchar reg );
HEAD_MACRO_RN8302 uchar GetRN8302CheckSum ( uchar *p );

/******************************************************************************************/

#endif 