/*
  -this file contains declarations of functions used in Common.c
*/

/*****************************************************************************************/
#ifndef COMMON_H

#define COMMON_H

/******************************************************************************************/
#ifndef _COMMON_DEFINE_
#define HEAD_MACRO_COMMON	extern
#else	
#define HEAD_MACRO_COMMON                      	 
#endif
/******************************************************************************************/

//variables definition
/*****************************************************************************************/

/*****************************************************************************************/

//function definition or declaration
/*****************************************************************************************/
HEAD_MACRO_COMMON void InitHardware ( void );

//HEAD_MACRO_COMMON void InitMemAndClock ( void );
//HEAD_MACRO_COMMON void InitEXLVI ( void );

HEAD_MACRO_COMMON void VeryfyPins ( void );
HEAD_MACRO_COMMON uchar CopyRamData ( uchar *des, uchar *src,uchar len );

HEAD_MACRO_COMMON void ProcessSecTaskFlags ( void );

HEAD_MACRO_COMMON uint32 Hex32ToBCD( uint32 a );
HEAD_MACRO_COMMON uint32 Hex16ToBCD( uint16 a );
HEAD_MACRO_COMMON uchar Hex8ToBCD( uchar a );
HEAD_MACRO_COMMON uint32 BCDToHex( uchar *p, uchar len );

#ifdef PRE_PAID_ENABLE			
	HEAD_MACRO_COMMON void ProcessPrice( void );
#endif

HEAD_MACRO_COMMON uchar GetSum ( const uchar *p, uchar len );

HEAD_MACRO_COMMON uint16 GetCrc16 ( const uchar *p, uchar len );

HEAD_MACRO_COMMON void InitPins ( void );
HEAD_MACRO_COMMON void ReadMeterParameter1 ( void );
HEAD_MACRO_COMMON void ReadMeterParameter2 ( void );
HEAD_MACRO_COMMON void StoreMeterPara1ToE2ROM ( void );
HEAD_MACRO_COMMON void StoreMeterPara2ToE2ROM ( void );
HEAD_MACRO_COMMON uchar StoreToE2ROM ( uint16 suba, uchar*buf,uchar len );
HEAD_MACRO_COMMON void ProcessRelay ( void );


HEAD_MACRO_COMMON void InitGlobalVariable ( void );

HEAD_MACRO_COMMON uchar Hex8ToBCD( uchar a );

HEAD_MACRO_COMMON uchar CompareEXLVI ( void );

HEAD_MACRO_COMMON void ReadMeterParameter3 ( void );
HEAD_MACRO_COMMON void StoreMeterPara3ToE2ROM ( void );

HEAD_MACRO_COMMON uchar CompareByteData ( uchar *psrc1, uchar *psrc2,uchar len);
HEAD_MACRO_COMMON uchar StoreToE2ROMAndBackup ( uint16 suba, uchar*buf,uchar len );
HEAD_MACRO_COMMON uchar ReloadFromE2ROMAndBackup ( uint16 suba, uchar*buf,uchar flag,uchar len );

HEAD_MACRO_COMMON void ProcessLowPower ( void );

HEAD_MACRO_COMMON void ProcessHalfSecTaskFlags ( void );
HEAD_MACRO_COMMON void ProcessRelayStatus ( void );
#ifdef RTC_8025T
HEAD_MACRO_COMMON void GetTime ( void );
HEAD_MACRO_COMMON uchar SetTime ( uchar* pbuf );
#endif

#if(METER_CHIP_MODEL==0x2208)
	HEAD_MACRO_COMMON uint16 GetCl2208CheckSum ( uint16 *p,uchar len );
#endif

/*****************************************************************************************/


#endif 