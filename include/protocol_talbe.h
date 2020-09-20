  //this header file contains the I2C.c file's interface function and variables.



#ifndef _PROTOCOL_TABLE_H

#define _PROTOCOL_TABLE_H


/********************************************************************************************/
#ifndef _PROTOCOL_TABLE_DEFINE_
#define HEAD_MACRO_PROTOCOL_TABLE extern
#else
#define HEAD_MACRO_PROTOCOL_TABLE  
#endif
/********************************************************************************************/

//global varialbes
/******************************************************************************************/

/******************************************************************************************/

//global fuctions
/******************************************************************************************/
HEAD_MACRO_PROTOCOL_TABLE uchar ProcessEnergyProtocol ( uchar* pID, PROTOCOL_REG* pReg );
HEAD_MACRO_PROTOCOL_TABLE uchar ProcessEventProtocol ( uint32 ID, PROTOCOL_REG* pReg );
HEAD_MACRO_PROTOCOL_TABLE uchar ProcessParaProtocol ( uint32 ID, PROTOCOL_REG* pReg );
HEAD_MACRO_PROTOCOL_TABLE uchar ProcessVariableProtocol ( uint32 ID, PROTOCOL_REG* pReg );
//����ʹ�� 11
#ifdef DEMAND_ENABLE
	HEAD_MACRO_PROTOCOL_TABLE uchar ProcessDemandProtocol ( uint32 ID, PROTOCOL_REG* pReg );
#endif

//����ʹ�� 11
#if ( (FREEZE_HOUR_EANBLED==1)|| (FREEZE_DAY_EANBLED==1))
	uchar ProcessFreezeProtocol ( uint32 ID, PROTOCOL_REG* pReg );
#endif
/******************************************************************************************/

#endif 

