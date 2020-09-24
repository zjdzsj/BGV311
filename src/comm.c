/******************************************************************************************
*  Copyright (c) 2007, KO Design Department 
*  All rights reserved 
*
*  FileName��   	comm.c
*  Author��     	Bona Co,LTD..
*  Date��  			2010-6-22 23:56:47
*  Revised Date��  	2010-6-22 23:56:50
*  Main Function��  this file contains all the functions used in IR communications
*					
*
*  Version:			BoanV3.1
*  Note�� 			      			
*********************************************************************************************/
#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

#pragma interrupt INTSR1	ISRFor485Rxd			//uart1�����ж� 11
#pragma interrupt INTSRE1	ISRFor485RcvErr		    //uart1���մ����ж� 11
#pragma interrupt INTST1	ISRFor485Txd		    //uart1�����ж� 11 11

#pragma interrupt INTTM05   ISRForIRTxd

#include "macro.h"
#if(PCB_VERSION_NEW_4==0)
	#pragma interrupt INTSR0	ISRForIRRxd			//uart0�����ж� 11 11
	#pragma interrupt INTSRE0	ISRForIRRcvErr		    //uart0���մ����ж� 11	
#else
	#pragma interrupt INTP5	ISRForIRStartRxd
	#pragma interrupt INTTM03 ISRForTimer3
	
	#if(IC_CARD_MODE==PLC_COMM_USED)
		#pragma interrupt INTSR0	ISRForPLCRxd			//uart0�����ж� 11
		#pragma interrupt INTSRE0	ISRForPLCRcvErr		    //uart0���մ����ж� 11
		#pragma interrupt INTST0	ISRForPLCTxd		    //uart0�����ж� 11 11
	#endif
#endif


/************************************************************************************************/
#define _COMM_DEFINE_
#include "include.h"
/************************************************************************************************/

#define IR_RCV_PIN			(P0.1)

static uchar CheckPassword ( const uchar *pdata );
static uchar GetChecksum ( uchar* buf, uchar len );
static uchar ProcessSelling( uchar *pchar, uchar len, uchar channel );
static uchar UpdatePara ( uchar* pbuf , uchar channel);
static uchar UpdateTariffPara ( uchar* pbuf, uchar channel );
static uchar UpdateTariffTable ( uchar* pbuf, uchar channel, uchar offset );
static uchar UpdateLadderPara ( uchar* pbuf, uchar channel );
static uchar ReturnbackPara ( uchar* pbuf, uchar errCode, uchar orderLen );
#if(MODBUS_ENABLED==1)
	static uint16 GetCRC16 (  uchar *pindata, uchar len );
	static uchar DecodeModbusMessage ( uchar channel, uchar rxdLen );
	static uchar BuildModbusMessage ( uchar channel );
#endif



//only used in IR.c file
/************************************************************************************************/
//variables definition
static uchar s_485SendHead;
static uchar s_485SendEnd;
static uchar s_485Buf[BUF_MAX_LEN];

static uchar s_IRSendHead;
static uchar s_IRSendEnd;
static uchar s_IRBuf[BUF_MAX_LEN];

#if(PROTOCOL_ADD_4_SERIALS==1)
	static uchar s_serials[4];
	static uchar s_serialsLen[3];
#endif

#if(IC_CARD_MODE==PLC_COMM_USED)
	static COMM_PARA s_commPara[3];			//ͨ��ʹ�ò��� 11
	static COMM_ERROR s_commError[3]; 	//ͨ�Ŵ���״̬�� 1 11
	
	static uchar s_PLCSendHead;
	static uchar s_PLCSendEnd;
	static uchar s_PLCBuf[BUF_MAX_LEN];
#else
	static COMM_PARA s_commPara[2];			//ͨ��ʹ�ò��� 11
	static COMM_ERROR s_commError[2]; 	//ͨ�Ŵ���״̬�� 1 11
#endif

//IR �����õ���varialbes 11
static uchar s_irBit;
static uchar s_IRTxdState;
static uchar s_IRParity;
static uchar s_IRByte;

static uchar s_IRRxdBitState;
static uchar s_IRRxdByte;
static uchar s_IRRxdParity;
static uchar s_IRErrByte;

static uchar s_IRState;

static uchar s_IRRcvFlag;

#if(METER_CHIP_MODEL==0x2208)
const uint16 CALIBRATE_TABLE_value[]={
	//0x0003,			//SYSCON 00
	//0x0003,			//EMUCON 01	
	0x0062,			//PSTART 03
	0x0000,			//GPQA 05
	0x0000,			//PHASE 07  1�ֽ� 11
	
	0x0000,			//APOR 0A
	0x0000,			//IARMSOS 0E	
	0x1000,			//HFCONST 02  0x1C00
	0X0000,
	
};
#endif

#if(MODBUS_ENABLED==1)
static const ModBus_RTU_ARRAY ModBUS_talbe1[]={

#if(SINGLE_OR_THREE_METER==3)
    //Reg    RegLen   Type 		DotPos	PointerType	RamAddr   
    0x0000,     2,     BCDType,	DOT1,   P_LONG,		(uint16)&g_uip[0].URms,	//A���ѹ 2	 11	
    0x0002,     2,     BCDType,	DOT1,   P_LONG,		(uint16)&g_uip[1].URms,	//B���ѹ 2	 11	
    0x0004,     2,     BCDType,	DOT1,   P_LONG,		(uint16)&g_uip[2].URms,	//C���ѹ 2	 11	
     
    //Reg    RegLen   Type 		DotPos	PointerType	RamAddr   
    0x0006,     2,     BCDType,	DOT3,   P_LONG,		(uint16)&g_uip[3].IRms,	//�ܵ��� 3 11
    0x0008,     2,     BCDType,	DOT3,   P_LONG,		(uint16)&g_uip[0].IRms,	//A����� 3 11
    0x000A,     2,     BCDType,	DOT3,   P_LONG,		(uint16)&g_uip[1].IRms,	//A����� 3 11
    0x000C,     2,     BCDType,	DOT3,   P_LONG,		(uint16)&g_uip[2].IRms,	//A����� 3 11
    
    0x000E,     2,     BCDType,	DOT0,   P_VOID,		(uint16)&g_uip[2].IRms,	//A����� 3  ���11
    
    //Reg    RegLen   Type 		DotPos	PointerType	RamAddr   
    0x0010,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[3].PRms,	//���๦�� 3	
    0x0012,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[0].PRms,	//A�๦�� 3 11
    0x0014,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[1].PRms,	//B�๦�� 3 11
    0x0016,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[2].PRms,	//C�๦�� 3 11

#ifdef INACITVE_POWER_ENABLE    
    //Reg    RegLen   Type 		DotPos	PointerType	RamAddr   
    0x0018,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[3].QRms,	//�����޹����� 3	
    0x001A,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[0].QRms,	//A���޹����� 3
    0x001C,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[1].QRms,	//B���޹����� 3
    0x001E,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[2].QRms,	//C���޹����� 3
    
    //Reg    RegLen   Type 		DotPos	PointerType	RamAddr   
    0x0020,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[3].SRms,	//�������ڹ��� 3	
    0x0022,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[3].SRms,	//A�����ڹ��� 3
    0x0024,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[3].SRms,	//B�����ڹ��� 3
    0x0026,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip[3].SRms,	//C�����ڹ��� 3    
#endif

	//Reg    RegLen   Type 		DotPos	PointerType	RamAddr   
    0x0028,     2,     BCDType,	DOT3,   P_INT,		(uint16)&g_uip[3].Factor,	//���๦������ 2  11	
    0x002A,     2,     BCDType,	DOT3,   P_INT,		(uint16)&g_uip[0].Factor,	//A�๦������ 2  11	
    0x002C,     2,     BCDType,	DOT3,   P_INT,		(uint16)&g_uip[1].Factor,	//B�๦������ 2  11	
    0x002E,     2,     BCDType,	DOT3,   P_INT,		(uint16)&g_uip[2].Factor,	//C�๦������ 2  11	

	//Reg    RegLen   Type 		DotPos	PointerType	RamAddr   	
    0x0030,     2,     BCDType,	DOT2,   P_INT,		(uint16)&g_uip[0].PhaseAngel,	//A����� 2  11	
    0x0032,     2,     BCDType,	DOT2,   P_INT,		(uint16)&g_uip[1].PhaseAngel,	//B����� 2  11	
    0x0034,     2,     BCDType,	DOT2,   P_INT,		(uint16)&g_uip[2].PhaseAngel,	//C����� 2  11
	
	0x0036,     2,     BCDType,	DOT2,   P_INT,		(uint16)&g_freqency,	//Ƶ�� 2  11
	
	//Reg    RegLen   Type 		DotPos	PointerType	RamAddr   	
    0x0100,     2,     HexType,	DOT2,   P_LONG,		(uint16)&g_energy.Accumulate,	//(��ǰ)����й��ܵ��� 11
    0x0200,     2,     HexType,	DOT2,   P_LONG,		(uint16)&g_energy.PosAccumulate,//(��ǰ)�����й��ܵ��� 11
	0x0300,     2,     HexType,	DOT2,   P_LONG,		(uint16)&g_energy.NegAccumulate,//(��ǰ)�����й��ܵ��� 11

#ifdef INACITVE_POWER_ENABLE 	
	0x0400,     2,     HexType,	DOT2,   P_LONG,		(uint16)&g_energy.InactiveQuan,	//(��ǰ)����޹��ܵ��� 11
	0x0500,     2,     HexType,	DOT2,   P_LONG,		(uint16)&g_energy.PosInactiveAcc,	//(��ǰ)�����޹��ܵ��� 11
	0x0600,     2,     HexType,	DOT2,   P_LONG,		(uint16)&g_energy.NegInactiveAcc,	//(��ǰ)�����޹��ܵ��� 11
#endif

#ifdef DEMAND_ENABLE	
	0x0700,     2,     HexType,	DOT2,   P_LONG,		(uint16)&g_demand.PosActive,	//(��ǰ)�����й�������� 11
	0x0800,     2,     HexType,	DOT2,   P_LONG,		(uint16)&g_demand.NegActive,	//(��ǰ)�����й�������� 11
	0x0900,     2,     HexType,	DOT2,   P_LONG_2,	(uint16)&g_demand.Q1Inactive,	//(��ǰ)�����޹�������� 11
	0x0A00,     2,     HexType,	DOT2,   P_LONG_2,	(uint16)&g_demand.Q3Inactive,	//(��ǰ)�����޹�������� 11
#endif
   
#endif

////////////////////////////////////////
#if(SINGLE_OR_THREE_METER==1)
	0x0000,     2,     BCDType,	DOT1,   P_LONG,		(uint16)&g_uip.URms,	//A���ѹ 2	 11	
	0x0008,     2,     BCDType,	DOT3,   P_LONG,		(uint16)&g_uip.IRms,	//A����� 3 11
	0x0012,     2,     BCDType,	DOT4,   P_LONG,		(uint16)&g_uip.PRms,	//A�๦�� 3 11
	0x002A,     2,     BCDType,	DOT3,   P_INT,		(uint16)&g_uip.Factor,	//A�๦������ 2  11	
	0x0036,     2,     BCDType,	DOT2,   P_INT,		(uint16)&g_uip.Freq,	//Ƶ�� 2  11
	0x0100,     2,     HexType,	DOT2,   P_LONG,		(uint16)&g_energy.Accumulate,	//(��ǰ)����й��ܵ��� 11
#endif
/////////////////////////////////////////
};

#define MB_TBL1_NUM  (sizeof(ModBUS_talbe1)/sizeof(ModBus_RTU_ARRAY)) //sizeof(ModBUS_talbe)

//static const ModBus_RTU_ARRAY ModBUS_talbe2[]={
//
//#if(SINGLE_OR_THREE_METER==3)
//    //Reg    RegLen   Type 		DotPos	PointerType	RamAddr   
//    0x0000,     2,     BCDType,	DOT1,   P_LONG,		(uint16)&g_uip[0].URms,	//������	 11	
//    0x0002,     2,     BCDType,	DOT1,   P_LONG,		(uint16)&g_uip[1].URms,	//У��λ 11	
//    0x0008,     2,     BCDType,	DOT1,   P_LONG,		(uint16)&g_uip[2].URms,	//ͨѶ��ַ	 11	
//	0x0010,     2,     BCDType,	DOT1,   P_LONG,		(uint16)&g_uip[2].URms,	//DO���	 11	
//#endif
//
//};
//#define MB_TBL2_NUM  (sizeof(ModBUS_talbe2)/sizeof(ModBus_RTU_ARRAY)) //sizeof(ModBUS_talbe)
#endif

/************************************************************************************************/


/*
*****************************************************************************************
* fuction name: InitRS485
*
* Description : this function intializes hardware of Uart
*
* Arguments   : none
*
* returns     : none
*
* Notes     : revised 07/07/05, add notes only
******************************************************************************************
*/
void InitRS485 ( void )
{			
	ST0 |= 0x000c;
						
	//���ò���ģʽ TXD1 0��Ԫͨ��2  11
	SMR02 = 0x0022;		//TXD
	SMR03 = 0x0122;		//RXD
	
	if ( g_baudRate.checkType == EVEN_CHK )
	{
		//����ͨ�Ÿ�ʽ 11
		SCR02 = 0x8297;		//żУ�� 8λ���� 1λֹͣλ  11
		SCR03 = 0x4297;		
	}
	else if ( g_baudRate.checkType == ODD_CHK )
	{
		//����ͨ�Ÿ�ʽ 11
		SCR02 = 0x8397;		//��У�� 8λ���� 1λֹͣλ  11
		SCR03 = 0x4397;		
	}
	else //if ( g_baudRate.checkType == NONE_CHK )
	{
		//����ͨ�Ÿ�ʽ 11
		SCR02 = 0x8097;		//��У�� 8λ���� 1λֹͣλ  11
		SCR03 = 0x4097;		
	}
	
	//���ò����� 11
	if ( g_baudRate.baudrate == 1200 )
	{
		//���ò���ʱ�� 2400bps,ѡ��125kHz=4MHz/32 CK00=CK01 11
		SPS0 = 0x0055;
		SDR02 = 51<<9;		//125kHz/((51+1)*2)=1.2
		SDR03 = 51<<9;		//125kHz/((51+1)*2)=1.2
	}	
	else if ( g_baudRate.baudrate == 4800 )
	{
		//���ò���ʱ�� 2400bps,ѡ��125kHz=4MHz/32 CK00=CK01 11
		SPS0 = 0x0055;
		SDR02 = 12<<9;		//125kHz/((12+1)*2)=4.8
		SDR03 = 12<<9;		//125kHz/((12+1)*2)=4.8
	}
	else if ( g_baudRate.baudrate == 9600 )
	{
		//���ò���ʱ�� 2400bps,ѡ��250kHz=4MHz/16 CK00=CK01 11
		SPS0 = 0x0044;
		SDR02 = 12<<9;		//250kHz/((12+1)*2)=9.6
		SDR03 = 12<<9;		//2505kHz/((12+1)*2)=9.6
	}
	else //if ( g_baudRate.baudrate == 2400 )
	{
		//���ò���ʱ�� 2400bps,ѡ��125kHz=4MHz/32 CK00=CK01 11
		SPS0 = 0x0055;
		SDR02 = 0x3200;		//125kHz/((25+1)*2)=2.4
		SDR03 = 0x3200;		//125kHz/((25+1)*2)=2.4
	}
	
	//���������ƽ 11 
	SOL0 = 0;		//������  11
	
	//���ô��������ʼ��ƽ 11
	SO0 = 0x0f0f;		//1
	
	//��������������� 11
	SOE0 |= 0x0004;		
	
	//�����˲����������� 11
	NFEN0 = 0x15;		//RXD1 RXD2
	
	//���ö˿� 11
	//PIM0 = 0x00;
	//POM0 = 0x01;
	
	Delay1MsTime ( 1 );
	//����ͨ����ʼ  11
	SS0 |= 0x000c;
		
	SIR02 = 0x0007;
	SIR03 = 0x0007;
			
	//�ж�����  11
	SRIF1 = 0;
	SREIF1 = 0;
	STIF1 = 0;
	SRMK1 = 0;
	SREMK1 = 0;
	STMK1 = 1;
	
	
	RS485CON_RXD();
	
  	return;
}


/*
*****************************************************************************************
* fuction name: InitPLCComm
*
* Description : this function intializes hardware of Uart0
*
* Arguments   : none
*
* returns     : none
*
* Notes     : revised 2020/04/07
******************************************************************************************
*/
#if(IC_CARD_MODE==PLC_COMM_USED)
void InitPLCComm ( void )
{			
	ST0 |= 0x0003;
						
	ISC = 0; 
    
    SMR00 = 0x0022;		//RXD 11
    SMR01 = 0x0122;		//TXD 11
	
	//����ͨ�Ÿ�ʽ 11
	SCR00 = 0x8297;	    //żУ�� 8λ���� 1λֹͣλ  11
	SCR01 = 0x4297;	    //żУ�� 8λ���� 1λֹͣλ  11
	
	
	if ( g_baudRate.baudrate == 9600 )
	{
		SDR00 = 51<<9;		//250kHz/((51+1)*2)=2.4
		SDR01 = 51<<9;		//250kHz/((51+1)*2)=2.4	
	}
    else
    {
    	SDR00 = 0x3200;		//125kHz/((25+1)*2)=2.4
		SDR01 = 0x3200;		//125kHz/((25+1)*2)=2.4
    }
	Delay1MsTime ( 1 );
    	
	SS0 |= (1<<1)+(1<<0);  // enable UART0 receive and TRANS  11
    
    //���������ƽ 11 
	SOL0 = 0;		//������  11
	
	//���ô��������ʼ��ƽ 11
	SO0 = 0x0f0f;		//1
	
	//��������������� 11
	SOE0 |= 0x0001;	
	
    // ����ͨ��1������RX 11
    SIR00 = 0x07;    // clear error flag 11 
    SIR01 = 0x07;    // clear error flag 11 
    
    SRIF0 = 0U;     // clear INTSR0 interrupt flag  11
    SRMK0 = 0U;     // enable INTSR0 interrupt  11
    SREIF0 = 0U;    // clear INTSRE0 interrupt flag  11
    SREMK0 = 0U;    // enable INTSRE0 interrupt  11
	STIF0 = 0;
	STMK0 = 1;
	
  	return;
}
#endif

/*
*****************************************************************************************
* fuction name: InitRS485AndIR
*
* Description : this function intializes hardware of Uart
*
* Arguments   : none
*
* returns     : none
*
* Notes     : revised 07/07/05, add notes only
******************************************************************************************
*/
void InitIR ( void )
{			
#if(PCB_VERSION_NEW_4==0)	
	//Now we initialize IR interface
								
	//IR 38k wave generate initialize, here we use timer51 to get 833us interrupt
	//13us(38kHz) output pin is used pin P1.7/TO50(this part initializing will be transmit function	
	//Timer4_38k_stop();
	
	//ʹ�ܽ��չܽ��жϹ��� 11
	ST0 |= (1<<1); // enable UART0 receive and transmit  11
    
    ISC = 0; 
    
    SMR00 = 0x0022;		//RXD 11
    SMR01 = 0x0122;		//RXD 11
	
	//����ͨ�Ÿ�ʽ 11
	SCR01 = 0x4297;	    //żУ�� 8λ���� 1λֹͣλ  11
	//SCR00 = 0x8297;	    //żУ�� 8λ���� 1λֹͣλ  11

    // ��7λ�����ò����� fMCK/fUART/2-1 11
    //SDR00 = 0x6600; //125kHz/((51+1)*2)=1.2 11
    SDR01 = 0x6600; //125kHz/((51+1)*2)=1.2 11
   	Delay1MsTime ( 1 );
   		
    SS0 |= (1<<1);  // enable UART0 receive  11
    
    // ����ͨ��1������RX 11
    SIR01 = 0x07;    // clear error flag 11 
    
    SRIF0 = 0U;     // clear INTSR0 interrupt flag  11
    SRMK0 = 0U;     // enable INTSR0 interrupt  11
    SREIF0 = 0U;    // clear INTSRE0 interrupt flag  11
    SREMK0 = 0U;    // enable INTSRE0 interrupt  11
    
    
	//s_IRState = IR_RXD_STATE;
#else
	//intp5 P0.1
	EGP0 &= ~BIT5;		//disable rising edge 
	EGN0 |= BIT5; 	    //enable falling edge 
	PIF5 = 0;       //clear interrupt flag
	PMK5= 1;	    //disable INTP0 interrupt	
	
#endif
							
  	return;
}

/*
*****************************************************************************************
* fuction name: ResetIRComm
*
* Description : this function reset 485 and IR channel when timeout
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ResetIRComm ( void )
{
	uchar *p,i;
	
	//clear recieve buffer
	s_commPara[0].RxdHead =
    s_commPara[0].RxdEnd = 0;
	
	s_commError[0].Byte = 0;
	
	//��մ���6ͨ������־ 11
	p = (uchar *)&s_commPara[0].RxdByte;
	for ( i=0; i<8; i++ )
		*p++ = 0;
	for ( i=0; i<BUF_MAX_LEN; i++ )
		s_IRBuf[i++] = 0;
		
	g_commStatus[0].Byte = 0;
	g_commForOneFrame[0] = 0;
	g_meterFlag.Bit.IrRcvOvertime = 0;
	
	//added by Roger 2019/08/28
	TT0 |= (1<<3);      //�ȹرն�ʱ��3 11
		
	IR_38K_OUTPUT;
	IR_38K_IO = 0;		//not send 38kHz 
	
    Timer4_38k_stop();

#if(PCB_VERSION_NEW_4==0) 	
 	SRIF0 = 0U;     // clear INTSR0 interrupt flag 
    SRMK0 = 0U;     // enable INTSR0 interrupt 
    SREIF0 = 0U;    // clear INTSRE0 interrupt flag 
    SREMK0 = 0U;    // enable INTSRE0 interrupt 
    SS0 |= (1<<1);  // enable UART0 receive 
#else
	PMK5= 0;	    //enable INTP0 interrupt	 	
		
#endif 	

	//s_IRState = IR_RXD_STATE;
			
  	return;
}

/*
*****************************************************************************************
* fuction name: ResetRS485Comm
*
* Description : this function reset 485  channel when timeout
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ResetRS485Comm ( void )
{
	uchar *p,i;
	
	//clear recieve buffer
	s_485SendEnd = 
	s_485SendHead = 0;
	
	s_commPara[1].RxdHead =
    s_commPara[1].RxdEnd = 0;
		
	//��մ���6ͨ������־ 11
	p = (uchar *)&s_commPara[1].RxdByte;
	for ( i=0; i<8; i++ )
		*p++ = 0;
	
	g_commStatus[1].Byte = 0;
	g_commForOneFrame[1] = 0;
	g_meterFlag.Bit.RS485RcvOvertime = 0;
	
	g_485CommDly = 0;
	
	RS485CON_RXD();
	
	//�ж�����  11
	SRIF1 = 0;
	SREIF1 = 0;
	STIF1 = 0;
	
	SS0 |= (1<<3);  // enable UART1 receive
	
	SRMK1 = 0;
	SREMK1 = 0;
	STMK1 = 1;  
	
	//����ͨ����ʼ  11
	//SS0 |= 0x000c;
	 
			
	s_commError[1].Byte = 0;
		
  	return;
}

/*
*****************************************************************************************
* fuction name: ResetPLCComm
*
* Description : this function reset PLC  channel when timeout
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
#if(IC_CARD_MODE==PLC_COMM_USED)
void ResetPLCComm ( void )
{
	uchar *p,i;
	
	//clear recieve buffer
	s_PLCSendEnd = 
	s_PLCSendHead = 0;
	
	s_commPara[2].RxdHead =
    s_commPara[2].RxdEnd = 0;
		
	//��մ���6ͨ������־ 11
	p = (uchar *)&s_commPara[2].RxdByte;
	for ( i=0; i<8; i++ )
		*p++ = 0;
	
	g_commStatus[2].Byte = 0;
	g_commForOneFrame[2] = 0;
	g_meterFlag.Bit.PLCRcvOvertime = 0;
	
	g_PLCCommDly = 0;	
	
	//�ж�����  11
	SRIF0 = 0;
	SREIF0 = 0;
	STIF0 = 0;
	
	SS0 |= (1<<1);  // enable UART0 receive
	
	SRMK0 = 0;
	SREMK0 = 0;
	STMK0 = 1;  
	
	//����ͨ����ʼ  11
	//SS0 |= 0x000c;
	 
			
	s_commError[2].Byte = 0;
		
  	return;
}
#endif

#if(PCB_VERSION_NEW_4==0)
/*
*****************************************************************************************
* fuction name: ISRForIRStartRxd
*
* Description : this interrupt service function get IR Rxd data
*
* Arguments   : none
*	
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISRForIRRxd ( void )
{		
    SRIF0 = 0;
    
    g_commStatus[0].Bit.Active = 1;
	
	// Set or clear the parity bit	
				
    s_IRBuf[s_commPara[0].RxdHead] = RXD0;
	s_commPara[0].RxdHead++;
	
	if ( s_commPara[0].RxdHead >= BUF_MAX_LEN )
		s_commPara[0].RxdHead = 0;
	
	g_commRxdFrameDly[0] = FRAME_DELAY_CNT;			
}

/*
*****************************************************************************************
* fuction name: ISRForIRRcvErr
*
* Description : uart0���մ����жϴ�������
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISRForIRRcvErr ( void )
{		
	SREIF0 = 0;
	
		
	s_commPara[0].ErrorByte = (uchar)SIR00;
	SIR00 |= 0x07;
	
	return;
}
#endif

#if(PCB_VERSION_NEW_4==1)
/*
*****************************************************************************************
* fuction name: ISRForIRStartRxd
*
* Description : this interrupt service function get IR Rxd data
*
* Arguments   : none
*	
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISRForIRStartRxd ( void )
{
	DI();
	
	PIF5 = 0;       //clear interrupt flag
	
	//start condition
	if ( IR_RCV_PIN == 0 )		
	{												
		TMIF03 = 0U;    // clear INTTM03 interrupt flag
		TMMK03 = 0U;    // enable INTTM03 interrupt 
		
		TS0 |= (1<<3);      //������ʱ��3��833.33us 11
		TDR03 = 1600; // 416us@4M 11
													
		s_IRState = 0;	
		s_IRByte = 0;
		
		PMK5= 1;	    //disable INTP5 interrupt				
	}
	
	EI();
	
	return;
}

/*
*****************************************************************************************
* fuction name: ISRForTimer3
*
* Description : this interrupt service function run in every 833.34us
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISRForTimer3( void )
{
	DI ( );
	TMIF03 = 0U;    // clear INTTM03 interrupt flag
	
	TDR03 = 3333; // 833.33us@4M 11		
	s_IRRcvFlag = 0;
	
	switch ( s_IRState )	
	{
	//start 
	case 0:
	{
		//if ( IR_RCV_PIN == 0 )
		{
			s_IRState++;	
			s_IRParity = 0;
		}
		//else
		{
			//s_IRRcvFlag = 1;
		}	
		break;
	}
	
	//bit0~bit7
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:	
	case 8:
	{		
		if ( IR_RCV_PIN == 1 )
		{
			s_IRByte += (uchar)(1<<(s_IRState-1));	
			s_IRParity++;	
		}
		s_IRState++;				
		break;
	}
	
	//even oddУ��  11
	case 9:
	{
		if ( IR_RCV_PIN == 1 )
		{
			if ( s_IRParity & BIT0 )
			{
				s_IRState++;	
				break;	
			}	
		}
		else
		{
			if ( (s_IRParity & BIT0) == 0 )
			{
				s_IRState++;	
				break;		
			}	
		}
		s_IRRcvFlag = 1;  
					
		break;
	}
				
	//������ 11
	case 10:
	{
		if ( IR_RCV_PIN == 1 )
		{
			g_commRxdFrameDly[0] = FRAME_DELAY_CNT;	
			
			s_IRBuf[s_commPara[0].RxdHead] = s_IRByte;
			s_commPara[0].RxdHead++;
			if ( s_commPara[0].RxdHead >= BUF_MAX_LEN )
				s_commPara[0].RxdHead = 0;	
				
			g_commStatus[0].Bit.Active = 1;	
			//s_IRState++;
			
			s_IRState = 0;
			PIF5 = 0;       //clear interrupt flag
			PMK5= 0;	    //enable INTP0 interrupt						
			TT0 |= (1<<3);      //�ȹرն�ʱ��3 11
			TMMK03 = 1U;    // disable INTTM03 interrupt
			//TDR03 = 1000; // 416us@4M 11								
		}
		else
		{
			s_IRRcvFlag = 1;  
		}
		
		break;	
	}	
	
//	case 11:
//	{
//		s_IRState = 0;
//		PIF5 = 0;       //clear interrupt flag
//		PMK5= 0;	    //enable INTP0 interrupt
//		TT0 |= (1<<3);      //�ȹرն�ʱ��3 11
//		TMMK03 = 1U;    // disable INTTM03 interrupt
//		break;
//	}
	
	default:
		s_IRRcvFlag = 1; 
		break;
	}
	
	if ( s_IRRcvFlag == 1 )
	{
		s_IRState = 0;
		TT0 |= (1<<3);      //�ȹرն�ʱ��3 11
		PIF5 = 0;       //clear interrupt flag
		PMK5= 0;	    //enable INTP0 interrupt
		TMMK03 = 1U;    // disable INTTM03 interrupt 	
		
		if ( g_commRxdFrameDly[0] > 0 )
		{
			g_commRxdFrameDly[0] = 0;
			s_commPara[0].RxdHead = 0;
		}
	}
		
	EI ( );
	
	return;
}
#endif

/*
*****************************************************************************************
* fuction name: ISRForIRTxdAndRxd
*
* Description : IR transfer one byte interrupt service function
*
* Arguments   : none
*
* returns     : none
*
* Notes     : revised 07/07/05, add notes only
******************************************************************************************
*/ 
__interrupt void ISRForIRTxd ( void )
{			
	DI ( );
	
	//STIF0 = 0;	
	
	switch ( s_IRTxdState++ )
	{
		case 0:
		    s_irBit = 0;
		    s_IRParity = 0;
		    break;
		
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
	    case 8:
			s_irBit = ( s_IRByte & 0x01 );
			s_IRByte >>= 1 ;
			if ( s_irBit ) s_IRParity++;
			break;
		case 9:
			if ( s_IRParity & 0x01 ) s_irBit = 1;
			else s_irBit = 0;
				
			break;
		case 10:
			s_irBit = 1;
			break;
		default:			
			s_irBit = 1;
			//Timer5_1200bps_stop();
			//TT0 |= (1<<5);
			s_IRTxdState = 0;
				
			   		
    		//������1�ֽ� 11
    		s_IRSendEnd++;
        	if ( s_IRSendEnd >= s_IRSendHead )	//�������ݷ��ͽ���11
        	{								
        		s_IRSendEnd = 
        		s_IRSendHead = 0;
        		
        		s_commPara[0].RxdHead =
        	    s_commPara[0].RxdEnd = 0;
        		
        		g_commStatus[0].Bit.TransmitComplete = 1;	
        		
        		//׼���������� 11
        		//g_commStatus[0].Bit.ToRevFrame = 1;	
        		//g_IRCommDly = FRAME_DELAY_CNT;	
        		
        		//ֹͣ 1200bps 11
        		TT0 |= (1<<5);
                TMMK05 = 1U;    // disable INTTM05 interrupt 11
                TMIF05 = 0U;    // clear INTTM05 interrupt flag  11
        	}
        	else	//�������� 11
        	{       	    	
        	    s_IRByte = s_IRBuf[s_IRSendEnd];   //send the character. 11	
        	    s_IRTxdState = 0;  	    	 				
        	}
		    
			break;
	}
	
	if ( s_irBit ) 
	{
		//TCE50 = 0;
		IR_38K_L();
		//Timer4_38k_stop();
		TT0 |= (1<<4);
        TOE0 &= ~(1<<4);
	}
	else 
	{
		//TCE50 = 1;
		//Timer4_38k_start();
		TOE0 |= (1<<4);
        TS0 |= (1<<4);
	}
			
	EI();  
	
	return;
}

/*
*****************************************************************************************
* fuction name: ISForS485Rxd
*
* Description : 485 recieve one byte interrupt service function
*
* Arguments   : none
*
* returns     : none
*
* Notes     : revised 07/07/05, add notes only
******************************************************************************************
*/
__interrupt void ISRFor485Rxd ( void )
{
	//this instruction do not need here because when an interrupt acknowledged,
	//this flag  will be cleared automatically.
	SRIF1 = 0;
	
	g_commStatus[1].Bit.Active = 1;
	
	// Set or clear the parity bit	
	
	s_485Buf[s_commPara[1].RxdHead] = RXD1;
	s_commPara[1].RxdHead++;
	
	if ( s_commPara[1].RxdHead >= BUF_MAX_LEN )
		s_commPara[1].RxdHead = 0;
	
	g_commRxdFrameDly[1] = FRAME_DELAY_CNT;		
	return;
}

/*
*****************************************************************************************
* fuction name: ISRFor485RcvErr
*
* Description : uart1���մ����жϴ�������
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISRFor485RcvErr( void )
{		
	SREIF1 = 0;
	
		
	s_commPara[1].ErrorByte = (uchar)SIR01;
	SIR01 |= 0x07;
	
	return;
}

/*
*****************************************************************************************
* fuction name: ISRFor485Txd
*
* Description : uart1�����жϴ�������
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ISRFor485Txd( void )
{
	
	STIF1 = 0;	
		
	s_485SendEnd++;
	if ( s_485SendEnd >= s_485SendHead )	//�������ݷ��ͽ���11
	{
		RS485CON_RXD();		//�лؽ���̬11	
					
		s_485SendEnd = 
		s_485SendHead = 0;
		
		s_commPara[1].RxdHead =
	    s_commPara[1].RxdEnd = 0;
		
		g_commStatus[1].Bit.TransmitComplete = 1;	
		
		ST0 |= (1<<2);  // disable UART1 transmit
		
		//׼���������� 11
		//g_485CommDly = FRAME_DELAY_CNT;	
		
//		g_485CommDly = 0;
//		ST0 |= (1<<2);  // disable UART1 transmit
//		STMK1 = 1;
//		SRMK1 = 0U;     // enable INTSR1 interrupt 
//		SS0 |= (1<<3);  // enable UART1 receive 
	}
	else	//�������� 11
	{
        TXD1 = s_485Buf[s_485SendEnd];
    	SOE0 |= 0x0004;		//	   	    	 				
	}
	
	return;
}

#if(IC_CARD_MODE==PLC_COMM_USED)
/*
*****************************************************************************************
* fuction name: ISForSPLCRxd
*
* Description : 485 recieve one byte interrupt service function
*
* Arguments   : none
*
* returns     : none
*
* Notes     : revised 07/07/05, add notes only
******************************************************************************************
*/
__interrupt void ISRForPLCRxd ( void )
{
	//this instruction do not need here because when an interrupt acknowledged,
	//this flag  will be cleared automatically.
	SRIF0 = 0;
	
	g_commStatus[2].Bit.Active = 1;
	
	// Set or clear the parity bit		
	s_PLCBuf[s_commPara[2].RxdHead] = RXD0;
	s_commPara[2].RxdHead++;
	
	if ( s_commPara[2].RxdHead >= BUF_MAX_LEN )
		s_commPara[2].RxdHead = 0;
	
	g_commRxdFrameDly[2] = FRAME_DELAY_CNT;		
	
	return;
}

/*
*****************************************************************************************
* fuction name: ISRForPLCRcvErr
*
* Description : uart1���մ����жϴ�������
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISRForPLCRcvErr( void )
{		
	SREIF0 = 0;
			
	s_commPara[2].ErrorByte = (uchar)SIR00;
	SIR00 |= 0x07;
	
	return;
}

/*
*****************************************************************************************
* fuction name: ISRForPLCTxd
*
* Description : uart1�����жϴ�������
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ISRForPLCTxd( void )
{
	
	STIF0 = 0;	
		
	s_PLCSendEnd++;
	if ( s_PLCSendEnd >= s_PLCSendHead )	//�������ݷ��ͽ���11
	{

					
		s_PLCSendEnd = 
		s_PLCSendHead = 0;
		
		s_commPara[2].RxdHead =
	    s_commPara[2].RxdEnd = 0;
		
		g_commStatus[2].Bit.TransmitComplete = 1;	
		
		ST0 |= (1<<0);  // disable UART0 transmit
		
	}
	else	//�������� 11
	{
        TXD0 = s_PLCBuf[s_PLCSendEnd];
    	SOE0 |= 0x0001;		//	   	    	 				
	}
	
	return;
}


#endif
/*
*****************************************************************************************
* fuction name: ProcessIRRxdOrTxd
*
* Description : this function receives byte from uart channel and set related falgs for
*		next step process. Or transmits byte .
*
* Arguments   : none
*
* returns     : none
*
* Notes     : this task is a high prioraty job.
******************************************************************************************
*/
void ProcessCommRxdOrTxd ( uchar channel )
{		
	uchar i,ret;
	
	//for ( i=0; i<2; i++ )
	{
		if ( g_commStatus[channel].Bit.RxdFrameDone == 1 )
		{
			g_commStatus[channel].Bit.RxdFrameDone = 0;
			
			ret = CheckCommMessage ( channel, s_commPara[channel].RxdHead );	
			
			if ( ret == TRUE )	
			{
			#if(MODBUS_ENABLED==1)		
				if ( g_commStatus[channel].Bit.IsModbusProtocol )
				{
					DecodeModbusMessage ( channel, s_commPara[channel].RxdHead );	
					g_commStatus[channel].Bit.IsModbusProtocol = 0;
				}
				else
			#endif
				{
					DecodeIRMessage ( channel );	
						
				}
				
			}						
		}	
	
	#if(MODBUS_ENABLED==1)		
		if ( g_commStatus[channel].Bit.ModbusSendData == 1 )
			BuildModbusMessage ( channel );
		else
	#endif
			BuildIRMessage( channel );
	}
		
	
  			
	if ( g_commStatus[channel].Bit.TransmitComplete )
    {
        g_commStatus[channel].Bit.TransmitComplete = 0;
        
        if ( channel == 0 )
    	{
	    	InitIR ( );
	    	ResetIRComm ( );
	    }
	    else if ( channel == 1 )
    	{
	    	InitRS485 ( );
	    	ResetRS485Comm ( );
	    	
	    	for ( i=0; i<20; i++ )
	    	    s_485Buf[i] = 0;    
	    }
	#if(IC_CARD_MODE==PLC_COMM_USED)	
	    else if ( channel == 2 )
    	{
	    	InitPLCComm ( );
			ResetPLCComm ( );
	    	
	    	for ( i=0; i<20; i++ )
	    	    s_PLCBuf[i] = 0;    
	    }        
    #endif    
    }
    		    		    			   		    		 		 	
  	return;
}

/*
*****************************************************************************************
* fuction name: CheckCommMessage
*
* Description : this function receives data from the selected port driver, checks for validity
*		and passes the formatted message to the message decoder.
*
* Arguments   : none
*
* returns     : uchar
*
* Notes     : It's not the most efficient routine, but it sure is small.
******************************************************************************************
*/
uchar CheckCommMessage ( uchar channel,  uchar rxdlen )
{
 	uchar pos,cs,len;	// Temporary variable to contain a character
 	uchar MCstate;		  	
   	uchar *pbuf;
 
#if(MODBUS_ENABLED==1)
	uint16 crc16,temp16;
#endif
	
 	if ( channel == 0 )
    	pbuf = &s_IRBuf[0];
    else if ( channel == 1 )
    	pbuf = &s_485Buf[0];
#if(IC_CARD_MODE==PLC_COMM_USED)
    else
    	pbuf = &s_PLCBuf[0];
#endif
    
    g_commStatus[channel].Bit.IsDL645Protocol = 1;
    g_commStatus[channel].Bit.IsModbusProtocol = 0;
    
    if ( rxdlen < 14 )
 		g_commStatus[channel].Bit.IsDL645Protocol = 0;
 			
 	//modbus�ж� 11
#if(MODBUS_ENABLED==1)
	
	g_commStatus[channel].Bit.IsModbusProtocol = 1;
	
	if ( ((pbuf[0] == 0xfe) && (pbuf[1] == 0xfe)) )
		g_commStatus[channel].Bit.IsModbusProtocol = 0;	
	else
	{
		//�������ж� 11
	   	if ( (pbuf[1] != 0x03) &&
	   			(pbuf[1] != 0x04) && 
	   			(pbuf[1] != 0x10) )	
	   		 g_commStatus[channel].Bit.IsModbusProtocol = 0;  
	  	else
	  	{
	  		//CRCУ���У�� 11
		    crc16 = GetCRC16 ( pbuf, rxdlen-2 );
		    temp16 = ((uint16)pbuf[rxdlen-1])<<8;
		    temp16 |= pbuf[rxdlen-2];
		    if ( crc16 != temp16 )
		        g_commStatus[channel].Bit.IsModbusProtocol = 0;	
	  	}	 		  	   		   	
	}		 	
 	
 	if (g_commStatus[channel].Bit.IsModbusProtocol )
    {
    	
    	if ( channel == 0 )
		{
			//RXE0 = 0;	//disable IR(Uart0) reciption
			SRMK0 = 1U;     // disable INTSR0 interrupt 
            ST0 |= (1<<1);  // disable UART0 receive 
				
			s_IRState = IR_TXD_STATE;
						
		}
		else if ( channel == 1 )
		{
			//RXE6 = 0;	//disable RS485(Uart6) reciption
			SRMK1 = 1U;     // disable INTSR1 interrupt 	
			ST0 |= (1<<3);  // disable UART1 receive 	
		} 	
		else if ( channel == 2 )
		{
			//RXE6 = 0;	//disable RS485(Uart6) reciption
			SRMK0 = 1U;     // disable INTSR0 interrupt 	
			ST0 |= (1<<1);  // enable UART0 receive 	
		} 	
    	return TRUE;		   		
    }   
    	
#endif 	
 	   	
// 	if ( (g_commStatus[channel].Bit.IsDL645Protocol == 0) &&
// 		 (g_commStatus[channel].Bit.IsModbusProtocol == 0) )
    if ( rxdlen < 14 )
    	goto error_or_not_for_me;
              			    		
    MCstate = 0;
    pos = 0;	
    while ( rxdlen > 0 )
   	{  		   	    
	 	switch ( MCstate++ )
	 	{
	 		//looking for the start delimiter byte
	 	 	case 0:
	 	 		if ( pbuf[pos] != 0x68 )
	 	 		{
	 	 			MCstate = 0;
	 	 			break;
	 	 		}
	        		
	        			     						    		     		  		  			
	    		cs = 0x68;		//checksum
	    		        		
	    		// Assume true until proven false
	    		g_commStatus[channel].Bit.IsBroadcast = 1;
	    		g_commStatus[channel].Bit.IsSuperAddress = 1;
	  			g_commStatus[channel].Bit.MessageIsForMe = 1;           // Ditto...
	  							
	  			break;
	      		
	    	// Case 1 is where it receives the meter address. With each byte, there is
			// the possibility that (1) the byte matches my address or wildcard; (2) the 
			// byte matches the broadcast address; or (3) the byte matches neither.
	    	case 1:
	    	case 2:
			case 3:	
			case 4:
	    	case 5:
			case 6:	
	    		if ( pbuf[pos] != 0x99 )
	    			g_commStatus[channel].Bit.IsBroadcast = 0;
	    		
	    		if ( pbuf[pos] != 0xaa )
	    			g_commStatus[channel].Bit.IsSuperAddress = 0;
	    				
	    		if ( (g_addr.MeterAddr[MCstate-2] != pbuf[pos]) &&
	    			( pbuf[pos] != 0xaa ) )
	    			g_commStatus[channel].Bit.MessageIsForMe = 0;
	    			  	    			
	    		cs += pbuf[pos];
	    		
	    		break;
	        	
	    	//if the input byte is not 68h, or both the "broadcast" and "my address"
	    	//flags have been cleared,abort the message.
	 		case 7:// Delimiter
					
				if ( (g_commStatus[channel].Bit.MessageIsForMe==0) && 
						(g_commStatus[channel].Bit.IsBroadcast==0) )
					goto error_or_not_for_me;	
				
				
				if( pbuf[pos] != 0x68 )
	      			         // ...and it's not for me.
					goto error_or_not_for_me;
				
				cs += pbuf[pos];
				break;
			
			// If get here, the address was okay. Perform a rudimentary check of
			// the command byte. Specifically, if the high-order bit is set, it's
			// not a message from the host and can be ignored except recieved data is 
			// 0xaa because this command code is added by our company as initialized code .
			case 8: // Command byte
	 			
	 			//if ( s_commPara[channel].RxdByte > 0x3f )	 			
	    		//	goto error_or_not_for_me; // message from another meter
	  			cs += pbuf[pos];
	  			g_commBuf[channel].Command = pbuf[pos];
	  			break;
	      			
	  		// Here check the length. The only check here is a range check:
			// can't handle a message longer than MAXHANDLEBYTES bytes.
			case 9:// Data Length

	  			if ( pbuf[pos] > MAXHANDLEBYTES )
	    			goto error_or_not_for_me; // not enough room in buffer
	  			cs += pbuf[pos];
	  			g_commBuf[channel].Len = pbuf[pos];
				break;
	    			
			// Default case is for (1)data, (2)checksum or (3)end marker.	
			default:
				len = MCstate - 11; // subtract 11 bytes of overhead
				if ( (len < g_commBuf[channel].Len) )	// Data
				{
					cs += pbuf[pos];
					g_commBuf[channel].Data[len] = pbuf[pos]-0x33;

				}
				else if ( len == g_commBuf[channel].Len )// Checksum byte
	    		{	
	    			if ( cs != pbuf[pos] )              // Is checksum valid?
	      				goto error_or_not_for_me; 	// No...abort receive.
	      		}
				else if (len == (g_commBuf[channel].Len + 1))// End marker.
	  			{
	    			if(pbuf[pos] != 0x16)  	// Is it the End Marker?
	      				goto error_or_not_for_me;   	// no...abort receive

					// If get here, it's a valid message...let the decoder know
	  				g_commStatus[channel].Bit.MessageReady = 1;
	  				g_commStatus[channel].Bit.MessageComplete = 1; 						 					
					  				     				
	  				rxdlen = 1;
	  				
	  				g_commForOneFrame[channel] = 0;
  				
	  				if ( channel == 0 )
					{
						//RXE0 = 0;	//disable IR(Uart0) reciption
						SRMK0 = 1U;     // disable INTSR0 interrupt 
	                    ST0 |= (1<<1);  // disable UART0 receive 
							
						s_IRState = IR_TXD_STATE;
									
					}
					else if ( channel == 1 )
					{
						//RXE6 = 0;	//disable RS485(Uart6) reciption
						SRMK1 = 1U;     // disable INTSR1 interrupt 	
						ST0 |= (1<<3);  // disable UART1 receive 	
					} 
				#if(IC_CARD_MODE==PLC_COMM_USED)
					else if ( channel == 2 )
					{						
						SRMK0 = 1U;     // disable INTSR0 interrupt 	
						ST0 |= (1<<1);  // disable UART0 receive 	
					} 	
				#endif				  									 																							
	  			}
					 						
			  	break;
		}
		
		rxdlen--;
		pos++;
	}
	
	if ( MCstate == 0 )
		goto error_or_not_for_me;
			
  	return TRUE;
 			  	
//    // If MessageReady is set, it means that the message chain is still busy
//  	// with a previous message.
//    if( g_commStatus[channel].Bit.MessageReady ) 
//    	return 0; 
//    		
//    //if there's a parity error, reset the number of bytes processed.
//    if ( g_commStatus[channel].Bit.DataLoss )
//    {
//    	if ( channel == 0 )
//    	{
//	    	InitIR ( );
//	    	ResetIRComm ( );
//	    	return 0;
//	    }
//	    else if ( channel == 1 )
//    	{
//	    	InitRS485 ( );
//	    	ResetRS485Comm ( );
//	    	return 0;
//	    }
//    }
//       	
//  	//return if there is no character available on the active interface.
//	//if ( !(g_commStatus[channel].Bit.ByteAvailable & g_commStatus[channel].Bit.Active) )
//	if ( (g_commStatus[channel].Bit.ByteAvailable==0) )
//		 return 0;
//	else
//	{
//		g_commStatus[channel].Bit.ByteAvailable = 0;
//		// Get here if it is in the process of receiving a message.
//		// Clear the byte available flag from the active interface.
//	}
//    	
//    // MessageBytesProcessed contains the number of bytes *under the
//  	// protocol* that have been processed. That is, any byte *not* under the
//  	// protocol causes this machine to abort and force retransmission.
// 	switch ( s_commPara[channel].MCState++ )
// 	{
// 		//looking for the start delimiter byte
// 	 	case 0:
// 	 		if ( s_commPara[channel].RxdByte != 0x68 )
//        		goto error_or_not_for_me;
//        			     						    		
//    		g_commForOneFrame[channel] = COMM_ONEFRAME_TIMEOUT;
//    		//s_commPara[channel].CommStart = 1; 
//    		  		  			
//    		s_commPara[channel].MCcs = 0x68;		//checksum
//    		        		
//    		// Assume true until proven false
//    		g_commStatus[channel].Bit.IsBroadcast = 1;
//    		g_commStatus[channel].Bit.IsSuperAddress = 1;
//  			g_commStatus[channel].Bit.MessageIsForMe = 1;           // Ditto...
//  							
//  			break;
//      		
//    	// Case 1 is where it receives the meter address. With each byte, there is
//		// the possibility that (1) the byte matches my address or wildcard; (2) the 
//		// byte matches the broadcast address; or (3) the byte matches neither.
//    	case 1:
//    	case 2:
//		case 3:	
//		case 4:
//    	case 5:
//		case 6:	
//    		if ( s_commPara[channel].RxdByte != 0x99 )
//    			g_commStatus[channel].Bit.IsBroadcast = 0;
//    		
//    		if ( s_commPara[channel].RxdByte != 0xaa )
//    			g_commStatus[channel].Bit.IsSuperAddress = 0;
//    				
//    		if ( (g_addr.MeterAddr[s_commPara[channel].MCState-2] != s_commPara[channel].RxdByte) &&
//    			( s_commPara[channel].RxdByte != 0xaa ) )
//    			g_commStatus[channel].Bit.MessageIsForMe = 0;
//    			  	    			
//    		s_commPara[channel].MCcs += s_commPara[channel].RxdByte;
//    		
//    		break;
//        	
//    	//if the input byte is not 68h, or both the "broadcast" and "my address"
//    	//flags have been cleared,abort the message.
// 		case 7:// Delimiter
//				
//			if ( (g_commStatus[channel].Bit.MessageIsForMe==0) && 
//					(g_commStatus[channel].Bit.IsBroadcast==0) )
//				goto error_or_not_for_me;	
//			
//			
//			if( s_commPara[channel].RxdByte != 0x68 )
//      			         // ...and it's not for me.
//					goto error_or_not_for_me;
//			
//			s_commPara[channel].MCcs += s_commPara[channel].RxdByte;
//			break;
//		
//		// If get here, the address was okay. Perform a rudimentary check of
//		// the command byte. Specifically, if the high-order bit is set, it's
//		// not a message from the host and can be ignored except recieved data is 
//		// 0xaa because this command code is added by our company as initialized code .
//		case 8: // Command byte
// 			
// 			//if ( s_commPara[channel].RxdByte > 0x3f )	 			
//    		//	goto error_or_not_for_me; // message from another meter
//  			s_commPara[channel].MCcs += s_commPara[channel].RxdByte;
//  			g_commBuf[channel].Command = s_commPara[channel].RxdByte;
//  			break;
//      			
//  		// Here check the length. The only check here is a range check:
//		// can't handle a message longer than MAXHANDLEBYTES bytes.
//		case 9:// Data Length
//
//  			if ( s_commPara[channel].RxdByte > MAXHANDLEBYTES )
//    			goto error_or_not_for_me; // not enough room in buffer
//  			s_commPara[channel].MCcs += s_commPara[channel].RxdByte;
//  			g_commBuf[channel].Len = s_commPara[channel].RxdByte;
//			break;
//    			
//		// Default case is for (1)data, (2)checksum or (3)end marker.	
//		default:
//			len = s_commPara[channel].MCState - 11; // subtract 11 bytes of overhead
//			if ( (len < g_commBuf[channel].Len) && 
//				(len<MAXHANDLEBYTES-11)  )	// Data
//			{
//				s_commPara[channel].MCcs += s_commPara[channel].RxdByte;
//				s_commPara[channel].RxdByte -= 0x33;
//				g_commBuf[channel].Data[len] = s_commPara[channel].RxdByte;
//
//			}
//			else if ( len == g_commBuf[channel].Len )// Checksum byte
//    		{	
//    			if ( s_commPara[channel].MCcs != s_commPara[channel].RxdByte )              // Is checksum valid?
//      				goto error_or_not_for_me; 	// No...abort receive.
//      		}
//			else if (len == (g_commBuf[channel].Len + 1))// End marker.
//  			{
//    			if(s_commPara[channel].RxdByte != 0x16)  	// Is it the End Marker?
//      				goto error_or_not_for_me;   	// no...abort receive
//
//				// If get here, it's a valid message...let the decoder know
//  				g_commStatus[channel].Bit.MessageReady = 1;
//  				g_commStatus[channel].Bit.MessageComplete = 1; 						      						    						     						   						
//								  				     				
//  				s_commPara[channel].MCState = 0;
//  				
//  				g_commForOneFrame[channel] = 0;
//  				
//  				if ( channel == 0 )
//				{
//					//RXE0 = 0;	//disable IR(Uart0) reciption
//					SRMK0 = 1U;     // disable INTSR0 interrupt 
//                    ST0 |= (1<<1);  // disable UART0 receive 
//						
//					s_IRState = IR_TXD_STATE;
//								
//				}
//				else if ( channel == 1 )
//				{
//					//RXE6 = 0;	//disable RS485(Uart6) reciption
//					SRMK1 = 1U;     // disable INTSR1 interrupt 	
//					ST0 |= (1<<3);  // disable UART1 receive 	
//				} 
//				 														
//								
//  			}
//				 						
//			
//  			break;
//	}
//		
//  	return 1;

	// This branch point is taken when a received byte breaks the protocol.
	error_or_not_for_me:
		
		
		
		if ( channel == 0 )
		{	
			InitIR();
			ResetIRComm();			
		}
		else if ( channel == 1 )
		{	
			InitRS485();
			ResetRS485Comm ( );	
		}
	#if(IC_CARD_MODE==PLC_COMM_USED)
		else if ( channel == 2 )
		{	
			InitPLCComm();
			ResetPLCComm ( );	
		}
	#endif
		
		s_commPara[channel].MCState = 0;				 			
		
		g_commStatus[channel].Byte = 0;
		
		g_commForOneFrame[channel] = 0;
			
  		return FALSE;
}

/*
*****************************************************************************************
* fuction name: DecodeMessage
*
* Description : Receives a message block from the message checker and determines  the
*  				destination for the message. Sends messages to the message builder for
*     			transmission to the network.
*
* Arguments   : none
*
* returns     : uchar
*
* Notes     :
******************************************************************************************
*/
uchar DecodeIRMessage ( uchar channel )
{
    uchar len,i;
    PROTOCOL_REG protocolReg;
    uint16* pint;
    uchar* pchar;
    uint32 ID,tempL; 
    uint16 tempInt;
    //	
	if ( g_commStatus[channel].Bit.MessageComplete == 0 ) 
		return 0;
	g_commStatus[channel].Bit.MessageComplete = 0;	
	
	
	
	// Next, if the message has the MORE bit set, it's inherently invalid.
	// Host can't send multipart messages
  	if ( g_commBuf[channel].Command&0x20  )       
  	{
    	// The error message will be transmitted as though it's a write ack.
    	//g_commStatus[channel].Bit.MoreData = 1;		
    }
			
	//broadcast address only uses in the below conditons:
	//1) ko internal expanded protocol to initialize the meter
	//2) broadcast time, broadcast set address with program key open, broadcadt read. 
	if ( g_commStatus[channel].Bit.IsBroadcast )
	{		
		
	}
	
	//super address only for read
	if ( g_commStatus[channel].Bit.IsSuperAddress )
	{
		//if( g_commBuffer.Command.Bit.Function != FUNCTION_READ ) 
		//{
			
		//}
	}

#if(PROTOCOL_ADD_4_SERIALS==1)
	s_serialsLen[channel] = 0;		
#endif
		
	ID = g_commBuf[channel].Data[3]; 
	ID = (ID<<8) | g_commBuf[channel].Data[2]; 
	ID = (ID<<8) | g_commBuf[channel].Data[1]; 
	ID = (ID<<8) | g_commBuf[channel].Data[0]; 
		
	switch ( g_commBuf[channel].Command & 0x7F )	
	{
#ifdef PRE_PAID_ENABLE
	case ID_AUTHENTICATE:
	{
	//2018����Э�� 11
	#if(NEW_PROTOCOL_2018==1)
		
		len = g_commBuf[channel].Len;
		i = ProcessSelling( &g_commBuf[channel].Data[0], len, channel );	
		
		if ( g_commBuf[channel].Len == 0 )
			goto no_answer;
		if ( i != 0 )		//���� 11
		{
			goto error_process;	 
		}
		
		g_meterStatus3.Bit.BtnKeepRelay = 0;
	#else //֮ǰЭ�� 11
		pchar = &g_commBuf[channel].Data[0];
		
		Decrypt(&pchar[0],0x00,g_commBuf[channel].Len,0xff);
		
		ID = pchar[3]; 
		ID = (ID<<8) | pchar[2]; 
		ID = (ID<<8) | pchar[1]; 
		ID = (ID<<8) | pchar[0];
		
		//��������λ��8��ʼ 11
		if ( ID == 0x070104ff )		//�������� 11
		{	
			if ( g_commBuf[channel].Len != 45 )
			{
				s_commError[channel].Byte = 0x84;
				goto error_process;			
			}
			
			//���²��� 11
			if ( (pchar[8]&0x40) == 0x40 ) 	
			{
				tempL = ((uint32)pchar[12]<<24) + ((uint32)pchar[11]<<16) + 
						((uint32)pchar[10]<<8)+ pchar[9];	
				if ( tempL > MAX_PRICE || tempL < MIN_PRICE )
				{
					s_commError[channel].Byte = 0x83;
					goto error_process;			
				}
				g_para.Price = 	(uint16)(tempL);
				g_para.MaximBalance = MAX_BALANCE_LIMIT;
				g_para.LimitPower = MAX_POWER_LIMIT;
								
				g_para.CreditLimit = ((uint32)pchar[44]<<8)+ pchar[43];
				
				g_para.CurrentRatio = CURRENT_RATIO;
				g_para.Checksum = GetSum ( (uchar*)&g_para.LimitPower, Meter_Para_Len-1 );
				FeedWatchdog ( );
	    		StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LimitPower),
					(uchar*)&g_para.LimitPower,Meter_Para_Len );
			
			#ifdef LADDER_ENABLE	
				//����ֵ 11
				for ( i=0; i<10; i++ )
			    {
			    	g_buf[i] = pchar[13+i];
			    }	
				g_buf[10] = GetSum ( g_buf, 10 );
	    		FeedWatchdog ( );
	    		StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Ladder1),
					(uchar*)&g_buf[0],11 );
					
				//���ݵ�� 11
				for ( i=0; i<12; i++ )
			    {
			    	g_buf[i] = pchar[23+i];
			    }
				g_buf[12] = GetSum ( g_buf, 12 );
				FeedWatchdog ( );
	    		StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LadderPrice1),
					(uchar*)&g_buf[0], 13 );
			#endif
				
				//�������1 2 11
				//�������1 06H~07H 2�ֽ�11
			    g_buyPara.Alarm1 = ((uint16)pchar[36]<<8) + pchar[35];
			    //�������2 08H~09H 2�ֽ�11
			    g_buyPara.Alarm2 = ((uint16)pchar[38]<<8) + pchar[37];			    
			    //g_buyPara.AlarmSet = 0x03;
			    
			    //Ԥ�ý�� 11
			    g_buyPara.BuyQuan = ((uint32)pchar[42]<<24) + ((uint32)pchar[41]<<16) + 
			    		((uint32)pchar[40]<<8)+ pchar[39];
			    g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
    			FeedWatchdog ( );
    			StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
					(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );
					
				g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
				FeedWatchdog ( );
			    StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
						(uchar*)&g_addr.Zone,Meter_Addr_Len );
			}
			
			g_commBuf[channel].Len = 38;				
			
			//��� 11
		    pchar[7] = 0x00;
			pchar[6] = 0x00;
			pchar[5] = (uchar)(g_para.Price>>8)&0xff;
			pchar[4] = (uchar)(g_para.Price)&0xff;
		
		#ifdef LADDER_ENABLE	    
			//����ֵ 11
			I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Ladder1), 
				&pchar[8], 10, 1  ) ;
				
			//���ݵ�� 11
			I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,LadderPrice1), 
				&pchar[18], 12, 1  ) ;
		#endif
			
			//�������1 11
			pchar[31] = (uchar)(g_buyPara.Alarm1>>8)&0xff;
			pchar[30] = (uchar)(g_buyPara.Alarm1)&0xff;
			
			//�������2 11
			pchar[33] = (uchar)(g_buyPara.Alarm2>>8)&0xff;
			pchar[32] = (uchar)(g_buyPara.Alarm2)&0xff;
			
			//Ԥ�� ���ӽ�� 11
			pchar[37] = (uchar)(g_buyPara.BuyQuan>>24)&0xff;
			pchar[36] = (uchar)(g_buyPara.BuyQuan>>16)&0xff;
			pchar[35] = (uchar)(g_buyPara.BuyQuan>>8)&0xff;
			pchar[34] = (uchar)(g_buyPara.BuyQuan)&0xff;
			
			g_energy.Balance += g_buyPara.BuyQuan;
			g_price = g_para.Price;
			g_meterFlag.Bit.LadderProcess = 1;
			g_relayFlag.Bit.RelayAlarmHappened = 0;
		}
		else if ( ID == 0x070102ff )		//Զ�̳�ֵ 11
		{			
			if ( g_commBuf[channel].Len != 30 )
			{
				s_commError[channel].Byte = 0x84;
				goto error_process;			
			}
			
			FeedWatchdog ( );
			//��������ж� 11			
			tempInt = ((uint16)pchar[13]<<8)+ pchar[12];
			//���ڹ�������� Զ�̹������С1��������������,���򷵻ش���֡ 11
			if ( ((g_buyPara.BuyCount+1) != tempInt) ||
					(pchar[14]!=0) ||
					(pchar[15]!=0) )
			{			
				//����������� 11
				s_commError[channel].Byte = 0x80;
				goto error_process;	
			}
			
		//�Ѳ����ư汾 11
		#if(JIA_CE_TEST==50)
			if(tempInt>50)
			{			
				//����������� 11
				s_commError[channel].Byte = COMM_JIA_CE_TEST_ERR;
				goto error_process;	
			}
				
		#endif	
			//�����ж� 11
			if ( tempInt == 1 ) //���� 11
			{
				
				//��¼�ͻ���ţ�ʹ�ñ��ر��еĿ��� 11
	            for ( i=0; i<6; i++ )
	    			g_addr.CardNum[i] = pchar[20+i]; 	
				g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
			    FeedWatchdog ( );
			    StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
						(uchar*)&g_addr.Zone,Meter_Addr_Len );
			}
			else	//�����ж� 11
			{
				for ( i=0; i<6; i++ )
		        {
		        	if ( g_addr.CardNum[i] != pchar[20+i] )
		        	{
		        		s_commError[channel].Byte = 0x81;
						goto error_process;		
		        	} 	 	
		        }  				
			}
			
			//�ڻ�����ж�11
            //if ( g_energy.Balance >= 0x5F5E100 ) //4λС�� 10000 0000
//            if ( g_energy.Balance >= 1000000000 ) //4λС�� 100000 0000	
//            {
//            	s_commError[channel].Byte = 0x82;
//				goto error_process;     
//            }
						
			//���紦�� ʣ������� ��������� 11
			tempL = ((uint32)pchar[11]<<24) + ((uint32)pchar[10]<<16) + 
						((uint32)pchar[9]<<8)+ pchar[8];
			g_buyPara.BuyQuan = tempL;
			//������� 11
			if ( g_energy.Overdraft > tempL )
			{
				g_energy.Overdraft -= tempL;			
			}
			else
			{
		    	g_energy.Balance += (tempL-g_energy.Overdraft);	
		    	g_energy.Overdraft = 0;
		    }  
		    g_buyPara.BuyCount += 1;
			//g_buyPara.AlarmSet = 0x03;
			
		    g_buyPara.Status = USER_STATUS;
		    g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
			FeedWatchdog ( );
			StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
				(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );
		    
		    //������ȷ����֡ 11
		    //ʣ���� 11
		    tempL = g_energy.Balance/100;
		    pchar[7] = (uchar)(tempL>>24)&0xff;
			pchar[6] = (uchar)(tempL>>16)&0xff;
			pchar[5] = (uchar)(tempL>>8)&0xff;
			pchar[4] = (uchar)(tempL)&0xff;
			
			//������� 11
			pchar[8] = (uchar)(g_buyPara.BuyCount)&0xff;
			pchar[9] = (uchar)(g_buyPara.BuyCount>>8)&0xff;			
			pchar[10] = 0x00;
			pchar[11] = 0x00;
			
			//�ͻ���� 11
			for ( i=0; i<6; i++ )
	    		pchar[12+i] = g_addr.CardNum[i]; 
	    		
	    	g_commBuf[channel].Len = 18;
	    	
			g_meterFlag.Bit.LadderProcess = 1;
			g_relayFlag.Bit.RelayAlarmHappened = 0;
						
		}
		else if ( ID == 0x070103ff )		//Ǯ��Ԥ�� 11
		{
			if ( g_commBuf[channel].Len != 24 )		//8+16
			{
				s_commError[channel].Byte = 0x84;
				goto error_process;			
			}
			
			//��������ж� ����Ϊ0 11			
			if ( (pchar[16]!=0x00) || (pchar[17]!=0x00) ||
					(pchar[18]!=0x00) || (pchar[19]!=0x00) ) 
			{
				s_commError[channel].Byte = 0x84;
				goto error_process;			
			}	 
						
			//Ԥ�ý����� ��������� 11
			tempL = ((uint32)pchar[11]<<24) + ((uint32)pchar[10]<<16) + 
						((uint32)pchar[9]<<8)+ pchar[8];
			g_energy.Balance = tempL;
			g_energy.Overdraft = 0;
			//g_energy.Accumulate = 0;	//�������� 11
			SaveEnergy ( NORMAL_SAVE );
	        g_meterFlag.Bit.PowerDown = 0;
	        
		    g_buyPara.BuyCount = 0;

		    g_buyPara.Status = OUT_STATUS;
		    g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
			FeedWatchdog ( );
			StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
				(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );
		    
		    //������ȷ����֡ 11
		    //ʣ���� 11
		    tempL = g_energy.Balance/100;
		    pchar[7] = (uchar)(tempL>>24)&0xff;
			pchar[6] = (uchar)(tempL>>16)&0xff;
			pchar[5] = (uchar)(tempL>>8)&0xff;
			pchar[4] = (uchar)(tempL)&0xff;
			
			//�������
			pchar[8] = (uchar)(g_buyPara.BuyCount)&0xff;
			pchar[9] = (uchar)(g_buyPara.BuyCount>>8)&0xff;		
			pchar[10] = 0x00;
			pchar[11] = 0x00;
			
			//�ͻ���� 11
			for ( i=0; i<6; i++ )
	    		pchar[12+i] = g_addr.CardNum[i]; 
	    		
	    	g_commBuf[channel].Len = 18;
	    	
			g_meterFlag.Bit.LadderProcess = 1;
			g_relayFlag.Bit.RelayAlarmHappened = 0;	
			
		}
		else
		{
			s_commError[channel].Bit.noRequestData = 1;
			goto error_process;	
		}
		
		g_relayFlag.Bit.RelayAlarmOn = 0;
		
		if ( g_energy.Balance > 0 )
        {
    		g_relayFlag.Bit.BalanceZeroHappened = 0;
    		g_relayFlag.Bit.BalanceZero5MinFlg = 0;
    	}
	#endif
	
	}
		break;
#endif	
	case READ_DATA_97:
	{
	#if(PROTOCOL_ADD_4_SERIALS==0)
		if ( g_commBuf[channel].Len != 2 )	
	#else
		if ( (g_commBuf[channel].Len != 2) && (g_commBuf[channel].Len != 6) )	
	#endif
		{
			s_commError[channel].Bit.noRequestData = 1;
			goto error_process;	
		}	
	
	#if(PROTOCOL_ADD_4_SERIALS==1)
		if ( g_commBuf[channel].Len == 6 )
		{
			s_serialsLen[channel] = 4;
			for( i=0; i<4; i++ )
				s_serials[i] = g_commBuf[channel].Data[2+i];
		}			
	#endif
		
		tempInt = g_commBuf[channel].Data[1]; 
		tempInt = (tempInt<<8) | g_commBuf[channel].Data[0];	
		
		if ( tempInt == 0x9010 )	//(��ǰ)�����й��ܵ��� 11
		{			
			g_commBuf[channel].Len = 6;	
			//tempL = *((uint16*)protocolReg.addr);
			tempL = Hex32ToBCD( g_energy.Accumulate );				
			g_commBuf[channel].Data[2] = (uchar)tempL&0xff;
			g_commBuf[channel].Data[3] = (uchar)(tempL>>8)&0xff;
			g_commBuf[channel].Data[4] = (uchar)(tempL>>16)&0xff;			
			g_commBuf[channel].Data[5] = (uchar)(tempL>>24)&0xff;	 
		}
		else if ( tempInt == 0x9020 )	//(��ǰ)�����й��ܵ��� 11
		{			
			g_commBuf[channel].Len = 6;	
			//tempL = *((uint16*)protocolReg.addr);
			tempL = Hex32ToBCD( g_energy.NegAccumulate );				
			g_commBuf[channel].Data[2] = (uchar)tempL&0xff;
			g_commBuf[channel].Data[3] = (uchar)(tempL>>8)&0xff;
			g_commBuf[channel].Data[4] = (uchar)(tempL>>16)&0xff;			
			g_commBuf[channel].Data[5] = (uchar)(tempL>>24)&0xff;	 
		}
	#if(SINGLE_OR_THREE_METER==3)
		else if ( tempInt == 0xB611 )	//A���ѹ 2	 11
		{			
			g_commBuf[channel].Len = 4;					
			g_commBuf[channel].Data[2] = (uchar)g_uip[0].URms&0xff;
			g_commBuf[channel].Data[3] = (uchar)(g_uip[0].URms>>8)&0xff; 
		}
		else if ( tempInt == 0xB612 )	//B���ѹ 2	 11
		{			
			g_commBuf[channel].Len = 4;					
			g_commBuf[channel].Data[2] = (uchar)g_uip[1].URms&0xff;
			g_commBuf[channel].Data[3] = (uchar)(g_uip[1].URms>>8)&0xff; 
		}
		else if ( tempInt == 0xB613 )	//C���ѹ 2	 11
		{			
			g_commBuf[channel].Len = 4;					
			g_commBuf[channel].Data[2] = (uchar)g_uip[2].URms&0xff;
			g_commBuf[channel].Data[3] = (uchar)(g_uip[2].URms>>8)&0xff; 
		}
		else if ( tempInt == 0xB621 )	//A����� 3 11
		{			
			g_commBuf[channel].Len = 4;					
			g_commBuf[channel].Data[2] = (uchar)(g_uip[0].IRms>>4)&0xff;
			g_commBuf[channel].Data[3] = (uchar)(g_uip[0].IRms>>12)&0xff; 
		}
		else if ( tempInt == 0xB622 )	//B����� 3 11
		{			
			g_commBuf[channel].Len = 4;					
			g_commBuf[channel].Data[2] = (uchar)(g_uip[1].IRms>>4)&0xff;
			g_commBuf[channel].Data[3] = (uchar)(g_uip[1].IRms>>12)&0xff; 
		}
		else if ( tempInt == 0xB623 )	//C����� 3 11
		{			
			g_commBuf[channel].Len = 4;					
			g_commBuf[channel].Data[2] = (uchar)(g_uip[2].IRms>>4)&0xff;
			g_commBuf[channel].Data[3] = (uchar)(g_uip[2].IRms>>12)&0xff; 
		}
		else if ( tempInt == 0xB630 )	//���๦�� 3 11	
		{			
			g_commBuf[channel].Len = 5;					
			g_commBuf[channel].Data[2] = (uchar)(g_uip[3].PRms>>0)&0xff;
			g_commBuf[channel].Data[3] = (uchar)(g_uip[3].PRms>>8)&0xff; 
			g_commBuf[channel].Data[4] = (uchar)(g_uip[3].PRms>>16)&0xff; 
		}
		else if ( tempInt == 0xB631 )	//A�๦�� 3	 11
		{			
			g_commBuf[channel].Len = 5;					
			g_commBuf[channel].Data[2] = (uchar)(g_uip[0].PRms>>0)&0xff;
			g_commBuf[channel].Data[3] = (uchar)(g_uip[0].PRms>>8)&0xff; 
			g_commBuf[channel].Data[4] = (uchar)(g_uip[0].PRms>>16)&0xff; 
		}
		else if ( tempInt == 0xB632 )	//B�๦�� 3	 11
		{			
			g_commBuf[channel].Len = 5;					
			g_commBuf[channel].Data[2] = (uchar)(g_uip[1].PRms>>0)&0xff;
			g_commBuf[channel].Data[3] = (uchar)(g_uip[1].PRms>>8)&0xff; 
			g_commBuf[channel].Data[4] = (uchar)(g_uip[1].PRms>>16)&0xff; 
		}
		else if ( tempInt == 0xB633 )	//C�๦�� 3	 11
		{			
			g_commBuf[channel].Len = 5;					
			g_commBuf[channel].Data[2] = (uchar)(g_uip[2].PRms>>0)&0xff;
			g_commBuf[channel].Data[3] = (uchar)(g_uip[2].PRms>>8)&0xff; 
			g_commBuf[channel].Data[4] = (uchar)(g_uip[2].PRms>>16)&0xff; 
		}
	#endif
		else
		{
			s_commError[channel].Bit.noRequestData = 1;
			goto error_process;		
		}
	}
		break;
		
	case READ_DATA:	//������ 11
		switch ( g_commBuf[channel].Len )
		{
		case 4:	//��׼645Э�� 11
	#if(PROTOCOL_ADD_4_SERIALS==1)
		case 8:			
	#endif
			#if(PROTOCOL_ADD_4_SERIALS==1)
				if ( g_commBuf[channel].Len == 8 )
				{
					s_serialsLen[channel] = 4;
					for( i=0; i<4; i++ )
						s_serials[i] = g_commBuf[channel].Data[4+i];
				}			
			#endif
			
			switch ( g_commBuf[channel].Data[3] )
			{
			//���������� 11	
			case 0:		
				{
					if ( ProcessEnergyProtocol ( &g_commBuf[channel].Data[0],
							 &protocolReg ) != 0 )
					{
						s_commError[channel].Bit.noRequestData = 1;
						goto error_process;	
					}
				}
				break;
		
		//����ʹ�� 11
		#ifdef DEMAND_ENABLE	
			case 1:
				{
					if ( ProcessDemandProtocol ( ID, &protocolReg ) != 0 )
					{
						s_commError[channel].Bit.noRequestData = 1;
						goto error_process;	
					}	
				}
				break;
		#endif
			
			//�������� 11
			case 0x02:
				{
					if ( ProcessVariableProtocol ( ID, &protocolReg ) != 0 )
					{
						s_commError[channel].Bit.noRequestData = 1;
						goto error_process;	
					}
				}
				break;
			
			//�¼���¼ 11
			case 0x03:
			case 0x13:
				{
					if ( ProcessEventProtocol ( ID, &protocolReg ) != 0 )
					{
						s_commError[channel].Bit.noRequestData = 1;
						goto error_process;	
					}
					
					
				}
				break;
			
			//�α��� 11
			case 0x04:
				{
					if ( ProcessParaProtocol ( ID, &protocolReg ) != 0 )
					{
						s_commError[channel].Bit.noRequestData = 1;
						goto error_process;	
					}
				}	
				break;
			
			//�������� 11
		#if ( (FREEZE_HOUR_EANBLED==1)|| (FREEZE_DAY_EANBLED==1))	
			case 5:
				{
					if ( ProcessFreezeProtocol ( ID, &protocolReg ) != 0 )
					{
						s_commError[channel].Bit.noRequestData = 1;
						goto error_process;	
					}	
				}
				break;
		#endif
		
			default:	//����Ӧ���� 11	
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;
	 
			}
			
			g_commBuf[channel].Len = protocolReg.size;	
			if ( g_commBuf[channel].Len > 0 )
			{
				if ( (protocolReg.location == L_RAM) || 
					(protocolReg.location == L_RAM_E2P) )
				{					
					
					pchar = (uchar*)protocolReg.addr;
					if ( protocolReg.type == Hex2To4 )	//����2�ֽ�ͨѶ��չΪ4�ֽڣ�2λС�� 11
					{										
						//pint = (uint16*)protocolReg.addr;
						tempInt = *((uint16*)protocolReg.addr);
						ID = Hex32ToBCD( (uint32)tempInt*100 );				
						g_commBuf[channel].Data[4] = (uchar)ID&0xff;
						g_commBuf[channel].Data[5] = (uchar)(ID>>8)&0xff;
						g_commBuf[channel].Data[6] = (uchar)(ID>>16)&0xff;			
						g_commBuf[channel].Data[7] = (uchar)(ID>>24)&0xff;	
					}
					else if ( protocolReg.type==Hex2To2 )	//�������2 11
					{
						tempInt = *((uint16*)protocolReg.addr);
						ID = Hex16ToBCD( tempInt );
						g_commBuf[channel].Data[4] = (uchar)ID&0xff;
						g_commBuf[channel].Data[5] = (uchar)(ID>>8)&0xff;								
					}			
					else if ( protocolReg.type==Hex4To4 )
					{
						//plong = (uint32*)protocolReg.addr;	
						tempL = *((uint32*)protocolReg.addr);
						if (  (g_commBuf[channel].Data[2] == 0x90) ||
								(g_commBuf[channel].Data[2] == 0x91) )
							tempL /= 100;				
						ID = Hex32ToBCD( tempL );
						
						g_commBuf[channel].Data[4] = (uchar)ID&0xff;
						g_commBuf[channel].Data[5] = (uchar)(ID>>8)&0xff;
						g_commBuf[channel].Data[6] = (uchar)(ID>>16)&0xff;
						g_commBuf[channel].Data[7] = (uchar)(ID>>24)&0xff;			
					}
					else if ( protocolReg.type==Hex2To4_0 )
					{
						//plong = (uint32*)protocolReg.addr;
						tempL = (uint32)*((uint16*)protocolReg.addr);				
						ID = Hex32ToBCD( tempL );
						g_commBuf[channel].Data[4] = (uchar)ID&0xff;
						g_commBuf[channel].Data[5] = (uchar)(ID>>8)&0xff;
						g_commBuf[channel].Data[6] = (uchar)(ID>>16)&0xff;
						g_commBuf[channel].Data[7] = (uchar)(ID>>24)&0xff;			
					}
					else if ( protocolReg.type==Hex2To4_10 )
					{
						tempL = (uint32)*((uint16*)protocolReg.addr);				
						ID = Hex32ToBCD( tempL*10 );
						g_commBuf[channel].Data[4] = (uchar)ID&0xff;
						g_commBuf[channel].Data[5] = (uchar)(ID>>8)&0xff;
						g_commBuf[channel].Data[6] = (uchar)(ID>>16)&0xff;
						g_commBuf[channel].Data[7] = (uchar)(ID>>24)&0xff;			
					}
					else if ( protocolReg.type==Hex2To2 )	//购电次数2
					{
						tempInt = *((uint16*)protocolReg.addr);
						ID = Hex32ToBCD( (uint32)tempInt );
						g_commBuf[channel].Data[4] = (uchar)ID&0xff;
						g_commBuf[channel].Data[5] = (uchar)(ID>>8)&0xff;								
					}	
					else
					{
						for ( i=0; i<g_commBuf[channel].Len-4; i++ )
						{
							g_commBuf[channel].Data[4+i] = pchar[i];						
						}
						
						if ( ID == 0x0280002E )		//��ָ��� 11
					    {
					        //�ܵ��� 4 11     
					        ID = Hex32ToBCD( g_energy.Accumulate );						
    						g_commBuf[channel].Data[4] = (uchar)ID&0xff;
    						g_commBuf[channel].Data[5] = (uchar)(ID>>8)&0xff;
    						g_commBuf[channel].Data[6] = (uchar)(ID>>16)&0xff;
    						g_commBuf[channel].Data[7] = (uchar)(ID>>24)&0xff;	
    					
    					#if(SINGLE_OR_THREE_METER==1)
    						//A���ѹ 2	 11
    						g_commBuf[channel].Data[8] = (uchar)g_uip.URms&0xff;
    						g_commBuf[channel].Data[9] = (uchar)(g_uip.URms>>8)&0xff;
    						//B���ѹ 2	 11
    						g_commBuf[channel].Data[10] = 0;
    						g_commBuf[channel].Data[11] = 0;
    						//C���ѹ 2	 11
    						g_commBuf[channel].Data[12] = 0;
    						g_commBuf[channel].Data[13] = 0;
    						//A����� 3 11
    						g_commBuf[channel].Data[14] = (uchar)g_uip.IRms&0xff;
    						g_commBuf[channel].Data[15] = (uchar)(g_uip.IRms>>8)&0xff;
    						g_commBuf[channel].Data[16] = (uchar)(g_uip.IRms>>16)&0xff;
    						//B����� 3 11
    						g_commBuf[channel].Data[17] = 0;
    						g_commBuf[channel].Data[18] = 0;
    						g_commBuf[channel].Data[19] = 0;
    						//C����� 3 11
    						g_commBuf[channel].Data[20] = 0;
    						g_commBuf[channel].Data[21] = 0;
    						g_commBuf[channel].Data[22] = 0;
    						//A�๦�� 3	11
    						g_commBuf[channel].Data[23] = (uchar)g_uip.PRms&0xff;
    						g_commBuf[channel].Data[24] = (uchar)(g_uip.PRms>>8)&0xff;
    						g_commBuf[channel].Data[25] = (uchar)(g_uip.PRms>>16)&0xff;
    						//B�๦�� 3	 11
    						g_commBuf[channel].Data[26] = 0;
    						g_commBuf[channel].Data[27] = 0;
    						g_commBuf[channel].Data[28] = 0;
    						//C�๦�� 3	 11
    						g_commBuf[channel].Data[29] = 0;
    						g_commBuf[channel].Data[30] = 0;
    						g_commBuf[channel].Data[31] = 0;
    						//�ܹ��� 3	 11
    						g_commBuf[channel].Data[32] = (uchar)g_uip.PRms&0xff;
    						g_commBuf[channel].Data[33] = (uchar)(g_uip.PRms>>8)&0xff;
    						g_commBuf[channel].Data[34] = (uchar)(g_uip.PRms>>16)&0xff;
    					#endif
    					
    					#if(SINGLE_OR_THREE_METER==3)
    						//A���ѹ 2	 11
    						g_commBuf[channel].Data[8] = (uchar)g_uip[0].URms&0xff;
    						g_commBuf[channel].Data[9] = (uchar)(g_uip[0].URms>>8)&0xff;
    						//B���ѹ 2	 11
    						g_commBuf[channel].Data[10] = (uchar)g_uip[1].URms&0xff;
    						g_commBuf[channel].Data[11] = (uchar)(g_uip[1].URms>>8)&0xff;
    						//C���ѹ 2	 11
    						g_commBuf[channel].Data[12] = (uchar)g_uip[2].URms&0xff;
    						g_commBuf[channel].Data[13] = (uchar)(g_uip[2].URms>>8)&0xff;
    						//A����� 3 11
    						g_commBuf[channel].Data[14] = (uchar)g_uip[0].IRms&0xff;
    						g_commBuf[channel].Data[15] = (uchar)(g_uip[0].IRms>>8)&0xff;
    						g_commBuf[channel].Data[16] = (uchar)(g_uip[0].IRms>>16)&0xff;
    						//B����� 3 11
    						g_commBuf[channel].Data[17] = (uchar)g_uip[1].IRms&0xff;
    						g_commBuf[channel].Data[18] = (uchar)(g_uip[1].IRms>>8)&0xff;
    						g_commBuf[channel].Data[19] = (uchar)(g_uip[1].IRms>>16)&0xff;
    						//C����� 3 11
    						g_commBuf[channel].Data[20] = (uchar)g_uip[2].IRms&0xff;
    						g_commBuf[channel].Data[21] = (uchar)(g_uip[2].IRms>>8)&0xff;
    						g_commBuf[channel].Data[22] = (uchar)(g_uip[2].IRms>>16)&0xff;
    						//A�๦�� 3	11
    						g_commBuf[channel].Data[23] = (uchar)g_uip[0].PRms&0xff;
    						g_commBuf[channel].Data[24] = (uchar)(g_uip[0].PRms>>8)&0xff;
    						g_commBuf[channel].Data[25] = (uchar)(g_uip[0].PRms>>16)&0xff;
    						//B�๦�� 3	 11
    						g_commBuf[channel].Data[26] = (uchar)g_uip[1].PRms&0xff;
    						g_commBuf[channel].Data[27] = (uchar)(g_uip[1].PRms>>8)&0xff;
    						g_commBuf[channel].Data[28] = (uchar)(g_uip[1].PRms>>16)&0xff;
    						//C�๦�� 3	 11
    						g_commBuf[channel].Data[29] = (uchar)g_uip[2].PRms&0xff;
    						g_commBuf[channel].Data[30] = (uchar)(g_uip[2].PRms>>8)&0xff;
    						g_commBuf[channel].Data[31] = (uchar)(g_uip[2].PRms>>16)&0xff;
    						//�ܹ��� 3	 11
    						g_commBuf[channel].Data[32] = (uchar)g_uip[3].PRms&0xff;
    						g_commBuf[channel].Data[33] = (uchar)(g_uip[3].PRms>>8)&0xff;
    						g_commBuf[channel].Data[34] = (uchar)(g_uip[3].PRms>>16)&0xff;
    					#endif
    						
    						//ʣ���� 4 11
    						ID = Hex32ToBCD( g_energy.Balance/100 );						
    						g_commBuf[channel].Data[35] = (uchar)ID&0xff;
    						g_commBuf[channel].Data[36] = (uchar)(ID>>8)&0xff;
    						g_commBuf[channel].Data[37] = (uchar)(ID>>16)&0xff;
    						g_commBuf[channel].Data[38] = (uchar)(ID>>24)&0xff;
    						//������� 11
    						ID = Hex32ToBCD( g_buyPara.BuyCount );						
    						g_commBuf[channel].Data[39] = (uchar)ID&0xff;
    						g_commBuf[channel].Data[40] = (uchar)(ID>>8)&0xff;
    						g_commBuf[channel].Data[41] = (uchar)(ID>>16)&0xff;
    						g_commBuf[channel].Data[42] = (uchar)(ID>>24)&0xff;
    						
    						//�̵���״̬ 11
    						if ( g_relayFlag.Bit.RelayOn )
    						    g_commBuf[channel].Data[43] = 1;
    					    else
    					        g_commBuf[channel].Data[43] = 0;   
					    }
//					    else if ( ID == 0x02F00001 )		//A����� 11
//					    {
//    					    					
//    					#if(SINGLE_OR_THREE_METER==3)
//    						//A���ѹ 2	 11
//    						g_commBuf[channel].Data[4] = (uchar)g_uip[0].URms&0xff;
//    						g_commBuf[channel].Data[5] = (uchar)(g_uip[0].URms>>8)&0xff;   						
//    						//A����� 3 11
//    						g_commBuf[channel].Data[6] = (uchar)g_uip[0].IRms&0xff;
//    						g_commBuf[channel].Data[7] = (uchar)(g_uip[0].IRms>>8)&0xff;
//    						g_commBuf[channel].Data[8] = (uchar)(g_uip[0].IRms>>16)&0xff;
//    						//A�๦�� 3	11
//    						g_commBuf[channel].Data[9] = (uchar)g_uip[0].PRms&0xff;
//    						g_commBuf[channel].Data[10] = (uchar)(g_uip[0].PRms>>8)&0xff;
//    						g_commBuf[channel].Data[11] = (uchar)(g_uip[0].PRms>>16)&0xff;    						
//    						//A����� 4	 11
//    						g_commBuf[channel].Data[12] = (uchar)g_energy.APhaseAccu&0xff;
//    						g_commBuf[channel].Data[13] = (uchar)(g_energy.APhaseAccu>>8)&0xff;
//    						g_commBuf[channel].Data[14] = (uchar)(g_energy.APhaseAccu>>16)&0xff;
//    						g_commBuf[channel].Data[15] =(uchar)(g_energy.APhaseAccu>>24)&0xff;
//    					#endif
//    						
//    						
//					    }
//					    else if ( ID == 0x02F00002 )		//B����� 11
//					    {
//    					    					
//    					#if(SINGLE_OR_THREE_METER==3)
//    						//B���ѹ 2	 11
//    						g_commBuf[channel].Data[4] = (uchar)g_uip[1].URms&0xff;
//    						g_commBuf[channel].Data[5] = (uchar)(g_uip[1].URms>>8)&0xff;   						
//    						//B����� 3 11
//    						g_commBuf[channel].Data[6] = (uchar)g_uip[1].IRms&0xff;
//    						g_commBuf[channel].Data[7] = (uchar)(g_uip[1].IRms>>8)&0xff;
//    						g_commBuf[channel].Data[8] = (uchar)(g_uip[1].IRms>>16)&0xff;
//    						//B�๦�� 3	11
//    						g_commBuf[channel].Data[9] = (uchar)g_uip[1].PRms&0xff;
//    						g_commBuf[channel].Data[10] = (uchar)(g_uip[1].PRms>>8)&0xff;
//    						g_commBuf[channel].Data[11] = (uchar)(g_uip[1].PRms>>16)&0xff;    						
//    						//B����� 4	 11
//    						g_commBuf[channel].Data[12] = (uchar)g_energy.BPhaseAccu&0xff;
//    						g_commBuf[channel].Data[13] = (uchar)(g_energy.BPhaseAccu>>8)&0xff;
//    						g_commBuf[channel].Data[14] = (uchar)(g_energy.BPhaseAccu>>16)&0xff;
//    						g_commBuf[channel].Data[15] =(uchar)(g_energy.BPhaseAccu>>24)&0xff;
//    					#endif
//    						
//    						
//					    }
//					    else if ( ID == 0x02F00003 )		//C����� 11
//					    {
//    					    					
//    					#if(SINGLE_OR_THREE_METER==3)
//    						//B���ѹ 2	 11
//    						g_commBuf[channel].Data[4] = (uchar)g_uip[2].URms&0xff;
//    						g_commBuf[channel].Data[5] = (uchar)(g_uip[2].URms>>8)&0xff;   						
//    						//B����� 3 11
//    						g_commBuf[channel].Data[6] = (uchar)g_uip[2].IRms&0xff;
//    						g_commBuf[channel].Data[7] = (uchar)(g_uip[2].IRms>>8)&0xff;
//    						g_commBuf[channel].Data[8] = (uchar)(g_uip[2].IRms>>16)&0xff;
//    						//B�๦�� 3	11
//    						g_commBuf[channel].Data[9] = (uchar)g_uip[2].PRms&0xff;
//    						g_commBuf[channel].Data[10] = (uchar)(g_uip[2].PRms>>8)&0xff;
//    						g_commBuf[channel].Data[11] = (uchar)(g_uip[2].PRms>>16)&0xff;    						
//    						//B����� 4	 11
//    						g_commBuf[channel].Data[12] = (uchar)g_energy.CPhaseAccu&0xff;
//    						g_commBuf[channel].Data[13] = (uchar)(g_energy.CPhaseAccu>>8)&0xff;
//    						g_commBuf[channel].Data[14] = (uchar)(g_energy.CPhaseAccu>>16)&0xff;
//    						g_commBuf[channel].Data[15] =(uchar)(g_energy.CPhaseAccu>>24)&0xff;
//    					#endif
//    						
//    						
//					    }
//					    else if ( ID == 0x02F00004 )		//�������� 11
//					    {
//    					    					
//    					#if(SINGLE_OR_THREE_METER==3)
//    						//�ܵ���1 4	 11
//    						g_commBuf[channel].Data[4] = (uchar)g_energy.Accumulate&0xff;
//    						g_commBuf[channel].Data[5] = (uchar)(g_energy.Accumulate>>8)&0xff;
//    						g_commBuf[channel].Data[6] = (uchar)(g_energy.Accumulate>>16)&0xff;
//    						g_commBuf[channel].Data[7] =(uchar)(g_energy.Accumulate>>24)&0xff;
//    						//״̬�� 11
//    						g_commBuf[channel].Data[8] = (uchar)(g_meterStatus3.Byte)&0x07;
//    					#endif    						   						
//					    }
					}										
				}
				else if ( protocolReg.location == L_EEPROM )
				{					
					I2CRead ( EEPROM_ADDR, protocolReg.eAddr, &g_commBuf[channel].Data[4], 
								protocolReg.size-4, 1  ) ;  
					
					if ( protocolReg.type==Hex4To4 )	//�ϴι����� 11
					{
						tempL = *((uint32*)&g_commBuf[channel].Data[4]);	
						//plong = (uint32*)&g_commBuf[channel].Data[4];
						if ( ((ID&0x009000)==0x009000) )
						{
							tempL /= 100;
						}
																		
						ID = Hex32ToBCD( tempL );
						g_commBuf[channel].Data[4] = (uchar)ID&0xff;
						g_commBuf[channel].Data[5] = (uchar)(ID>>8)&0xff;
						g_commBuf[channel].Data[6] = (uchar)(ID>>16)&0xff;
						g_commBuf[channel].Data[7] = (uchar)(ID>>24)&0xff;		
					}	
					else if ( protocolReg.type==Hex2To4 )	//
					{
						pint = (uint16*)&g_commBuf[channel].Data[4];
						ID = Hex32ToBCD( (uint32)*pint );
						g_commBuf[channel].Data[4] = (uchar)ID&0xff;
						g_commBuf[channel].Data[5] = (uchar)(ID>>8)&0xff;
						g_commBuf[channel].Data[6] = (uchar)(ID>>16)&0xff;
						g_commBuf[channel].Data[7] = (uchar)(ID>>24)&0xff;
								
					}
					else if ( protocolReg.type==Hex2To3_1 )	//����ֵ 11
					{
						tempInt = *((uint16*)&g_commBuf[channel].Data[4]);
						ID = Hex32ToBCD( (uint32)tempInt );
						g_commBuf[channel].Data[4] = 0;
						g_commBuf[channel].Data[5] = (uchar)ID&0xff;
						g_commBuf[channel].Data[6] = (uchar)(ID>>8)&0xff;
						g_commBuf[channel].Data[7] = (uchar)(ID>>16)&0xff;						
								
					} 	
					else if ( protocolReg.type==Hex2To2 )	//�������2 11
					{
						tempInt = *((uint16*)&g_commBuf[channel].Data[4]);
						ID = Hex32ToBCD( tempInt );
						g_commBuf[channel].Data[4] = (uchar)ID&0xff;
						g_commBuf[channel].Data[5] = (uchar)(ID>>8)&0xff;								
					}																		
				}
				
					
			}
															
			break;
		
		//У��ָ�� 11
	#if(METER_CHIP_MODEL==0x2208)
		case 7:  //У����ʼ��ָ�� 11
			//2218ָ�� 11
			if ( g_buyPara.Status != MANUFACTURE_STATUS )
	        {
	        	s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
	        }
	        
	        if ( g_energy.Accumulate > 1000 )
			{
	        	s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
	        }
			
			if ( (g_commBuf[channel].Data[0] != 0x01) ||  
				(g_commBuf[channel].Data[1] != 0xFC) )
			{
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;	
			}	
			
			g_initDispCnt = LCD_CLEAR_CNT;
			DisplayCali ( 0 );
			
			CopyRamData ( (uchar*)&g_cl2208para.pstart, (uchar*)&CALIBRATE_TABLE_value[0],CALIREG_NUM*2 );
			
			//�޸�HFCONST 11
			if ( g_commBuf[channel].Data[2] == 0x02 ) 
			{
				 //g_cl2208para.hfconst= ((uint16)g_commBuf[channel].Data[4]<<8)
				 //		+g_commBuf[channel].Data[3];					
			}
			else if ( g_commBuf[channel].Data[2] == 0x00 ) 
			{					
			}
			
			g_cl2208para.Reserved = 0;
			g_factor.UFactor = U_FACTOR;
			g_factor.IFactor = IL_FACTOR;
			g_factor.PFactor = P_FACTOR;
			g_factor.Reserved[0] = 0;
			g_factor.Reserved[1] = 0;
			g_factor.Reserved[2] = 0;
			
			SoftResetCl2208 ( );
			if ( CalibrateCl2208 ( ) != 0 )
			{
				s_commError[channel].Bit.otherErr = 1;
				goto error_process;	
			}	
				
			g_commBuf[channel].Len = 0;	
							
			break;
		
		case 0x11: //У��ָ�� 11
			//2218ָ�� 11
			if ( g_buyPara.Status != MANUFACTURE_STATUS )
	        {
	        	s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
	        }
	        
	        if ( g_energy.Accumulate > 1000 )
			{
	        	s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
	        }
	        
			switch ( g_commBuf[channel].Data[0] )  //У�������� 11
			{
			case 0x11://100%Ib��1.0(L) 11
				g_initDispCnt = LCD_CLEAR_CNT;
				DisplayCali ( 1 );
				if ( CaliGainPara ( &g_commBuf[channel].Data[1] ) != 0 )
				{
					s_commError[channel].Bit.otherErr = 1;
					goto error_process;	
				}		
				break;
			case 0x15://100%Ib��0.5L(L) 11
				g_initDispCnt = LCD_CLEAR_CNT;
				DisplayCali ( 2 );
			
				if ( CaliPhasePara ( &g_commBuf[channel].Data[1] ) != 0 )
				{
					s_commError[channel].Bit.otherErr = 1;
					goto error_process;	
				}	
				break;
			case 0x31://5%Ib��1.0(L) 11
				g_initDispCnt = LCD_CLEAR_CNT;
				DisplayCali ( 3 );
			
				if ( CaliOffsetPara ( &g_commBuf[channel].Data[1] ) != 0 )
				{
					s_commError[channel].Bit.otherErr = 1;
					goto error_process;	
				}	
				break;
			case 0xc0://����У��(L) 11
				g_initDispCnt = LCD_CLEAR_CNT;
				DisplayCali ( 4 );
				if ( CaliIARMSPara ( &g_commBuf[channel].Data[1] ) != 0 )
				{
					s_commError[channel].Bit.otherErr = 1;
					goto error_process;	
				}	
				break;
			//case 0x51://100%Ib��1.0(N) 11
				
			//	break;
			default:
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;	
				//break;
			}
			g_commBuf[channel].Len = 0;	
			break;
			
	#else if (METER_CHIP_MODEL==0x8302)	
		case 7:  //У����ʼ��ָ�� 11
			//2218ָ�� 11
			
			if ( g_buyPara.Status != MANUFACTURE_STATUS )
	        {
	        	s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
	        }
	        
	        if ( g_energy.Accumulate > 1000 )
			{
	        	s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
	        }
	        
			if ( (g_commBuf[channel].Data[0] != 0x01) ||  
				(g_commBuf[channel].Data[1] != 0xFC) )
			{
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;	
			}	
			
			g_initDispCnt = LCD_CLEAR_CNT;
			DisplayCali ( 0 );
			
			pchar = (uchar*)&g_rn8302para.gsua;
			for ( i=0; i<32; i++ )
			{
				*pchar++ = 0;		
			}
			
			if ( StoreCaliPara ( ) != 0 )
			{								
				s_commError[channel].Bit.otherErr = 1;
				
				goto error_process;	
			}
					
			if ( SetRN8302_Register ( ) != 0 )
			{
				s_commError[channel].Bit.otherErr = 1;
				goto error_process;	
			}	
												
			g_commBuf[channel].Len = 0;	
							
			break;
		
		case 0x11: //У��ָ�� 11
			if ( g_buyPara.Status != MANUFACTURE_STATUS )
	        {
	        	s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
	        }
			
			if ( g_energy.Accumulate > 1000 )
			{
	        	s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
	        }
	        
			switch ( g_commBuf[channel].Data[0] )  //У�������� 11
			{
			case 0x11://100%Ib��1.0(L) 11
				g_initDispCnt = LCD_CLEAR_CNT;
				DisplayCali ( 1 );
			
				if ( CaliGainPara ( ) != 0 )
				{
					s_commError[channel].Bit.otherErr = 1;
					goto error_process;	
				}		
				break;
			case 0x15://100%Ib��0.5L(L) 111
				g_initDispCnt = LCD_CLEAR_CNT;
				DisplayCali ( 2 );
				
				if ( CaliPhasePara ( ) != 0 )
				{
					s_commError[channel].Bit.otherErr = 1;
					goto error_process;	
				}	
				break;
			case 0x31://5%Ib��1.0(L) 11
				g_initDispCnt = LCD_CLEAR_CNT;
				DisplayCali ( 3 );
				
				if ( CaliOffsetPara ( ) != 0 )
				{
					s_commError[channel].Bit.otherErr = 1;
					goto error_process;	
				}	
				break;
			case 0xc0://����У��(L) 11
				g_initDispCnt = LCD_CLEAR_CNT;
				DisplayCali ( 4 );
				
				if ( CaliOffsetPara ( ) != 0 )
				{
					s_commError[channel].Bit.otherErr = 1;
					goto error_process;	
				}	
				break;
			//case 0x51://100%Ib��1.0(N) 11
				
			//	break;
			default:
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;	
				//break;
			}
			g_commBuf[channel].Len = 0;	
			break;
	#endif
			
		default:
			s_commError[channel].Bit.noRequestData = 1;
			goto error_process;	
			//break;	
		}
				
		break;	
	
	case WRITE_DATA:	//д���� 11
		{
			//Ӳ��ģʽ�����������п���������·������� 11
			
			
			//�����ж� 11
			if ( CheckPassword ( &g_commBuf[channel].Data[4] ) != 0 )
			{
				s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
			}
			
			//ֻ�вα���֧��д���� 11
			if ( ProcessParaProtocol ( ID, &protocolReg ) != 0 )
			{
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;	
			}
			
			//����ֻ�����ڳ���ģʽ�� 11
			if ( R_Enable == protocolReg.readOrWrite )
			{
				s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
			}	
			
			//ʹ�����ݳ����ж��Ƿ��β�����к� 11
		#if(PROTOCOL_ADD_4_SERIALS==1)
			if ( (g_commBuf[channel].Len-12) == protocolReg.size )
			{
				len = g_commBuf[channel].Len-4;
				s_serialsLen[channel] = 4;
				for( i=0; i<4; i++ )
					s_serials[i] = g_commBuf[channel].Data[len+i];
			}			
		#endif	
						
			//����RAM���� 11
			if ( L_EEPROM != (uchar)protocolReg.location )
			{								
				pchar = (uchar*)protocolReg.addr;
				if ( protocolReg.type == Hex2To4 )	//����2�ֽ�ͨѶ��չΪ4�ֽڣ�2λС�� 11
				{															
					tempL = BCDToHex( &g_commBuf[channel].Data[12], 4 );
    				*((uint16*)protocolReg.addr) = (uint16)(tempL/100);
				}
				else if ( protocolReg.type==Hex4To4 )
				{
					tempL = BCDToHex( &g_commBuf[channel].Data[12], 4 );
    				*((uint32*)protocolReg.addr) = tempL;		
				}
				else if ( protocolReg.type==Hex2To4_0 )
				{
					tempL = BCDToHex( &g_commBuf[channel].Data[12], 4 );
    				*((uint16*)protocolReg.addr) = (uint16)(tempL);		
				}
				else if ( protocolReg.type==Hex2To4_10 )
				{
					tempL = BCDToHex( &g_commBuf[channel].Data[12], 4 );
    				*((uint16*)protocolReg.addr) = (uint16)(tempL/10);		
				}
				else
				{
					
					for ( i=0; i<protocolReg.size-4; i++ )
					{
						pchar[i] = g_commBuf[channel].Data[12+i];						
					}	
				}
			}
			
			if ( protocolReg.checksumOrNot == BCDCheck_E )	//�������ݵ��洢��������У��洢 11
			{
				EEPROM_NOWP();
			    I2CWrite ( EEPROM_ADDR, protocolReg.eAddr, 
			    	pchar, protocolReg.size-4, 1  );
				EEPROM_WP();
			
			#ifdef RTC_8025T 
				if ( ID == 0x04000101 )	//���� 11
				{
					if ( SetTime ( (uchar*)&g_date.sec ) != 0 )
					{
						s_commError[channel].Bit.otherErr = 1;
						goto error_process;	
					}	
				}
				else if ( ID == 0x04000102 )	//ʱ�� 11
				{
					if ( SetTime ( (uchar*)&g_date.sec ) != 0 )
					{
						s_commError[channel].Bit.otherErr = 1;
						goto error_process;	
					}			
				}
			#endif
			
			}
			else if ( protocolReg.checksumOrNot == BCDCheck_ECS )	//�������ݵ��洢����У�鱸�ݴ洢 11
			{
				if ( (ID == 0x04000401) )	//ͨѶ��ַ 11
						
				{
					g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
					StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
							(uchar*)&g_addr.Zone,Meter_Addr_Len );
					
				}
			
			#ifdef DEMAND_ENABLE	
				else if ( (ID == 0x04000103) || (ID == 0x04000104) )	//11
				{ 										
					g_demand.Checksum = GetSum ( (uchar*)&g_demand.PosActive, Meter_Demand_Len-1 );
					EEPROM_NOWP();
					Delay1MsTime ( 1 );
				    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,DemandPara),
						(uchar*)&g_demand.PosActive,Meter_Demand_Len, 1  );
					EEPROM_WP();
					
				}
			#endif
			
				else if ( (ID == 0x04001001) || 	//�������1 11
							(ID == 0x04001002)	||	//�������2 11
							(ID == 0x04090E10)	||	//����������ֵ 4�ֽ� 11
							(ID == 0x04090E11)	)	//����������Ӧ������ֵ 4�ֽ� 11
				{
					g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
					StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
						(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );
					
				}
				else if ( (ID == 0x04001003) ||  //͸֧�����ֵ 4�ֽ� 11
							( ID == 0x04001004 ) ||	//�ڻ������ֵ 11
							( ID == 0x04900102 ) ||	//���������Ч���ģʽ�� 11
							( ID == 0x04500005 ) ||	//�޵繦�� 11
							( ID == 0x04900103 ) )	//ʱ�����բ����ʹ�ܱ�ʶ 11
				{ 
					g_para.Checksum = GetSum ( (uchar*)&g_para.LimitPower, Meter_Para_Len-1 );
					StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LimitPower),
						(uchar*)&g_para.LimitPower,Meter_Para_Len );	
				}
				else if ( (ID == 0x04001003) )	//��������ۼ�ģʽ�� 11
				{ 
					SaveEnergy ( NORMAL_SAVE );	
				}
				
			#ifdef TARIFF_ENABLE
				else if ( ID == 0x04000201 )	//��ʱ���� 11
				{				 					
					I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,YearZoneNum), 
					    &g_buf[0], 16, 1  ) ; 
				    g_buf[0] = g_commBuf[channel].Data[12]; 
					g_buf[15] = GetSum ( &g_buf[0], 15 );
	    			StoreToE2ROM ( offsetof(EEPROM_DATA,YearZoneNum),
							&g_buf[0], 16 );
				}
				else if ( ID == 0x04000202 )	//ʱ�α��� 11
				{ 					
					I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,YearZoneNum), 
					    &g_buf[0], 16, 1  ) ; 
				    g_buf[1] = g_commBuf[channel].Data[12]; 
					g_buf[15] = GetSum ( &g_buf[0], 15 );
	    			StoreToE2ROM ( offsetof(EEPROM_DATA,YearZoneNum),
							&g_buf[0], 16 );
				}
				else if ( ID == 0x04000203 )	//��ʱ�α��� 11
				{ 					
					I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,YearZoneNum), 
					    &g_buf[0], 16, 1  ) ; 
				    g_buf[2] = g_commBuf[channel].Data[12]; 
					g_buf[15] = GetSum ( &g_buf[0], 15 );
	    			StoreToE2ROM ( offsetof(EEPROM_DATA,YearZoneNum),
							&g_buf[0], 16 );
				}
				else if ( ID == 0x04010000 )	//��ʱ��ʱ�α� 11
				{ 					
					I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,YearZoneNum), 
					    &g_buf[0], 16, 1  ) ; 
				    for ( i=0; i<12; i++ )
				        g_buf[3+i] = g_commBuf[channel].Data[12+i]; 
					g_buf[15] = GetSum ( &g_buf[0], 15 );
	    			StoreToE2ROM ( offsetof(EEPROM_DATA,YearZoneNum),
							&g_buf[0], 16 );
				}
				else if ( (ID >= 0x04010001) && (ID < 0x04010009) )	//8��ʱ�α� 11
				{ 					
					tempInt = (uint16)(ID - 0x04010001);
					g_commBuf[channel].Data[54] = GetSum ( &g_commBuf[channel].Data[12], 42 );
	    			StoreToE2ROM ( offsetof(EEPROM_DATA,HHMMNN1)+48*tempInt,
							(uchar*)&g_commBuf[channel].Data[12],43 );
				}
				else if ( (ID>=0x04050101) && (ID<=0x04050106)) //���ʵ��1 11
				{
					i = g_commBuf[channel].Data[0];
					ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,TariffPrice1),
						&g_buf[0],CHECK_ADDMOD,16 );
										
					tempL = BCDToHex( &g_commBuf[channel].Data[12], 4 );
					
					g_buf[2*(i-1)] = (uchar)(tempL&0xff);
					g_buf[2*(i-1)+1] = (uchar)((tempL>>8)&0xff);
					g_buf[15] = GetSum ( g_buf, 15 );
	    			StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,TariffPrice1),
						(uchar*)g_buf,16 );	
				}
			#endif
			
			#ifdef LADDER_ENABLE
				else if ( (ID>=0x04060001) && (ID<=0x04060005)) //����ֵ1 11
				{
					i = g_commBuf[channel].Data[0];
					I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Ladder1), 
					    &g_buf[0], 16, 1  ) ;					
					
					//С����Ҫ 11
					tempL = BCDToHex( &g_commBuf[channel].Data[12], 4 );
					tempL /= 100;
					
					g_buf[2*(i-1)] = (uchar)(tempL&0xff);
					g_buf[2*(i-1)+1] = (uchar)((tempL>>8)&0xff);
					
					g_buf[14] = GetSum ( g_buf, 14 );
	    			StoreToE2ROM ( offsetof(EEPROM_DATA,Ladder1),
						(uchar*)g_buf,15 );	
						
					g_meterFlag.Bit.LadderProcess = 1;
				}
				else if ( (ID>=0x04060101) && (ID<=0x04060106)) //���ݵ��1 11
				{
					i = g_commBuf[channel].Data[0];
					I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,LadderPrice1), 
					    &g_buf[0], 16, 1  ) ;
					
					tempL = BCDToHex( &g_commBuf[channel].Data[12], 4 );
					
					g_buf[2*(i-1)] = (uchar)(tempL&0xff);
					g_buf[2*(i-1)+1] = (uchar)((tempL>>8)&0xff);
					
	    			StoreToE2ROM ( offsetof(EEPROM_DATA,LadderPrice1),
							(uchar*)g_buf,16 );	
							
					g_meterFlag.Bit.LadderProcess = 1;
				}
				else if ( (ID==0x04060f01) ) //���ݵ��1 11
				{
					I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,YYMM), 
					    &g_buf[0], 16, 1  ) ;
					
					g_buf[0] = g_commBuf[channel].Data[12];
					g_buf[1] = g_commBuf[channel].Data[13];
					
					//У��� 11
					g_buf[15] = GetSum ( &g_buf[0], 15 );
					StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,YYMM),
						 (uchar*)&g_buf[0],16 );												
				}
				else if ( (ID==0x04060f02) ) //���ݵ��1 11
				{
					I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,YYMM), 
					    &g_buf[0], 16, 1  ) ;
					
					g_buf[2] = g_commBuf[channel].Data[12];
					g_buf[3] = g_commBuf[channel].Data[13];
					g_buf[4] = g_commBuf[channel].Data[14];
					
					//У��� 11
					g_buf[15] = GetSum ( &g_buf[0], 15 );
					StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,YYMM),
						 (uchar*)&g_buf[0],16 );
							
					g_meterFlag.Bit.LadderProcess = 1;
				}
			#endif
			
			}
			g_commBuf[channel].Len = 0;	
			
		}	
		break;
	
	//��ͨ�ŵ�ַ 11
	case READ_ADDRESS:
		{
		#if(PROTOCOL_ADD_4_SERIALS==0)
			if ( g_commBuf[channel].Len != 0 )
		#else
			if ( (g_commBuf[channel].Len != 0) && (g_commBuf[channel].Len != 4) )
		#endif
			{
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;	
			}	
		
		#if(PROTOCOL_ADD_4_SERIALS==1)
			if ( g_commBuf[channel].Len == 4 )
			{
				s_serialsLen[channel] = 4;
				for( i=0; i<4; i++ )
					s_serials[i] = g_commBuf[channel].Data[i];
			}			
		#endif	
			
			g_commBuf[channel].Len = 6;	
			for ( i=0; i<6; i++ )
			{
				g_commBuf[channel].Data[i] = g_addr.MeterAddr[i];						
			}	
			
		}
		break;
	
	//дͨ�ŵ�ַ 11
	case WRITE_ADDRESS:
		{
		#if(PROTOCOL_ADD_4_SERIALS==0)
			if ( g_commBuf[channel].Len != 6 )
		#else
			if ( (g_commBuf[channel].Len != 6) && (g_commBuf[channel].Len != 10) )
		#endif
			{
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;	
			}	
		
		#if(PROTOCOL_ADD_4_SERIALS==1)
			if ( g_commBuf[channel].Len == 10 )
			{
				s_serialsLen[channel] = 4;
				for( i=0; i<4; i++ )
					s_serials[i] = g_commBuf[channel].Data[6+i];
			}			
		#endif	
			
			g_commBuf[channel].Len = 1;	
			for ( i=0; i<6; i++ )
			{
				g_addr.MeterAddr[i] = g_commBuf[channel].Data[i];						
			}	
			//����ַ���� 11
		    FeedWatchdog ( );
			g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
			StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
				(uchar*)&g_addr.Zone,Meter_Addr_Len );
		}
		break;
		
	#ifdef RTC_8025T
		//ע����չΪ7�ֽ� ���ʱ�������� 11
	case BROADCAST_TIMESET:
		g_date.sec = g_commBuf[channel].Data[0];
		g_date.min = g_commBuf[channel].Data[1];
		g_date.hour = g_commBuf[channel].Data[2];
		
		g_date.week = g_commBuf[channel].Data[3];
		
		g_date.day = g_commBuf[channel].Data[4];
		g_date.month = g_commBuf[channel].Data[5];
		g_date.year = g_commBuf[channel].Data[6];
		
		if ( SetTime ( (uchar*)&g_date.sec ) != 0 )
		{
			s_commError[channel].Bit.otherErr = 1;
			goto error_process;	
		}	
		goto no_answer1;
	#endif
		
	//Զ�̿��� 11
	case REMOTE_CONTROL:
		{
		#if(PROTOCOL_ADD_4_SERIALS==0)
			//���ݳ����ж� 11
			if ( g_commBuf[channel].Len != 16 )
		#else
			if ( (g_commBuf[channel].Len != 16) && (g_commBuf[channel].Len != 20) )
		#endif			
			{
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;	
			}
		
		#if(PROTOCOL_ADD_4_SERIALS==1)
			if ( g_commBuf[channel].Len == 20 )
			{
				s_serialsLen[channel] = 4;
				for( i=0; i<4; i++ )
					s_serials[i] = g_commBuf[channel].Data[16+i];
			}			
		#endif	
							
			//�����ж� 11
			if ( CheckPassword ( &g_commBuf[channel].Data[0] ) != 0 )
			{
				s_commError[channel].Bit.badPassword = 1;
				goto error_process;	
			}
			
			//ʱ���ж� N3~N8 sshhmmddmmyy 11
			
			
			//���Ʒ�ʽ�ж� 11
			if ( g_commBuf[channel].Data[8] == 0x1A ) //��բ 11
			{
				if ( (g_para.MeterTpye&BIT0) == BIT0 )		//����ģʽ 11
				{
					s_commError[channel].Bit.otherErr = 1;
					goto error_process;	
				}	
				
				g_relayFlag.Bit.RelayOn = 1;
	           	g_relayFlag.Bit.RelayNeedToOperate = 1;
	           		           		
	           	g_para.MeterTpye |= BIT1;  //����Զ����բ 11
	           	g_meterStatus3.Bit.RelayCmdStatus = 1;
			}
			else if( (g_commBuf[channel].Data[8] == 0x1b) || 
						(g_commBuf[channel].Data[8] == 0x1c)) //��բ 11
			{
			#ifdef PRE_PAID_ENABLE
				//ʣ�����ж� 11
    			if ( g_energy.Balance == 0)
    			{
    				s_commError[channel].Bit.balanceZero = 1;
    				goto error_process;	
    			}
			#endif
				
				g_relayFlag.Bit.RelayOn = 0;				
	           	g_relayFlag.Bit.RelayNeedToOperate = 1; 
	           	g_para.MeterTpye &= ~BIT1;  //ȡ��Զ����բ 11	
	           	g_meterStatus3.Bit.RelayCmdStatus = 0;
	           	
	           	g_meterStatus3.Bit.MagliStatus = 0;
			}
			else if( g_commBuf[channel].Data[8] == 0x2A ) //���� 11 
			{
				ALARM_ON();
			}
			else if( g_commBuf[channel].Data[8] == 0x2B ) //����  11
			{
				ALARM_OFF();
			}
			else if( g_commBuf[channel].Data[8] == 0x3A ) //���� 11
			{
				g_relayFlag.Bit.RelayOn = 0;				
	           	g_relayFlag.Bit.RelayNeedToOperate = 1;  //ִ�к�բ���� 11
	           		
	           	g_para.MeterTpye |= BIT0;  //���ñ���ģʽ 11
	           	g_meterStatus3.Bit.KeepRelay = 1; 
	           	
	           	g_para.MeterTpye &= ~BIT1;  //ȡ��Զ����բ 11	
	           	g_meterStatus3.Bit.RelayCmdStatus = 0;
	           	
	           	g_meterStatus3.Bit.MagliStatus = 0;
			}
			else if( g_commBuf[channel].Data[8] == 0x3B ) //������ 11
			{
				g_para.MeterTpye &= ~BIT0; 	
				g_meterStatus3.Bit.KeepRelay = 0; 
			}
		#if(IC_CARD_MODE==PLC_COMM_USED)
			else if( g_commBuf[channel].Data[8] == 0x4A ) //��λģ�� 11
			{
				g_PLCRstCnt = 500; 
			}
		#endif
			else
			{
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;		
			}	
			
			//����բ������ʶ 11
			if( g_relayFlag.Bit.RelayAlarmOn )
				g_relayFlag.Bit.RelayAlarmOn = 0;
				
			g_para.Checksum = GetSum ( (uchar*)&g_para.LimitPower, Meter_Para_Len-1 );
           	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LimitPower),
				(uchar*)&g_para.LimitPower,Meter_Para_Len );
           	
           	g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
			StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
				(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );	
				
			ProcessRelayStatus ( );
    		ProcessRelay ( );				
				
		}
		
		if ( g_commStatus[channel].Bit.IsBroadcast ||
			g_commStatus[channel].Bit.IsSuperAddress )
		{		
			goto no_answer;	
		}
		
		g_commBuf[channel].Len = 6;	
		g_commBuf[channel].Data[0] = g_commBuf[channel].Data[8];
		g_commBuf[channel].Data[1] = 0x00;
		g_commBuf[channel].Data[2] = 0x00;
		g_commBuf[channel].Data[3] = 0x00;	
		g_commBuf[channel].Data[4] = (uchar)(g_meterStatus3.Byte&0xff);
		g_commBuf[channel].Data[5] = (uchar)((g_meterStatus3.Byte>>8)&0xff);	
				
		break;
		
	case 0x60:	//������оƬ�Ĵ���	 11
	#if(METER_CHIP_MODEL==0x2208)
		{
			len = GetRegLen ( g_commBuf[channel].Data[0] );
			SPIRead_REG ( g_commBuf[channel].Data[0],&g_commBuf[channel].Data[10], len );
			g_commBuf[channel].Len = 5;			
			for ( i=0; i<len; i++ )
			{
				//if ( len == 4 )
					g_commBuf[channel].Data[i+1] = g_commBuf[channel].Data[10+i];
				//else	
					//g_commBuf[channel].Data[i+1] = g_commBuf[channel].Data[10+i];
			}
			
		}
	#else if (METER_CHIP_MODEL==0x8302)
		{
			len = GetRegLen ( g_commBuf[channel].Data[1], g_commBuf[channel].Data[0]);
			SPIRead_REG ( g_commBuf[channel].Data[1],&g_commBuf[channel].Data[10], len,g_commBuf[channel].Data[0] );
			g_commBuf[channel].Len = 6;			
			for ( i=0; i<4; i++ )
			{
				//if ( len == 4 )
					g_commBuf[channel].Data[i+2] = g_commBuf[channel].Data[10+i];
				//else	
					//g_commBuf[channel].Data[i+1] = g_commBuf[channel].Data[10+i];
			}
			
		}	
	#endif
		
		break;	
	
	case 0x61:	//д����оƬ�Ĵ���	 11
	#if(METER_CHIP_MODEL==0x2208)
		{
			//���Ӻ�������̬�ж� 11
			if ( g_buyPara.Status != MANUFACTURE_STATUS )
			{
				s_commError[channel].Bit.badPassword = 1;
				goto error_process;		
			}
			//if ( g_commBuf[channel].Len != 5 )
			if ( g_commBuf[channel].Data[0] == 0xff ) 		//��ʼ��ָ�� 11
			{
				CopyRamData ( (uchar*)&g_cl2208para.pstart, (uchar*)&CALIBRATE_TABLE_value[0],CALIREG_NUM*2 );
					
				g_cl2208para.Reserved = 0;
				g_factor.UFactor = U_FACTOR;
				g_factor.IFactor = IL_FACTOR;
				g_factor.PFactor = P_FACTOR;
				g_factor.Reserved[0] = 0;
				g_factor.Reserved[1] = 0;
				g_factor.Reserved[2] = 0;
				
				SoftResetCl2208 ( );
				CalibrateCl2208 ( );
				
				g_commBuf[channel].Len = 0;			
				break;
			}
			
			len = GetRegLen ( g_commBuf[channel].Data[0] );
			if ( SPIWrite_REG ( g_commBuf[channel].Data[0],
					&g_commBuf[channel].Data[1], len ) != 0 )
			{
				s_commError[channel].Bit.otherErr = 1;
				goto error_process;	
			}	
			i = (uchar)GetCl2208ParaAddr ( g_commBuf[channel].Data[0] );
			if ( i < 6 )	//��Ҫ���浽EEPROM 11
			{
				pint = &g_cl2208para.pstart;
				pint += i;
				
				if ( i == 2 ) 
					*pint = (uint16)g_commBuf[channel].Data[1];		
				else
					*pint = ((uint16)g_commBuf[channel].Data[1]<<8)+g_commBuf[channel].Data[2];				

				for ( i=0; i<8; i++ )
				{	    	    
					Delay1MsTime ( 200 );
				    FeedWatchdog  (  );	    
				}
				g_cl2208para.checksum = 
					GetCl2208CheckSum ( (uint16 *)&g_cl2208para.pstart,CALIREG_NUM );
				StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1),
					(uchar*)&g_cl2208para.pstart,CALIREG_NUM*2+2 );
			}
			
				
			g_commBuf[channel].Len = 0;			
			
			break;
		}
	#else if (METER_CHIP_MODEL==0x8302)
		{
			//���Ӻ�������̬�ж� 11
			if ( g_buyPara.Status != MANUFACTURE_STATUS )
			{
				s_commError[channel].Bit.badPassword = 1;
				goto error_process;		
			}
			//if ( g_commBuf[channel].Len != 5 )
			if ( g_commBuf[channel].Data[0] == 0xff ) 		//��ʼ��ָ�� 11
			{
				pchar = (uchar*)&g_rn8302para.gsua;
				for ( i=0; i<32; i++ )
				{
					*pchar++ = 0;		
				}
				if ( StoreCaliPara ( ) != 0 )
				{
					s_commError[channel].Byte = 0x88;
					goto error_process;	
				}
				
				//CopyRamData ( (uchar*)&g_cl2208para.pstart, (uchar*)&CALIBRATE_TABLE_value[0],sizeof(CALI_REG_ORDER)*2 );
					
				if ( SetRN8302_Register ( ) != 0 )
				{
					s_commError[channel].Byte = 0x89;
					goto error_process;	
				}	
				
				g_commBuf[channel].Len = 0;			
				break;
			}
			
			len = GetRegLen ( g_commBuf[channel].Data[1],g_commBuf[channel].Data[0] );
			i = SPIWrite_REG ( g_commBuf[channel].Data[1],
					&g_commBuf[channel].Data[2], len, g_commBuf[channel].Data[0] );
			if ( i != 0 )
			{
				//s_commError[channel].Bit.otherErr = 1;
				s_commError[channel].Byte = i;
				goto error_process;	
			}	
			i = (uchar)GetRN8302ParaAddr ( g_commBuf[channel].Data[1] );
			if ( i != 0xff )	//��Ҫ���浽EEPROM 11
			{
				pint = &g_rn8302para.gsua;
				pint += i;
				
				*pint = ((uint16)g_commBuf[channel].Data[2]<<8)+g_commBuf[channel].Data[3];				

				for ( i=0; i<4; i++ )
				{	    	    
					Delay1MsTime ( 200 );
				    FeedWatchdog  (  );	    
				}
				
				if ( GetRN8302CheckSum ( &g_commBuf[channel].Data[10] ) != 0 )
				{
					s_commError[channel].Byte = 0x71;
					goto error_process;		
				}	
					
				g_rn8302para.checksum[0] = g_commBuf[channel].Data[10];
				g_rn8302para.checksum[1] = g_commBuf[channel].Data[11];
				g_rn8302para.checksum[2] = g_commBuf[channel].Data[12];
				
				g_rn8302para.sum = GetSum ( (uchar*)&g_rn8302para.gsua, 27 );
				
				for ( i=0; i<2; i++ )
				{	    	    
					StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1)+16*i,
						(uchar*)&g_rn8302para.gsua+16*i,16 );		    
				}								
			}
			
				
			g_commBuf[channel].Len = 0;			
			
			break;
		}
	#endif
	
	case 0x1f:	//��չָ�� 11
		{
			switch (g_commBuf[channel].Data[0])	
			{
			case 0x13:		//��ʼ�� 11
			#if(PROTOCOL_ADD_4_SERIALS==0)
				//���ݳ����ж� 11
				if ( g_commBuf[channel].Len != 4 )
			#else
				if ( (g_commBuf[channel].Len != 4) && (g_commBuf[channel].Len != 8) )
			#endif				
				{
					s_commError[channel].Bit.noRequestData = 1;
					goto error_process;
				}
			
			#if(PROTOCOL_ADD_4_SERIALS==1)
				if ( g_commBuf[channel].Len == 8 )
				{
					s_serialsLen[channel] = 4;
					for( i=0; i<4; i++ )
						s_serials[i] = g_commBuf[channel].Data[4+i];
				}			
			#endif
											
				if ( (g_commBuf[channel].Data[1]!=0x14) ||
					(g_commBuf[channel].Data[2]!=0x54) ||
					(g_commBuf[channel].Data[3]!=0x80) )
				{
					s_commError[channel].Bit.badPassword = 1;
					goto error_process;	
				}
				
				DisplayClear ( ); 
                g_initDispCnt = LCD_CLEAR_CNT;
                               
		        ClearAllData ( 500000, 10000 );	
		       																								
			#ifdef PRE_PAID_ENABLE		
				ProcessPrice( );
			#endif	
								
				g_commBuf[channel].Len = 0;
    			
				break;
			
			case 0x15:		//��ȡEEPROM 11
				{
					tempInt = ((uint16)g_commBuf[channel].Data[3]<<8)+g_commBuf[channel].Data[2];
					I2CRead ( EEPROM_ADDR, tempInt, &g_commBuf[channel].Data[4], 
								g_commBuf[channel].Data[1], 1  ) ;
					g_commBuf[channel].Len = g_commBuf[channel].Data[1]+4;	 
				}				
				break;
			
			case 0x17:		//д��EEPROM 11
				{
					tempInt = ((uint16)g_commBuf[channel].Data[3]<<8)+g_commBuf[channel].Data[2];
					//Page 1
	                EEPROM_NOWP();
					I2CWrite ( EEPROM_ADDR, tempInt, &g_commBuf[channel].Data[4], 
								g_commBuf[channel].Data[1], 1  ) ;
					EEPROM_WP();
					g_commBuf[channel].Len = 0;	 
				}				
				break;
					
			case 0x16:		//��ȡRAM 11
				{
					tempInt = ((uint16)g_commBuf[channel].Data[3]<<8)+g_commBuf[channel].Data[2];
					for( i=0; i<g_commBuf[channel].Data[1]; i++ )
						g_commBuf[channel].Data[4+i] =  *((uchar*)tempInt++);	
					g_commBuf[channel].Len = g_commBuf[channel].Data[1]+4;	 
				}				
				break;
			default:
				s_commError[channel].Bit.noRequestData = 1;
				goto error_process;	
			}
		}
		break;	
	default:
		s_commError[channel].Bit.noRequestData = 1;
		goto error_process;	
		
	}
	
	g_commStatus[channel].Bit.MessageFormatetterComplete = 1; 
	g_commStatus[channel].Bit.TransmitReady = 0;	
	
	g_commStatus[channel].Bit.TransmitComplete = 0;
	STIF1 = 0;
	
	
	s_commPara[channel].MBState = 0;
	s_commPara[channel].MBPtr = 0;
	
	g_commFor20msDly[channel] = COMM_DELAY_20MS;
	if ( channel == 0 )		//����������� 11
		LCDLED_ON();
	g_commLCDDisp = 3 ;

	//通讯正常后 解除按键保电 11
	if ( channel < 2 )
		g_meterStatus3.Bit.BtnKeepRelay = 0;

	return 1;
	
	error_process:
		g_commBuf[channel].Command |= BIT6 ;
		g_commBuf[channel].Len = 1;
		g_commBuf[channel].Data[0] = s_commError[channel].Byte;
			
		g_commStatus[channel].Bit.MessageFormatetterComplete = 1; 
		g_commStatus[channel].Bit.TransmitReady = 0;		
	
		g_commStatus[channel].Bit.TransmitComplete = 0;	
		STIF1 = 0;		
		s_commPara[channel].MBState = 0;
		s_commPara[channel].MBPtr = 0;		
		g_commFor20msDly[channel] = COMM_DELAY_20MS;
		return 0;



#if(NEW_PROTOCOL_2018==1) 
no_answer:
    	if ( channel == 0 )
    	{
	    	InitIR ( );
	    	ResetIRComm ( );
	    }
	    else if ( channel == 1 )
    	{
	    	InitRS485 ( );
	    	ResetRS485Comm ( );
	    	
	    }	        		  
		return 0;
#endif

#ifdef RTC_8025T
	no_answer1:
    	if ( channel == 0 )
    	{
	    	InitIR ( );
	    	ResetIRComm ( );
	    }
	    else if ( channel == 1 )
    	{
	    	InitRS485 ( );
	    	ResetRS485Comm ( );
	    	
	    }	        		  
		return 0;

#endif

}

/*
*****************************************************************************************
* fuction name: CheckModbusMessage
*
* Description : 
*
* Arguments   : uchar channel
*
* returns     : uchar
*
* Notes     : It's not the most efficient routine, but it sure is small.
******************************************************************************************
*/
#if(MODBUS_ENABLED==1)
static uchar DecodeModbusMessage ( uchar channel, uchar rxdLen )
{
 	uchar i,len,pos;	// Temporary variable to contain a character	  	
   	uchar *pbuf,*pchar;
   	uchar addr;
   	uint16 regAddr;
	float tempF;
	uint32 tempL;
	uchar buf[2];
	
 	if ( channel == 0 )
    	pbuf = &s_IRBuf[0];
    else //if ( channel == 1 )
    	pbuf = &s_485Buf[0];
    	
    //��ַ�ж� 11
    g_commStatus[channel].Bit.IsBroadcast = 1;
    buf[0] = g_addr.MeterAddr[0];
    buf[1] = g_addr.MeterAddr[1]&0x0f;
    addr = (uchar)BCDToHex( &buf[0], 2 );
    if ( (pbuf[0] != addr) &&
    	(pbuf[0] != 255) &&
    	(pbuf[0] != 0) )	
    	goto modbus_error_process;
  	if ( (pbuf[0] != 255) &&
  		 (pbuf[0] != 0) )
  		g_commStatus[channel].Bit.IsBroadcast = 0;
  	
  	//�Ĵ�����ַ�ж� 11
	regAddr = (uint16)pbuf[2];
	regAddr <<= 8;
	regAddr += pbuf[3];
	 	         	
    //�������ж� 11		
	switch( pbuf[1] )
	{
	case 0x04:		//��ȡ��һ�� 11
	
	{ 	
		//���ݳ����ж� 11
		if ( pbuf[4] != 0 )
			goto modbus_error_process;
		
		len = pbuf[5];
		
		//�����Ĵ�����ַ 11
		for ( i=0; i<MB_TBL1_NUM; i++ )
		{
			if ( regAddr== ModBUS_talbe1[i].Reg )	
				break;
		}	
		if ( i >= MB_TBL1_NUM )
			goto modbus_error_process;
		
		pos = 2;
		pbuf[pos++] = len<<1;
		while ( len > 0 )
		{
			if ( ModBUS_talbe1[i].PointerType == P_CHAR )
			{
				if ( ModBUS_talbe1[i].Type == BCDType )
					tempL = BCDToHex( (uchar *)ModBUS_talbe1[i].RamAddr, 1 );	
				else if ( ModBUS_talbe1[i].Type == HexType )
					tempL =(uint32) *((uchar *)ModBUS_talbe1[i].RamAddr);	
			}
			else if ( ModBUS_talbe1[i].PointerType == P_INT )
			{
				if ( ModBUS_talbe1[i].Type == BCDType )
					tempL = BCDToHex( (uchar *)ModBUS_talbe1[i].RamAddr, 2 );	
				else if ( ModBUS_talbe1[i].Type == HexType )
					tempL =(uint32) *((uint16 *)ModBUS_talbe1[i].RamAddr);	
			}
			else if ( ModBUS_talbe1[i].PointerType == P_LONG )
			{
				if ( ModBUS_talbe1[i].Type == BCDType )
					tempL = BCDToHex( (uchar *)ModBUS_talbe1[i].RamAddr, 4 );	
				else if ( ModBUS_talbe1[i].Type == HexType )
					tempL = *((uint32 *)ModBUS_talbe1[i].RamAddr);	
			}
			else if ( ModBUS_talbe1[i].PointerType == P_LONG_2 )
			{
				if ( ModBUS_talbe1[i].Type == HexType )
				{
					tempL = *((uint32 *)ModBUS_talbe1[i].RamAddr);	
					tempL += *(((uint32 *)ModBUS_talbe1[i].RamAddr)+1);	
				}
						
				else 
					goto modbus_error_process;	
			}
			else
				tempL = 0;
				
			tempF = tempL; 
			
			if ( ModBUS_talbe1[i].DotPos == DOT0 )
          	{           	    
          	    tempF /= 1.0;      
          	}    
          	else if ( ModBUS_talbe1[i].DotPos == DOT1 )
          	{           	    
          	    tempF /= 10;      
          	} 
          	else if ( ModBUS_talbe1[i].DotPos == DOT2 )
          	{           	    
          	    tempF /= 100;      
          	} 
          	else if ( ModBUS_talbe1[i].DotPos == DOT3 )
          	{           	    
          	    tempF /= 1000;      
          	} 
          	else if ( ModBUS_talbe1[i].DotPos == DOT4 )
          	{           	    
          	    tempF /= 10000;      
          	} 
          	else
            {
                goto modbus_error_process;      
            } 
			
			pchar = (uchar*)&tempF;
			
			pbuf[pos++] = pchar[3]; 
			pbuf[pos++] = pchar[2]; 
			pbuf[pos++] = pchar[1]; 
			pbuf[pos++] = pchar[0]; 
			
			
			len -= ModBUS_talbe1[i].RegLen;
			i++;
		}
		
		
		break;		
	}
	
	case 0x10:	//д�ڶ��� 11
	{
//		if ( channel == 0 )	//����ӿ� 11
//			goto modbus_error_process;	
		
		//�����ж� 11
		if ( (pbuf[4]!=0)  || (pbuf[5]!=2) || (pbuf[6]!=4) )
			goto modbus_error_process;
			
		pchar = (uchar*)&tempF;
        pchar[3] = pbuf[7];              
        pchar[2] = pbuf[8];    
        pchar[1] = pbuf[9]; 
        pchar[0] = pbuf[10];
        
        tempL = (uint32)tempF;
                    
        if ( regAddr == 0x0000 )     //������ 11            
        {				
            g_baudRate.baudrate = (uint16)tempL;	 
                
            g_baudRate.checksum = GetSum ( (uchar*)&g_baudRate.baudrate, 3 );
        	FeedWatchdog ( );
        	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,baudrate),
        		(uchar*)&g_baudRate.baudrate,4 ); 
        }
		else if ( regAddr == 0x0002 )	//У��λ 11	
		{
			g_baudRate.checkType = tempL;	 
                
            g_baudRate.checksum = GetSum ( (uchar*)&g_baudRate.baudrate, 3 );
        	FeedWatchdog ( );
        	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,baudrate),
        		(uchar*)&g_baudRate.baudrate,4 ); 	
		}
		else if ( regAddr == 0x0008 )	//ͨѶ��ַ	 11
		{
			regAddr = (uint16)Hex32ToBCD( tempL );
			g_addr.MeterAddr[0]	= (uchar)(regAddr&0xff);
			g_addr.MeterAddr[1]	&= 0xf0;
			g_addr.MeterAddr[1]	+= (uchar)((regAddr>>8)&0x0f);
			
			g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
	        StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
		        (uchar*)&g_addr.Zone,Meter_Addr_Len );
		}
//		else if ( regAddr == 0x0010 )	//DO���	 11		
//		{
//				
//		}
		else
			goto modbus_error_process; 
		
		pos = 6;
				
		break;
	}	
	
	case 0x03:		//��ȡ�ڶ��� 11
	{
		//���ݳ����ж� 11
		if ( pbuf[4] != 0 )
			goto modbus_error_process;
		
		len = pbuf[5];
		
		
		if ( regAddr == 0x0000 )		//������ 11
		{
			tempL = (uint32)g_baudRate.baudrate;				
			tempF = tempL; 	
		}
		else if ( regAddr == 0x0002 )		//У��λ 11
		{
			tempL = (uint32)g_baudRate.checkType;				
			tempF = tempL; 	
		}
		else if ( regAddr == 0x0008 )		//ͨѶ��ַ 11
		{	
			buf[0] = g_addr.MeterAddr[0];
    		buf[1] = g_addr.MeterAddr[1]&0x0f;
    		tempL = (uint32)BCDToHex( &buf[0], 2 );
			
			tempF = tempL;
		}
		else 
		{
			goto modbus_error_process;	
		}
		
		pos = 2;
		pbuf[pos++] = len<<1;
		
		pchar = (uchar*)&tempF;
			
		pbuf[pos++] = pchar[3]; 
		pbuf[pos++] = pchar[2]; 
		pbuf[pos++] = pchar[1]; 
		pbuf[pos++] = pchar[0]; 
			
			
		break;
	}
		
	default:
		break;
	}
	
	//У��� 11
	regAddr = GetCRC16 ( &pbuf[0], pos ); 
	pbuf[pos++] = ((uchar)(regAddr))&0xff;       
   	pbuf[pos++] = ((uchar)(regAddr>>8))&0xff; 
  	
  	g_modbusTxdLen = pos;
  	  	
	g_commStatus[channel].Bit.ModbusSendData = 1; 
	if ( g_commStatus[channel].Bit.IsBroadcast )		//дָ�� �㲥������ 11
	{
		if ( pbuf[1] == 0x10 )	
			g_commStatus[channel].Bit.ModbusSendData = 0;
	}	
			
	g_commFor20msDly[channel] = COMM_DELAY_20MS;
	g_commLCDDisp = 2 ;
	
	
	
	return TRUE;
	
modbus_error_process:
	
	if ( channel == 0 )
	{
    	InitIR ( );
    	ResetIRComm ( );
    }
    else if ( channel == 1 )
	{
    	InitRS485 ( );
    	ResetRS485Comm ( );
    	
    }	        		  

	return FALSE;
	
}
#endif


#ifdef PRE_PAID_ENABLE
#if(NEW_PROTOCOL_2018==1)
/*
*****************************************************************************************
* fuction name: ProcessSelling
*
* Description : �����۵�������� ��������
*
* Arguments   : uchar *pbuf, �������Ĵ���������
*
* returns     : uchar ������� 0--�ɹ� 1--ʧ�� 
*
* Notes     :
******************************************************************************************
*/
static uchar ProcessSelling( uchar *pchar, uchar len, uchar channel )
{
	uchar i,errCode,broadflag,orderLen;
	uchar *pbuf;
	uint16 temp16;
	uint32 ID,tempL;

#if(ENCRYPT_ENABLED==1)	
	//���� 11	
	Decrypt( &pchar[0], 0x00, len, 0xff);
#endif
		
	ID = pchar[3]; 
	ID = (ID<<8) | pchar[2]; 
	ID = (ID<<8) | pchar[1]; 
	ID = (ID<<8) | pchar[0];
	
	//�û������ݲ����� 11
	//4 5 6 7
	
	pbuf = pchar + 8;
	broadflag = BIT0+BIT1+BIT7;		//��ʼ��Ϊ�㲥 11
	//��������ж� 11
	for ( i=0; i<6; i++ )
	{
		if ( pbuf[i] != 0x99 )	
			broadflag &= ~BIT0;			//�㲥ָ�� 11
		
		if ( pbuf[i] != 0xAA )	
			broadflag &= ~BIT7;			//ͨ�����ַ 11
			
		//�Ƿ��뱾�ص�ַ��� 11 		
		if ( pbuf[i] != g_addr.MeterAddr[i] )	
		{
			broadflag &= ~BIT1;			//���ص�ַ 11		
		}
	}
	//	
	if ( broadflag==0 )
	{
		s_commError[channel].Byte = COMM_METER_NUM_ERR;
		return 1;	
	}
	
	//�㲥�ж� 11
	if ( ((broadflag&BIT0)==BIT0) && 
			(ID != 0x070140ff) &&
			(ID != 0x070141ff) )
	{
		s_commError[channel].Byte = COMM_METER_NUM_ERR;
		return 1;	
	}	
	//ͨ�����ַ 11
	if ( ((broadflag&BIT7)==BIT7) && 
			(ID != 0x070140ff) &&		//��������ֵ 11
			(ID != 0x070141ff) )		//��������˷� 11
	{
		s_commError[channel].Byte = COMM_METER_NUM_ERR;
		return 1;	
	}
			
	//��������λ��8��ʼ 11
	if ( ID == 0x07010Bff )		//�����ʼ�� 11
	{			
		//���ݳ����ж� 11
	#if(PROTOCOL_ADD_4_SERIALS==0)
		if ( len != 22 )
	#else
		if ( ( len != 22 ) && (len != 26) )
	#endif
		{
			
			s_commError[channel].Byte = COMM_LEN_ERR;
			return 1;				
		}
	
	#if(PROTOCOL_ADD_4_SERIALS==1)
		if ( len == 26 )
		{
			s_serialsLen[channel] = 4;
			for( i=0; i<4; i++ )
				s_serials[i] = g_commBuf[channel].Data[len-4+i];
		}			
	#endif
		
		DisplayClear ( ); 
        g_initDispCnt = LCD_CLEAR_CNT;
        						
		//Ԥ�ý�� 11	
		tempL = BCDToHex( &pbuf[6], 4 );
		tempL *= 100;	//����Ϊ4λС�� 11	                       
        //������� 11
        ID = BCDToHex( &pbuf[10], 4 );
        if ( ID > MAX_PRICE || ID < MIN_PRICE )
		{
			s_commError[channel].Byte = COMM_PRICE_ERR;
			return 1;			
		}
        ClearAllData ( tempL,(uint16)ID );
        
        ProcessRelayStatus ( );
    	ProcessRelay ( );											
		g_commBuf[channel].Len = 6;	
		//���״̬3 11
    	pchar[4] = (uchar)(g_meterStatus3.Byte&0xff);
		pchar[5] = (uchar)((g_meterStatus3.Byte>>8)&0xff);							
	}
	else if ( ID == 0x070104ff )		//�������� 11
	{	
	#if(PROTOCOL_ADD_4_SERIALS==0)
		if ( g_commBuf[channel].Len != 45 )		//37+8
	#else
		if ( ( g_commBuf[channel].Len != 45 ) && (g_commBuf[channel].Len != 49) )		
	#endif			
		{
			s_commError[channel].Byte = COMM_LEN_ERR;
			return 1;			
		}
	
	#if(PROTOCOL_ADD_4_SERIALS==1)
		if ( len == 49 )
		{
			s_serialsLen[channel] = 4;
			for( i=0; i<4; i++ )
				s_serials[i] = g_commBuf[channel].Data[len-4+i];
		}			
	#endif
		
		//���²��� 11
		if ( UpdatePara ( pbuf, channel ) != 0 )
		{
			return 1;			
		}
		
		ProcessRelayStatus ( );
    	ProcessRelay ( );											
		g_commBuf[channel].Len = 6;	
		//���״̬3 11
    	pchar[4] = (uchar)(g_meterStatus3.Byte&0xff);
		pchar[5] = (uchar)((g_meterStatus3.Byte>>8)&0xff);
						
		g_relayFlag.Bit.RelayAlarmHappened = 0;		    	

		g_meterFlag.Bit.LadderProcess = 1;
				
	}		
	else if ( (ID == 0x070102ff) ||		//Զ�̿���/��ֵ 11
		 		(ID == 0x070112ff) ||		//Զ�̳�ֵ�˷� 11
		 		(ID == 0x070113ff) ||		//Զ�������˷� 11	
		 		(ID == 0x070115ff) ||		//�����������ŵ��۵�Э�� 11
		 		(ID == 0x070116ff) ||		//����������Զ�̳�ֵ�˷� 11
		 		(ID == 0x070117ff) )		//����������Զ�������˷� 11
	{	
		//�����ж� 11
	#if(PROTOCOL_ADD_4_SERIALS==0)
		if ( (len != 58)  &&		//50+8
			(len != 27) )		//19+8	
	#else
		if ( ( (len != 58)  &&		//50+8
			(len != 27) ) &&
			(len != 62) &&
			(len != 31) &&
			(len != 70) &&
			(len != 39) &&
			(len != 66) &&
			(len != 35) )		
	#endif			
		{
			s_commError[channel].Byte = COMM_LEN_ERR;
			return 1;			
		}
	
	#if(PROTOCOL_ADD_4_SERIALS==1)
		if ( (len == 62) || (len == 31) || (len == 70) || (len == 39) )
		{
			s_serialsLen[channel] = 4;
			for( i=0; i<4; i++ )
				s_serials[i] = g_commBuf[channel].Data[len-4+i];
		}			
	#endif
		
		//�û�����ж� 11
		if ( g_buyPara.BuyCount > 0 ) //�ѷ��������� 11
		{			
			for ( i=0; i<6; i++ )
	        {
	        	if ( g_addr.CardNum[i] != pbuf[6+i] )
	        	{
	        		s_commError[channel].Byte = COMM_CARD_NUM_ERR;
					return 1;			
	        	} 	 	
	        }  				
		}
		
		FeedWatchdog ( );
		
		orderLen = 0;
        if ( ID >= 0x070115ff )
        	orderLen = len-8-s_serialsLen[channel];
        	
		//���ù��紦������ 11
		if (ID == 0x070102ff)	//Զ�̿���/��ֵ 11
			errCode = ProcessUserBuyQuan ( pbuf, NORMAL_MODE,orderLen );		
		else if (ID == 0x070112ff)	//Զ�̳�ֵ�˷� 11
			errCode = ProcessUserBuyQuan ( pbuf, BUY_RETURN_MODE,orderLen );
		else if (ID == 0x070113ff)	//Զ�������˷� 11
			errCode = ProcessUserBuyQuan ( pbuf, CANCEL_RETURN_MODE,orderLen );               
        else if (ID == 0x070115ff)	//����������Զ�̿���/��ֵ 11
			errCode = ProcessUserBuyQuan ( pbuf, NORMAL_MODE, orderLen );
		else if (ID == 0x070116ff)	//����������Զ�̳�ֵ�˷� 11
			errCode = ProcessUserBuyQuan ( pbuf, BUY_RETURN_MODE, orderLen );
        else if (ID == 0x070117ff)	//����������Զ�������˷� 11
			errCode = ProcessUserBuyQuan ( pbuf, CANCEL_RETURN_MODE, orderLen );
			
        if ( errCode > 0 )
        {
        	g_commBuf[channel].Command |= BIT6 ;	
        }
        else
        {
        	//���²��� 11
	        if ( pbuf[18] > 0 )
	        {
	        	UpdatePara ( pbuf+13, channel );
	        }	
        }
                        			
		//返回数据 11
		g_commBuf[channel].Len = 4 + ReturnbackPara ( &pchar[4], errCode, orderLen );				
		g_relayFlag.Bit.RelayAlarmHappened = 0;		    	
				
	}
	
	//��׼Ǯ����ʼ�� 11
	else if ( ID == 0x070103ff )		//��׼Ǯ����ʼ�� 11
	{
		//���ݳ����ж� 11
	#if(PROTOCOL_ADD_4_SERIALS==0)
		if ( len != 26 )
	#else
		if ( ( len != 26 ) && (len != 30) )
	#endif
		{
			s_commError[channel].Byte = COMM_LEN_ERR;
			return 1;				
		}
	
	#if(PROTOCOL_ADD_4_SERIALS==1)
		if ( (len == 30) )
		{
			s_serialsLen[channel] = 4;
			for( i=0; i<4; i++ )
				s_serials[i] = g_commBuf[channel].Data[len-4+i];
		}			
	#endif
		
		//�û�����ж� 11
		//deleted by Roger 2020/08/18
//		if ( g_buyPara.BuyCount > 0 ) //�ѷ��������� 11
//		{			
//			for ( i=0; i<6; i++ )
//	        {
//	        	if ( g_addr.CardNum[i] != pbuf[6+i] )
//	        	{
//	        		s_commError[channel].Byte = COMM_CARD_NUM_ERR;
//					return 1;			
//	        	} 	 	
//	        }  				
//		}
		       						
		//Ԥ�ý�� 11	
		tempL = BCDToHex( &pbuf[12], 4 );
		tempL *= 100;	//����Ϊ4λС�� 11	                       
        //������� 11
        ID = BCDToHex( &pbuf[16], 2 );
        if ( ID != 0 )
		{
			s_commError[channel].Byte = COMM_BUY_CNT_ERR;
			return 1;			
		}
        
        g_energy.Balance = tempL;
        g_energy.PrePaid = tempL;		
		g_energy.Overdraft = 0;
		//g_energy.Accumulate = 0;	//�������� 11
		g_energy.BonusBalance = 0;
		g_energy.BonusBuyQuan = 0;
		g_energy.BonusBuyCount = 0;
		g_energy.BonusMode = 0;
		SaveEnergy ( NORMAL_SAVE );
        g_meterFlag.Bit.PowerDown = 0;
        
	    g_buyPara.BuyCount = 0;

	    g_buyPara.Status = OUT_STATUS;
	    g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
		FeedWatchdog ( );
		StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
			(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );
		
		//
		for(i=0; i<18; i++)
			g_buf[i++] = 0;
		EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_BuyQuan), 
    		    &g_buf[0], 18, 1  );
		EEPROM_WP();		     	
		
		EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_BonusQuan), 
    		    &g_buf[0], 6, 1  );
		EEPROM_WP();	
				        											
		ProcessRelayStatus ( );
    	ProcessRelay ( );	

		//										
		//g_commBuf[channel].Len = 6;	
		//状态字3 11
    	//pchar[4] = (uchar)(g_meterStatus3.Byte&0xff);
		//pchar[5] = (uchar)((g_meterStatus3.Byte>>8)&0xff);	
		//修改返回数 2020/09/20 11
		g_commBuf[channel].Len = 4 + ReturnbackPara ( &pchar[4], 0, 0 );						
	}
	
	else if ( ID == 0x070133ff )		//扩展钱包初始化 11
	{
		//���ݳ����ж� 11
	#if(PROTOCOL_ADD_4_SERIALS==0)
		if ( len != 36 )
	#else
		if ( ( len != 36 ) && (len != 40) )
	#endif
		{
			s_commError[channel].Byte = COMM_LEN_ERR;
			return 1;				
		}
	
	#if(PROTOCOL_ADD_4_SERIALS==1)
		if ( (len == 40) )
		{
			s_serialsLen[channel] = 4;
			for( i=0; i<4; i++ )
				s_serials[i] = g_commBuf[channel].Data[len-4+i];
		}			
	#endif
		
		//�û�����ж� 11
		//deleted by Roger 2020/08/18
//		if ( g_buyPara.BuyCount > 0 ) //�ѷ��������� 11
//		{			
//			for ( i=0; i<6; i++ )
//	        {
//	        	if ( g_addr.CardNum[i] != pbuf[6+i] )
//	        	{
//	        		s_commError[channel].Byte = COMM_CARD_NUM_ERR;
//					return 1;			
//	        	} 	 	
//	        }  				
//		}
		       						
		//Ԥ�ý�� 11	
		tempL = BCDToHex( &pbuf[12], 4 );	
		tempL *= 100;	//����Ϊ4λС�� 11	                       
        g_energy.Balance = tempL;
        g_energy.PrePaid = tempL;
        g_energy.Overdraft = 0;
        //������� 11
        ID = BCDToHex( &pbuf[16], 2 );
        g_buyPara.BuyCount = (uint16)ID;
        //�ۼƵ��� 11
        tempL = BCDToHex( &pbuf[18], 4 );
       	g_energy.Accumulate = tempL;	
       	//����Ԥ�ý�� 11 
		tempL = BCDToHex( &pbuf[22], 4 );
		g_energy.BonusBalance = tempL;
		g_energy.BonusPrePaid = tempL;
		//�������� 11
        ID = BCDToHex( &pbuf[26], 2 );
        g_energy.BonusBuyCount = (uint16)ID;
		
		SaveEnergy ( NORMAL_SAVE );
        g_meterFlag.Bit.PowerDown = 0;
        	    
	    g_buyPara.Status = OUT_STATUS;
	    g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
		FeedWatchdog ( );
		StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
			(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );
		
		//
		for(i=0; i<18; i++)
			g_buf[i++] = 0;
		g_buf[4] = (uchar)(g_buyPara.BuyCount & 0xff);
        g_buf[5] = (uchar)((g_buyPara.BuyCount>>8) & 0xff);
		EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_BuyQuan), 
    		    &g_buf[0], 18, 1  );
		EEPROM_WP();		     	
		
		g_buf[4] = (uchar)(g_energy.BonusBuyCount & 0xff);
        g_buf[5] = (uchar)((g_energy.BonusBuyCount>>8) & 0xff);
		EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_BonusQuan), 
    		    &g_buf[0], 6, 1  );
		EEPROM_WP();	
		   											
		ProcessRelayStatus ( );
    	ProcessRelay ( );											
		
		//g_commBuf[channel].Len = 6;	
		//
    	//pchar[4] = (uchar)(g_meterStatus3.Byte&0xff);
		//pchar[5] = (uchar)((g_meterStatus3.Byte>>8)&0xff);
		//修改返回数 2020/09/20 11
		g_commBuf[channel].Len = 4 + ReturnbackPara ( &pchar[4], 0, 0 );							
	}
	
	//����ʹ�� 11
	
#ifdef TARIFF_ENABLE
	else if ( ID == 0x070150ff )		//Զ�̲������£�����ʲ����� 11
	{	
	#if(PROTOCOL_ADD_4_SERIALS==0)
		if ( len != 41 )
	#else
		if ( ( len != 41 ) && (len != 45) )
	#endif	
		{
			s_commError[channel].Byte = COMM_LEN_ERR;
			return 1;			
		}
	
	#if(PROTOCOL_ADD_4_SERIALS==1)
		if ( (len == 45) )
		{
			s_serialsLen[channel] = 4;
			for( i=0; i<4; i++ )
				s_serials[i] = g_commBuf[channel].Data[len-4+i];
		}			
	#endif
		
		//���·��ʲ��� 11
		if ( UpdateTariffPara ( pbuf, channel ) != 0 )
		{
			return 1;			
		}
		
		ProcessRelayStatus ( );
    	ProcessRelay ( );											
		g_commBuf[channel].Len = 6;	
		//���״̬3 11
    	pchar[4] = (uchar)(g_meterStatus3.Byte&0xff);
		pchar[5] = (uchar)((g_meterStatus3.Byte>>8)&0xff);
						
		g_relayFlag.Bit.RelayAlarmHappened = 0;		    	

		g_meterFlag.Bit.TariffProcess = 1;
				
	}	
	else if ( (ID >= 0x070151ff) && 
				(ID <= 0x070158ff) )		//�����ʱ�α�1~8��ʱ�� 11
	{	
	#if(PROTOCOL_ADD_4_SERIALS==0)
		if ( len != 56 )
	#else
		if ( ( len != 56 ) && (len != 60) )
	#endif	
		{
			s_commError[channel].Byte = COMM_LEN_ERR;
			return 1;			
		}
	
	#if(PROTOCOL_ADD_4_SERIALS==1)
		if ( (len == 60) )
		{
			s_serialsLen[channel] = 4;
			for( i=0; i<4; i++ )
				s_serials[i] = g_commBuf[channel].Data[len-4+i];
		}			
	#endif
		
		//���·��ʲ��� 11
		if ( UpdateTariffTable ( pbuf, channel, (pchar[1]-0x51)) != 0 )
		{
			return 1;			
		}
		
		ProcessRelayStatus ( );
    	ProcessRelay ( );											
		g_commBuf[channel].Len = 6;	
		//���״̬3 11
    	pchar[4] = (uchar)(g_meterStatus3.Byte&0xff);
		pchar[5] = (uchar)((g_meterStatus3.Byte>>8)&0xff);
						
		g_relayFlag.Bit.RelayAlarmHappened = 0;		    	

		g_meterFlag.Bit.TariffProcess = 1;
				
	}
#endif
	

	//����ʹ�� 11
	
#ifdef LADDER_ENABLE
	else if ( ID == 0x070160ff )		//������ز��� 11
	{	
	#if(PROTOCOL_ADD_4_SERIALS==0)
		if ( len != 41 )
	#else
		if ( ( len != 41 ) && (len != 45) )
	#endif	
		{
			s_commError[channel].Byte = COMM_LEN_ERR;
			return 1;			
		}
	
	#if(PROTOCOL_ADD_4_SERIALS==1)
		if ( (len == 45) )
		{
			s_serialsLen[channel] = 4;
			for( i=0; i<4; i++ )
				s_serials[i] = g_commBuf[channel].Data[len-4+i];
		}			
	#endif
		
		//���·��ʲ��� 11
		if ( UpdateLadderPara ( pbuf, channel ) != 0 )
		{
			return 1;			
		}
		
		ProcessRelayStatus ( );
    	ProcessRelay ( );											
		g_commBuf[channel].Len = 6;	
		//���״̬3 11
    	pchar[4] = (uchar)(g_meterStatus3.Byte&0xff);
		pchar[5] = (uchar)((g_meterStatus3.Byte>>8)&0xff);
						
		g_relayFlag.Bit.RelayAlarmHappened = 0;		    	

		g_meterFlag.Bit.LadderProcess = 1;
				
	}	

#endif
	
	
#ifdef	BONUS_PADI_ENABLE
	else if ( (ID == 0x070140ff)  )		//��������ֵ 11	
	{	
	#if(PROTOCOL_ADD_4_SERIALS==0)
		if ( len != 48 )
	#else
		if ( ( len != 48 ) && (len != 52) )
	#endif	
		{
			s_commError[channel].Byte = COMM_LEN_ERR;
			return 1;			
		}
	
	#if(PROTOCOL_ADD_4_SERIALS==1)
		if ( (len == 52) )
		{
			s_serialsLen[channel] = 4;
			for( i=0; i<4; i++ )
				s_serials[i] = g_commBuf[channel].Data[len-4+i];
		}			
	#endif
		
		//�û�����ж� 11
		if ( g_buyPara.BuyCount > 0 ) //�ѷ��������� 11
		{			
			broadflag |= BIT3+BIT2;
			
			for ( i=0; i<6; i++ )
	        {
	        	if ( pbuf[6+i] != 0x99 )
	        	{
	        		broadflag &= ~BIT2;	
	        	}
	        	
	        	if ( (g_addr.CardNum[i] != pbuf[6+i]) )
	        	{
	        		broadflag &= ~BIT3;		        				
	        	} 	 	
	        }
	        
	        if ( ((broadflag&(BIT0+BIT2))!=(BIT0+BIT2)) &&
	        		((broadflag&(BIT1+BIT3))!=(BIT1+BIT3)) )
	       	{
	       		s_commError[channel].Byte = COMM_CARD_NUM_ERR;
				return 1;	
	       	}	        	  				
		}
		
		FeedWatchdog ( );
		
		//�������� 11
		//��ȡ�������� 11
    	temp16 = (uint16)BCDToHex( &pbuf[36], 2 );
    	if ( temp16 > g_energy.BonusBuyCount )
    	{
    		g_energy.BonusBuyCount = temp16;
    		
    		if ( g_para.BonusMoneyNum > 6 )
				g_para.BonusMoneyNum = 1;
			else if (g_para.BonusMoneyNum== 0)
				g_para.BonusMoneyNum = 1;	
				
    		//��ȡ������� 11
    		i = 8+g_para.BonusMoneyNum*4;
    		tempL = BCDToHex( &pbuf[i], 4 );
    		g_energy.BonusBuyQuan = tempL*100;	
    		g_energy.BonusMode = pbuf[39];
    		g_energy.BonusDate = pbuf[38];
    		if ( (pbuf[39]&BIT0) != BIT0 )	//���������� 11
    		{   			
    			g_energy.BonusBalance = g_energy.BonusBuyQuan;		
    		}
    		else	//�������ۼ� 11
    		{
    			g_energy.BonusBalance += g_energy.BonusBuyQuan;		
    		}
    		
    		SaveEnergy ( NORMAL_SAVE ); 
    	
    	#ifdef BONUS_PADI_ENABLE	
    		StoreBonusBuyHistroy ( );
    	#endif
    	
    	}
    				
		//�������ݴ��� 11
		if  (  ((g_buyPara.BuyCount>0)&&((broadflag&(BIT0+BIT2))==(BIT0+BIT2))) ||
				((g_buyPara.BuyCount==0)&&((broadflag&(BIT0))==(BIT0)) )) 
		{
			g_commBuf[channel].Len = 0;	
		}
	    else
	    {
	    	g_commBuf[channel].Len = 4 + ReturnbackPara ( &pchar[4], errCode, 0 );		
	    }  		
					
		g_relayFlag.Bit.RelayAlarmHappened = 0;		    	
				
	}
	
//	else if ( (ID == 0x070141ff)  )		//��������˷� 11	
//	{	
//		//�����ж� 11
//		if ( g_commBuf[channel].Len != 26 )		//18+8
//		{
//			s_commError[channel].Byte = COMM_LEN_ERR;
//			return 1;			
//		}
//		
//		//�û�����ж� 11
//		if ( g_buyPara.BuyCount > 0 ) //�ѷ��������� 11
//		{			
//			broadflag |= BIT3+BIT2;
//			
//			for ( i=0; i<6; i++ )
//	        {
//	        	if ( pbuf[6+i] != 0x99 )
//	        	{
//	        		broadflag &= ~BIT2;	
//	        	}
//	        	
//	        	if ( (g_addr.CardNum[i] != pbuf[6+i]) )
//	        	{
//	        		broadflag &= ~BIT3;	
//	        				
//	        	} 	 	
//	        }
//	        
//	        if ( ((broadflag&(BIT0+BIT2))!=(BIT0+BIT2)) &&
//	        		((broadflag&(BIT1+BIT3))!=(BIT1+BIT3)) )
//	       	{
//	       		s_commError[channel].Byte = COMM_CARD_NUM_ERR;
//				return 1;	
//	       	}	         	  				
//		}
//		
//		FeedWatchdog ( );
//		
//		//�������� 11
//		//��ȡ�������� 11
//    	temp16 = (uint16)BCDToHex( &pbuf[16], 2 );
//    	if ( temp16 > g_energy.BonusBuyCount )
//    	{
//    		g_energy.BonusBuyCount = temp16;
//    		
//    		//��ȡ������� 11
//    		tempL = BCDToHex( &pbuf[12], 4 );
//    		tempL *= 100;
//    		if ( g_energy.BonusBalance >= tempL )	
//    		{   			
//    			if ( tempL == 0 )
//    				tempL = g_energy.BonusBuyQuan;	
//    			
//    			g_energy.BonusBalance -= tempL;		
//    		}
//    		else	
//    		{
//    			g_energy.BonusBalance = 0;		
//    		}
//    		
//    		SaveEnergy ( NORMAL_SAVE ); 
//    		
//    		//�������1�β����˷ѽ�� 11
//    		EEPROM_NOWP();
//	    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_ReturnBonusQuan), 
//	    		    (uchar*)&tempL, 4, 1  );
//			EEPROM_WP();
//    	}
//    				
//		//�������ݴ��� 11
//		//�������ݴ��� 11
//		if  (  ((g_buyPara.BuyCount>0)&&((broadflag&(BIT0+BIT2))==(BIT0+BIT2))) ||
//				((g_buyPara.BuyCount==0)&&((broadflag&(BIT0))==(BIT0)) ))
//		{
//			g_commBuf[channel].Len = 0;	
//		}
//	    else
//	    {
//	    	g_commBuf[channel].Len = 4 + ReturnbackPara ( &pchar[4], errCode );		
//	    }  		
//					
//		g_relayFlag.Bit.RelayAlarmHappened = 0;		    	
//				
//	}
#endif
	
	else
	{
		s_commError[channel].Byte = 0x01;
		return 1;			 
	}
	
#ifdef PRE_PAID_ENABLE		
	ProcessPrice( );
#endif	
	
	if ( g_energy.Balance > 0 )
    {
    	g_relayFlag.Bit.BalanceZeroHappened = 0;
    	g_relayFlag.Bit.BalanceZero5MinFlg = 0;
    }
    	
	return 0;
}


/*
*****************************************************************************************
* fuction name: UpdatePara
*
* Description : ���²���
*
* Arguments   : uchar* buf, uchar len
*		
* returns     : uchar checksum
*
* Notes     :	 
******************************************************************************************
*/
static uchar UpdatePara ( uchar* pbuf, uchar channel )
{
   	uint32 tempL;
   	
   	//���²��� 11
		
	//��� 11
	tempL = BCDToHex( &pbuf[6], 4 );	
	if ( tempL > MAX_PRICE || tempL < MIN_PRICE )
	{
		s_commError[channel].Byte = COMM_PRICE_ERR;
		return 1;		
	}		
	g_para.Price = 	(uint16)(tempL);
	//�������1  2�ֽ�11
	tempL = BCDToHex( &pbuf[10], 2 );
    g_buyPara.Alarm1 = (uint16)(tempL);
    //�������2  2�ֽ�11
    tempL = BCDToHex( &pbuf[12], 2 );
    g_buyPara.Alarm2 = (uint16)(tempL);
	//������б� 11
	tempL = BCDToHex( &pbuf[14], 3 );
	g_para.CurrentRatio = (uint16)tempL;
	//͸֧�����ֵ 2λС�� 11
	tempL = BCDToHex( &pbuf[17], 4 );
	g_para.CreditLimit = tempL;
	//�ڻ������ֵ 11
	tempL = BCDToHex( &pbuf[21], 4 );
	g_para.MaximBalance = (uint16)tempL;
	//�ڻ������ֵ 11
	tempL = BCDToHex( &pbuf[21], 4 );
	g_para.MaximBalance = (uint16)tempL;
	//�޵繦�� 11
	tempL = BCDToHex( &pbuf[25], 3 );
	g_para.LimitPower = (uint16)(tempL);
	//����������ֵ 11
	tempL = BCDToHex( &pbuf[28], 3 );
	g_buyPara.FactorLimit = (uint16)tempL;
	//����������Ӧ������ֵ 11
	tempL = BCDToHex( &pbuf[31], 3 );
	g_buyPara.FactorPower = (uint16)tempL;
	//��������ۼ�ģʽ�� 11
	//if ( (pbuf[34]&BIT0) == BIT0 )
	//	g_energy.BonusMode |= BIT0;
	//else
	//	g_energy.BonusMode &= ~BIT0;
	//���������Ч��� 11
	g_para.BonusMoneyNum = 	pbuf[35];	
	if ( g_para.BonusMoneyNum > 6 )
		g_para.BonusMoneyNum = 0;
	//ʱ�����բ����ʹ�ܱ�ʶ 11
	if ( (pbuf[36]&BIT6)==BIT6 )
		g_para.MeterTpye |=	BIT6;
	else
		g_para.MeterTpye &=	~BIT6;	
	
    
    g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
	FeedWatchdog ( );
	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
		(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );
	
	g_para.Checksum = GetSum ( (uchar*)&g_para.LimitPower, Meter_Para_Len-1 );
	FeedWatchdog ( );
	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LimitPower),
		(uchar*)&g_para.LimitPower,Meter_Para_Len );
		
	g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
	FeedWatchdog ( );
    StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
			(uchar*)&g_addr.Zone,Meter_Addr_Len );
    
    SaveEnergy ( NORMAL_SAVE );
      
    return 0;    
}

#ifdef TARIFF_ENABLE
/*
*****************************************************************************************
* fuction name: UpdateTariffPara
*
* Description : ���·��ʲ���
*
* Arguments   : uchar* buf, uchar len
*		
* returns     : uchar checksum
*
* Notes     :	 
******************************************************************************************
*/
static uchar UpdateTariffPara ( uchar* pbuf, uchar channel )
{
   	uchar i;
   	
	//��ʱ������ 11	
	//ʱ�α���
	//ʱ����
	//��1��ʱ����
	//��2��ʱ����
	//��3��ʱ����
	//��4��ʱ����	
	//���µ�EEPROM�� 11
	for(i=0; i<15; i++ )
		g_buf[i] = 	pbuf[6+i];
    g_buf[15] = GetSum ( g_buf, 15 );
    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,YearZoneNum),
			(uchar*)&g_buf[0], 16 ) != 0 )
		return 1;
			
	//���ʵ��1
	//���ʵ��2
	//���ʵ��3
	//���ʵ��4
	//���ʵ��5
	//���ʵ��6
	for(i=0; i<12; i++ )
		g_buf[i] = 	pbuf[21+i];
	for ( i=12; i<15; i++ )
    	g_buf[i] = 0;	
    g_buf[15] = GetSum ( g_buf, 15 );
    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,TariffPrice1),
			(uchar*)&g_buf[0],16 ) != 0 )
		return 1;	
      
    return 0;    
}

/*
*****************************************************************************************
* fuction name: UpdateTariffTable
*
* Description : ���·���ʱ�α�
*
* Arguments   : uchar* buf, uchar len
*		
* returns     : uchar checksum
*
* Notes     :	 
******************************************************************************************
*/
static uchar UpdateTariffTable ( uchar* pbuf, uchar channel, uchar offset )
{
   	uchar i;
   	
	//���µ�EEPROM�� 11
	for(i=0; i<42; i++ )
		g_buf[i] = 	pbuf[6+i];
    //���µ�EEPROM�� 11
    g_buf[42] = GetSum ( g_buf, 42 );
    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,HHMMNN1)+48*(uint16)offset,
			(uchar*)g_buf, 43 ) != 0 )
		return 1;		
      
    return 0;    
}
#endif

#ifdef LADDER_ENABLE
/*
*****************************************************************************************
* fuction name: UpdateLadderPara
*
* Description : ���½��ݲ���
*
* Arguments   : uchar* buf, uchar len
*		
* returns     : uchar checksum
*
* Notes     :	 
******************************************************************************************
*/
static uchar UpdateLadderPara ( uchar* pbuf, uchar channel )
{
   	uchar i;
   	
   	//����ֵ1~5 11
	//���µ�EEPROM�� 11
	for(i=0; i<10; i++ )
		g_buf[i] = 	pbuf[6+i];
    g_buf[10] = GetSum ( g_buf, 10 );
    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Ladder1),
			(uchar*)g_buf,11 ) != 0 )   
		return 1;
			
	//���ݵ��1~6 11
	for(i=0; i<12; i++ )
		g_buf[i] = 	pbuf[16+i];
    g_buf[12] = GetSum ( g_buf, 12 );
    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LadderPrice1),
			(uchar*)g_buf,13 ) != 0 )  
		return 1;	
    
    //�����ۼ����� 11 
    for ( i=0; i<5; i++ )
   	{
    	g_buf[i] = pbuf[28+i];
    }	
    I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,YYMM), 
				(uchar*)&g_buf[20], 8, 1  ) ;
    if ( (g_buf[2]==g_buf[22]) && 
    		(g_buf[3]==g_buf[23]) &&
    		(g_buf[4]==g_buf[24]) &&
    		(g_buf[0]==g_buf[20]) && 
    		(g_buf[1]==g_buf[21]) ) 
    {
    	
    }
    else
    {
   		g_buf[5] = g_buf[2];
   		g_buf[6] = g_buf[3];
   		g_buf[7] = g_buf[4];
   		for ( i=8; i<15; i++ )
	   	{
	    	g_buf[i] = 0x00;
	    }			    
	    g_buf[15] = GetSum ( g_buf, 15 );
	    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,YYMM),
				(uchar*)g_buf,16 ) != 0 )
			return 1;	
		
		g_dateLadder = 0x99; 	//����Ⱦͽ����л� 11  	
    }
    return 0;    
}

#endif

/*
*****************************************************************************************
* fuction name: ReturnbackPara
*
* Description : ���ز�������
*
* Arguments   : uchar* buf, uchar len
*		
* returns     : uchar checksum
*
* Notes     :	 
******************************************************************************************
*/
static uchar ReturnbackPara ( uchar* pbuf, uchar errCode, uchar orderLen )
{
   	uchar i,j;
   	uint32 tempL;
   	uchar buf[8];
   	uchar *pserialBuf;
   	
   	//�������� 11
	pserialBuf = pbuf + 4 + orderLen -8;
	for ( j=0; j<8; j++ )
		buf[j] = pserialBuf[j]; 
	
	//�û���� 11
	for ( i=0; i<6; i++ )
	{
		pbuf[i] = g_addr.CardNum[i];	
		i++;
	}		
	//���1�ι����� 11
	tempL =  Hex32ToBCD( g_buyPara.BuyQuan );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);
	pbuf[i++] = (uchar)((tempL>>24)&0xff);
	//���ڹ������	11
	tempL =  Hex16ToBCD( g_buyPara.BuyCount );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	//��ǰ�й��ܵ��� 11
	tempL =  Hex32ToBCD( g_energy.Accumulate );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);
	pbuf[i++] = (uchar)((tempL>>24)&0xff);
	//ʣ���� 11
	tempL =  Hex32ToBCD( g_energy.Balance/100 );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);
	pbuf[i++] = (uchar)((tempL>>24)&0xff);
	//͸֧��� 11
	tempL =  Hex32ToBCD( g_energy.Overdraft/100 );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);
	pbuf[i++] = (uchar)((tempL>>24)&0xff);	
	//��� 4�ֽ� 11
	tempL =  Hex32ToBCD( g_para.Price );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);
	pbuf[i++] = (uchar)((tempL>>24)&0xff);		
	//�������1  2�ֽ�11
	tempL =  Hex16ToBCD( g_buyPara.Alarm1 );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
    //�������2  2�ֽ�11
    tempL =  Hex16ToBCD( g_buyPara.Alarm2 );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	//������б� 3�ֽ� 11
	tempL =  Hex16ToBCD( g_para.CurrentRatio );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);
	//͸֧�����ֵ 4�ֽ� 11
	tempL =  Hex32ToBCD( g_para.CreditLimit );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);
	pbuf[i++] = (uchar)((tempL>>24)&0xff);	
	//�ڻ������ֵ 4�ֽ� 11
	tempL =  Hex16ToBCD( g_para.MaximBalance );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);
	pbuf[i++] = (uchar)((tempL>>24)&0xff);	
	//�޵繦����ֵ 3�ֽ� 11
	tempL =  Hex32ToBCD( g_para.LimitPower );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);	
	//����������ֵ 3�ֽ� 11
	tempL =  Hex16ToBCD( g_buyPara.FactorLimit );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);	
	//����������Ӧ������ֵ 3�ֽ� 11
	tempL =  Hex16ToBCD( g_buyPara.FactorPower );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);	
	//��������ۼ�ģʽ�� 11	
	pbuf[i++] = g_energy.BonusMode;
	//ʱ�����բ����ʹ�ܱ�ʶ 11
	if ( (g_para.MeterTpye&BIT6) == BIT6 )
		pbuf[i++] = BIT6;
	else
		pbuf[i++] = 0;	
	//����ʣ���� 4�ֽ� 11
	tempL = g_energy.BonusBalance/100;
    tempL =  Hex32ToBCD( tempL );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
	pbuf[i++] = (uchar)((tempL>>16)&0xff);
	pbuf[i++] = (uchar)((tempL>>24)&0xff);	  
    //�������� 2�ֽ� 11
    tempL =  Hex16ToBCD( g_energy.BonusBuyCount );
	pbuf[i++] = (uchar)(tempL&0xff);
	pbuf[i++] = (uchar)((tempL>>8)&0xff);
    //������������ 11
    pbuf[i++] = (uchar)(g_energy.BonusDate);
    //���������Ч��� 11
    pbuf[i++] = g_para.BonusMoneyNum;
    //���״̬3 11
    pbuf[i++] = (uchar)(g_meterStatus3.Byte&0xff);
	pbuf[i++] = (uchar)((g_meterStatus3.Byte>>8)&0xff);
	//���1�γ�ֵ������ 11
	pbuf[i++] = errCode;
#if(PROTOCOL_ADD_ORDERS==1)	
	//�����ŷ��� 11
	if ( orderLen > 0 )
	{
		//I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_Order), 
		//		&pbuf[i], 8, 1  ) ;	
		for ( j=0; j<8; j++ )
			pbuf[i+j] = buf[j]; 
		i += 8;
	}			
#endif	
	     
    return i;    
}





#ifdef BONUS_PADI_ENABLE
/*
*****************************************************************************************
* fuction name: StoreBonusBuyHistroy
*
* Description : �洢���Ѽ�¼ 
*
* Arguments   : uchar* buf, uchar len
*		
* returns     : uchar checksum
*
* Notes     :	 
******************************************************************************************
*/
void StoreBonusBuyHistroy ( void )
{
    uchar i;
    uchar temp[6];
    uint32 tempL;
    
   	//ת���9���������� 11
    for ( i=0; i<9; i++ )
    {
        I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last9_BonusQuan)-6*i, 
				&temp[0], 6, 1  ) ;
        
        EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last10_BonusQuan)-6*i, 
    		    &temp[0], 6, 1  );
		EEPROM_WP();
		
		FeedWatchdog ( );
		
		//�洢���1�����Ѽ�¼���� 11 
		tempL =  Hex32ToBCD( g_energy.BonusBuyQuan/100 );
		temp[0] = (uchar)(tempL&0xff);
		temp[1] = (uchar)((tempL>>8)&0xff);
		temp[2] = (uchar)((tempL>>16)&0xff);
		temp[3] = (uchar)((tempL>>24)&0xff);
		//�������� 2�ֽ� 11
	    tempL =  Hex16ToBCD( g_energy.BonusBuyCount );
		temp[4] = (uchar)(tempL&0xff);
		temp[5] = (uchar)((tempL>>8)&0xff);  
	   	EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_BonusQuan), 
    		    &temp[0], 6, 1  );
		EEPROM_WP();
		
		FeedWatchdog ( );
    }
    
    return;
}
#endif

#endif

#endif

/*
*****************************************************************************************
* fuction name: BuildIRMessage
*
* Description : The message builder accepts a message from the formatter and prepares
*    		the header and checksum. It then feeds the serial driver one character
*    		at a time. When all the characters are gone, it notifies the formatter
*    		so that potentially the next block can be sent.
*
* Arguments   : none
*
* returns     : uchar
*
* Notes     :
******************************************************************************************
*/
uchar BuildIRMessage( uchar channel )
{
	uchar *pbuf;
	uchar i,pos;
	
	//s_MBState reflects the state of the Message Builder itself.
	//static uchar s_MBState = 0;
	//s_MBcs is the locally-generated checksum; it will be transmitted at the end of the
	// message.
  	//static uchar s_MBcs = 0;	
	//s_MBPtr is a local pointer into the message buffer.
	//static uchar s_MBPtr = 0;
	//20ms����������ʱ ...
	if ( g_commFor20msDly[channel] > 0 )
		return 0;
	
	if( g_commStatus[channel].Bit.MessageFormatetterComplete == 0 )
	    return 0;
	g_commStatus[channel].Bit.MessageFormatetterComplete = 0;
	    
    if ( channel == 1 )
        pbuf = &s_485Buf[0];
    else if ( channel == 0 )
        pbuf = &s_IRBuf[0]; 
#if(IC_CARD_MODE==PLC_COMM_USED)    
	else if ( channel == 2 )
        pbuf = &s_PLCBuf[0]; 
#endif
	
	pos = 0;
	pbuf[pos++] = 0xfe;
	pbuf[pos++] = 0xfe;
	pbuf[pos++] = 0xfe;
	pbuf[pos++] = 0xfe;
	
	pbuf[pos++] = 0x68;
	//��ַ 11
	for ( i=0; i<6; i++ )
	    pbuf[pos++] = g_addr.MeterAddr[i];
	pbuf[pos++] = 0x68;
	
	//������ 11
	pbuf[pos++] = g_commBuf[channel].Command|0x80;
	//���ݳ��� 11
	pbuf[pos++] = g_commBuf[channel].Len+s_serialsLen[channel];
	//������ 11
	for ( i=0; i<g_commBuf[channel].Len; i++ )
	    pbuf[pos++] = g_commBuf[channel].Data[i] + 0x33;
#if(PROTOCOL_ADD_4_SERIALS==1)
	if ( s_serialsLen[channel] == 4)
		for( i=0; i<4; i++ )
			pbuf[pos++] = s_serials[i]+0x33;
	s_serialsLen[channel] = 0;		
#endif


	//У��� 11
	i = GetChecksum ( &pbuf[4], pos-4 );
	pbuf[pos++] = i;
	//������ 11
	pbuf[pos++] = 0x16;
		
	if ( channel==0 ) 
    {
	    //�رս���  11
	    PIF5 = 0;       //clear interrupt flag
		PMK5= 1;	    //disable INTP0 interrupt
	    
	    s_IRByte = s_IRBuf[0];   //send the character 11
		s_IRParity = 0;
		s_IRTxdState = 0;
        Timer5_1200bps_start();    
	    
	    s_IRSendEnd = 0;
        s_IRSendHead = pos;   
	}
	else if ( channel==1 ) 
    {    		
    	g_485CommDly = FRAME_SEND_CNT;	
        //g_IRFlag.Bit.InOnChangeCmd = 1;
        
    	//�����������ݣ������͵�1������11
    	SS0 |= (1<<2);  // enable UART1 transmit 
    	STMK1 = 0;
    	RS485CON_TXD();
        TXD1 = s_485Buf[0];
        SOE0 |= 0x0004;		//	
    
        s_485SendEnd = 0;
        s_485SendHead = pos;
         
	}
#if(IC_CARD_MODE==PLC_COMM_USED)
    else if ( channel== 2 ) 
    {
	    g_PLCCommDly = FRAME_SEND_CNT;	
        //g_IRFlag.Bit.InOnChangeCmd = 1;
        
    	//�����������ݣ������͵�1������11
    	SS0 |= (1<<0);  // enable UART0 transmit 
    	STMK0 = 0;
        TXD0 = s_PLCBuf[0];
        SOE0 |= 0x0001;		//	
    
        s_PLCSendEnd = 0;
        s_PLCSendHead = pos;
	}
#endif
				       	
    return 1;
}

#if(MODBUS_ENABLED==1)	
/*
*****************************************************************************************
* fuction name: BuildModbusMessage
*
* Description : The message builder accepts a message from the formatter and prepares
*    		the header and checksum. It then feeds the serial driver one character
*    		at a time. When all the characters are gone, it notifies the formatter
*    		so that potentially the next block can be sent.
*
* Arguments   : none
*
* returns     : uchar
*
* Notes     :
******************************************************************************************
*/
static uchar BuildModbusMessage ( uchar channel )
{
	uchar *pbuf;
	
	//20ms����������ʱ ...
	if ( g_commFor20msDly[channel] > 0 )
		return 0;
	
	g_commStatus[channel].Bit.ModbusSendData = 0;
	    
    if ( channel == 1 )
        pbuf = &s_485Buf[0];
    else
        pbuf = &s_IRBuf[0];     
				
	if ( channel==0 ) 
    {
	    s_IRByte = s_IRBuf[0];   //send the character 11
		s_IRParity = 0;
		s_IRTxdState = 0;
        Timer5_1200bps_start();    
	    
	    s_IRSendEnd = 0;
        s_IRSendHead = g_modbusTxdLen;   
	}
	else if ( channel==1 ) 
    {    		
    	g_485CommDly = FRAME_SEND_CNT;	
        //g_IRFlag.Bit.InOnChangeCmd = 1;
        
    	//�����������ݣ������͵�1������11
    	SS0 |= (1<<2);  // enable UART1 transmit 
    	STMK1 = 0;
    	RS485CON_TXD();
        TXD1 = s_485Buf[0];
        SOE0 = 0x0004;		//	
    
        s_485SendEnd = 0;
        s_485SendHead = g_modbusTxdLen;
         
	}
    			       	
    return 1;
}
#endif

/*
*****************************************************************************************
* fuction name: CheckPassword
*
* Description : This function is designed to check the password whether right. Here are two class
*		password. Class 0 is for energy clear(0xC119) and change its password(0xC212).
*		Class 1 is for programm the parameters of the meter.
*
* Arguments   : 1)uint16 commCode, it is used to check password's class
*				2)uchar *pdata, pointer to the password array
*	
*
* returns     : bit, 1:password is right; 0:password is false.
*
* Notes     :
******************************************************************************************
*/
static uchar CheckPassword ( const uchar *pdata )
{
	uchar temparray[16];
	
	if ( pdata == NULL )
		return 1;
		
	//only two class: 0 and 1 2	
	if ( *pdata > 2 )
		return 1;
		
	if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,PASSWORD),temparray,CHECK_ADDMOD,5 ) != 0 )
		return 1;
				 		  
	if ( (temparray[0] != *pdata) ||
	     (temparray[1] != *(pdata+1)) ||
	     (temparray[2] != *(pdata+2)) ||
	     (temparray[3] != *(pdata+3))  )
		return 1;
		
	return 0;
}

/*
*****************************************************************************************
* fuction name: GetChecksum
*
* Description : this function caculate the checksum of the releated bytes
*
* Arguments   : uchar* buf, uchar len
*		
* returns     : uchar checksum
*
* Notes     :	 
******************************************************************************************
*/
uchar GetChecksum ( uchar* buf, uchar len )
{
    uchar ret;
    uchar i;
    
    ret = 0;
    for ( i=0; i<len; i++)
    {
        ret += buf[i]; 
    }
       
    return ret;    
}

#if(MODBUS_ENABLED==1)
/*
*****************************************************************************************
* fuction name: GetCRC16
*
* Description : 
*
* Arguments   : 1)uchar *pindata, pointer to the array
*				2)uchar len, length of data
*	
*
* returns     : bit, 1:password is right; 0:password is false.
*
* Notes     :
******************************************************************************************
*/
static uint16 GetCRC16 (  uchar *pindata, uchar len )
{
    uint16 crc16;
    uchar i,j;
    
    crc16 = 0xffff;       
    
    for ( i=0; i<len; i++ )
    {
        crc16 ^= pindata[i];      
        
        for ( j=0; j<8; j++ )
        {
            if ( (crc16&BIT0) == BIT0 )
            {
                crc16 >>= 1;
                crc16 ^= 0xa001;       
            }
            else
            {
                crc16 >>= 1;
            }
        }        
    }
    
    return crc16;
    
}
#endif

//#endif
