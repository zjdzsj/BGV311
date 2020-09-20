  //this header file contains the I2C.c file's interface function and variables.

#ifndef _CL2208_H

#define _CL2208_H


/********************************************************************************************/
#ifndef _CL2208_DEFINE_
#define HEAD_MACRO_CL2208	extern
#else
#define HEAD_MACRO_CL2208	  
#endif
/********************************************************************************************/

//global fuctions
/******************************************************************************************/

HEAD_MACRO_CL2208 uchar CalibrateCl2208 ( void );

HEAD_MACRO_CL2208 uchar CaliGainPara ( uchar *p );
HEAD_MACRO_CL2208 uchar CaliPhasePara ( uchar *p );
HEAD_MACRO_CL2208 uchar CaliOffsetPara ( uchar *p );
HEAD_MACRO_CL2208 uchar CaliIARMSPara ( uchar *p );
HEAD_MACRO_CL2208 uchar GetRegLen ( uchar reg );
HEAD_MACRO_CL2208 uchar CheckCl2208Reg ( void );
HEAD_MACRO_CL2208 void ReadFromCL2208 ( void );

HEAD_MACRO_CL2208 void ReadRmsValue ( void );
HEAD_MACRO_CL2208 uint16 GetCl2208ParaAddr ( uchar reg );
/******************************************************************************************/

#endif 