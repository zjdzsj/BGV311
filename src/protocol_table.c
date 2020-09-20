/******************************************************************************************
*  Copyright (c) 2013,  R&D Department
*  All rights reserved 
*
*  FileName��   	protocol.c
*  Author��     	Roger
*  Date��  			2013-3-8 20:10
*  Revised Date��  	
*  Main Function��  this file contains all the functions used in I2C communications.
*  Version:			V1.0
*  Note�� 			 			
*********************************************************************************************/
#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

#define _PROTOCOL_TALBE_DEFINE_
#include "include.h"


//variables statement
/******************************************************************************************/

    
/******************************************************************************************/

//function statement
/******************************************************************************************/
//���������ݱ�ʶ 00
//4�ֽ�BCD��ʽ��ֻ������ǰ����RAM�У������շ���EEPROM��, 11
//����ֻ֧���ܵ��������������������Ĭ��Ϊһ�� 11
static const ENERGY_DEMAND_TABLE_ELEMENT energy_protocol_table[]={
	//ID2  	ramAddr 							extendAddr	
#ifdef RTC_8025T	
	0x00,	(unsigned int)&g_energy.Accumulate,	offsetof(EEPROM_DATA,Last1_A1),	//(��ǰ)����й��ܵ��� 11
	0x01,	(unsigned int)&g_energy.Accumulate,	offsetof(EEPROM_DATA,Last1_A1),	//(��ǰ)�����й��ܵ��� 11
#else
	0x00,	(unsigned int)&g_energy.Accumulate,	0,	//(��ǰ)����й��ܵ��� 11
	0x01,	(unsigned int)&g_energy.Accumulate,	0,	//(��ǰ)�����й��ܵ��� 11
#endif	
	0x02,	(unsigned int)&g_energy.NegAccumulate,offsetof(EEPROM_DATA,NegAccumulate),	//(��ǰ)�����й��ܵ��� 11
	
	//�޹����� 11
#ifdef INACITVE_POWER_ENABLE	
	0x03,	(unsigned int)&g_Inactivenergy.PosInactiveAcc,offsetof(EEPROM_DATA,PosInactiveAcc),	//(��ǰ)�����޹��ܵ��� 11
	0x04,	(unsigned int)&g_Inactivenergy.NegInactiveAcc,offsetof(EEPROM_DATA,NegInactiveAcc),	//(��ǰ)�����޹��ܵ��� 11
#endif
	

	
	0x0F,	(unsigned int)&g_energy.CurrentQ,	offsetof(EEPROM_DATA,CurrentQ),		// ��չ��ǰ���õ��� 11
	0x90,	(unsigned int)&g_energy.Balance,	offsetof(EEPROM_DATA,Balance),		//(��ǰ��ʣ���� 11
#ifdef BONUS_PADI_ENABLE
	0x91,	(unsigned int)&g_energy.BonusBalance,offsetof(EEPROM_DATA,BonusBalance),	//����ǰ��ʣ�ಹ����� 4 11
#endif	
};

//����ʹ�� 11
#ifdef DEMAND_ENABLE
//�������������ʱ�� 01
static const EVENT_TABLE demand_protocol_table[]={
	//ID   		size type  		readOrWrite  location  	bcdCheck 	extendAddr
	0x01010000,	8,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,CurDemand),	//(��ǰ)�����й����������������ʱ�� 11
	0x01020000,	8,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,CurDemand)+8,	//(��ǰ)�����й����������������ʱ�� 11
	//�޹����� û�� ���治��  11
#ifdef INACITVE_POWER_ENABLE
	0x01050000,	8,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,CurDemand)+18,	//(��ǰ)��1�����޹������������ ����ʱ�� 11
	0x01060000,	8,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,CurDemand)+12,	//(��ǰ)��2�����޹������������ ����ʱ�� 11  4 11
	0x01070000,	8,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,CurDemand)+16,	//(��ǰ)��3�����޹������������ ����ʱ�� 11
	0x01080000,	8,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,CurDemand)+20,	//(��ǰ)��4�����޹������������ ����ʱ�� 11		
#endif

};	
#endif

//�������� 02
static const VARIABLE_TABLE variable_protocol_table[]={
#if(SINGLE_OR_THREE_METER==1)
	//ID   		size type  		readOrWrite  location  	bcdCheck 	ramAddr
	0x02010100,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip.URms,	//A���ѹ 2	 11
	0x02020100,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip.IRms,	//A����� 3 11
	0x02030000,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip.PRms,	//���๦�� 3			//���� 11
	0x02030100,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip.PRms,	//A�๦�� 3	 11
#endif

#if(SINGLE_OR_THREE_METER==3)
	//ID   		size type  		readOrWrite  location  	bcdCheck 	ramAddr
	0x02010100,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[0].URms,	//A���ѹ 2	 11
	0x02010200,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[1].URms,	//B���ѹ 2	 11
	0x02010300,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[2].URms,	//B���ѹ 2	 11
	0x02020100,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[0].IRms,	//A����� 3 11
	0x02020200,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[1].IRms,	//B����� 3  11
	0x02020300,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[2].IRms,	//C����� 3 11
	0x02030000,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[3].PRms,	//���๦�� 3			//���� 11
	0x02030100,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[0].PRms,	//A�๦�� 3	 11
	0x02030200,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[1].PRms,	//B�๦�� 3	 11
	0x02030300,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[2].PRms,	//C�๦�� 3  11	
#endif

#ifdef INACITVE_POWER_ENABLE
	0x02040000,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[3].QRms,	//�����޹����� 3			//���� 11
	0x02040100,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[0].QRms,	//A���޹����� 3	 11
	0x02040200,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[1].QRms,	//B���޹����� 3	 11
	0x02040300,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[2].QRms,	//C���޹����� 3  11	
	
	0x02050000,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[3].SRms,	//�������ڹ��� 3			//���� 11
	0x02050100,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[0].SRms,	//A�����ڹ��� 3	 11
	0x02050200,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[1].SRms,	//B�����ڹ��� 3	 11
	0x02050300,	3,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[2].SRms,	//C�����ڹ��� 3  11	
#endif	

#if(SINGLE_OR_THREE_METER==1)		
	0x02060000,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip.Factor,	//���๦������ 2  11	
	0x02060100,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip.Factor,	//A�๦������ 2  11	
#endif

#if(SINGLE_OR_THREE_METER==3)		
	0x02060000,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[3].Factor,	//���๦������ 2  11	
	0x02060100,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[0].Factor,	//A�๦������ 2  11	
	0x02060200,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[1].Factor,	//B�๦������ 2  11	
	0x02060300,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[2].Factor,	//C�๦������ 2  11	
	0x02800002,	2,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_freqency,	//����Ƶ�� 2	 11
#endif
	

	0x02800020,	4,	 Hex4To4,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_price,	//��ǰ���� 4 11

#if(SINGLE_OR_THREE_METER==1)	
	0x0280002E,	4,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip.URms,	//1��ָ��� 4 11
#endif

#if(SINGLE_OR_THREE_METER==3)	
	0x0280002E,	4,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[0].URms,	//1��ָ��� 4 11
//	0x02F00001,	4,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[0].URms,		//1��ָ��� 4 11
//	0x02F00002,	4,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[1].URms,		//1��ָ��� 4 11
//	0x02F00003,	4,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[2].URms,		//1��ָ��� 4 11
//	0x02F00004,	4,	 BCDType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_uip[3].URms,		//1��ָ��� 4 11
#endif


};


//�¼���¼ 03 11
static const EVENT_TABLE event_protocol_table[]={
	//ID   		size type  		readOrWrite  location  	bcdCheck 	extendAddr	
	0x03110000,	3,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,TotalDropPowerCnt),	//�����ܴ��� 3 11
	0x03110001,	12,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1DropPower),		//(��1��)���緢��ʱ�̣�����ʱ�� 11
	
	0x03300100,	3,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,TotalMeterClearCnt),	//���������ܴ��� 3 11
	0x03300101,	18,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1MeterClear),		//(��1��)���������¼ 11

	0x03330101,	5,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1BuyDate),	//��1�ι������� 5 11
	0x03330201,	2,	 BCDType,	R_Enable,	 L_EEPROM,		BCDCheck_D,	offsetof(EEPROM_DATA,Last1_BuyCounter),	//��1�ι�����ܹ������  2 11
	
	0x03330203,	4,	 Hex4To4,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,PrePaid),	//Ԥ�ý��  4 11
	
	0x03330301,	4,	 BCDType,	R_Enable,	 L_EEPROM,		BCDCheck_D,	offsetof(EEPROM_DATA,Last1_BuyQuan),	//��1�ι��繺����  4 11
			
	0x03330401,	4,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1_BuyPreQuan),	//��1�ι���ǰʣ����  4 11
	0x03330501,	4,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1_BuyAfterQuan),	//��1�ι����ʣ����  4 11
	0x03330601,	4,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1_TotalBuyQuan),	//��1�ι�����ۼƹ�����  4 11
	
	//0x03340201,	2,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1_BuyCounter),	   //��ǰ�������  2 11
	0x03340301,	4,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1_ReturnQuan),	//���1���˷ѽ��  4 11
		
	0x03350201,	2,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1_BonusCounter),	//��1�β������ܲ�������  2 11
	0x03350301,	4,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1_BonusQuan),	//��1�β������  4 11
	//0x03350401,	4,	 Hex4To4,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1_ReturnBonusQuan),	//���1�β����˷ѽ��  4 11

//����Խ�� 11
#if(OVER_POWER_EANBLED==1)	
	0x030F0000,	6,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,OverPowerCnt),	//������ܴ��������ۼ�ʱ�� 6 11
	0x030F0101,	20,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,OverPowerLast1),		//����1�� ����ؼ�¼ 11
#endif

#if(LOSS_PHASE_EANBLED==1)	
	0x13010000,	3,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,PhaseALossCnt),	//A������ܴ��� 3 11
	0x13010101,	3,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,PhaseALossMinute),	//A��������ۼ�ʱ�� 3 11
	

	0x13010002,	3,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,PhaseBLossCnt),	//B������ܴ��� 3 11
	0x13010003,	3,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,PhaseBLossMinute),	//B��������ۼ�ʱ�� 3 11
	
	
	0x13010004,	3,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,PhaseCLossCnt),	//C������ܴ��� 3 11
	0x13010005,	3,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,PhaseCLossMinute),	//C��������ۼ�ʱ�� 3 11
	
	0x13010101,	14,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,PhaseALossLast1Record),		//(��1��)A����� 11
	0x13020101,	14,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,PhaseBLossLast1Record),		//(��1��)B����� 11
	0x13030101,	14,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,PhaseCLossLast1Record),		//(��1��)C����� 11
#endif

};	

//�α������� 04
static const PARAMETER_TABLE parameter_protocol_table[]={
	//ID   		size type  		readOrWrite  location  bcdCheck  		ramAddr  						extendAddr
	0x04000101,	4,	 BCDType,	WR_Enable,	 L_RAM_E2P,BCDCheck_E,(uint16)&g_date.week,			offsetof(EEPROM_DATA,WEEK),			//���ڼ����� 4�ֽ� 11
	0x04000102,	3,	 BCDType,	WR_Enable,	 L_RAM_E2P,BCDCheck_E,(uint16)&g_date.sec,			offsetof(EEPROM_DATA,SECOND),			//ʱ�� 3�ֽ� 11

#ifdef DEMAND_ENABLE
	0x04000103,	1,	 HexType,	WR_Enable,	 L_RAM_E2P,BCDCheck_ECS,(uint16)&g_demand.DemandCycle,			offsetof(EEPROM_DATA,SECOND),			//ʱ�� 3�ֽ� 11
	0x04000104,	1,	 HexType,	WR_Enable,	 L_RAM_E2P,BCDCheck_ECS,(uint16)&g_demand.DemandSlide,			offsetof(EEPROM_DATA,SECOND),			//ʱ�� 3�ֽ� 11
#endif
	
#ifdef TARIFF_ENABLE	
	0x04000201,	1,	 BCDType,	WR_Enable,	 L_EEPROM, BCDCheck_ECS,(uint16)0,			        offsetof(EEPROM_DATA,YearZoneNum),			//��ʱ���� 1�ֽ� 11
	0x04000202,	1,	 BCDType,	WR_Enable,	 L_EEPROM, BCDCheck_ECS,(uint16)0,			        offsetof(EEPROM_DATA,TimeZoneNum),			//ʱ�α��� 1�ֽ� 11
	0x04000203,	1,	 BCDType,	WR_Enable,	 L_EEPROM, BCDCheck_ECS,(uint16)0,			        offsetof(EEPROM_DATA,DayTalbeNum),			//��ʱ�α��� 1�ֽ� 11
#endif	
	0x04000306,	3,	 Hex2To4_0,	R_Enable,	 L_RAM_E2P,BCDCheck, (uint16)&g_para.CurrentRatio,	offsetof(EEPROM_DATA,CurrentRatio),//�������б� 3�ֽ� 11
//	0x04000307,	3,	 Hex2To4_0,	R_Enable,	 L_RAM_E2P,BCDCheck, (uint16)&g_para.VoltageRatio,	offsetof(EEPROM_DATA,VoltageRatio),//��ѹ���б� 3�ֽ� 11
    	
	0x04000401,	6,	 BCDType,	WR_Enable,	 L_RAM_E2P,BCDCheck_ECS, (uint16)&g_addr.MeterAddr[0],	offsetof(EEPROM_DATA,Zone),	//ͨѶ��ַ 11
	0x04000503,	2,	 HexType,	R_Enable,	 L_RAM,	   BCDCheck_ECS, (uint16)&g_meterStatus3.Byte,	0,							//�����״̬�� 3 11
	0x04001001,	4,	 Hex2To4,	WR_Enable,	 L_RAM_E2P,BCDCheck_ECS, (uint16)&g_buyPara.Alarm1,		offsetof(EEPROM_DATA,Alarm1),		//�������1 2�ֽڴ���4�ֽ� 11
	0x04001002,	4,	 Hex2To4,	WR_Enable,	 L_RAM_E2P,BCDCheck_ECS, (uint16)&g_buyPara.Alarm2,		offsetof(EEPROM_DATA,Alarm2),		//�������2 2�ֽڴ���4�ֽ� 11
	0x04001003,	4,	 Hex2To4_0,	WR_Enable,	 L_RAM_E2P,BCDCheck_ECS, (uint16)&g_para.CreditLimit,	offsetof(EEPROM_DATA,CreditLimit),//͸֧�����ֵ 4�ֽ� 11
	0x04001004,	4,	 Hex2To4,	WR_Enable,	 L_RAM_E2P,BCDCheck_ECS, (uint16)&g_para.MaximBalance,	offsetof(EEPROM_DATA,MaximBalance),//�ڻ������ֵ 4�ֽ� 11
	
#ifdef TARIFF_ENABLE	
	0x04010000,	3,	 BCDType,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,MMDDY1),//ʱ���� 24=3*8�ֽ� 11
	0x04010001,	3,	 BCDType,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,HHMMNN1),//ʱ�α� 24=3*8�ֽ� 11
	
	0x04050101,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,TariffPrice1),//���ʵ��1 4�ֽ� 11
	0x04050102,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,TariffPrice2),//���ʵ��2 4�ֽ� 11
	0x04050103,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,TariffPrice3),//���ʵ��3 4�ֽ� 11
	0x04050104,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,TariffPrice4),//���ʵ��4 4�ֽ� 11
	0x04050105,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,TariffPrice5),//���ʵ��5 4�ֽ� 11
	0x04050106,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,TariffPrice6),//���ʵ��6 4�ֽ� 11
#endif

#ifdef LADDER_ENABLE	
	0x04060001,	4,	 Hex2To3_1,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,Ladder1),//����ֵ1 2�ֽ� 11
	0x04060002,	4,	 Hex2To3_1,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,Ladder2),//����ֵ2 2�ֽ� 11
	0x04060003,	4,	 Hex2To3_1,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,Ladder3),//����ֵ3 2�ֽ� 11
	0x04060004,	4,	 Hex2To3_1,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,Ladder4),//����ֵ4 2�ֽ� 11
	0x04060005,	4,	 Hex2To3_1,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,Ladder5),//����ֵ5 2�ֽ� 11
	
	0x04060101,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,LadderPrice1),//���ݵ��1 4�ֽ� 11
	0x04060102,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,LadderPrice2),//���ݵ��1 4�ֽ� 11
	0x04060103,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,LadderPrice3),//���ݵ��1 4�ֽ� 11
	0x04060104,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,LadderPrice4),//���ݵ��1 4�ֽ� 11
	0x04060105,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,LadderPrice5),//���ݵ��1 4�ֽ� 11
	0x04060106,	4,	 Hex2To4,	WR_Enable,	 L_EEPROM, BCDCheck_ECS, (uint16)0,					offsetof(EEPROM_DATA,LadderPrice6),//���ݵ��1 4�ֽ� 11

	0x04060f01,	2,	 BCDType,	WR_Enable,	 L_EEPROM, BCDCheck_N, (uint16)0,					offsetof(EEPROM_DATA,YYMM),//�����л�����1 2�ֽ� 11
	0x04060f02,	3,	 BCDType,	WR_Enable,	 L_EEPROM, BCDCheck_N, (uint16)0,					offsetof(EEPROM_DATA,LADDER_BASE_YYMMDD),//���ݻ�׼�л�ʱ�� 3�ֽ� 11
	0x04060f03,	3,	 BCDType,	WR_Enable,	 L_EEPROM, BCDCheck_N, (uint16)0,					offsetof(EEPROM_DATA,LADDER_YYMMDD),//�����´��л�ʱ�� 3�ֽ� 11
#endif
	
	//ID   		size type  		readOrWrite  location  bcdCheck  		ramAddr  						extendAddr
	0x04090E10,	4,	 Hex2To4_0,	WR_Enable,	 L_RAM_E2P, BCDCheck_ECS, (uint16)&g_buyPara.FactorLimit,	offsetof(EEPROM_DATA,FactorLimit),//����������ֵ 4�ֽ� 11
	0x04090E11,	4,	 Hex2To4_0,	WR_Enable,	 L_RAM_E2P, BCDCheck_ECS, (uint16)&g_buyPara.FactorPower,	offsetof(EEPROM_DATA,FactorPower),//����������Ӧ������ֵ 4�ֽ� 11
	
	//��չָ��
	//ID   		size type  		readOrWrite  location  	bcdCheck 	ramAddr  					extendAddr
	0x04500002,	1,	 HexType,	R_Enable,	 L_RAM_E2P,	BCDCheck_N,	(uint16)&g_buyPara.Status,	offsetof(EEPROM_DATA,Status),//��״̬ 1 11
	0x04500003,	1,	 HexType,	R_Enable,	 L_RAM,		BCDCheck_N,	(uint16)&g_errStatus.Byte,	0,				//������״̬�� 1 11
//	0x04500004,	1,	 HexType,	R_Enable,	 L_RAM_E2P,	BCDCheck_N,	(uint16)&g_buyPara.AlarmSet,offsetof(EEPROM_DATA,AlarmSet),//���ñ���2 1 11
	0x04500005,	4,	 Hex2To4_0,	WR_Enable,	 L_RAM_E2P,	BCDCheck_ECS,(uint16)&g_para.LimitPower,offsetof(EEPROM_DATA,LimitPower),//�޵繦�� 2 11
	
	0x04500006,	1,	 HexType,	R_Enable,	 L_RAM_E2P,	BCDCheck_N,	(uint16)&g_energy.TariffNum,offsetof(EEPROM_DATA,TariffNum),	//��ǰ���� 1 11

#if(METER_CHIP_MODEL==0x2208)
	0x04500009,	4,	 HexType,	R_Enable,	 L_RAM_E2P,	BCDCheck_N,	(uint16)&g_factor.PFactor	,offsetof(EEPROM_DATA,PFactor),	//����ϵ��  4 11
	0x0450000A,	4,	 HexType,	R_Enable,	 L_RAM_E2P,	BCDCheck_N,	(uint16)&g_factor.UFactor	,offsetof(EEPROM_DATA,UFactor),	//��ѹϵ��  4 11
	0x0450000B,	4,	 HexType,	R_Enable,	 L_RAM_E2P,	BCDCheck_N,	(uint16)&g_factor.IFactor	,offsetof(EEPROM_DATA,IFactor),	//����ϵ��  4 11
#endif
	0x0450000C,	2,	 HexType,	R_Enable,	 L_RAM_E2P,	BCDCheck_N,	(uint16)&g_addr.Zone	,	0,	//���������  2 11
	0x0450000D,	6,	 BCDType,	R_Enable,	 L_RAM_E2P,	BCDCheck_N,	(uint16)&g_addr.CardNum[0]	,	0,	//�����Ӧ����  6 11
	0x0450000E,	4,	 Hex4To4,	R_Enable,	 L_RAM_E2P,	BCDCheck_N,	(uint16)&g_energy.CurrentQ,	offsetof(EEPROM_DATA,CurrentQ),	//�����ۼ��õ���  4 11
	
	//ID   		size type  		readOrWrite  location  	bcdCheck 	ramAddr  		extendAddr
	0x04800001,	8,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_N,	(uint16)0,		offsetof(EEPROM_DATA,SoftVersion),	//�����汾�� 11
	
	//ID   		size type  		readOrWrite  location  	bcdCheck 	ramAddr  					extendAddr
	0x04900101,	1,	 HexType,	WR_Enable,	 L_RAM_E2P,	BCDCheck_ECS,	(uint16)&g_energy.BonusMode,offsetof(EEPROM_DATA,BonusMode),	//��������ۼ�ģʽ�� 11
	0x04900102,	1,	 HexType,	WR_Enable,	 L_RAM_E2P,	BCDCheck_ECS,	(uint16)&g_para.BonusMoneyNum,offsetof(EEPROM_DATA,BonusMoneyNum),	//���������Ч���ģʽ�� 11
	
	0x04900103,	2,	 Hex2To2,	WR_Enable,	 L_RAM_E2P,	BCDCheck_ECS,	(uint16)&g_buyPara.BuyCount,offsetof(EEPROM_DATA,BuyCount),
	0x04900104,	4,	 Hex4To4,	WR_Enable,	 L_RAM_E2P,	BCDCheck_ECS,	(uint16)&g_buyPara.BuyQuan,offsetof(EEPROM_DATA,BuyQuan),
	0x04900105,	1,	 HexType,	WR_Enable,	 L_RAM_E2P,	BCDCheck_ECS,	(uint16)&g_para.MeterTpye, offsetof(EEPROM_DATA,MeterTpye),
};	


#if ( (FREEZE_HOUR_EANBLED==1)|| (FREEZE_DAY_EANBLED==1))
//�������� 05
static const EVENT_TABLE freeze_protocol_table[]={
	//ID   		size type  		readOrWrite  location  	bcdCheck 	extendAddr
#if ( FREEZE_HOUR_EANBLED==1)
	0x0504FF01,	9,	 HexType,	R_Enable,	 L_RAM_E2P,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1FreezeTime),	//����1�Σ����㶳��ʱ�� 5 11
	//0x05040101,	4,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_D,	offsetof(EEPROM_DATA,Last1FreezeActive),	//����1�Σ����㶳�������й��ܵ��� 4 11
#endif

#if (FREEZE_DAY_EANBLED==1) 
	0x05060101,	4,	 BCDType,	R_Enable,	 L_EEPROM,	BCDCheck_N,	offsetof(EEPROM_DATA,LastDay1FreezeActive),	//����1�죩���������й��ܵ��� 4 11
#endif

};
#endif
//���ɼ�¼���� 06

//��ȫ��֤ר�ö����� 07

/******************************************************************************************/



/*
*****************************************************************************************
* fuction name: ProcessEnergyProtocol
*
* Description : 
*
* Arguments   : 
*
* returns     : uchar
*
* Notes       : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar ProcessEnergyProtocol ( uchar* pID, PROTOCOL_REG* pReg )
{	
	uchar startelem,endelem,i,j;
	
	//���������ж� 11
	if ( (pID[0] > 0x0c) && (pID[0] != 0xff) )
  		return 1;
	//������������4����,�Ǳ�׼��Լ��ʣ�������ʣ����Ⱦ�С��4 11
	if( pID[1] > 0x04 )		
		return 1;
	
	//The table of registers is ordered, and a binary search can be performed.
  	//The variable startelem points to the beginning of the table, and endelem
  	//points to the end. As the table is searched, these variables are updated.
 	// If the register is not found, dummy register FFFF is returned.
  	startelem = 0;
  	endelem = sizeof(energy_protocol_table) / sizeof(energy_protocol_table[0]);
 	i = 0;

 	while(1)
  	{
   		j = (endelem + startelem) / 2;
		if(i != j)
		{
  			i = j;
 			if(energy_protocol_table[i].ID2 == pID[2]) break;
 			if(energy_protocol_table[i].ID2 < pID[2]) startelem = i; else endelem = i;
		}
		else
		{
  			i = sizeof(energy_protocol_table) / sizeof(energy_protocol_table[0]);
  			i -= 1;
 		 	break;
		}
  	}
  	
  	if( energy_protocol_table[i].ID2 != pID[2] )
  		return 1;	
  	
  	//
  	pReg->size = 4+4;
  	pReg->type = Hex4To4;
  	pReg->readOrWrite = R_Enable; 	
  	pReg->CheckType = BCDCheck;
  	pReg->keyOrNot = KEY_NOT;
  	
  	//λ���ж� 11
  	if (  pID[0] == 0x00 )	//��ǰ 11
  	{
  		pReg->location = L_RAM;	
  		//���ݿ� 11
  		if ( pID[1] == 0xff )
  		{
  			pReg->addr = energy_protocol_table[i].ramAddr;
  			pReg->size = 4+20;				
  		}
  		else
  			pReg->addr = energy_protocol_table[i].ramAddr+4*pID[1];  	 		   		
  	}
  	else	//��12�µ��� 11
  	{ 		
  		pReg->location = L_EEPROM;	
  		//���ݿ� 11
  		if ( pID[1] != 0x00 )
  		{
  			if ( pID[1] == 0xff )
  			{
  				pReg->addr = energy_protocol_table[i].extendAddr+4*(pID[0]-1);
  				pReg->size = 4+20;	
  				pReg->eAddr = pReg->addr;
  			}
  			else
  			{
  				pReg->addr = energy_protocol_table[i].extendAddr+4*(pID[1]-1)+20*(pID[0]-1);	 					
  				pReg->eAddr = pReg->addr;
  			}			
  		}
  		else
  		{
  			pReg->addr = energy_protocol_table[i].extendAddr+20*(pID[0]-1);
  			pReg->eAddr = pReg->addr;
  		}   		
  	}
  	
  	
  				
  	//�˴�Э�������⴦�� ����ǰ��ʣ����/͸֧��� 11
  	if (  pID[2] == 0x90 )
  	{
  		//Ŀǰֻ֧��ʣ�����͸֧��� 11
  		if ( pID[1] != 0x02 )
  			return 1;
  		if ( pID[0] >= 0x02 )
  			return 1;
  			
  		pReg->location = L_RAM;	
  		pReg->addr = energy_protocol_table[i].ramAddr+4*pID[0];
  	}	
#ifdef BONUS_PADI_ENABLE
  	else if (  pID[2] == 0x91 )
  	{
  		//Ŀǰֻ֧�ֲ���ʣ���� 11
  		if ( pID[1] != 0x01 )
  			return 1;
  		if ( pID[0] != 0x01 )
  			return 1;
  			
  		pReg->location = L_RAM;	
  		pReg->addr = energy_protocol_table[i].ramAddr;
  	}	
#endif
	   	
	return 0;		
	

}

//����ʹ�� 11
#ifdef DEMAND_ENABLE
/*
*****************************************************************************************
* fuction name: ProcessDemandProtocol
*
* Description : 
*
* Arguments   : 
*
* returns     : uchar
*
* Notes       : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar ProcessDemandProtocol ( uint32 ID, PROTOCOL_REG* pReg )
{	
	uchar startelem,endelem,i,j;	
	uchar offset;
	
	//���������ж� 11
	if ( (ID<0x01010000) || (ID>0x0109FFFF) )
  		return 1;
	offset = (uchar)(ID&0x000000FF);
	if ( offset >= 0x0d )
	    return 1;
	ID -= offset;
		
	//The table of registers is ordered, and a binary search can be performed.
  	//The variable startelem points to the beginning of the table, and endelem
  	//points to the end. As the table is searched, these variables are updated.
 	// If the register is not found, dummy register FFFF is returned.
  	startelem = 0;
  	endelem = sizeof(demand_protocol_table) / sizeof(demand_protocol_table[0]);
 	i = 0;
 	while(1)
  	{
   		j = (endelem + startelem) / 2;
		if(i != j)
		{
  			i = j;
 			if(demand_protocol_table[i].ID == ID) break;
 			if(demand_protocol_table[i].ID < ID) startelem = i; else endelem = i;
		}
		else
		{
  			i = sizeof(demand_protocol_table) / sizeof(demand_protocol_table[0]);
  			i -= 1;
 		 	break;
		}
  	}
  	
  	//��һ���жϱ�ʶ���Ƿ���ͬ 11
  	if( demand_protocol_table[i].ID != ID )
  		return 1;	
  	
  	//
  	pReg->size = demand_protocol_table[i].size+4;
  	pReg->type = demand_protocol_table[i].type;
  	pReg->readOrWrite = demand_protocol_table[i].readOrWrite;
  	pReg->keyOrNot = KEY_NOT;
  	pReg->location = demand_protocol_table[i].location;
  	
  	pReg->eAddr = demand_protocol_table[i].extendAddr; 	
	if ( offset> 0 )		//������ʷ��¼ 11
  		pReg->eAddr = demand_protocol_table[i].extendAddr+offset*16;
  			
	return 0;			
}

#endif

/*
*****************************************************************************************
* fuction name: ProcessVariableProtocol
*
* Description : 
*
* Arguments   : 
*
* returns     : uchar
*
* Notes       : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar ProcessVariableProtocol ( uint32 ID, PROTOCOL_REG* pReg )
{	
	uchar startelem,endelem,i,j;
	
	//���������ж� 11
	if ( (ID<0x02000000) || (ID>0x02ffffff) )
  		return 1;
	
	//The table of registers is ordered, and a binary search can be performed.
  	//The variable startelem points to the beginning of the table, and endelem
  	//points to the end. As the table is searched, these variables are updated.
 	// If the register is not found, dummy register FFFF is returned.
  	startelem = 0;
  	endelem = sizeof(variable_protocol_table) / sizeof(variable_protocol_table[0]);
 	i = 0;
 	while(1)
  	{
   		j = (endelem + startelem) / 2;
		if(i != j)
		{
  			i = j;
 			if(variable_protocol_table[i].ID == ID) break;
 			if(variable_protocol_table[i].ID < ID) startelem = i; else endelem = i;
		}
		else
		{
  			i = sizeof(variable_protocol_table) / sizeof(variable_protocol_table[0]);
  			i -= 1;
 		 	break;
		}
  	}
  	
  	//��һ���жϱ�ʶ���Ƿ���ͬ 11
  	if( variable_protocol_table[i].ID != ID )
  		return 1;	
  	
  	//
  	pReg->size = variable_protocol_table[i].size+4;
  	pReg->type = variable_protocol_table[i].type;
  	pReg->readOrWrite = variable_protocol_table[i].readOrWrite;
  	pReg->location = variable_protocol_table[i].location;
  	pReg->checksumOrNot = variable_protocol_table[i].bcdCheck;
  	pReg->addr = variable_protocol_table[i].ramAddr;
  	
  	if ( 0x0280002E == ID )
  	    pReg->size = 40+4;  
// 	else if ( (ID==0x02F00001) || (ID==0x02F00002) || (ID==0x02F00003))
//  	    pReg->size = 12+4;  
//  	else if ( (ID==0x02F00004))
//  		pReg->size = 5+4; 
  		 		    	
	return 0;		
	

}

/*
*****************************************************************************************
* fuction name: ProcessEventProtocol
*
* Description : 
*
* Arguments   : 
*
* returns     : uchar
*
* Notes       : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar ProcessEventProtocol ( uint32 ID, PROTOCOL_REG* pReg )
{	
	uchar startelem,endelem,i,j;	
	uchar offset;
	
  	if ( ((ID&0x000000FF) > 0) && 
  			((ID<0x03FFFFFF)||(ID>0x1301000F) ) )  		
		offset = (uchar)(ID&0x000000FF) - 1;		
	else
		offset = 0;	
	if ( offset >= 0x0A )
	    return 1;
	ID -= offset;
		
	//The table of registers is ordered, and a binary search can be performed.
  	//The variable startelem points to the beginning of the table, and endelem
  	//points to the end. As the table is searched, these variables are updated.
 	// If the register is not found, dummy register FFFF is returned.
  	startelem = 0;
  	endelem = sizeof(event_protocol_table) / sizeof(event_protocol_table[0]);
 	i = 0;
 	while(1)
  	{
   		j = (endelem + startelem) / 2;
		if(i != j)
		{
  			i = j;
 			if(event_protocol_table[i].ID == ID) break;
 			if(event_protocol_table[i].ID < ID) startelem = i; else endelem = i;
		}
		else
		{
  			i = sizeof(event_protocol_table) / sizeof(event_protocol_table[0]);
  			i -= 1;
 		 	break;
		}
  	}
  	
  	//��һ���жϱ�ʶ���Ƿ���ͬ 11
  	if( event_protocol_table[i].ID != ID )
  		return 1;	
  	
  	//
  	pReg->size = event_protocol_table[i].size+4;
  	pReg->type = event_protocol_table[i].type;
  	pReg->readOrWrite = event_protocol_table[i].readOrWrite;
  	pReg->keyOrNot = KEY_NOT;
  	pReg->location = event_protocol_table[i].location;;	
  	
  	pReg->eAddr = event_protocol_table[i].extendAddr; 	
	if ( (ID >= 0x03330101) && (ID <= 0x03330601) )		//������ʷ��¼ 11
  		pReg->eAddr = event_protocol_table[i].extendAddr+offset*18;
  	else if ( (ID >= 0x03350201) && (ID <= 0x03350301) )		//������ʷ��� 11
  		pReg->eAddr = event_protocol_table[i].extendAddr+offset*6;
  	else if ( (ID >= 0x03300101) && (ID <= 0x0330010A) )		//�����¼ 11
  		pReg->eAddr = event_protocol_table[i].extendAddr+offset*18;
  	else if ( (ID >= 0x03110001) && (ID <= 0x0311000A) )		//�����¼ 11
  		pReg->eAddr = event_protocol_table[i].extendAddr+offset*6;

#if(OVER_POWER_EANBLED==1)	  		
  	else if ( (ID >= 0x030F0101) && (ID <= 0x030F010A) )		//���ؼ�¼ 11
  		pReg->eAddr = event_protocol_table[i].extendAddr+offset*20;
#endif

#if(LOSS_PHASE_EANBLED==1)	
  	else if ( (ID >= 0x13010101) && (ID <= 0x1303010A) )		//�����¼ 11
  		pReg->eAddr = event_protocol_table[i].extendAddr+offset*20;
#endif
					
	return 0;		
	

}

/*
*****************************************************************************************
* fuction name: ProcessParaProtocol 04
*
* Description : 
*
* Arguments   : 
*
* returns     : uchar
*
* Notes       : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar ProcessParaProtocol ( uint32 ID, PROTOCOL_REG* pReg )
{	
	uchar startelem,endelem,i,j;
	uint32 tempID;
	
	//���������ж� 11
	if ( (ID<0x04000000) || (ID>0x04ffffff) )
  		return 1;
	
	if ( (ID<0x0401000F) && (ID>0x04010000) )
    {
  		tempID = ID;
  		ID = 0x04010001;
  	}
  	
	//The table of registers is ordered, and a binary search can be performed.
  	//The variable startelem points to the beginning of the table, and endelem
  	//points to the end. As the table is searched, these variables are updated.
 	// If the register is not found, dummy register FFFF is returned.
  	startelem = 0;
  	endelem = sizeof(parameter_protocol_table) / sizeof(parameter_protocol_table[0]);
 	i = 0;
 	while(1)
  	{
   		j = (endelem + startelem) / 2;
		if(i != j)
		{
  			i = j;
 			if(parameter_protocol_table[i].ID == ID) break;
 			if(parameter_protocol_table[i].ID < ID) startelem = i; else endelem = i;
		}
		else
		{
  			i = sizeof(parameter_protocol_table) / sizeof(parameter_protocol_table[0]);
  			i -= 1;
 		 	break;
		}
  	}
  	
  	//��һ���жϱ�ʶ���Ƿ���ͬ 11
  	if( parameter_protocol_table[i].ID != ID )
  		return 1;	
  	
  	//  
  	pReg->type = parameter_protocol_table[i].type;
  	pReg->readOrWrite = parameter_protocol_table[i].readOrWrite;
  	pReg->keyOrNot = KEY_NOT;
  	pReg->location = parameter_protocol_table[i].location;;	
  	
  	pReg->addr = parameter_protocol_table[i].ramAddr;
  	pReg->eAddr = parameter_protocol_table[i].extendAddr;	
  	pReg->checksumOrNot = parameter_protocol_table[i].bcdCheck;	
    
    pReg->size = parameter_protocol_table[i].size+4;
    if ( 0x04010000 == ID )     //ʱ���� 11
    {
  		pReg->size = 12+4;	
  	}
  	else if ( 0x04010001 == ID )
    {
  		pReg->size = 42+4;	
  		pReg->eAddr += (uint16)(tempID-ID)*48;
  	}
  	
	return 0;		
	

}

//����ʹ�� 11
#if ( (FREEZE_HOUR_EANBLED==1)|| (FREEZE_DAY_EANBLED==1))
/*
*****************************************************************************************
* fuction name: ProcessFreezeProtocol
*
* Description : 
*
* Arguments   : 
*
* returns     : uchar
*
* Notes       : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar ProcessFreezeProtocol ( uint32 ID, PROTOCOL_REG* pReg )
{	
	uchar startelem,endelem,i,j;	
	uchar offset;
	uint32 tempID;
	
	tempID = ID;
	//���������ж� 11
	if ( (ID<0x05010000) || (ID>0x05FFFFFF) )
  		return 1;
	offset = (uchar)(ID&0x000000FF);
	if ( offset > 100 )
	    return 1;
	ID -= offset;
	if ( ID > 0x05060000 )
		ID += 1;
		
	//The table of registers is ordered, and a binary search can be performed.
  	//The variable startelem points to the beginning of the table, and endelem
  	//points to the end. As the table is searched, these variables are updated.
 	// If the register is not found, dummy register FFFF is returned.
  	startelem = 0;
  	endelem = sizeof(freeze_protocol_table) / sizeof(freeze_protocol_table[0]);
 	i = 0;
 	while(1)
  	{
   		j = (endelem + startelem) / 2;
		if(i != j)
		{
  			i = j;
 			if(freeze_protocol_table[i].ID == ID) break;
 			if(freeze_protocol_table[i].ID < ID) startelem = i; else endelem = i;
		}
		else
		{
  			i = sizeof(freeze_protocol_table) / sizeof(freeze_protocol_table[0]);
  			i -= 1;
 		 	break;
		}
  	}
  	
  	//��һ���жϱ�ʶ���Ƿ���ͬ 11
  	if( freeze_protocol_table[i].ID != ID )
  		return 1;	
  	
  	//
  	pReg->size = freeze_protocol_table[i].size+4;
  	pReg->type = freeze_protocol_table[i].type;
  	pReg->readOrWrite = freeze_protocol_table[i].readOrWrite;
  	pReg->keyOrNot = KEY_NOT;
  	pReg->location = freeze_protocol_table[i].location;
  	
  	if ( tempID < 0x05050000 )
  	{
	#if ( FREEZE_HOUR_EANBLED==1)
  		I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,FreezePoint), 
	 					&startelem, 1, 1  ) ;
	  	if ( startelem >= 100 )
	  		startelem = 0;
	  		
	  	pReg->eAddr = freeze_protocol_table[i].extendAddr+((uint16)startelem)*9; 
	#endif	
  	}
	else 
	{
		pReg->eAddr = freeze_protocol_table[i].extendAddr + 4*(offset-1);			
	}	
  	
  			
	return 0;			
}

#endif
					