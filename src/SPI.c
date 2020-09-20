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

#define _SPI_DEFINE_
#include "include.h"

#if (METER_CHIP_MODEL==0x8302)

//variables statement
/******************************************************************************************/


static uchar Write_OneByte ( uchar dataOut ); 
static uchar Read_OneByte ( void );     
/******************************************************************************************/

//function statement
/******************************************************************************************/


/******************************************************************************************/





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
uchar SPIRead_REG ( uchar reg,uchar *s, uchar regLen, uchar bank )
{
	unsigned char i,tmpByte;
	uchar checksum1,checksum2;
	
	//first we check the pointer whether has valid address.
	if ( s == NULL )
		return 1;
	
	SPI_CS_H;	
	SPI_MCLK_H;
	SPI_CS_L;
	delay_time();
	
	//trasmit ADDR  byte
	checksum1 = reg;
	Write_OneByte ( reg );	
	delay_time();
	
	//trasmit command byte
	tmpByte = (uchar)((bank<<4)&0x70);
	checksum1 += tmpByte;
	Write_OneByte ( tmpByte );	
	delay_time();
	
	//
	for ( i=0; i<regLen; i++ )
	{
		s[i] = Read_OneByte ( );
		checksum1 += s[i];										
	}				
	
	//read checksum data
	checksum2 = Read_OneByte ( );
	
	delay_time();
	SPI_CS_H;
	SPI_MCLK_H;
	
	checksum1 ^= 0xff;
	if ( checksum1 != checksum2 )
		return 1;	
	/*
	//读取上次读取数据寄存器，进行判断确认读取数据是否正确 11
	delay_time();
	SPI_MCLK_H;
	SPI_CS_L;
	delay_time();
	
	//trasmit ADDR  byte
	tmpByte = (uchar)(RDATA&0xff);
	Write_OneByte ( tmpByte );	
	delay_time();
	
	//trasmit command byte
	Write_OneByte ( RDATA );	
	delay_time();
	
	//
	for ( i=0; i<4; i++ )
	{
		buf[i] = Read_OneByte ( );										
	}				
	
	delay_time();
	SPI_CS_H;
	SPI_MCLK_H;
	
	for ( i=0; i<regLen; i++ )
	{
		if ( buf[4-regLen+i] !=	s[i] )	//寄存器为4字节，有错位 11
		{
			g_2208ErrCnt++;
			return 1;	
		}	
	}
	*/
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
uchar SPIWrite_REG ( uchar reg,uchar *s, uchar regLen, uchar bank )
{
	unsigned char i,checksum1,tmpByte;
	uchar buf[2];
	
	//first we check the pointer whether has valid address.
	if ( s == NULL )
		return 0x50;
	if ( regLen > 3 )
		return 0x51;
	
	//写使能 11
	if ( Write_Register_CMD(WREN, WRITE_ENABLE) != 0 )
		return 0x52;
	
				
	//读取是否使能成功 11
	if ( SPIRead_REG ( WREN, buf, 1, 1 ) != 0 )
		return 0x53;	
	if ( buf[0] != 0x01 )
		return 0x54;
	
			
	//trasmit write frame
	SPI_MCLK_H;
	SPI_CS_L;
	delay_time();
	
	//trasmit ADDR  byte
	checksum1 = reg;
	Write_OneByte ( reg );	
	delay_time();
	
	//trasmit command byte
	tmpByte = (uchar)((bank<<4)&0x70)|0x80;
	checksum1 += tmpByte;
	Write_OneByte ( tmpByte );	
	delay_time();
		
	//
	for ( i=0; i<regLen; i++ )
	{
		Write_OneByte ( s[i] );	
		checksum1 += s[i];					
	}				
	
	checksum1 ^= 0xff;
	Write_OneByte ( checksum1 );	
	delay_time();
	
	SPI_CS_H;
	SPI_MCLK_H;
	delay_time();
		
	/*
	//读上次写入数据 11
	if ( SPIRead_REG ( 0x8d, buf, 3, 0x01 ) != 0 )
	{
		//Write_Register_CMD(WREN, WRITE_DISABLE);
		return 0x55;
	}
			
	for ( i=0; i<regLen; i++ )
	{
		if ( buf[3-regLen-i] != s[i] )
		{
			//Write_Register_CMD(WREN, WRITE_DISABLE);
			return 0x56;	
		}	
	}
	*/
	//关写使能 11
	if ( Write_Register_CMD(WREN, WRITE_DISABLE) != 0 )
		return 0x57;
	
	Delay1MsTime( 2 );
	
	//读取是否关闭使能 11
	if ( SPIRead_REG ( WREN, buf, 1, 1 ) != 0 )
		return 0x58;	
	if ( buf[0] != 0x00 )
		return 0x59;
						
	return 0;
}

/*
*****************************************************************************************
* fuction name: Write_Enable
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
uchar Write_Register_CMD ( uchar reg, uchar cmd )
{
	uchar checksum,tmpByte;
	
	SPI_CS_H;	
	SPI_MCLK_H;
	SPI_CS_L;
	delay_time();
	
	//open write protection
	//trasmit ADDR  byte
	checksum = reg;
	Write_OneByte ( reg );	
	delay_time();
	
	//trasmit command byte
	tmpByte = (uchar)((0x01<<4)&0x70)|0x80;
	checksum += tmpByte;
	Write_OneByte ( tmpByte );	
	delay_time();
	
	checksum += cmd;
	Write_OneByte ( cmd );	
	delay_time();
	
	checksum ^= 0xff;
	Write_OneByte ( checksum );	
	delay_time();
	
	SPI_CS_H;
	SPI_MCLK_H;
	delay_time();
	
	
			
	return 0;
	
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
	uchar i;
	
	for ( i=0; i<8; i++ )
	{
		SPI_MCLK_H;		//上升沿MCU放数据  11

		if ( dataOut & BIT7 ) 
			SPI_MOSI_H;
		else
			SPI_MOSI_L;
				
		delay_time();
		//delay_time();
		
		SPI_MCLK_L;		//下降沿2208取数据 11
		
		delay_time();	
		dataOut<<=1;
	}	
	
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
static uchar Read_OneByte ( void )
{
	uchar i,retData;
	
	retData = 0;
	for ( i=0; i<8; i++ )
	{
		retData<<=1;
		
		SPI_MCLK_H;		//上升沿2208放数据  11		
		
		delay_time();
		//delay_time();
		
		SPI_MCLK_L;		//下降沿MCU取数据 11
		
		delay_time();
				
		if ( SPI_MISO_IO )
			retData |= BIT0;								
	}
	
	return retData;
	
}


				
		
#endif					