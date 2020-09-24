/******************************************************************************************
*  Copyright (c) 2010, Bona R&D Department
*  All rights reserved 
*
*  FileName?�??o   	common.c
*  Author?�??o     	Roger
*  Date?�??o  			2010-3-8 20:11
*  Revised Date?�??o  	
*  Main Function?�??o  this file contains all the functions may be used in all project files
*  Version:			BonaV2.0
*  Note?�??o 			 			
*********************************************************************************************/
#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

#pragma interrupt INTLVI ISREXLVI
//#pragma interrupt INTAD ISRForAD

/************************************************************************************************/
#define _COMMON_DEFINE_
#include "include.h"
/************************************************************************************************/
//�������Ķ���� 11
//only used in SPI.c file
/************************************************************************************************/
/*
static uint16 const CRC16_TABLE[256]={
	0x0000,0x1189,0x2312,0x329b,0x4624,0x57ad,0x6536,0x74bf,
	0x8c48,0x9dc1,0xaf5a,0xbed3,0xca6c,0xdbe5,0xe97e,0xf8f7,
	0x1081,0x0108,0x3393,0x221a,0x56a5,0x472c,0x75b7,0x643e,
	0x9cc9,0x8d40,0xbfdb,0xae52,0xdaed,0xcb64,0xf9ff,0xe876,
	0x2102,0x308b,0x0210,0x1399,0x6726,0x76af,0x4434,0x55bd,
	0xad4a,0xbcc3,0x8e58,0x9fd1,0xeb6e,0xfae7,0xc87c,0xd9f5,
	0x3183,0x200a,0x1291,0x0318,0x77a7,0x662e,0x54b5,0x453c,
	0xbdcb,0xac42,0x9ed9,0x8f50,0xfbef,0xea66,0xd8fd,0xc974,
	0x4204,0x538d,0x6116,0x709f,0x0420,0x15a9,0x2732,0x36bb,
	0xce4c,0xdfc5,0xed5e,0xfcd7,0x8868,0x99e1,0xab7a,0xbaf3,
	0x5285,0x430c,0x7197,0x601e,0x14a1,0x0528,0x37b3,0x263a,
	0xdecd,0xcf44,0xfddf,0xec56,0x98e9,0x8960,0xbbfb,0xaa72,
	0x6306,0x728f,0x4014,0x519d,0x2522,0x34ab,0x0630,0x17b9,
	0xef4e,0xfec7,0xcc5c,0xddd5,0xa96a,0xb8e3,0x8a78,0x9bf1,
	0x7387,0x620e,0x5095,0x411c,0x35a3,0x242a,0x16b1,0x0738,
	0xffcf,0xee46,0xdcdd,0xcd54,0xb9eb,0xa862,0x9af9,0x8b70,
	0x8408,0x9581,0xa71a,0xb693,0xc22c,0xd3a5,0xe13e,0xf0b7,
	0x0840,0x19c9,0x2b52,0x3adb,0x4e64,0x5fed,0x6d76,0x7cff,
	0x9489,0x8500,0xb79b,0xa612,0xd2ad,0xc324,0xf1bf,0xe036,
	0x18c1,0x0948,0x3bd3,0x2a5a,0x5ee5,0x4f6c,0x7df7,0x6c7e,
	0xa50a,0xb483,0x8618,0x9791,0xe32e,0xf2a7,0xc03c,0xd1b5,
	0x2942,0x38cb,0x0a50,0x1bd9,0x6f66,0x7eef,0x4c74,0x5dfd,
	0xb58b,0xa402,0x9699,0x8710,0xf3af,0xe226,0xd0bd,0xc134,
	0x39c3,0x284a,0x1ad1,0x0b58,0x7fe7,0x6e6e,0x5cf5,0x4d7c,
	0xc60c,0xd785,0xe51e,0xf497,0x8028,0x91a1,0xa33a,0xb2b3,
	0x4a44,0x5bcd,0x6956,0x78df,0x0c60,0x1de9,0x2f72,0x3efb,
	0xd68d,0xc704,0xf59f,0xe416,0x90a9,0x8120,0xb3bb,0xa232,
	0x5ac5,0x4b4c,0x79d7,0x685e,0x1ce1,0x0d68,0x3ff3,0x2e7a,
	0xe70e,0xf687,0xc41c,0xd595,0xa12a,0xb0a3,0x8238,0x93b1,
	0x6b46,0x7acf,0x4854,0x59dd,0x2d62,0x3ceb,0x0e70,0x1ff9,
	0xf78f,0xe606,0xd49d,0xc514,0xb1ab,0xa022,0x92b9,0x8330,
	0x7bc7,0x6a4e,0x58d5,0x495c,0x3de3,0x2c6a,0x1ef1,0x0f78	
};
*/
//variables definition

//�����¼���¼���� 11
#if(LOSS_PHASE_EANBLED==1)    
	static uint32 s_lossPhaseCnt[3];		//�����ж�ʱ�� 11

#endif

//�����¼���¼ 11
#if(OVER_POWER_EANBLED==1)	
	static uint32 s_overPowerCnt;		//�����¼�ʱ�� 11
#endif



//static uchar s_volLCnt;
/************************************************************************************************/

				
//this function defined in this file
/************************************************************************************************/
static void InitMemAndClock ( void );
static void InitEXLVI ( void );
static void InitPins ( void );
static void InitLCD ( void );
/************************************************************************************************/


/*
*****************************************************************************************
* fuction name: InitHardware
*
* Description : this function initializes MCU hardware  
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void InitHardware ( void )
{   
	//?�??�??????????�??|???·?3?μ???????ˉ???a?????????�??μ 11
    PIOR = 0x10U;   // �ܽŹ������¶��� 11
    CRC0CTL = 0x00U;// ����CRC ���ƼĴ��� 11
    RPECTL = 0x00U; // RAM ��żУ�������ƼĴ��� 11
    IAWCTL = 0x00U; // �Ƿ��洢����ȡ�����ƼĴ� 11
    PMS = 0x00U;    // �˿�ģʽѡ��Ĵ��� 11
    
	
	//CPU???±?�??�??�????�??±?�??¨?�??? 11
	InitMemAndClock (  );
	
	//?μ?�??μ?§?�??|?�??---?????2???μ?§?�??�????2???�??2???????2?�? 11
	InitEXLVI ( );
	
	//?1?????�??�??¤?�??? 11
	InitPins ( );	
	
	InitLCD();
	
	//?3?μ???????ˉ???????????????? 11
	//InitCl2208 ( );	
}							
/*
*****************************************************************************************
* fuction name: InitMemAndClock
*
* Description : this function initializes the clock of CPU and peripheral hardware.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
static void InitMemAndClock ( void )
{	
	// ?�????μ?§?o?3???μ???3???1?�????????????????2?????±?μ?�????·???±?�??�??�??????? 11
    // ???±?�??�??3?μ?????�??¨?�??????1?�????�????????�??�??????�??¨?�????�????2??????option.asm 11

    // ???�??�??�????-???�??�??o?????????????????????μ???�??¨?�??? 11

    // ?�??????1?�????????2?????????????±?μ?�????·???±?�??�? 11 
   
    // Set fMX 
    // ???±?�??�??�??????????�????? 11
    // CMC???�??????o?3?�??????????�??�????�??? 11
    CMC = 0;        // ?2?????1?�???X1?o??XT1???±?�??�? 11

    MSTOP = 1U;     // ???�??�??1X1 11

    // Set fMAIN 
    MCM0 = 0U;      // ?�????�??±?????????????2?????±?μ?�????·???±?�??�? 11

    // Set fSUB
    XTSTOP = 1U;    // ???�??�??1XT1 11
    OSMC = (1<<4);  // RTC,IT,LCD?�????�??±?????2???μ?????????±?�??�? 15khZ 11

    // Set fCLK 4MHz
    CSS = 0U;       // CPU???±?�??�??�????�??±?�??·???μ???3???±?�??�? 11
    
    // Set fIH 
    HIOSTOP = 0U;   // ?????????????2?????±?μ?�????·?�??????? 11
								 				
	return;	
}


/*
*****************************************************************************************
* fuction name: InitEXLVI
*
* Description : this function initializes external voltage detect interrupt. 
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
static void InitEXLVI ( void )
{
	//LVD?3?μ???????ˉ?�????�???????LVIF?±?a???????�????�??�???11 
	LVIM &= 0x7f;		//?????±???�???????11
	LVIS = 0x01;
	LVIMK = 0;		//???1?????�??????? 11
	    
	return;
}

/*
*****************************************************************************************
* fuction name: InitPins
*
* Description : this function initializes MCU pins 
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/

static void InitPins ( void )
{   
	//seg6~seg7 is seg  
    PFSEG0 = 0xf0; 
    //seg8 seg11 seg12 is seg      
	PFSEG1 = 0xff; 
    //seg16~seg23 is seg   	   
	PFSEG2 = 0xff;       
	//seg29 is port; P4.5 is port 
	PFSEG3 = 0xBB;  
	//seg33~seg36 is SEG;seg37 seg38 is port       
	PFSEG4 = 0x3F;   
	//seg39~seg46 is port       
	PFSEG5 = 0x00;   
	//seg47~seg50 is port       
	PFSEG6 = 0x00; 
	
	//i/o pin set
	
	//ȫ��ʹ�����ֽӿ� 11
	PMC1 = 0xF0;
	PMC2 = 0x3f;	//P2.2 ģ��� 11
	PMC4 = 0x00;

	
	//P2.0 P2.1 ad��ѡ�� ���ֿ� 11
	ADPC = 0x01;
	
	//LCD�ܽ� P12.5 P12.6��ΪLCD���� 11
	ISCLCD = 0x00;	
	
	//PIM �˿�����ģʽ�Ĵ��� 11
	PIM0 = 0x48;
	PIM1 = 0x80;
	PIM4 = 0x00;
	PIM5 = 0x00;
	
	//POM �˿����ģʽ�Ĵ��� 11
	POM0 = 0x40;        //485RXD P0.6
	POM1 = 0x00;        //
	POM4 = 0x00;
	POM5 = 0x00;

#if(PCB_VERSION_NEW_4==0)
	#if(METER_CHIP_MODEL==0x2208)	
		//P0 port(P0.0~P0.7), P0.0->Card_Power,output;P0.1->Card_RST,output;
		//P0.2->Card_CLK,output;P0.3->CPU_MTR_RXD,input;
		//P0.4->CPU_MTR_TXD,output;P0.5->485CTL,output,
		//P0.6->485RXD,input,P0.7->485TXD,output
		PM0 = 0x48;	
		PU0 = 0;	
		P0 = BIT0+BIT2+BIT4+BIT5+BIT7;
	#else
		//P0 port(P0.0~P0.7), P0.0->Card_Power,output;P0.1->Card_RST,output;
		//P0.2->Card_CLK,output;P0.3->SCLK,output;
		//P0.4->ccs,output;P0.5->485CTL,output,
		//P0.6->485RXD,input,P0.7->485TXD,output
		PM0 = 0x40;	
		PU0 = 0;	
		P0 = BIT0+BIT2+BIT4+BIT5+BIT7;
	#endif
#else
///////////////////////////////////////////////////////////////////////////////////////
	#if(METER_CHIP_MODEL==0x2208)	
			//P0 port(P0.0~P0.7), P0.0->PLCTxD,output;P0.1->HWRxd,input;
			//P0.2->PLCRsT,output;P0.3->CPU_MTR_RXD,input;
			//P0.4->CPU_MTR_TXD,output;P0.5->485CTL,output,
			//P0.6->485RXD,input,P0.7->485TXD,output
		#if(IC_CARD_MODE==PLC_COMM_USED)
			PM0 = 0x4A;	
			PU0 = 0;	
			P0 = BIT0+BIT2+BIT4+BIT5+BIT7;
		#else	
			//P0 port(P0.0~P0.7), P0.0->Card_Power,output;P0.1->HWRxd,input;
			//P0.2->Card_CLK,output;P0.3->CPU_MTR_RXD,input;
			//P0.4->CPU_MTR_TXD,output;P0.5->485CTL,output,
			//P0.6->485RXD,input,P0.7->485TXD,output
			PM0 = 0x4A;	
			PU0 = 0;	
			P0 = BIT0+BIT2+BIT4+BIT5+BIT7;
		#endif
		
	
	#else
		//P0 port(P0.0~P0.7), P0.0->Card_Power,output;P0.1->HWRxd,input;
		//P0.2->Card_CLK,output;P0.3->SCLK,output;
		//P0.4->ccs,output;P0.5->485CTL,output,
		//P0.6->485RXD,input,P0.7->485TXD,output
		#if(IC_CARD_MODE!=PLC_COMM_USED)
			PM0 = 0x42;	
			PU0 = 0;	
			P0 = BIT0+BIT2+BIT4+BIT5+BIT7;
		#endif
		
		//P0 port(P0.0~P0.7), P0.0->PLCTxD,output;P0.1->HWRxd,input;
		//P0.2->PLCRsT,output;P0.3->SCLK,output;
		//P0.4->ccs,output;P0.5->485CTL,output,
		//P0.6->485RXD,input,P0.7->485TXD,output
		#if(IC_CARD_MODE==PLC_COMM_USED)
			PM0 = 0x42;	
			PU0 = 0;	
			P0 = BIT0+BIT2+BIT4+BIT5+BIT7;
		#endif
		
	#endif

#endif

//P1 port
/////////////////////////////////////////////////////////////////////////////////
#if(PCB_VERSION_NEW_4==0)			
		//P1 port(P1.0~P1.7), P1.0->SEG35,output;P1.1->SEG36,output;
		//P1.2->Card_SDA,output;P1.3->Card_CTE,input;
		//P1.4->HWTXD,output;P1.5->JA,output,P1.6->JB,output,P1.7->HWRXD,input 11
	#if(IC_CARD_MODE==AT24_CARD)
		PM1 = 0x88;		
		PU1 = 0;
		P1 = BIT2+BIT7;
	#endif
		//P1 port(P1.0~P1.7), P1.0->SEG35,output;P1.1->SEG36,output;
		//P1.2->Card_SDA,input;P1.3->Card_CTE,input;
		//P1.4->HWTXD,output;P1.5->JA,output,P1.6->JB,output,P1.7->HWRXD,input 11
	#if(IC_CARD_MODE==RF_CARD)
		PM1 = 0x84;		
		PU1 = 0;
		P1 = BIT7;
	#endif
#else
		//P1 port(P1.0~P1.7), P1.0->SEG35,output;P1.1->SEG36,output;
		//P1.2->Card_SDA,output;P1.3->Card_CTE,input;
		//P1.4->HWTXD,output;P1.5->JA,output,P1.6->JB,output,P1.7->Card_Rst,output 11
	#if(IC_CARD_MODE==AT24_CARD)
		PM1 = 0x08;		
		PU1 = 0;
		P1 = BIT2+BIT7;
	#endif
		//P1 port(P1.0~P1.7), P1.0->SEG35,output;P1.1->SEG36,output;
		//P1.2->Card_SDA,input;P1.3->Card_CTE,output;
		//P1.4->HWTXD,output;P1.5->JA,output,P1.6->JB,output,P1.7->Card_Rst,output 11
	#if(IC_CARD_MODE==RF_CARD)
		PM1 = 0x04;		
		PU1 = 0;
		P1 = BIT7;
	#endif
		
		//P1 port(P1.0~P1.7), P1.0->SEG35,output;P1.1->SEG36,output;
		//P1.2->PLCSTA,input;P1.3->PLCEVENT,output;
		//P1.4->HWTXD,output;P1.5->JA,output,P1.6->JB,output,P1.7->PLCRxD0,input 11
	#if(IC_CARD_MODE==PLC_COMM_USED)
		PM1 = 0x84;		
		PU1 = BIT7;
		P1 = BIT7+BIT3;
	#endif

#endif

/////////////////////////////////////////////////////////////////////////////////////////////
	
	//P2 port(P2.0~P2.2 P2.6 P2.7), P2.0->BEEP,output;P2.1->WP,output;
	//P2.2->BATIN,input;P2.6->SEG33,output;P2.7->SEG34,output;
	PM2 = 0x04; 
	P2 = 0;
	
	//P3 port(P3.0~P3.3), P3.0~P3.3->SEG20~SEG23,output
	PM3 = 0;
	P3 = 0;

//pin4
/////////////////////////////////////////////////////////////////////////////////
#if(PCB_VERSION_NEW==0)
	#if(METER_CHIP_MODEL==0x2208)		
		//P4 port(P4.0~P4.3), P4.0->TOOL0,output;P4.2->PLED,output,
		//P4.3->SWITCH,output,P4.4->ALARM,output,P4.5->LCDLED,output,
		PM4 = 0;
		P4 = BIT2+BIT3+BIT4+BIT5;
	#else
		//P4 port(P4.0~P4.3), P4.0->TOOL0,output;P4.2->M-SDO,output,
		//P4.3->CF2,input,P4.4->SLED,output,P4.5->LCDLED,output,
		PM4 = BIT3;
		P4 = BIT2+BIT4;
		#ifdef INACITVE_POWER_ENABLE
			//intp7 P4.3 inactive 
			EGP0 |= BIT7;		//enable rising edge 
			EGN0 |= BIT7; 	    //enable falling edge 
			PIF7 = 0;       //clear interrupt flag
			PMK7= 0;	    //enable INTP0 interrupt
		#endif	
	#endif
#else
	#if(METER_CHIP_MODEL==0x2208)		
		//P4 port(P4.0~P4.3), P4.0->TOOL0,output;P4.2->PLED,output,
		//P4.3->Disp_Button,input,P4.4->ALARM,output,P4.5->LCDLED,output,
		PM4 = BIT3;
		P4 = BIT0+BIT2+BIT4+BIT5;
	#else
		//P4 port(P4.0~P4.3), P4.0->TOOL0,output;P4.2->M-SDO,output,
		//P4.3->UP,input,P4.4->SLED,output,P4.5->LCDLED,output,
		PM4 = BIT3;
		P4 = BIT2+BIT4;		
	#endif
	
		//intp7 P4.3 UP Button 
		EGP0 |= BIT7;		//enable rising edge 
		EGN0 |= BIT7; 	    //enable falling edge 
		PIF7 = 0;       //clear interrupt flag
		PMK7= 0;	    //enable INTP0 interrupt
#endif	

///////////////////////////////////////////////////////////////////////////

	//P5 port(P5.2~P5.7), P5.2->SEG6,output;P5.3->SEG7,output;
	//P5.4->SEG8,output;P5.7->SEG11,output;
	PM5 = 0;
	P5 = 0;
	
	//P6 port(P6.0~P6.1), P6.0->SCL,output;P6.1->SDA,output;
	PM6 = 0;
	P6 = BIT0+BIT1;
	
	//P7 port(P7.0~P7.4), P7.0->SEG11,output;P7.4->SEG16,output;
	//P7.5->SEG17,output;P7.6->SEG18,output;P7.7->SEG19,output;
	PM7 = 0;
	P7 = 0;

/////////////////////////////////////////////////////////////////////////////////////
#if(PCB_VERSION_NEW==0)
	#if(METER_CHIP_MODEL==0x2208)	
		//P12 port(P12.1~P12.7), P12.1->Dis_key,input;P12.2->Pro_KEY,input;
		//P12.3->XT2,input;P12.4->XT1,input;
		//P12.5->VL3,output,P12.6->CAPL,input,P12.7->CAPH,input	
		PM12 = 0xcf;		
		P12 = 0;
	#else	
		//P12 port(P12.1~P12.7), P12.1->KEY1-DOWN,input;P12.2->KEY2-UP,input;
		//P12.3->XT2,output;P12.4->M-SDI,input;
		//P12.5->VL3,output,P12.6->CAPL,input,P12.7->CAPH,input	
		PM12 = 0xd6;		
		P12 = 0;
	#endif	
#else
	#if(METER_CHIP_MODEL==0x2208)	
		//P12 port(P12.1~P12.7), P12.1->reserved,output;P12.2->Pro_KEY,input;
		//P12.3->XT2,input;P12.4->XT1,input;
		//P12.5->VL3,output,P12.6->CAPL,input,P12.7->CAPH,input	
		PM12 = 0xce;		
		P12 = 0;
	#else	
		//P12 port(P12.1~P12.7), P12.1->KEY1-DOWN,input;P12.2->CF2,input;
		//P12.3->PROG,input;P12.4->M-SDI,input;
		//P12.5->VL3,output,P12.6->CAPL,input,P12.7->CAPH,input	
		PM12 = 0xdD;		
		P12 = 0;
	#endif	
#endif

/////////////////////////////////////////////////////////////////////////////////////////
	//P13 port( P13.7)P13.7->ACTIVEP,input
	EGP0 |= 0x01;		//enable rising edge 
	EGN0 |= 0x01; 	    //enable falling edge 
	PIF0 = 0;       //clear interrupt flag
	PMK0= 0;	    //enable INTP0 interrupt
		
}

/*
**********************************************************************************************
** Function name	:	InitLCD
**
** Descriptions		:	Initialize the LCD handler
**
** Parameters		:	none
**
** Returned value	:	none
**
***********************************************************************************************
*/
static void InitLCD ( void )
{		
    LCDM0 = 0x56;    //capacitor spilt mode,waveform A,8com,8-time,1/4 bias 
	
	//VLCD = 0x04;     //vl4 voltage is 4.00v;  
 	//VLCD = 0x05;     //vl4 voltage is 4.2v;	
 	//VLCD = 0x06;     //vl4 voltage is 4.4v;  
 	VLCD = 0x07;     //vl4 voltage is 4.6v;  
 	//VLCD = 0x09;     //vl4 voltage is 5.0v;
 	//VLCD = 0x0a;     //vl4 voltage is 5.2v;
 	
	ISCLCD = 0x00;   //vl3 and capl,caph is invalid  
	Delay1MsTime ( 50 );
	
	LCDM1 = 0x20; 
	
	LCDC0 = 0x04;    	//0x06:116Hz; 0x07:fLCD = fsub/2^9 = 64Hz  V8.5.10
	//LCDC0 = 0x06;    	//0x06:116Hz; 0x07:fLCD = fsub/2^9 = 64Hz  V8.5.10
	//LCDC0 = 0x05;    	//0x06:116Hz; 0x07:fLCD = fsub/2^8 = 128Hz  V8.5.11
		
	Delay1MsTime ( 2 );
	     	    	  	
	LCDM1 |= 0xC0;    //display on 
 		
	return;
}

/*
*****************************************************************************************
* fuction name: ProcessHalfSecTaskFlags
*
* Description : This function process tasks flags when every half second
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ProcessHalfSecTaskFlags ( void )
{
  
    
    
}

/*
*****************************************************************************************
* fuction name: ProcessHalfSecTaskFlags
*
* Description : This function process tasks flags when every half second
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ProcessSecTaskFlags ( void )
{        
    uchar i;
    
#ifdef LADDER_ENABLE
	uchar temp1,temp2;
#endif
    
#if( (FREEZE_HOUR_EANBLED==1)||(FREEZE_DAY_EANBLED==1) ||(LOSS_PHASE_EANBLED==1) ||(OVER_POWER_EANBLED==1) ||(OVER_POWER_RELAY_OFF_EANBLED==1) )	
	uint32 tempL;
	uchar j;
#endif

//#if ((LOSS_PHASE_EANBLED==1))
//	uint32 tempL;
//	uchar j;
//#endif

#if(MALIGNANT_LOAD_CHK_EANBLED==1)
	uint32 tempL1;
	uchar lastCnt;
#endif

    //����оƬУ�� 11
    //if ( g_buyPara.Status != MANUFACTURE_STATUS )
    {
        if ( g_2208ErrCnt >= 5 )
        {
        	g_2208ErrCnt = 0;
        	FeedWatchdog ( );
        
        #if(METER_CHIP_MODEL==0x2208)	
        	SoftResetCl2208 ( );
    		Delay1MsTime ( 5 );
    		CheckCl2208Reg ( );	
    	#else if (METER_CHIP_MODEL==0x8302)	    		       	
    		CheckRN8302Reg ( );	
    	#endif
    		
    		
        }
       
        if ( (g_meterFlag.Bit.Cl2208check) &&
        		(g_errStatus.Bit.E2ROM==0) )
        {
        	g_meterFlag.Bit.Cl2208check = 0;
        	FeedWatchdog ( );
        
        #if(METER_CHIP_MODEL==0x2208)	
        	if ( CheckCl2208Reg ( ) != 0 )
        #else if (METER_CHIP_MODEL==0x8302)	   
        	if ( CheckRN8302Reg ( ) != 0 )
        #endif
        	{
        		g_2208ErrCnt++;  
        		
        	} 	
        }	
    }
	
	//�̵���״̬���� 11
	if ( (g_relayCnt==0) && 
			(g_relayFlag.Bit.StatusToSave==1) )
	{
		g_relayFlag.Bit.StatusToSave = 0;
		g_para.RelayFlag = g_relayFlag.Byte;
	    g_para.Checksum = GetSum ( (uchar*)&g_para.LimitPower, Meter_Para_Len-1 );
		FeedWatchdog ( );
		StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LimitPower),
			(uchar*)&g_para.LimitPower,Meter_Para_Len );
		
	}
	
	//�̵���������,5���Ӻ����� 11
    if ( g_5minutesCnt > 0 )
    {
        g_5minutesCnt--;   
        
        if ( g_5minutesCnt == 0 )
        {
            
            if ( g_relayFlag.Bit.RelayInTest )
            {
                g_relayFlag.Bit.RelayNeedToOperate = 1;
                g_relayFlag.Bit.RelayInTest = 0;                    
            }  
            
            
            //������բ��5���Ӻ�բ���� 11
            if ( g_relayFlag.Bit.RelayAlarmOn )
            {
                g_relayFlag.Bit.RelayAlarmOn = 0; 
                
                g_meterStatus3.Bit.Alarm2Status = 0;  
//            #ifdef PRE_PAID_ENABLE
//                if( (g_energy.Balance > 0) && (g_relayFlag.Bit.RelayOn == 0) )
//                    g_relayFlag.Bit.RelayNeedToOperate = 1; 
//            #else
//                if( (g_relayFlag.Bit.RelayOn == 0) )
//                    g_relayFlag.Bit.RelayNeedToOperate = 1;   
//            #endif            
            } 
            
            //������բʹ�� 11
		#if(OVER_POWER_RELAY_OFF_EANBLED==1)   	
		   	//���֮ǰ�ǹ��ر����Ͽ��̵�����5���Ӻ�ɱպϼ̵��� 11
		   	if ( g_errStatus.Bit.OverPower )
		   	{
		   	    g_errStatus.Bit.OverPower = 0;
 		   	    g_OverPowerFlagCnt = 0;		//added by Roger 2013/09/07
		   	    
		   	    g_meterStatus3.Bit.OverPower = 0;   
		   	    
		   	    //if ( g_energy.Balance > 0 )
		        //    if( g_relayFlag.Bit.RelayOn == 0 )
		        //        g_relayFlag.Bit.RelayNeedToOperate = 1;       	    
		   	}
		#endif 
		
			//���Ը���ʹ�� 11
		#if(MALIGNANT_LOAD_CHK_EANBLED==1)	
			//���֮ǰ�ǹ��ر����Ͽ��̵�����1���Ӻ�ɱպϼ̵��� 11
			if(g_errStatus.Bit.RLoadHappened)
			{
				g_errStatus.Bit.RLoadHappened = 0;
				g_meterStatus3.Bit.MagliStatus = 0;
				
				for ( i=0; i<8; i++ )
            	{
            	    g_curRLoad[i].PRms = 0;
            	    g_curRLoad[i].SRms = 0;    
            	} 	
			}
		#endif
		  
        }
    }
    
    //5����ͨ�� 11
    if ( g_5minutesBalanceCnt > 0 )
    {
        g_5minutesBalanceCnt--;   
        if ( g_5minutesBalanceCnt == 0 )
        {       
            //ʣ��������բ��5���Ӻ�պ� 11
            if ( g_relayFlag.Bit.BalanceZeroHappened )
            {                 
                g_relayFlag.Bit.BalanceZero5MinFlg = 1;
            }                 
        }
    }
	
	//�ն������ 11
#if (FREEZE_DAY_EANBLED==1)
	if ( g_daySeconds >= 86400 )		//3600*24		
	{
		I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,LastDay1FreezeActive), 
				&g_buf[4], 12, 1  );
		//��ǰ�й��ܵ��� 11
		tempL =  Hex32ToBCD( g_energy.Accumulate );
		g_buf[0] = (uchar)(tempL&0xff);
		g_buf[1] = (uchar)((tempL>>8)&0xff);
		g_buf[2] = (uchar)((tempL>>16)&0xff);
		g_buf[3] = (uchar)((tempL>>24)&0xff);
		//�洢���1�ι����¼���� 11    
	    EEPROM_NOWP();
		I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,LastDay1FreezeActive), 
			    &g_buf[0], 16, 1  );
		EEPROM_WP();
		FeedWatchdog ( );	
		
		g_daySeconds -= 86400;	
	}
#endif     
            
    //��ʱ����ش��� 11
#ifdef RTC_8025T
	//�����¼� 11
	if ( g_meterFlag.Bit.ZeroSecondFlag )
    {

        g_meterFlag.Bit.ZeroSecondFlag = 0;
    
    #ifdef TARIFF_ENABLE                
        g_meterFlag.Bit.TariffProcess = 1;
    #endif 
    	
    }
	
#ifdef LADDER_ENABLE
	if ( (g_dateLadder!=g_date.day) &&
		(CheckDay31(g_date.day)==0) )
	{
		g_dateLadder = g_date.day;
		
		if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,YYMM), 
				(uchar*)&g_buf[0],CHECK_ADDMOD,16 ) == 0 )
		{
			if ( (g_date.year>=g_buf[5]) &&	
					(g_date.month>=g_buf[6]) &&			
					(g_date.day>=g_buf[7]) )		//һ������������� 11
			{
				g_meterFlag.Bit.LadderProcess = 1; 
 				g_energy.CurrentQ = 0;	
 				
 				if ( (g_buf[0]>0) && (g_buf[0]<5) )		//�� 11
 				{
 					temp1 = (uchar)BCDToHex( &g_buf[0], 1 );
 					temp2 = (uchar)BCDToHex( &g_buf[5], 1 );
 					
 					temp2 += temp1;
 					g_buf[5] = Hex8ToBCD( temp2 );
 				}
 				else if ( (g_buf[1]>0) && (g_buf[1]<10) )	//�� 11
 				{
 					temp1 = (uchar)BCDToHex( &g_buf[1], 1 );
 					temp2 = (uchar)BCDToHex( &g_buf[6], 1 );
 					
 					temp2 += temp1;
 					if ( temp2 > 12 )		//���� 11
 					{
 						temp2 -= 12;
 						g_buf[6] = Hex8ToBCD( temp2 );
 						
 						temp2 = (uchar)BCDToHex( &g_buf[5], 1 );
 						temp2 += 1;
 						g_buf[5] = Hex8ToBCD( temp2 );	
 					}
 					else
 					{
 						g_buf[6] = Hex8ToBCD( temp2 );
 					}	
 				}
 				else
 				{
 					g_buf[5] = 0x99;	
 				}
 				//У��� 11
				g_buf[15] = GetSum ( &g_buf[0], 15 );
 				StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,YYMM),
 					 (uchar*)&g_buf[0],16 );	
			   			 
 				SaveEnergy ( LOW_POWER_SAVE );	 							   		
 				SaveEnergy ( NORMAL_SAVE );	
 				
			}	
		}						 				 		 					
	}

#endif	
	
    //�����¶��¼� 11
	if( (g_date.day == 0x01) &&
		(g_date.hour == 0x00) &&
		(g_date.min == 0x00) &&
		(g_date.sec == 0x05) )	
	{		 			
 			
 		
 		//��ʷ����ת�� 11
 		for ( i=0; i<11; i++ )
 		{
 			I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last11_A1)-4*i, 
 					&g_buf[0], 4, 1  ) ;
 			
 			EEPROM_NOWP();
	    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last12_A1)-4*i, 
	    		&g_buf[0], 4, 1  );
			EEPROM_WP();
			
			FeedWatchdog ( );
 		}
 		EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_A1), 
	    	(uchar*)&g_energy.Accumulate, 4, 1  );
		EEPROM_WP();
				 		
 	//����ʹ�� 11
	#ifdef DEMAND_ENABLE
		//��ʷ����ת�� 11
 		for ( i=0; i<12; i++ )
 		{
 			I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last11Demand)-16*i, 
 					&g_buf[0], 16, 1  ) ;
 			
 			EEPROM_NOWP();
	    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last12Demand)-16*i, 
	    		&g_buf[0], 16, 1  );
			EEPROM_WP();
			
			FeedWatchdog ( );
 		}
 		
 		for ( i=0; i<16; i++ )
 			g_buf[i] = 0;	
 		EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,CurDemand), 
	    	(uchar*)&g_buf[0], 16, 1  );
		EEPROM_WP();
			
	#endif
 	}				
    
    //���������¼� 11
#if(FREEZE_HOUR_EANBLED==1)	
	if( (g_date.min == 0x00) &&
		(g_date.sec == 0x03) )	
	{
		
		I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,FreezePoint), 
 						&g_buf[0], 1, 1  ) ;
	  	if ( g_buf[0] >= 100 )
	  		g_buf[0] = 0;
		
		g_buf[1] = g_date.year;
		g_buf[2] = g_date.month;
		g_buf[3] = g_date.day;
		g_buf[4] = g_date.hour;
		g_buf[5] = g_date.min;
		tempL = Hex32ToBCD( g_energy.Accumulate );				
		g_buf[6] = (uchar)tempL&0xff;
		g_buf[7] = (uchar)(tempL>>8)&0xff;
		g_buf[8] = (uchar)(tempL>>16)&0xff;			
		g_buf[9] = (uchar)(tempL>>24)&0xff;
						
		EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1FreezeTime)+9*g_buf[0], 
    		&g_buf[1], 9, 1  );
		EEPROM_WP();
		
		//����ָ�� 11
		g_buf[0]++;
		if ( g_buf[0] > 100 )	
			g_buf[0] = 0;
		EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,FreezePoint), 
 						&g_buf[0], 1, 1  ) ;
		EEPROM_WP();
	}
#endif

#endif    

#ifdef PRE_PAID_ENABLE   
    //����ж� 11
    if ( (g_price > MAX_PRICE) || 
    	(g_price < MIN_PRICE) )
    {
    	ProcessPrice( );
    }    
#endif
   	
	

	
	//�����¼���¼���� 11
#if((LOSS_PHASE_EANBLED==1) && (SINGLE_OR_THREE_METER==3))    
	for ( i=0; i<3; i++ )
	{
		//50.0V
		if ( g_uip[i].URms < 500 )	
		{
			s_lossPhaseCnt[i]++;
			if ( s_lossPhaseCnt[i] > 60 )
			{
				
				//��ʷ��¼ת�� 11
		 		for ( j=0; j<9; j++ )
		 		{
		 			I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,PhaseALossLast9Record)+146*i-j*14, 
		 					&g_buf[0], 14, 1  ) ;
		 			
		 			EEPROM_NOWP();
			    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,PhaseALossLast10Record)+146*i-14*j, 
			    		&g_buf[0], 14, 1  );
					EEPROM_WP();
					
					FeedWatchdog ( );
		 		}
		 		//��¼����ʱ�̺͵������� 11
		 		g_buf[0] = g_date.year;
				g_buf[1] = g_date.month;
				g_buf[2] = g_date.day;
				g_buf[3] = g_date.hour;
				g_buf[4] = g_date.min;
				g_buf[5] = g_date.sec;
				tempL = Hex32ToBCD( g_energy.PosAccumulate );				
				g_buf[6] = (uchar)tempL&0xff;
				g_buf[7] = (uchar)(tempL>>8)&0xff;
				g_buf[8] = (uchar)(tempL>>16)&0xff;			
				g_buf[9] = (uchar)(tempL>>24)&0xff;
				tempL = Hex32ToBCD( g_energy.NegAccumulate );				
				g_buf[10] = (uchar)tempL&0xff;
				g_buf[11] = (uchar)(tempL>>8)&0xff;
				g_buf[12] = (uchar)(tempL>>16)&0xff;			
				g_buf[13] = (uchar)(tempL>>24)&0xff;
				EEPROM_NOWP();
			    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,PhaseALossLast1Record)+146*i, 
			    		&g_buf[0], 14, 1  );
				EEPROM_WP();
			}
		}
		else if ( g_uip[i].URms > 2000 )
		{
			if ( s_lossPhaseCnt[i] > 60 )
			{
				//�ۼƶ���ʱ��ͼ�¼���ζ����¼� 11
				I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,PhaseALossCnt)+146*i, 
			 			&g_buf[0], 6, 1  ) ;
			  	tempL = BCDToHex( &g_buf[0], 3 );
			  	tempL++;
			  	tempL = Hex32ToBCD( tempL );
				g_buf[0] = (uchar)tempL&0xff;
				g_buf[1] = (uchar)(tempL>>8)&0xff;
				g_buf[2] = (uchar)(tempL>>16)&0xff;			
				
				tempL = BCDToHex( &g_buf[3], 3 );
			  	tempL += s_lossPhaseCnt[i]/60;
			  	tempL = Hex32ToBCD( tempL );
				g_buf[3] = (uchar)tempL&0xff;
				g_buf[4] = (uchar)(tempL>>8)&0xff;
				g_buf[5] = (uchar)(tempL>>16)&0xff;
				
				EEPROM_NOWP();
				I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,PhaseALossCnt)+146*i, 
			 			&g_buf[0], 6, 1  ) ;
				EEPROM_WP();
					
			}
			s_lossPhaseCnt[i] = 0;
		}
	}

#endif

	//����Խ�� 11
#if(OVER_POWER_EANBLED==1)	
	#if(SINGLE_OR_THREE_METER==3)		//���� 11
		if ( (g_uip[0].PRms>(g_para.LimitPower*10)) ||
				(g_uip[1].PRms>(g_para.LimitPower*10)) ||
				(g_uip[2].PRms>(g_para.LimitPower*10)) )		
	#else		//���� 11
		tempL = BCDToHex( (uchar*)&g_uip.PRms, 4 );
		if ( tempL>(g_para.LimitPower*10) &&
				(g_para.LimitPower>0) )
	#endif
		{
			if ( g_errStatus.Bit.OverPower==0 )
			{
				s_overPowerCnt++;
				if ( (s_overPowerCnt > 60) )	
				{
					//��ʷ��¼ת�� 11
			 		for ( i=0; i<9; i++ )
			 		{
			 			I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,OverPowerLast9)-i*20, 
			 					&g_buf[0], 20, 1  ) ;
			 			
			 			EEPROM_NOWP();
				    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,OverPowerLast9)-20*i, 
				    		&g_buf[0], 20, 1  );
						EEPROM_WP();
						
						FeedWatchdog ( );
			 		}
			 		//��¼����ʱ�̺͵������� 11
			 		g_buf[0] = g_date.year;
					g_buf[1] = g_date.month;
					g_buf[2] = g_date.day;
					g_buf[3] = g_date.hour;
					g_buf[4] = g_date.min;
					g_buf[5] = g_date.sec;
					g_buf[6] = 0;
					g_buf[7] = 0;
					g_buf[8] = 0;
					g_buf[9] = 0;
					g_buf[10] = 0;
					g_buf[11] = 0;
					tempL = Hex32ToBCD( g_energy.PosAccumulate );				
					g_buf[12] = (uchar)tempL&0xff;
					g_buf[13] = (uchar)(tempL>>8)&0xff;
					g_buf[14] = (uchar)(tempL>>16)&0xff;			
					g_buf[15] = (uchar)(tempL>>24)&0xff;
					tempL = Hex32ToBCD( g_energy.NegAccumulate );				
					g_buf[16] = (uchar)tempL&0xff;
					g_buf[17] = (uchar)(tempL>>8)&0xff;
					g_buf[18] = (uchar)(tempL>>16)&0xff;			
					g_buf[19] = (uchar)(tempL>>24)&0xff;
					EEPROM_NOWP();
				    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,OverPowerLast1), 
				    		&g_buf[0], 20, 1  );
					EEPROM_WP();	
					
					g_errStatus.Bit.OverPower = 1;	
					g_meterStatus3.Bit.OverPower = 1;   
				}	
			}
		}
		else
		{
			if ( s_overPowerCnt > 60 )
			{
				//�ۼƶ���ʱ��ͼ�¼���ζ����¼� 11
				I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,OverPowerCnt), 
			 			&g_buf[0], 26, 1  ) ;
			  	tempL = BCDToHex( &g_buf[0], 3 );
			  	tempL++;
			  	tempL = Hex32ToBCD( tempL );
				g_buf[0] = (uchar)tempL&0xff;
				g_buf[1] = (uchar)(tempL>>8)&0xff;
				g_buf[2] = (uchar)(tempL>>16)&0xff;			
				
				tempL = BCDToHex( &g_buf[3], 3 );
			  	tempL += s_overPowerCnt/60;
			  	tempL = Hex32ToBCD( tempL );
				g_buf[3] = (uchar)tempL&0xff;
				g_buf[4] = (uchar)(tempL>>8)&0xff;
				g_buf[5] = (uchar)(tempL>>16)&0xff;
				
				g_buf[12] = g_date.year;
				g_buf[13] = g_date.month;
				g_buf[14] = g_date.day;
				g_buf[15] = g_date.hour;
				g_buf[16] = g_date.min;
				g_buf[17] = g_date.sec;
				
				//�ܵ������� 11
				tempL = BCDToHex( &g_buf[18], 4 );
				tempL = g_energy.PosAccumulate - tempL;
				tempL = Hex32ToBCD( tempL );				
				g_buf[18] = (uchar)tempL&0xff;
				g_buf[19] = (uchar)(tempL>>8)&0xff;
				g_buf[20] = (uchar)(tempL>>16)&0xff;			
				g_buf[21] = (uchar)(tempL>>24)&0xff;
				
				tempL = BCDToHex( &g_buf[22], 4 );
				tempL = g_energy.NegAccumulate - tempL;
				tempL = Hex32ToBCD( tempL );				
				g_buf[22] = (uchar)tempL&0xff;
				g_buf[23] = (uchar)(tempL>>8)&0xff;
				g_buf[24] = (uchar)(tempL>>16)&0xff;			
				g_buf[25] = (uchar)(tempL>>24)&0xff;
				
				EEPROM_NOWP();
				I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,OverPowerCnt), 
			 			&g_buf[0], 26, 1  ) ;
				EEPROM_WP();
					
			}
			s_overPowerCnt = 0;		
		}
#endif

//���Ը���ʹ�� 11
#if(MALIGNANT_LOAD_CHK_EANBLED==1) 
	#if(SINGLE_OR_THREE_METER==1)		//���� 11
	{
		if ( (g_buyPara.FactorLimit<1000) &&
				(g_buyPara.FactorLimit>500) &&		//0.500 3λС�� 11
				(g_buyPara.FactorPower>0) &&
				(g_meterFlag.Bit.MaglentFlag==1) ) 			    
		{
			g_meterFlag.Bit.MaglentFlag = 0;
			if ( g_errStatus.Bit.RLoadHappened==0 )
		    {
				    			
    		    lastCnt = 0;
    		    for ( i=1; i<8; i++ )
    		    {								   			
        			//�й������ж� ��100WΪ�жϻ�׼ 
        			if ( g_curRLoad[i].PRms > 
        					(g_curRLoad[0].PRms+g_buyPara.FactorPower) )
        			{
    			        tempL1 = g_curRLoad[i].PRms - g_curRLoad[0].PRms;
    			        
    			        //���ж����ӵ��й����ʴ�С 11
    			        if ( tempL1 >= (g_buyPara.FactorPower) )
    			        {    			        
        			        if ( g_curRLoad[i].SRms > g_curRLoad[0].SRms )
        			        {
        			            tempL = g_curRLoad[i].SRms - g_curRLoad[0].SRms;  
        			        }
        			        else
        			        {
        			        	tempL = g_curRLoad[0].SRms - g_curRLoad[i].SRms;	
        			        }
        			        
        			        if ( tempL == 0 )
        			        {
        			        	//���ӹ��ʵĹ������� 11
        			        	tempL = 1000;	
        			        }
        			        else
        			        {
        			        	//���ӹ��ʵĹ������� 11
        			        	tempL = (tempL1*1000)/tempL; 
        			        
        			        	if ( tempL > 1000 )
        			            	tempL = 1000; 		
        			        }
        			        		        
        			        if ( tempL > (g_buyPara.FactorLimit) )
        			        { 
        			            lastCnt++;    
        			        }
 
        			    }    			        			       			    
        		    }
        		} 
        		
        		if ( lastCnt >= 5 )
        	    {        	            
        	        lastCnt = 0;          	        
        	        g_errStatus.Bit.RLoadHappened = 1;	   	            
			        g_5minutesCnt = ONE_MININUTES_CNT;	
			        
			        g_meterStatus3.Bit.MagliStatus = 1;		                          	                    	        
        	    }
    	    }    
		}	
	}
	#endif
#endif

//������բʹ�� 11
#if(OVER_POWER_RELAY_OFF_EANBLED==1) 
	//�޵繦���ж�
	if ( g_para.LimitPower != 0 )
	{
	#if(SINGLE_OR_THREE_METER==1)
		tempL = BCDToHex( (uchar*)&g_uip.PRms, 4 );
		if ( tempL >  (((uint32)g_para.LimitPower)*10) )
		{
			g_OverPowerFlagCnt++;
			if ( g_OverPowerFlagCnt > 5 )
		    {
				g_errStatus.Bit.OverPower = 1;	
				g_meterStatus3.Bit.OverPower = 1;          
                g_5minutesCnt = ONE_MININUTES_CNT;
			}
		}
		else
			g_OverPowerFlagCnt = 0;	
	#endif
	
	#if(SINGLE_OR_THREE_METER==3)		
		tempL = BCDToHex( (uchar*)&g_uip[3].PRms, 4 );
		if ( tempL >  (((uint32)g_para.LimitPower)*10) )
		{
			g_OverPowerFlagCnt++;
			if ( g_OverPowerFlagCnt > 5 )
		    {
				g_errStatus.Bit.OverPower = 1;	
				g_meterStatus3.Bit.OverPower = 1;          
                g_5minutesCnt = ONE_MININUTES_CNT;
			}
		}
		else
			g_OverPowerFlagCnt = 0;	
	#endif
	}
#endif
	
	
	
	      	        
}
/*
*****************************************************************************************
* fuction name: ISRForAD
*
* Description : AD transform interrupt service function
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
/*
#pragma vector=INTAD_vect
__interrupt void ISRForAD ( void )
{		
	DI ( );
	
	ADIF = 0;	//clear flag
	
	switch ( ADS )
	{
		case 0x07:	//	
			//here I have a buffer to avoid ad transform not accurate
			if ( ADCRH > 0xAB )		//350V 0xAB			
			{ 
			    s_volHCnt++;
			    if ( s_volHCnt > 10 )
			    {
			        ADCS = 0;
			        ADCE = 0;
			        
			        g_adSampleCnt = AD_SAMPLE_5MIN;
			        g_errStatus.Bit.OverVoltage = 1; 
			        
			        //?·?¤???1???·?�??|?�??-    
	                g_BeepDelay = BEEP_DELAY_2S;
	                g_BeepCnt = BEEP_DELAY_5MIN;   			        
			    }		
			}					
			else 
			{
				if ( s_volHCnt > 0 )
				    s_volHCnt--;   
				else
				{
				    ADCS = 0;
			        ADCE = 0; 
			        g_errStatus.Bit.OverVoltage = 0;   
				}	
				//if ( ADCRH < 0x40 ) 	//220->18v sample value is 0x6c
				    
			}
			
			break;
		
		default:
			s_volHCnt = 0;
			ADCS = 0;
			ADCE = 0;
			break;
	}
	
	EI ( );
			
	return;
}
*/	
/*
*****************************************************************************************
* fuction name: InitGlobalVariable
*
* Description : this function initializes all the global varialbes which have default 
*				values when power on.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/

void InitGlobalVariable ( void )
{
	uchar i;
	uint32 tempL;
	uchar cnt,j;
	Meter_EQuan temp_energy;
#ifdef INACITVE_POWER_ENABLE
	Meter_InactiveQuan temp_Inactive;
#endif
	
	
	g_relayFlag.Bit.RelayInTest = 0;
	g_5minutesCnt = 0;
	g_adSampleCnt = AD_SAMPLE_3S;
	
	g_overPower5Cnt = 0;			                    	
	g_1MinpowerCnt = 0;

	g_relayProCnt = 20;
    
    g_lcdCycleCnt = LCD_CYCLE_TIME;
    g_LCDFrame = 9;	    
    g_meterFlag.Bit.CycleDisp = 0;
    //g_meterFlag.Bit.CardAndPowerupDisp = 1;	


    
    g_errStatus.Byte = 0;
    FeedWatchdog  (  );	
    //��EEPROM��ȡ���� 11
	//��ַ�����Ϣ 11
    if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone), 
				(uchar*)&g_addr.Zone,CHECK_ADDMOD,Meter_Addr_Len ) != 0 )
	{
		g_errStatus.Bit.E2ROM = 1;
		
		g_addr.Zone = 0;
		for ( i=0; i<6; i++ )
			g_addr.CardNum[i] = 0x00;
		for ( i=0; i<6; i++ )
			g_addr.MeterAddr[i] = 0x11;
		
	}
	
	
	FeedWatchdog  (  );		
	//������ز�����Ϣ  11
	if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount), 
				(uchar*)&g_buyPara.BuyCount,CHECK_ADDMOD,Meter_Buy_PARA_Len ) != 0 )
	{
		g_errStatus.Bit.E2ROM = 1;
		
		g_buyPara.Status = 0;	
		g_buyPara.BuyCount = 65535;
		g_buyPara.BuyQuan = 0;
	}
	
	FeedWatchdog  (  );	
	//������Ϣ 11
	if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,LimitPower), 
				(uchar*)&g_para.LimitPower,CHECK_ADDMOD,Meter_Para_Len ) != 0 )
	{
		g_errStatus.Bit.E2ROM = 1;
			
	}
	g_relayFlag.Byte = g_para.RelayFlag;
	g_relayFlag.Bit.RelayInTest = 0;
	if ( g_relayFlag.Bit.BalanceZeroHappened )
		g_relayFlag.Bit.BalanceZero5MinFlg = 1;	
	if ( g_relayFlag.Bit.RelayAlarmOn )
		g_5minutesCnt = 5;
	//״̬��3��ֵ 11
	g_meterStatus3.Byte = 0;
	if ( g_relayFlag.Bit.RelayOn == 0)
	{
		g_meterStatus3.Bit.RelayStatus = 1; 
		#if(METER_CHIP_MODEL==0x2208)    
        	LED2_ON();
    	#endif    
    	ALARM_ON();  	
	}
		
	if ( (g_para.MeterTpye&BIT0) == BIT0 )		//����ģʽ 11
		g_meterStatus3.Bit.KeepRelay = 1; 
	if ( (g_para.MeterTpye&BIT1) == BIT1 )		//Զ����բ 11
		g_meterStatus3.Bit.RelayCmdStatus = 1;	
			
	FeedWatchdog  (  );	
	//��ȡ�й������������  11
	I2CRead ( EEPROM_ADDR,offsetof(EEPROM_DATA,Accumulate_Low), 
		(uchar*)&temp_energy.Accumulate,sizeof(Meter_EQuan), 1 );
	ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,Accumulate), 
				(uchar*)&g_energy.Accumulate,CHECK_ADDMOD,sizeof(Meter_EQuan) ) ;		
	cnt = GetSum ( (uchar*)&temp_energy.Accumulate, Meter_EQuan_Len-1 );
	if ( temp_energy.Checksum != cnt )  		 
	{
		if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,Accumulate), 
				(uchar*)&g_energy.Accumulate,CHECK_ADDMOD,sizeof(Meter_EQuan) ) != 0 )
		{				
			g_errStatus.Bit.E2ROM = 1;			
		}		
	}
	else
	{
		//���ݱȶ� 11
		if ( (temp_energy.Accumulate <= g_energy.Accumulate) )
		{
			if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,Accumulate), 
				(uchar*)&g_energy.Accumulate,CHECK_ADDMOD,sizeof(Meter_EQuan) ) != 0 )
			{				
				g_energy = temp_energy;	
			}	
		}
		else
		{
			g_energy = temp_energy;	
		}
	}

#ifdef INACITVE_POWER_ENABLE	
	FeedWatchdog  (  );	
	//��ȡ�޹������������  11
	I2CRead ( EEPROM_ADDR,offsetof(EEPROM_DATA,InactiveQuan_Low), 
		(uchar*)&temp_Inactive.InactiveQuan,Meter_InactiveQuan_Len, 1 );
	ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,InactiveQuan), 
				(uchar*)&g_Inactivenergy.InactiveQuan,CHECK_ADDMOD,Meter_InactiveQuan_Len );
	cnt = GetSum ( (uchar*)&temp_Inactive.InactiveQuan, Meter_InactiveQuan_Len-1 );
	if ( temp_Inactive.Checksum != cnt )  		 
	{
		if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,InactiveQuan), 
				(uchar*)&g_Inactivenergy.InactiveQuan,CHECK_ADDMOD,Meter_InactiveQuan_Len ) != 0 )
		{				
			g_errStatus.Bit.E2ROM = 1;
		}		
	}
	else
	{
		//���ݱȶ� 11
		if ( temp_Inactive.InactiveQuan < g_Inactivenergy.InactiveQuan)
		{
			if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,InactiveQuan), 
				(uchar*)&g_Inactivenergy.InactiveQuan,CHECK_ADDMOD,Meter_InactiveQuan_Len ) != 0 )
			{				
				g_errStatus.Bit.E2ROM = 1;
			}		
		}
		else
		{
			g_Inactivenergy = temp_Inactive;	
		}
	}
#endif
	
	//��������ز��� 11
	if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,baudrate), 
				(uchar*)&g_baudRate.baudrate,CHECK_ADDMOD,4 ) != 0 )
	{		
		g_baudRate.baudrate = 2400;	
		g_baudRate.checkType = EVEN_CHK;	
		g_baudRate.relayMode = NO_MODE;
	}
	
	
#ifdef PRE_PAID_ENABLE	
	// 11
	ProcessPrice( );
#endif	

//������բʹ�� 11
#if(OVER_POWER_RELAY_OFF_EANBLED==1)
    g_OverPowerFlagCnt = 0;
#endif

	
//����ʹ�� 11
#ifdef DEMAND_ENABLE
	I2CRead ( EEPROM_ADDR,offsetof(EEPROM_DATA,DemandPara), 
		(uchar*)&g_demand.PosActive,Meter_Demand_Len, 1 );
	cnt = GetSum ( (uchar*)&g_demand.PosActive, Meter_Demand_Len-1 );
	if ( (g_demand.Checksum != cnt) || 
		(g_demand.DemandSlide==0) ||
		(g_demand.DemandCycle==0) )
	{				
		g_demand.DemandCycle = 0x15;
		g_demand.DemandSlide = 1;
	}
	g_demand.PosActive = 0;	
	g_demand.NegActive = 0;
	g_demandSlideCnt = ((uint16)BCDToHex( (uchar *)&g_demand.DemandSlide, 1 ))*60;	
	
	//2019/12/26
//	g_demand.Q1Inactive = 0;
//	g_demand.Q2Inactive = 0;
//	g_demand.Q3Inactive = 0;
//	g_demand.Q4Inactive = 0;
#endif
	 							
	g_commLCDDisp = 0;
	g_dirFlag.Byte = 0;
			
	FeedWatchdog ( );
#ifdef RTC_8025T	
	FeedWatchdog ( );
	GetTime();
	I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,DropDateTime),
		(uchar*)&g_buf[0], 7, 1  );
	if ( (CheckMinAndSec(g_buf[0])==0) &&
	    (CheckMinAndSec(g_buf[1])==0) &&  
	    (CheckHour(g_buf[2])==0) &&  
		(CheckWeek(g_buf[3])==0) && 
	    (CheckDay31(g_buf[4])==0) && 
	    (CheckMonth(g_buf[5])==0)  )
    {		     
    	cnt = 0;
    	
    	g_buf[10] = (g_date.year>>4)*10+(g_date.year&0x0f); 
    	g_buf[11] = (g_date.month>>4)*10+(g_date.month&0x0f); 
    	g_buf[8] = (g_buf[6]>>4)*10+(g_buf[6]&0x0f); 
    	g_buf[9] = (g_buf[5]>>4)*10+(g_buf[5]&0x0f); 
    	if ( g_buf[10]>g_buf[8] )	//a11
    	{				
    		cnt = (g_buf[10]-g_buf[8])*12;
    		cnt += g_buf[11];	
    		cnt -= g_buf[9];		
    	}
    	else
    	{
    		cnt = g_buf[11]-g_buf[9];				
    	}
    
    	FeedWatchdog ( );
    	if ( cnt > 12 )
    		cnt = 12; 
    	//?�??|?�??�??μ?§????????????11
    	for ( j=0; j<cnt; j++ )
    	{
     		for ( i=0; i<11; i++ )
     		{
     			I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last11_A1)-4*i, 
     					&g_buf[0], 4, 1  ) ;
     			
     			EEPROM_NOWP();
    	    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last12_A1)-4*i, 
    	    		&g_buf[0], 4, 1  );
    			EEPROM_WP();
    			
    			FeedWatchdog ( );
     		}
     		EEPROM_NOWP();
    	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_A1), 
    	    	(uchar*)&g_energy.Accumulate, 4, 1  );
    		EEPROM_WP();
    		
    		g_energy.CurrentQ = 0;
    		
    		//
    		//����ʹ�� 11
		#ifdef DEMAND_ENABLE
			//��ʷ����ת�� 11
	 		for ( i=0; i<12; i++ )
	 		{
	 			I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last11Demand)-16*i, 
	 					&g_buf[0], 16, 1  ) ;
	 			
	 			EEPROM_NOWP();
		    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last12Demand)-16*i, 
		    		&g_buf[0], 16, 1  );
				EEPROM_WP();
				
				FeedWatchdog ( );
	 		}
	 		
	 		for ( i=0; i<16; i++ )
	 			g_buf[i] = 0;	
	 		EEPROM_NOWP();
		    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,CurDemand), 
		    	(uchar*)&g_buf[0], 16, 1  );
			EEPROM_WP();	
		#endif
    		
    		
    		////////////////////
    	}
    	
    	
    }
      		
    //��¼����  11
	for ( i=0; i<9; i++ )
	{
		I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last9DropPower)-12*i, 
				&g_buf[0], 12, 1  ) ;
		
		EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last10DropPower)-12*i, 
    		&g_buf[0], 12, 1  );
		EEPROM_WP();
		
		FeedWatchdog ( );
	}
	
   				
	I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,TotalDropPowerCnt), 
 			&g_buf[0], 3, 1  ) ;
  	tempL = BCDToHex( &g_buf[0], 3 );
  	tempL++;
  	tempL = Hex32ToBCD( tempL );
	g_buf[0] = (uchar)tempL&0xff;
	g_buf[1] = (uchar)(tempL>>8)&0xff;
	g_buf[2] = (uchar)(tempL>>16)&0xff;	
	//����ʱ�� 11
	I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,DropDateTime),
			(uchar*)&g_buf[3], 6, 1  );
    //�������ʱ�� 11
    g_buf[9] = g_date.year;
	g_buf[10] = g_date.month;
	g_buf[11] = g_date.day;
	g_buf[12] = g_date.hour;
	g_buf[13] = g_date.min;
	g_buf[14] = g_date.sec;
	EEPROM_NOWP();
    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,DropDateTime), 
    		&g_buf[0], 15, 1  );
	EEPROM_WP();

   			
#endif

#ifdef PRE_PAID_ENABLE
#if((IC_CARD_MODE==AT24_CARD) || (IC_CARD_MODE==RF_CARD))	   
    InitICCarVariables ( );
#endif
#endif	

#if(LOSS_PHASE_EANBLED==1)  	
   	s_lossPhaseCnt[0] =
   	s_lossPhaseCnt[1] = 
   	s_lossPhaseCnt[2] = 0;
#endif  

#ifdef	LADDER_ENABLE
	g_dateLadder = 0x99;
	
#endif	

	g_lcdFlag.Byte = 0;
	//g_lcdFlag.Bit.TariffDisp = 0;
	g_meterFlag.Bit.TariffProcess = 1;
	g_meterFlag.Bit.LadderProcess = 1;

#ifdef TARIFF_ENABLE	
	g_tariffOffset = 0;
#endif

	//上电默认不进行按键保电 11
	

	return;
}


/*
*****************************************************************************************
* fuction name: ProcessPrice
*
* Description : This function visualize the request item according to the diffrent condition.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
#ifdef PRE_PAID_ENABLE	
void ProcessPrice( void )
{
    
    if ( g_price != g_para.Price )
        g_price = g_para.Price;
    
#ifdef TARIFF_ENABLE        
    g_meterFlag.Bit.TariffProcess = 1;
#endif  

#ifdef TARIFF_ENABLE	
	ManageTariff ( );
#endif
	
#ifdef LADDER_ENABLE
	ManageLadder ( );	
#endif
	
	
	if ( g_energy.Balance > 0 )
	{
		if ( (g_price > MAX_PRICE) || (g_price < MIN_PRICE) )
		{
			if ( g_buyPara.Status != MANUFACTURE_STATUS )
			{
				g_price = DEFAULT_PRICE;	
				
				I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,PriceErrCnt),
					 &g_buf[0], 1, 1  ) ;
				if ( g_buf[0] < 5 )
				{
					g_buf[0]++;
					EEPROM_NOWP();
				    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,PriceErrCnt), 
				    	&g_buf[0], 1, 1  ) ;
					EEPROM_WP();
					
				}
			}
				
		}
	}



			
	return;	
    
}
#endif

/*
*****************************************************************************************
* fuction name: Hex32ToBCD
*
* Description : This function visualize the request item according to the diffrent condition.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
uint32 Hex32ToBCD( uint32 a )
{
	uint32 bcd=0; 
    uchar j=0; 
    
    j = 0;
               
    while( a > 9 )
    {
    	bcd |= ((a % 10) << j);  
    	a /= 10;          
    	j += 4;
    } 
                         
    return( bcd | (a << j) );
    
}
/*
*****************************************************************************************
* fuction name: Hex16ToBCD
*
* Description : This function visualize the request item according to the diffrent condition.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/

uint32 Hex16ToBCD( uint16 a )
{
	uint32 bcd=0;
	uint32 tempA; 
    uchar j=0; 
    
    j = 0;
	tempA = (uint32)a;               
    while( tempA > 9 )
    {
    	bcd |= ((tempA % 10) << j);  
    	tempA /= 10;          
    	j += 4;
    } 
                         
    return( bcd | (tempA << j) );
    
}

/*
*****************************************************************************************
* fuction name: BCDToHex
*
* Description : This function visualize the request item according to the diffrent condition.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/

uint32 BCDToHex( uchar *p, uchar len )
{
	uint32 h=0; 
    uchar i; 
    
    if ( len > 4 ) return 0xffffffff;
    
    for ( i=len; i>0; i-- )
    {
    	h *= 100;
    	h += (p[i-1]>>4)*10+(p[i-1]&0x0f);		
    }	
                                
    return( h );
    
}




/*
*****************************************************************************************
* fuction name: Hex8ToBCD
*
* Description : This function visualize the request item according to the diffrent condition.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/

uchar Hex8ToBCD( uchar a )
{                         
    
    return( ((a/10)<<4) | (a%10) );   
     
}

/*
*****************************************************************************************
* fuction name: GetSum
*
* Description : this function gets the checksum of the input struture data
*
* Arguments   : 1)uchar *p, input data which will be caculated 
*				2)uchar len, input data length
*
* returns     : uchar byte, the checksum of the input data.
*
* Notes     :
******************************************************************************************
*/
uchar GetSum ( const uchar *p, uchar len )
{
	uchar checkSum;
	uchar i;
	
	if ( p == NULL ) return 0;
	
	checkSum=0;
	for ( i=0; i<len; i++ )
		checkSum += *p++;
	
	return ( checkSum );

}

/*
*****************************************************************************************
* fuction name: CopyRamData
*
* Description : this function gets the checksum of the input struture data
*
* Arguments   : 1)uchar *p, input data which will be caculated 
*				2)uchar len, input data length
*
* returns     : uchar byte, the checksum of the input data.
*
* Notes     :
******************************************************************************************
*/
uchar CopyRamData ( uchar *des, uchar *src,uchar len )
{
	uchar i;
	
	if ( des == NULL ) return 1;
	if ( src == NULL ) return 1;	
	if ( len == 0 ) return 1;
	
	for ( i=0; i<len; i++ )
		*des++ = *src++;
	
	return (0);

}
	
/*
*****************************************************************************************
* fuction name: GetCrc16
*
* Description : this function gets the checksum of the input struture data
*
* Arguments   : 1)uchar *p, input data which will be caculated 
*				2)uchar len, input data length
*
* returns     : uchar byte, the checksum of the input data.
*
* Notes     :
******************************************************************************************
*/
/*
uint16 GetCrc16 ( const uchar *p, uchar len )
{
	uint16 crc16;
	uchar i,temp;
	
	
	crc16=0;
	
	for ( i=0; i<len; i++ )
	{
		temp = (uchar)(crc16&0xff);
		crc16 = (crc16>>8) ^CRC16_TABLE[(*p)^temp];		
	}
	
	temp = (uchar)(crc16&0xff);
	crc16 = (uint16)(temp<<8) | (crc16>>8);
	
	return ( crc16 );

}
*/
/*
*****************************************************************************************
* fuction name: CompareByteData
*
* Description : this function gets the checksum of the input struture data
*
* Arguments   : 1)uchar *p, input data which will be caculated 
*				2)uchar len, input data length
*
* returns     : uchar byte, the checksum of the input data.
*
* Notes     :
******************************************************************************************
*/
uchar CompareByteData ( uchar *psrc1, uchar *psrc2,uchar len)
{
	uchar i;
	
	if ( psrc1 == NULL ) return 1;
	if ( psrc2 == NULL ) return 1;
		
	for ( i=0; i<len; i++ )
	{
		if ( psrc1[i] != psrc2[i] )
			return 1;	
	}
		
	return ( 0 );

}

/*
*****************************************************************************************
* fuction name: StoreToE2ROMAndBackup
*
* Description : this function update meter parameters2 stored in EEPROM
*
* Arguments   : uchar *buf
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
uchar StoreToE2ROMAndBackup ( uint16 suba, uchar*buf,uchar len )
{  
    uchar i,trytime,flag;
    uint16 tempsuba;
    
    if ( len > 100 )
    	return 1;
    	
    trytime = 3;
    flag = 0;

    while ( trytime )
    {
	    FeedWatchdog  (  );	
	    trytime--;
	    
	    tempsuba = suba;
	    	    	    
	    //Page 1
	    EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, tempsuba, buf, len, 1  );
		EEPROM_WP();
		
		for ( i=0; i<16; i++ )
			g_buf[i]=0;	
	    I2CRead ( EEPROM_ADDR, tempsuba, &g_buf[0], len, 1  ) ;    

	    {
	    	 if ( CompareByteData ( g_buf, buf, len) != 0 )
	    	 	flag = 1;
	    }
	    

		//���������ݱ��� 11
		if ( suba < (offsetof(EEPROM_DATA,PASSWORD)+16) )
			tempsuba = suba + offsetof(EEPROM_DATA,PSTART1_B)-64;
	#ifdef TARIFF_ENABLE 
		else if ( suba == offsetof(EEPROM_DATA,TariffPrice1) )
			tempsuba = offsetof(EEPROM_DATA,TariffPrice1_B);
	#endif
	
	#ifdef LADDER_ENABLE
		else if ( (suba <= offsetof(EEPROM_DATA,LADDER_Checksum)) &&
					(suba >= offsetof(EEPROM_DATA,Ladder1)) )
			tempsuba = suba+48;
	#endif							
		else
		{
			//�ޱ��������� 11
			return 0;	
		}	
			
		EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, tempsuba, buf, len, 1 );
	    EEPROM_WP();
	    
	   	for ( i=0; i<16; i++ )
			g_buf[i]=0;	 	    	    	    
	   	I2CRead ( EEPROM_ADDR, tempsuba, g_buf, len, 1  );
	    {
	    	if ( CompareByteData ( g_buf, buf, len) != 0 )
	    		flag = 1;
	    }
	    
	    if ( flag == 0 )
	    	break;
	}
      
   	if ( trytime == 0 )
    	return 1;
    else
   		return 0;   
}

/*
*****************************************************************************************
* fuction name: StoreToE2ROM
*
* Description : this function update meter parameters2 stored in EEPROM
*
* Arguments   : uchar *buf
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
uchar StoreToE2ROM ( uint16 suba, uchar*buf,uchar len )
{  
   	    	    	    
    //Page 1
    EEPROM_NOWP();
    I2CWrite ( EEPROM_ADDR, suba, buf, len, 1  );
	EEPROM_WP();
		
    return 0;
}

/*
*****************************************************************************************
* fuction name: SaveRelayByte
*
* Description : 
*
* Arguments   : uchar *buf
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
/*
uchar SaveRelayByte ( void )
{  
   	uchar buf[2];
   	
   	buf[0] = g_relayFlag.Byte;
   	buf[1] = g_relayFlag.Byte;
   	
    //Page 1
    EEPROM_NOWP();
    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,RelayByte),
		(uchar*)&buf[0],2, 1  );
	EEPROM_WP();
	
	
    //Page 1
    EEPROM_NOWP();
    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,RelayByte_B),
		(uchar*)&buf[0],2, 1  );
	EEPROM_WP();
    

   	return 0;
}
*/

/*
*****************************************************************************************
* fuction name: ReloadFromE2ROMAndBackup
*
* Description : this function update meter parameters2 stored in EEPROM
*
* Arguments   : uchar *buf
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
uchar ReloadFromE2ROMAndBackup ( uint16 suba, uchar*buf,uchar flag,uchar len )
{  
    uchar i;
    uchar checksum,inflag;
    uint16 tempsuba;
#if(METER_CHIP_MODEL==0x2208)
    uint16 tempInt,temp,tempArray[8];
#endif
   
    FeedWatchdog  (  );	   		    
    inflag = 0;	    	    
    I2CRead ( EEPROM_ADDR, suba, &g_buf[0], len, 1  ) ;    
    {
    	if ( flag == 0 )		//����ϵ��У�� 11
    	{
    	#if(METER_CHIP_MODEL==0x2208)
    	 	for ( i=0;i<8;i++)
    	 	    tempArray[i] = (uint16)g_buf[2*i]+(((uint16)g_buf[2*i+1])<<8);
    	 	
    	 	tempInt =  GetCl2208CheckSum ( (uint16*)&tempArray[0],CALIREG_NUM ); 
    	 	temp = g_buf[CALIREG_NUM*2+1];
    	 	temp = (temp<<8) | g_buf[CALIREG_NUM*2];
    	 	if ( tempInt != temp )
    	 	{
    	 		inflag = 1;    	 		
    	 	}
    	#endif    	 	
    	}
    	else if ( flag == 1 )		//?�??????�????�??�????o?? 11
    	{
    		checksum = GetSum ( g_buf, len-1 ); 	
    		if ( g_buf[len-1] != checksum )
    	 	{
    	 		inflag = 1;    	 		
    	 	}
    	}	    	
    }
    
    if ( inflag == 0 )
    {
    	for ( i=0; i<len; i++ )
    		buf[i] = g_buf[i];
    	return 0;
    }
    
    inflag = 0; 
    //���������ݵ�ַ 11
	if ( suba < (offsetof(EEPROM_DATA,PASSWORD)+16) )
		tempsuba = suba + offsetof(EEPROM_DATA,PSTART1_B);
#ifdef TARIFF_ENABLE 
	else if ( suba == offsetof(EEPROM_DATA,TariffPrice1) )
		tempsuba = offsetof(EEPROM_DATA,TariffPrice1_B);
#endif

#ifdef LADDER_ENABLE
	else if ( (suba <= offsetof(EEPROM_DATA,LADDER_Checksum)) &&
				(suba >= offsetof(EEPROM_DATA,Ladder1)) )
		tempsuba = suba+offsetof(EEPROM_DATA,Ladder1)-
				offsetof(EEPROM_DATA,Ladder1_B);
#endif		
	
	else
	{
		//�ޱ��������� 11
		return 1;	
	}	
		  	    	    	    
   	I2CRead ( EEPROM_ADDR, tempsuba, g_buf, len, 1  );
    {
    	if ( flag == 0 )		//����ϵ��У�� 11
    	{
    	#if(METER_CHIP_MODEL==0x2208)
    	 	for ( i=0;i<8;i++)
    	 	    tempArray[i] = (uint16)g_buf[2*i]+(((uint16)g_buf[2*i+1])<<8);
    	 	
    	 	tempInt =  GetCl2208CheckSum ( (uint16*)&tempArray[0],CALIREG_NUM ); 
    	 	temp = g_buf[CALIREG_NUM*2+1];
    	 	temp = (temp<<8) | g_buf[CALIREG_NUM*2];
    	 	if ( tempInt != temp )
    	 	{
    	 		inflag = 1;    	 		
    	 	}
    	#endif     	 	
    	}
    	else if ( flag == 1 )		//?�??????�????�??�????o?? 11
    	{
    		checksum = GetSum ( g_buf, len-1 ); 	
    		if ( g_buf[len-1] != checksum )
    	 	{
    	 		inflag = 1;    	 		
    	 	}
    	}	
    }
    
    if ( inflag == 0 )
    {
    	for ( i=0; i<len; i++ )
    		buf[i] = g_buf[i];
    	return 0;
    }
	
	
	return 1;
     
}

/*
*****************************************************************************************
* fuction name: ProcessRelayStatus
*
* Description : this function operates the relay
*
* Arguments   : uchar *buf
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
void ProcessRelayStatus ( void )
{          
	//�̵������� 11
	if ( (g_para.MeterTpye&BIT0) == BIT0 )		//����ģʽ 11
	{
	    if ( g_relayChkDly == 0 )
	    {
    	    if ( g_relayFlag.Bit.RelayOn==0 )  
                g_relayFlag.Bit.RelayNeedToOperate = 1;
            else
                g_relayFlag.Bit.RelayNeedToOperate = 0;	
        }	
        
	}
	//����������բ״̬ 11
	else if ( (g_para.MeterTpye&BIT1) == BIT1 )		//Զ����բ 11
	{
		if ( g_relayChkDly == 0 )
	    {
    	    if ( g_relayFlag.Bit.RelayOn==1 )  //����Ǻ�բ״̬��Ҫ��բ 11
                g_relayFlag.Bit.RelayNeedToOperate = 1;
            else
                g_relayFlag.Bit.RelayNeedToOperate = 0;	
        }		
	}

#if(MALIGNANT_LOAD_CHK_EANBLED==1)		
	//���Ը�����բ 11
	else if ( g_errStatus.Bit.RLoadHappened==1 )
	{
		if ( g_relayChkDly == 0 )
	    {
    	    if ( g_relayFlag.Bit.RelayOn==1 )  //����Ǻ�բ״̬��Ҫ��բ 11
                g_relayFlag.Bit.RelayNeedToOperate = 1;
            else
                g_relayFlag.Bit.RelayNeedToOperate = 0;	
        }	
	}
#endif

#if(OVER_POWER_RELAY_OFF_EANBLED==1)   	
   	//���֮ǰ�ǹ��ر����Ͽ��̵������Ͽ��̵��� 11
   	else if ( g_errStatus.Bit.OverPower )
   	{
   		if ( g_relayChkDly == 0 )
	    {
    	    if ( g_relayFlag.Bit.RelayOn==1 )  //����Ǻ�բ״̬��Ҫ��բ 11
                g_relayFlag.Bit.RelayNeedToOperate = 1;
            else
                g_relayFlag.Bit.RelayNeedToOperate = 0;	
        }		
   	}
#endif
	
	//增加按键保电功能 11
	else if ( g_meterStatus3.Bit.BtnKeepRelay )
	{
		if ( g_relayChkDly == 0 )
	    {
    	    if ( g_relayFlag.Bit.RelayOn==0 )  
                g_relayFlag.Bit.RelayNeedToOperate = 1;
            else
                g_relayFlag.Bit.RelayNeedToOperate = 0;	
        }	
	}

#ifdef PRE_PAID_ENABLE
	else  // 11
	{
		//����ģʽ ����ʣ�����ж� 11
		//added by Roger 2015/07/24
		if ( g_buyPara.Status == BACK_MONEY_STATUS )
		{
			if ( g_relayFlag.Bit.RelayOn )  
	            g_relayFlag.Bit.RelayNeedToOperate = 1;
	        else
	            g_relayFlag.Bit.RelayNeedToOperate = 0;
	       	
	       	g_meterStatus3.Bit.StatusMeter = 1; 
		}		
		//����ʣ����������̵���  11
		else if ( (g_relayFlag.Bit.RelayInTest == 0) &&
		    (g_relayFlag.Bit.RelayAlarmOn==0) )
	    {
	        if ( g_energy.Balance> 0)
	        {
	            if ( g_relayFlag.Bit.RelayOn==0 )  
	                g_relayFlag.Bit.RelayNeedToOperate = 1; 
	            else
	                g_relayFlag.Bit.RelayNeedToOperate = 0;  
	                
	           	g_meterStatus3.Bit.NoMoney = 0;     
	        }
	        else
	        {
	            //û�з�����ʣ����Ϊ0 11 11
	            if ( g_relayFlag.Bit.BalanceZeroHappened == 0 )
	            {
	                g_5minutesBalanceCnt = FIVE_MININUTES_CNT; 
	                g_relayFlag.Bit.BalanceZeroHappened = 1;
	                g_relayFlag.Bit.RelayOn = 1;
    	            g_relayFlag.Bit.RelayNeedToOperate = 1;   
    	            
    	            g_meterStatus3.Bit.NoMoney = 1;  
	            }
	            else //������ 11
	            {    	            
    	            //��Ƿ�޶����0��������Ƿ 11
    	            if ( g_para.CreditLimit > 0 )
    	            {
    	            	if ( g_relayFlag.Bit.BalanceZero5MinFlg==1 )
    	            	{
	    	                //����������޶����բ 11
	    	           	#if(NEW_PROTOCOL_2018==1)
	    	           		if ( g_energy.Overdraft > (g_para.CreditLimit*100) )
	    	           	#else
	    	           		if ( g_energy.Overdraft > (g_para.CreditLimit*10000) )
	    	           	#endif	                
	    	                {
	    	                    //��բ 11
	    	                    if ( g_relayFlag.Bit.RelayOn )  
	            	                g_relayFlag.Bit.RelayNeedToOperate = 1;
	            	            else
	            	                g_relayFlag.Bit.RelayNeedToOperate = 0;   
	            	                
	            	           	g_meterStatus3.Bit.NoMoney = 1;    	                    
	    	                } 
	    	                else
	    	                {
	    	                	//��բ 11
	    	                	if ( g_relayFlag.Bit.RelayOn == 0 )  
	            	                g_relayFlag.Bit.RelayNeedToOperate = 1;
	            	            else
	            	                g_relayFlag.Bit.RelayNeedToOperate = 0;	
	    	                }
	    	           	}   
    	            }
    	            else
    	            {
    	                if ( g_relayFlag.Bit.RelayOn )  
        	                g_relayFlag.Bit.RelayNeedToOperate = 1;
        	            else
        	                g_relayFlag.Bit.RelayNeedToOperate = 0;    	                
    	            }  
        	        
    	        }          
	        }    
	    }
	    
  
	    //�����ƴ���  11      
	    //if ( g_2208ErrCnt < 5 )
	    {
		    if ( (g_energy.Balance==0)  
		    	|| (g_energy.Overdraft>0) 
		    	|| (g_relayFlag.Bit.RelayOn==0) 
		#if(MALIGNANT_LOAD_CHK_EANBLED==1)	
				|| (g_errStatus.Bit.RLoadHappened==1) 
		#endif    	
		#if(OVER_POWER_RELAY_OFF_EANBLED==1)	    	
		 		|| (g_errStatus.Bit.OverPower==1)     	
		#endif     	
		    	)
		    	ALARM_ON();		 
		    else
		    {
			    ALARM_OFF();		
		    	//��������ж� 11
			    if ( g_buyPara.Alarm1 > 0 )	
			    {
			    	if ( g_energy.Balance <= (((uint32)g_buyPara.Alarm1)*10000) )
			    	{
			    		ALARM_ON();			
			    	}	
				} 	
				//�������2�ж� 11
		    	if ( g_buyPara.Alarm2 > 0 )	
			    {	    		    		    		
			    	if ( g_relayFlag.Bit.RelayAlarmHappened==0 )
			    	{
				    	if ( g_energy.Balance <= (((uint32)g_buyPara.Alarm2)*10000) )
				        {
				            g_relayFlag.Bit.RelayAlarmOn = 1;           
				            g_relayFlag.Bit.RelayAlarmHappened = 1;		
				            
				            g_relayFlag.Bit.RelayOn = 1;                         
				            g_relayFlag.Bit.RelayNeedToOperate = 1;  
				            
				            g_5minutesCnt = FIVE_MININUTES_CNT;   
				            ALARM_ON();	
				            
				            g_meterStatus3.Bit.Alarm2Status = 1;  
				            
				            //����̨�����֣���բ�����̫�� ������ 2016/01/06  11
				            g_energy.Checksum = GetSum ( (uchar*)&g_energy.Accumulate, Meter_EQuan_Len-1 );     
		        			StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Accumulate),
								(uchar*)&g_energy.Accumulate,Meter_EQuan_Len );	    
				        }
				    }
			    } 
			        	
		    }
	    }
		//else
		//{
			//ALARM ^= 1;
		//}
	}	
	
#else		
	//��Ԥ���ѱ� ����Ӧ���Ǻ�բ 11
	else
	{
		if ( g_relayChkDly == 0 )
	    {
    	    if ( g_relayFlag.Bit.RelayOn==0 )  
                g_relayFlag.Bit.RelayNeedToOperate = 1;
            else
                g_relayFlag.Bit.RelayNeedToOperate = 0;	
        }			
	}	
#endif    
        		
    return;   
}

/*
*****************************************************************************************
* fuction name: ProcessRelay
*
* Description : this function operates the relay
*
* Arguments   : uchar *buf
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
void ProcessRelay ( void )
{          
    if ( g_relayFlag.Bit.RelayNeedToOperate == 0 )
        return;
    g_relayFlag.Bit.RelayNeedToOperate = 0;
  	
  	LCDLED_OFF();
  	SaveEnergy ( NORMAL_SAVE );
  	 	
    g_relayCnt = RELAY_DELAY_CNT;
    g_relayProCnt = 8;
    g_relayChkDly = 8;
    
    
    
    if( g_relayFlag.Bit.RelayOn )   //relay off  11
    {
        RelayA_L;    
        RelayB_H;
        g_relayFlag.Bit.RelayOn = 0; 
    
    #ifdef RELAY_OUT_CONTROL	    
    	if ( (g_para.MeterTpye&BIT5) == BIT5 )		//��ƽʽ 11
    	{
    		g_relayCnt = 0;			
    	}
    	else
    	{
    		g_relayCnt = 400;		//4*400=1600ms
    	}
    #endif
    	
    	g_meterStatus3.Bit.RelayStatus = 1;
    	
    #if(METER_CHIP_MODEL==0x2208)    
        LED2_ON();
    #endif 
    
    	ALARM_ON();              
    }
    else        //relay on 11
    { 
    #ifdef RELAY_OUT_CONTROL 
    	RelayA_L;    
        RelayB_L;
        g_relayFlag.Bit.RelayOn = 1;	
    #else                            
        RelayA_H;    
        RelayB_L;      
        g_relayFlag.Bit.RelayOn = 1;
   	#endif 
   	    
     #if(METER_CHIP_MODEL==0x2208)    
        LED2_OFF();
    #endif   
    	
    	ALARM_OFF();
    	g_meterStatus3.Bit.RelayStatus = 0;  
    }
    
    //�����ı��豣�� 11
    g_relayFlag.Bit.StatusToSave = 1;
        		
    return;   
}	


/*
*****************************************************************************************
* fuction name: ISREXLVI
*
* Description : this function is external voltage detect interrupt service. This interrupt 
*				service rountine is just for wake up
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISREXLVI ( void )
{
	DI ( );
	
	LVIIF = 0;
	
	EI ( );
	
}

/*
*****************************************************************************************
* fuction name: CompareEXLVI
*
* Description : this function judge whether is truely power drop. 
*
* Arguments   : none
*
* returns     : a bit, 1 represents power drop already.
*		       0 represents power normal.
*
* Notes     :
******************************************************************************************
*/
uchar CompareEXLVI ( void )
{	
	uchar cmp_num,i;
    
    //DI();
	for(cmp_num=10;cmp_num>0;cmp_num--)
	{
		if ( LVIF )
		{
			for(i=0;i<200;i++);		//???§?1???μ?�??μ?§?�??2?????????????2?�?10?�??? 11			
		}
		else
			break;					//?????μ?�??μ?§?�??2?·?μ???? 11
	}
    //EI();
	if(cmp_num==0)
		return(1);
	else
		return(0);	
}

#ifdef RTC_8025T
/*
*****************************************************************************************
* fuction name: ProcessLowPower
*
* Description : this function process low power 
*
* Arguments   : uchar *buf
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
void ProcessLowPower ( void )
{          	
	uint32 tempL;
	uchar i;
	
		
	
//	uchar buftes[3];
//	
//	buftes[0] = 0x00;
//	buftes[1] = 0x00;
//	buftes[2] = 0x40;
//   	I2CWrite ( RX8025T_ADDR, EXTENTION_REG,buftes, 3, 1  );
		
	//
	LCDM1 &= 0x3F;    //display off 

#if(METER_CHIP_MODEL==0x2208)	
	//P0 port(P0.0~P0.7), P0.0->Card_Power,output;P0.1->Card_RST,output;
	//P0.2->Card_CLK,output;P0.3->CPU_MTR_RXD,input;
	//P0.4->CPU_MTR_TXD,output;P0.5->485CTL,output,
	//P0.6->485RXD,input,P0.7->485TXD,output
	PM0 = 0;
	P0 = 0;
#else
	//P0 port(P0.0~P0.7), P0.0->Card_Power,output;P0.1->Card_RST,output;
	//P0.2->Card_CLK,output;P0.3->SCLK,output;
	//P0.4->ccs,output;P0.5->485CTL,output,
	//P0.6->485RXD,input,P0.7->485TXD,output
	PM0 = 0;
	P0 = 0;
#endif	
		
	//P1 port(P1.0~P1.7), P1.0->SEG35,output;P1.1->SEG36,output;
	//P1.2->Card_SDA,output;P1.3->Card_CTE,input;
	//P1.4->HWTXD,output;P1.5->JA,output,P1.6->JB,output,P1.7->HWRXD,input 11
	PM1 = 0;
	P1 = 0;
		
	//P2 port(P2.0~P2.2 P2.6 P2.7), P2.0->BEEP,output;P2.1->WP,output;
	//P2.2->BATIN,input;P2.6->SEG33,output;P2.7->SEG34,output;
	PM2 = BIT2;
	P2 = BIT1;
		
	//P3 port(P3.0~P3.3), P3.0~P3.3->SEG17~SEG20,output
	PM3 = 0;
	P3 = 0;
	
#if(METER_CHIP_MODEL==0x2208)		
	//P4 port(P4.0~P4.3), P4.0->TOOL0,output;P4.2->PLED,output,
	//P4.3->SWITCH,output,P4.4->ALARM,output,P4.5->LCDLED,output,
	PM4 = 0;
	P4 = BIT0;
#else
	//P4 port(P4.0~P4.3), P4.0->TOOL0,output;P4.2->M-SDO,output,
	//P4.3->CF2,input,P4.4->SLED,output,P4.5->LCDLED,output,
	PM4 = BIT3;
	P4 = BIT0+BIT3;
#endif	
	
	//P5 port(P5.2~P5.7), P5.2->SEG6,output;P5.3->SEG7,output;
	//P5.4->SEG8,output;P5.7->SEG11,output;
	PM5 = 0;
	P5 = 0;
	
	//P6 port(P6.0~P6.1), P6.0->SCL,output;P6.1->SDA,output;
	PM6 = 0;
	P6 = BIT0+BIT1;
	
	//P7 port(P7.0~P7.4), P7.0->SEG11,output;P7.4->SEG16,output;
	//P7.5->SEG17,output;P7.6->SEG18,output;P7.7->SEG19,output;
	PM7 = 0;
	P7 = 0;

#if(METER_CHIP_MODEL==0x2208)	
	//P12 port(P12.1~P12.7), P12.1->Dis_key,input;P12.2->Pro_KEY,input;
	//P12.3->XT2,input;P12.4->XT1,input;
	//P12.5->VL3,output,P12.6->CAPL,input,P12.7->CAPH,input	
	PM12 = BIT0+BIT1;
	P12 = BIT0+BIT1;
#else	
	//P12 port(P12.1~P12.7), P12.1->KEY1-DOWN,input;P12.2->KEY2-UP,input;
	//P12.3->XT2,output;P12.4->M-SDI,input;
	//P12.5->VL3,output,P12.6->CAPL,input,P12.7->CAPH,input	
	PM12 = BIT0+BIT1+BIT2;
	P12 = BIT0+BIT1+BIT2;
#endif					
	//11
    //IC_CHK interrupt 

#if(PCB_VERSION_NEW==0)		
	while ( LVIF==1 )
	{
		FeedWatchdog (  );
		STOP();					
	}			
	//11
	while(1);
#else	//
	g_btnFreezeLCD = 0;
	
	
	while ( LVIF==1 )
	{
		FeedWatchdog (  );
		if ( g_btnFreezeLCD == 0 )
		{
			LCDM1 &= 0x3F;
			STOP();		
		}
					
		//wake up 11
		LCDM1 |= 0XC0;
		
		ManageLCDDisplay ( );
								
	}	
	
	while(1);			
#endif
	
	return;
	 
}	
#endif

/*
*****************************************************************************************
* fuction name: GetTime
*
* Description : ???±???????±???¤ 
*
* Arguments   : uchar *buf
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
#ifdef RTC_8025T
void GetTime ( void )
{  
	uchar buf1[7];
	
	I2CRead ( RX8025T_ADDR, START_ADDR, buf1, 7, 1  );
	
	g_date.sec = buf1[0];
	g_date.min = buf1[1];
	g_date.hour = buf1[2];
	
	g_date.week = buf1[3];
	
	g_date.day = buf1[4];
	g_date.month = buf1[5];
	g_date.year = buf1[6];
}
#endif

/*
*****************************************************************************************
* fuction name: SetTime
*
* Description : ?�??¨?�??????±???¤ 
*
* Arguments   : uchar *buf
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
#ifdef RTC_8025T
uchar SetTime ( uchar* pbuf )
{  
    uchar buftes[3];
	
	buftes[0] = 0x00;
	buftes[1] = 0x00;
	buftes[2] = 0x00;
   	if ( I2CWrite ( RX8025T_ADDR, EXTENTION_REG,buftes, 2, 1  ) != 1 )
   		return 1;
   		
	if ( I2CWrite ( RX8025T_ADDR, START_ADDR, pbuf, 7, 1  ) != 1 )
		return 1;
	
	g_date.sec = pbuf[0];
	g_date.min = pbuf[1];
	g_date.hour = pbuf[2];
	
	g_date.week = pbuf[3];
	
	g_date.day = pbuf[4];
	g_date.month = pbuf[5];
	g_date.year = pbuf[6];
	
	return 0;
	
}
#endif	

#if(METER_CHIP_MODEL==0x2208)
/*
*****************************************************************************************
* fuction name: GetCl2208CheckSum
*
* Description : this function initialize the cl2208 chip
*
* Arguments   : 1)sla: The I2C slave id. The bit0 should be set if it is a read access.
*		2)suba: The byte address to be accessed
*		3)*s:   The read/write data buffer in idata
*		4)len:	The amount of bytes to be accessed.
*
* returns     : uchar
*
* Notes     : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uint16 GetCl2208CheckSum ( uint16 *p,uchar len )
{
	uchar i;
	uint16 ret;
	
	ret = 0;
	for ( i=0; i<len; i++ )
	{
		ret += p[i];
				 
	}	
	
	ret += 0x0120;
	ret += 0x1603;
	ret += 0x0003;
	return (0xFFFF - ret);

	
}	
#endif			