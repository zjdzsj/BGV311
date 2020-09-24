/******************************************************************************************
*  Copyright (c) 2010, Bona R&D Department
*  All rights reserved 
*
*  FileName£º   	main.h
*  Author£º     	Roger.
*  Date£º  			2010-3-8 20:11
*  Revised Date£º  	
*  Main Function£º  below are global variables used in this project
*  Version:			BonaV2.0
*  Note£º 			 			
*********************************************************************************************/
#ifndef _MAIN_H

#define _MAIN_H


/******************************************************************************************/
#ifndef _MAIN_DEFINE_
#define HEAD_MACRO_MAIN	extern
#else	
#define HEAD_MACRO_MAIN     	 
#endif
/******************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////		

//////////////////////////////////////////////////////////
HEAD_MACRO_MAIN Meter_Addr g_addr;			//µç±í±íµØÖ·Ïà¹Ø	16×Ö½Ú 11
HEAD_MACRO_MAIN Meter_Buy_PARA g_buyPara;	//µç±í¹ºµçÐÅÏ¢Ïà¹Ø	16×Ö½Ú 11
HEAD_MACRO_MAIN Meter_Para g_para;			//µç±í²ÎÊýÐÅÏ¢	16×Ö½Ú 11
HEAD_MACRO_MAIN Meter_EQuan g_energy;		//µçÁ¿Ïà¹Ø º¬·ÑÂÊ  40×Ö½Ú 11
HEAD_MACRO_MAIN Date_Time g_date;			//ÄêÔÂÈÕÖÜÊ±·ÖÃë		8×Ö½Ú 11

HEAD_MACRO_MAIN METER_STATUS3 g_meterStatus3;           //×´Ì¬×Ö3 11

//ÎÞ¹¦µçÁ¿²ÎÊý 11
#ifdef INACITVE_POWER_ENABLE	
	HEAD_MACRO_MAIN Meter_InactiveQuan g_Inactivenergy;		//ÎÞ¹¦µçÁ¿Êý¾Ý 48×Ö½Ú 11	
#endif

//ÐèÁ¿Ê¹ÄÜ 11
#ifdef DEMAND_ENABLE
	HEAD_MACRO_MAIN Meter_Demand g_demand;		//ÐèÁ¿  24×Ö½Ú 11
#endif

//¶ñÐÔ¸ºÔØÊ¹ÄÜ 11
#if(MALIGNANT_LOAD_CHK_EANBLED==1)
	HEAD_MACRO_MAIN Meter_Demand g_demand;		
	HEAD_MACRO_MAIN CL2208_SF g_curRLoad[8];
#endif

//¼ÆÁ¿Ð£Õý²ÎÊý start
//////////////////////////////////////////////////////////////////////////////
#if(METER_CHIP_MODEL==0x2208)

	HEAD_MACRO_MAIN CL2208_FACTOR g_factor;			//ÓÐÐ§ÖµÏµÊý 16 11
	HEAD_MACRO_MAIN CL2208_UIP g_uip;				//µçÑ¹¡¢µçÁ÷¡¢¹¦ÂÊÓÐÐ§Öµ 16×Ö½Ú 11
	HEAD_MACRO_MAIN CL2208_Para g_cl2208para;		//2218Ð£ÑéÏµÊý 16
	
	
#else if (METER_CHIP_MODEL==0x8302)

	HEAD_MACRO_MAIN RN8302_UIP g_uip[4];				//µçÑ¹¡¢µçÁ÷¡¢¹¦ÂÊÓÐÐ§Öµ 16×Ö½Ú 11
	HEAD_MACRO_MAIN RN8302_Para g_rn8302para;			//RN8302Ð£ÑéÏµÊý 32 11
	HEAD_MACRO_MAIN uint16 g_freqency;          //µçÍøÆµÂÊ 11	

#endif

#if(MALIGNANT_LOAD_CHK_EANBLED==1)
	HEAD_MACRO_MAIN CL2208_SF g_curRLoad[8];		//¶ñÐÔ¸ºÔØÅÐ¶Ï¼ÆËãÓÃ 11
#endif
//////////////////////////////////////////////////////////////////////////////
//¼ÆÁ¿Ð£Õý²ÎÊý end

//HEAD_MACRO_MAIN unsigned long g_couter;
//HEAD_MACRO_MAIN unsigned char g_flagStart;
	
HEAD_MACRO_MAIN  unsigned char g_randCnt;    //1
HEAD_MACRO_MAIN  unsigned int g_relayCnt;   //1
HEAD_MACRO_MAIN  unsigned char g_relayProCnt;   //¼ÌµçÆ÷¶¯×÷ºó,1ÃëÄÚ²»ÅÐ¶ÏÊÇ·ñµôµç 11

HEAD_MACRO_MAIN  unsigned char g_BeepDelay;   //1

HEAD_MACRO_MAIN  unsigned int g_BeepCnt;   //1
HEAD_MACRO_MAIN  unsigned int g_1MinpowerCnt;		//ÏÞµç¹¦ÂÊ·ÖÖÓÍ³¼Æ 11

HEAD_MACRO_MAIN unsigned int g_5minutesCnt; //2         //35
HEAD_MACRO_MAIN unsigned int g_5minutesBalanceCnt; //2         //35
HEAD_MACRO_MAIN unsigned int g_adSampleCnt; //2         


HEAD_MACRO_MAIN RELAY_FLAG g_relayFlag;

HEAD_MACRO_MAIN  uint16 g_485CommDly;	
HEAD_MACRO_MAIN  uint16 g_IRCommDly;



HEAD_MACRO_MAIN uchar g_overPower5Cnt;

HEAD_MACRO_MAIN  unsigned char g_relayChkDly; //4

HEAD_MACRO_MAIN  uchar g_IRTxdFlag;	

HEAD_MACRO_MAIN CARD_FLAG g_cardFlag;		//1×Ö½Ú£¬¿¨±êÊ¶ 1 11

//////////////////////////////////////////////////////////////////
//RAMÊ¹ÓÃÖµ£¬²»ÐèÒª±£´æ¼°ÉÏµç»ñÈ¡²ÎÊý 11
HEAD_MACRO_MAIN  uchar g_buf[64];				//40
HEAD_MACRO_MAIN  uchar g_lcdCycleCnt;			//1 Òº¾§ÆÁÏÔÊ±¼ä¿ØÖÆ 11
HEAD_MACRO_MAIN  uchar g_cardStatus;   			//1
HEAD_MACRO_MAIN  METER_ErrSTATUS g_errStatus;	//1 ±í´íÎó×´Ì¬×Ö 11
HEAD_MACRO_MAIN  uchar g_LCDFrame;				//1 Òº¾§ÏÔÊ¾Ïî 11
HEAD_MACRO_MAIN  uchar g_LCDSec;					//1 Òº¾§ÏÔÊ¾Ê±¼ä¼ÆÊýÆ÷ 11
HEAD_MACRO_MAIN  uchar g_commLCDDisp;			//1 Òº¾§ÏÔÊ¾Í¨Ñ¶±êÊ¶ 11
HEAD_MACRO_MAIN  uchar g_errLCDDispCnt;			//1  11

//__no_init U8 variable1;

#if(METER_CHIP_MODEL==0x2208)
	HEAD_MACRO_MAIN  uchar g_pulseLedDly;			//1 Âö³åÊä³öÑÓÊ± 11
	HEAD_MACRO_MAIN  uchar g_Uart2Dly;			//1 8209C´®¿ÚÍ¨ÐÅÑÓÊ±¼ÆÊýÆ÷  11
#endif
HEAD_MACRO_MAIN  uchar g_2208ErrCnt;	    		//1 2208Ð¾Æ¬¶ÁÈ¡¼°µçÑ¹Öµ´íÎó¼ÆÊý 11
HEAD_MACRO_MAIN  METER_FLAG g_meterFlag;	    	//2 ±íÈ«¾Ö±äÁ¿±êÊ¶  11

//¸ºÔØÌøÕ¢Ê¹ÄÜ 11
#if(OVER_POWER_RELAY_OFF_EANBLED==1) 
	HEAD_MACRO_MAIN  uchar g_OverPowerFlagCnt;		//ÏÞµç¹¦ÂÊ¼ÆÊýÆ÷ added by Roger 2013/09/07 11
#endif

HEAD_MACRO_MAIN  uchar g_buttonCnt;
HEAD_MACRO_MAIN unsigned int g_zeroSecCnt; //1 
HEAD_MACRO_MAIN unsigned int g_initDispCnt; //2 
HEAD_MACRO_MAIN unsigned int g_cardDispCnt; //2 
HEAD_MACRO_MAIN unsigned long g_price; //4

HEAD_MACRO_MAIN POWER_DIR_FLAG g_dirFlag;

HEAD_MACRO_MAIN unsigned char g_eepromErrCnt;

#if(IC_CARD_MODE==PLC_COMM_USED)
	HEAD_MACRO_MAIN COMM_STATUS g_commStatus[3]; //通信状态字 2 11
	HEAD_MACRO_MAIN uint16 g_commRxdFrameDly[3];	//数据帧延时	1字节 11
	HEAD_MACRO_MAIN  COMM_BUFFER g_commBuf[3];		//Í¨ÐÅ»º³åÇø	66*2=132×Ö½Ú 11
	HEAD_MACRO_MAIN  uchar g_commForOneFrame[3];		//2	 Í¨ÐÅÃ¿Ö¡×î³¤µÈ´ýÊ±¼ä 11
	HEAD_MACRO_MAIN  uchar g_commFor20msDly[3];		//2						
	HEAD_MACRO_MAIN  uchar g_commForLCD[3];			//2
	HEAD_MACRO_MAIN  uint16 g_PLCCommDly;	
#else
	HEAD_MACRO_MAIN COMM_STATUS g_commStatus[2]; //通信状态字 2 11
	HEAD_MACRO_MAIN uint16 g_commRxdFrameDly[2];	//数据帧延时	1字节 11
	HEAD_MACRO_MAIN  COMM_BUFFER g_commBuf[2];		//Í¨ÐÅ»º³åÇø	66*2=132×Ö½Ú 11
	HEAD_MACRO_MAIN  uchar g_commForOneFrame[2];		//2	 Í¨ÐÅÃ¿Ö¡×î³¤µÈ´ýÊ±¼ä 11
	HEAD_MACRO_MAIN  uchar g_commFor20msDly[2];		//2						
	HEAD_MACRO_MAIN  uchar g_commForLCD[2];			//2
#endif
HEAD_MACRO_MAIN unsigned char g_modbusTxdLen;

HEAD_MACRO_MAIN BAUDRATE_PARA g_baudRate;		//波特率相关参数 11

HEAD_MACRO_MAIN LCD_DISP_FLAG g_lcdFlag;

HEAD_MACRO_MAIN unsigned char g_btnFreezeLCD;	//单次按键显示保留时间 11

//added by Rogor 2020/09/23 增加按键保电功能 掉电不保存 11
//HEAD_MACRO_MAIN uchar g_btnKeepRelayOnFlag; //1

#if(CALI_CHANGE_CARD_ENABLED==1)	
	HEAD_MACRO_MAIN unsigned char g_RunCoef;
#endif
///////////////////////////////////////////////////////////////////////////////////////////
#ifdef LADDER_ENABLE
	static uchar g_dateLadder;		
#endif

#ifdef TARIFF_ENABLE
	HEAD_MACRO_MAIN uint16 g_tariffOffset;		
#endif

//日冻结操作 11
#if (FREEZE_DAY_EANBLED==1)
	HEAD_MACRO_MAIN uint32 g_daySeconds;			
#endif

#if (IC_CARD_MODE==RF_CARD)
	HEAD_MACRO_MAIN uchar g_rfRevBuf[64];	//CV520命令接收缓冲区 11 
	HEAD_MACRO_MAIN uchar g_rfSendBuf[64];	//CV520命令发送缓冲区 11

	HEAD_MACRO_MAIN uchar g_rfUID[5];        //序列号  11
	HEAD_MACRO_MAIN uchar g_rftagtype[2];	  //卡片标识字符11 
	HEAD_MACRO_MAIN RF_STATUS_FLAG g_rfFlag;	//射频卡操作相关标志 11
	
	HEAD_MACRO_MAIN uchar g_rfTryCnt;
	
	HEAD_MACRO_MAIN uchar g_rfInFieldCnt; //1
#endif

//需量使能 11
#ifdef DEMAND_ENABLE
	HEAD_MACRO_MAIN uint16 g_demandSlideCnt;			
#endif

//HEAD_MACRO_MAIN uint16 g_1200BPScNT; //1

HEAD_MACRO_MAIN void _BCDAccOne ( uchar *pdata );

#if(IC_CARD_MODE==PLC_COMM_USED)
	HEAD_MACRO_MAIN uint16 g_PLCRstCnt;
	HEAD_MACRO_MAIN uchar g_PLCInTransCnt;
#endif

#endif
