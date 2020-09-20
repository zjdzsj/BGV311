/******************************************************************************************
*  Copyright (c) 2013,  R&D Department
*  All rights reserved 
*
*  FileName：   	SPI.c
*  Author：     	Roger
*  Date：  			2010-3-8 20:10
*  Revised Date：  	
*  Main Function：  this file contains all the functions used in I2C communications.
*  Version:			V1.0
*  Note： 			 			
*********************************************************************************************/
#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

#pragma interrupt INTSR2	ISRForUart2Rxd			//uart2接收中断 11
#pragma interrupt INTSRE2	ISRForUart2RcvErr		    //uart2接收错误中断 11
#pragma interrupt INTST2	ISRForUart2Txd			//uart2发送中断 11
	

#define _UART2_DEFINE_
#include "include.h"

#if(METER_CHIP_MODEL==0x2208)



//variables statement
/******************************************************************************************/
static uchar s_txdFlag;

static uchar s_head,s_buf[6];   
/******************************************************************************************/

//function statement
/******************************************************************************************/
static uchar Write_OneByte ( uchar dataOut ); 
//static uchar Read_OneByte ( void ); 

/******************************************************************************************/

/*
*****************************************************************************************
* fuction name: InitUart0
*
* Description : this function intializes hardware of Uart0
*
* Arguments   : none
*
* returns     : none
*
* Notes     : revised 07/07/05, add notes only
******************************************************************************************
*/
void InitUart2 ( void )
{			
	ST1 |= 0x0003;
	
	//解除串行阵列单元的复位状态,并开始提供时钟  11
	PER0 |= BIT3;	
	NOP();
    NOP();
    NOP();
    NOP();
    
	//设置操作时钟 9600bps,选择250kHz=8MHz/32 CK00=CK01
	SPS1 = 0x0044;
	//设置操作时钟 9600bps,选择500kHz=8MHz/16 CK00=CK01
	//SPS1 = 0x0044;
	
	//设置操作模式 TXD1 0单元通道2 11
	SMR10 = 0x0022;		//TXD
	SMR11 = 0x0122;		//RXD
	
	//设置通信格式
	SCR10 = 0x8297;		//偶校验 8位数据 1位停止位 11
	//SCR11 = 0x4697;	
	SCR11 = 0x4297;	
	
	//设置波特率
	SDR10 = 0x3200;		//250kHz/((25+1)*2)=4.8
	SDR11 = 0x3200;		//250kHz/((25+1)*2)=4.8
	//SDR10 = 0x3200;		//500kHz/((51+1)*2)=9.6
	//SDR11 = 0x3200;		//500kHz/((51+1)*2)=9.6
	
	//设置输出电平
	SOL1 = 0;		//不反相  11
	
	//设置串行输出初始电平 11
	SO1 = 0x0f0f;		//1
	
	//允许串行数据输出  11
	SOE1 = 0x0001;		//
	
	//噪声滤波器允许运行  11
	//NFEN0 = 0x14;		//RXD1 RXD2
	
	//设置端口  11
	
	Delay1MsTime ( 1 );
	//串行通道开始  11
	SS1 = 0x0003;
	
	
	SIR10 = 0x0007;
	SIR11 = 0x0007;
	
		
	//中断屏蔽  11
	SRIF2 = 0;
	SREIF2 = 0;
	STIF2 = 0;
	
	SRMK2 = 0;		//使能接收中断  11
	SREMK2 = 0;
	STMK2 = 0;		//使能发送中断  11
	
	UART_TXD = 1;
								
  	return;
}

/*
*****************************************************************************************
* fuction name: ISRForUart0Rxd
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
__interrupt void ISRForUart2Rxd ( void )
{
	//this instruction do not need here because when an interrupt acknowledged,
	//this flag  will be cleared automatically.
	SRIF2 = 0; 
	
	if ( (SSR11&0x0020) == 0x0020 )//有效数据已保存在SDR03中  11
	//if ( SSR11 == 0x0020 )	
	{		
		s_buf[s_head++]  = (uchar)(RXD2&0xff);
	}
	else
	{
		SIR11 |= 0x07;	
	}
		
	
					
	return;
}

/*
*****************************************************************************************
* fuction name: ISRForUart2RcvErr
*
* Description : uart2接收错误中断处理函数
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISRForUart2RcvErr( void )
{		
	SREIF2 = 0;
	
		
	s_buf[0] = (uchar)(RXD2&0xff);
	SIR11 |= 0x07;
	
	//ST1 = 0;
	
	return;
}

/*
*****************************************************************************************
* fuction name: ISRForUart0Txd
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
__interrupt void ISRForUart2Txd ( void )
{
	//this instruction do not need here because when an interrupt acknowledged,
	//this flag  will be cleared automatically.
	STIF2 = 0; 
	
	s_txdFlag = 1;
						
	return;
}

/*
*****************************************************************************************
* fuction name: SPIRead_REG
*
* Description : this function exits because eeprom can only page writes. We can use this
*		function to write n bytes into eeprom without cosiderating its address and numbers
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
uchar SPIRead_REG ( uchar reg, uchar *s, uchar regLen )
{
	unsigned char i,cs;
	
	//first we check the pointer whether has valid address.
	if ( s == NULL )
		return 1;
	
	s_head = 0;
	
	SRIF2 = 0; 
	SRMK2 = 0;		//enable Uart2 recieve interrupt
		
	//trasmit command byte
	Write_OneByte ( reg );	
	cs = reg;
	delay_time ( );
	
	g_Uart2Dly = 20;
	//增加校验字节 11
	while ( 1 )
	{	
		if ( g_Uart2Dly == 0 )
			break;					

		if ( s_head == (regLen+1) )
		{
			break;			
		}													
	}				
	SRMK2 = 1;		//disable Uart2 recieve interrupt
	
	if ( g_Uart2Dly == 0 )
	{
		InitUart2();
		return 1;
	}
		
	
	cs += GetSum ( s_buf, regLen );	
	cs ^= 0xff;
	if ( cs != s_buf[regLen] ) 
		return 1;
	
	for ( i=0; i<regLen; i++ )
		s[i] = s_buf[i];
		
	return 0;
}

/*
*****************************************************************************************
* fuction name: SPIWrite_REG
*
* Description : this function exits because eeprom can only page writes. We can use this
*		function to write n bytes into eeprom without cosiderating its address and numbers
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
uchar SPIWrite_REG ( uchar reg,uchar *s, uchar regLen )
{
	unsigned char i,cs,ret;
	uchar buf[3];
	
	ret = 0;
	//first we check the pointer whether has valid address.
	if ( s == NULL )
		return 1;
	if ( regLen > 2 )
		return 1;				
	//open write protection
	Write_OneByte ( 0xea );	
	Write_OneByte ( 0xe5 );	
	//校验 11
	cs = (uchar)(0xea + 0xe5);
	cs ^= 0xff;
	Write_OneByte ( cs );
	Delay1MsTime ( 2 );
		
	//trasmit command byte
	Write_OneByte ( reg|0x80 );	
	cs = reg|0x80;
	
	//
	for ( i=0; i<regLen; i++ )
	{
		Write_OneByte ( s[i] );	
		cs += s[i];					
	}	
	cs ^= 0xff;		
	Write_OneByte ( cs );	
					
	//读上次写入数据		
	//trasmit command byte
	if ( SPIRead_REG ( WDATA, buf, 2 ) != 0 )
		ret = 1;	
	
	if ( regLen == 1 )
	{
		buf[0] =  buf[1];	
	}
			
	for ( i=0; i<regLen; i++ )
	{
		if ( buf[i] !=	s[i] )
			ret = 1;			
	}
	
	//关闭写使能
	//open write protection
	Write_OneByte ( 0xea );	
	Write_OneByte ( 0xdc );	
	//校验 11
	cs = (uchar)(0xea + 0xdc);
	cs ^= 0xff;
	Write_OneByte ( cs );
	Delay1MsTime ( 2 );
			
	return ret;
}


/*
*****************************************************************************************
* fuction name: Write_OneByte
*
* Description : this function exits because eeprom can only page writes. We can use this
*		function to write n bytes into eeprom without cosiderating its address and numbers
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
static uchar Write_OneByte ( uchar dataOut )
{
	TXD2 = dataOut;
    SOE1 = 0x0001;	
    	
	s_txdFlag = 0;
	while ( s_txdFlag == 0 );
		
	return 0;
	
}

/*
*****************************************************************************************
* fuction name: Read_OneByte
*
* Description : this function exits because eeprom can only page writes. We can use this
*		function to write n bytes into eeprom without cosiderating its address and numbers
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
/*
static uchar Read_OneByte ( void )
{
	uchar retData;
	
	retData = 0;
	
	while ( s_rxdFlag == 0 );
	
	return retData;
	
}
*/
/*
*****************************************************************************************
* fuction name: SoftResetCl2208
*
* Description : this function resets the cl2208 chip
*
* Arguments   : 
*
* returns     : uchar
*
* Notes     : Here we will try to read 5 times if it is false
******************************************************************************************
*/
void SoftResetCl2208 ( void )
{	
	uchar cs;
	
	//first hardware reset
	UART_TXD = 0;		
	Delay1MsTime ( 40 );
	UART_TXD = 1;	
	Delay1MsTime ( 50 );
	FeedWatchdog  (  );	    
		
	//open write protection
	Write_OneByte ( 0xea );	
	Write_OneByte ( 0xe5 );	
	//校验 11
	cs = (uchar)(0xea + 0xe5);
	cs ^= 0xff;
	Write_OneByte ( cs );
	
	//软件复位指令
	Write_OneByte ( 0xea );	
	Write_OneByte ( 0xfa );	
	//校验 11
	cs = (uchar)(0xea + 0xfa);
	cs ^= 0xff;
	Write_OneByte ( cs );
		
	//close write protection
	//open write protection
	Write_OneByte ( 0xea );	
	Write_OneByte ( 0xdc );	
	//校验 11
	cs = (uchar)(0xea + 0xdc);
	cs ^= 0xff;
	Write_OneByte ( cs );
	
	return;
	
}

#else
/*
*****************************************************************************************
* fuction name: ISRForUart0Rxd
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
__interrupt void ISRForUart2Rxd ( void )
{
		
					
	return;
}

/*
*****************************************************************************************
* fuction name: ISRForUart2RcvErr
*
* Description : uart2接收错误中断处理函数
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISRForUart2RcvErr( void )
{		
	
	
	return;
}

/*
*****************************************************************************************
* fuction name: ISRForUart0Txd
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
__interrupt void ISRForUart2Txd ( void )
{
	
						
	return;
}

#endif							