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
#pragma NOP

#define _RF_SPI_DEFINE_
#include "include.h"

//variables statement
/******************************************************************************************/
#define RF_SPI_RST_H ( P1.7 = 1 )
#define RF_SPI_RST_L ( P1.7 = 0 )

#define RF_SPI_CS_H ( P0.0 = 1 )
#define RF_SPI_CS_L ( P0.0 = 0 )

#define RF_SPI_MCLK_H ( P0.2 = 1 )
#define RF_SPI_MCLK_L ( P0.2 = 0 )

#define RF_SPI_MOSI_H ( P1.3 = 1 )
#define RF_SPI_MOSI_L ( P1.3 = 0 )

#define RF_SPI_MISO_IO (P1.2)

static uchar Write_OneByte ( uchar dataOut ); 
static uchar Read_OneByte ( uchar addr );     
/******************************************************************************************/

//function statement
/******************************************************************************************/


/******************************************************************************************/


#if (IC_CARD_MODE==RF_CARD)


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
uchar RF_SPIRead_REG ( uchar reg )
{
	uchar addr,ret;
	
	RF_SPI_CS_H;	
	RF_SPI_MCLK_L;
	RF_SPI_CS_L;
	//DelayShort(DELAY_CNT);
	//NOP();
	//NOP();
	
	addr = (reg<<1) & 0x7e;
	addr |= 0x80;
	Write_OneByte ( addr );								
	ret = Read_OneByte ( 0x00 );
	
	RF_SPI_CS_H;
	RF_SPI_MCLK_H;
	
	return ret;
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
uchar RF_SPIWrite_REG ( uchar reg, uchar sendData )
{							
	uchar addr;
	
	//trasmit write frame
	RF_SPI_CS_H;
	RF_SPI_MCLK_L;
	RF_SPI_CS_L;
	//DelayShort(DELAY_CNT);
	//NOP();
	//NOP();
	
	//trasmit ADDR  byte
	addr = (reg<<1) & 0x7e;
	Write_OneByte ( addr );	
	Write_OneByte ( sendData );	

	RF_SPI_CS_H;
	RF_SPI_MCLK_H;
								
	return 0;
}

/*
*****************************************************************************************
* fuction name: SetBitMask
*
* Description : 
*
* Arguments   : 
*
* returns     : uchar
*
* Notes     : Here we will try to read 5 times if it is false
******************************************************************************************
*/
void SetBitMask (uchar reg, uchar mask )
{
    uchar temp =0x00;
    
    temp = RF_SPIRead_REG( reg );
        
    RF_SPIWrite_REG ( reg, temp |mask );  // set bit mask
}

/*
*****************************************************************************************
* fuction name: ClearBitMask
*
* Description : 
*
* Arguments   : 
*
* returns     : uchar
*
* Notes     : Here we will try to read 5 times if it is false
******************************************************************************************
*/
void ClearBitMask (uchar reg, uchar mask )
{
    uchar temp =0x00;
    
    temp = RF_SPIRead_REG( reg );
        
    RF_SPIWrite_REG ( reg, temp&(~mask) );  // clear bit mask
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
		RF_SPI_MCLK_L;		//上升沿MCU放数据 

		if ( dataOut & BIT7 ) 
			RF_SPI_MOSI_H;
		else
			RF_SPI_MOSI_L;
				
		//DelayShort(DELAY_CNT);
		NOP();
		NOP();
		NOP();
		
		NOP();
		NOP();
						
		RF_SPI_MCLK_H;		//下降沿2208取数据
		
		//DelayShort(DELAY_CNT);
		NOP();
		NOP();
		NOP();
		
		NOP();
		NOP();
				
		dataOut<<=1;
	}	
	
	RF_SPI_MCLK_L;	
	
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
static uchar Read_OneByte ( uchar addr )
{
	uchar i,retData;
	
	addr = (addr<<1) & 0x7e;
	retData = 0;
	for ( i=0; i<8; i++ )
	{
		retData<<=1;
		
		RF_SPI_MCLK_L;		//上升沿2208放数据 		
		
		if ( addr & BIT7 ) 
			RF_SPI_MOSI_H;
		else
			RF_SPI_MOSI_L;
			
		//DelayShort(DELAY_CNT);
		NOP();
		NOP();
		NOP();
		NOP();
		
		NOP();
		NOP();
		
		RF_SPI_MCLK_H;		//下降沿MCU取数据
		
		//DelayShort(DELAY_CNT);
		NOP();
		NOP();
		NOP();
		NOP();

		NOP();
		NOP();

		
		if ( RF_SPI_MISO_IO )
			retData |= BIT0;
			
		addr<<=1;
		NOP();	
								
	}
	
	RF_SPI_MCLK_L;
	
	return retData;
	
}

/*
*****************************************************************************************
* fuction name: HardwareReset
*
* Description : this function resets the CV520 chip
*
* Arguments   : none
*
* returns     : void
*
* Notes     : 
******************************************************************************************
*/
void HardwareReset ( void )
{
		
	RF_SPI_RST_H;
	//DelayTime ( 1 );
	delay_4us ( );
	RF_SPI_RST_L;
	//DelayTime ( 1 );
	delay_4us ( );
	delay_4us ( );
	delay_4us ( );
	RF_SPI_RST_H;

}

/*
*****************************************************************************************
* fuction name: HardwareHalt
*
* Description : this function halt the CV520 chip
*
* Arguments   : none
*
* returns     : void
*
* Notes     : 
******************************************************************************************
*/
void HardwareHalt ( void )
{
	RF_SPI_RST_L;
	//DelayTime ( 1 );
	delay_4us ( );
	delay_4us ( );
	
	RF_SPI_CS_L;
	RF_SPI_MCLK_L;
}				

/*
*****************************************************************************************
* fuction name: CalulateCRC
*
* Description : this function cacalate CRC16 checksum by CV520
*
* Arguments   : none
*
* returns     : void
*
* Notes     : 
******************************************************************************************
*/		
void CalulateCRC ( uchar *pIndata, uchar len, uchar *pOutData )
{	
    uchar i,n;
    
	FeedWatchdog();
	
    ClearBitMask ( DivIrqReg, 0x04 );
    RF_SPIWrite_REG ( CommandReg, PCD_IDLE );
    SetBitMask ( FIFOLevelReg, 0x80 );
    
    for (i=0; i<len; i++) 
        RF_SPIWrite_REG ( FIFODataReg, *(pIndata+i) );
    RF_SPIWrite_REG ( CommandReg, PCD_CALCCRC );
    
    i = 0xFF;
    do 
    {
        n = RF_SPIRead_REG( DivIrqReg );
        i--;
    }
    while ((i!=0) && !(n&0x04));
    
    pOutData[0] = RF_SPIRead_REG ( CRCResultRegL );
    pOutData[1] = RF_SPIRead_REG ( CRCResultRegM );
    
}

/*
*****************************************************************************************
* fuction name: CalulateCRC
*
* Description : this function cacalate CRC16 checksum by CV520
*
* Arguments   : none
*
* returns     : void
*
* Notes     : 
******************************************************************************************
*/		
void memcpy ( uchar* pdest, uchar *psource, uchar len )
{	
    uchar i;
    
    if ( pdest == NULL )
    	return;
   	if ( psource == NULL )
    	return;
    	   
    for (i=0; i<len; i++) 
        pdest[i] = psource[i] ;
       
    return;    
}

#endif
				