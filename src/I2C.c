/******************************************************************************************
*  Copyright (c) 2010, Bona R&D Department
*  All rights reserved 
*
*  FileName£º   	I2C.c
*  Author£º     	Roger
*  Date£º  			2010-3-8 20:10
*  Revised Date£º  	
*  Main Function£º  this file contains all the functions used in I2C communications.
*  Version:			BonaV2.0
*  Note£º 			 			
*********************************************************************************************/
#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

#define _I2C_DEFINE_
#include "include.h"

#define I2CSCL	P6.0
#define I2CSDA	P6.1
#define DIRSCL	PM6.0
#define DIRSDA	PM6.1
#define MAX_RETRY 	3

//variables statement
/******************************************************************************************/
static uchar Ack;        
/******************************************************************************************/

//function statement
/******************************************************************************************/
static void start_I2C ( void );
static void stop_I2C ( void );
static void ack_I2C ( void );
static void noack_I2C ( void );
static uchar recieve_Byte ( void );
static uchar transmit_Byte  ( uchar TransmitByte );

/******************************************************************************************/



/*
*****************************************************************************************
* fuction name: start_I2C 
*
* Description : this function generates a start condition in I2C bus.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : 
******************************************************************************************
*/
static void start_I2C ( void )
{
	I2CSDA = 1;		//send data signal of start condition
	//delay_4us (  );
	I2CSCL = 1;		//send clock signal of start condition
	delay_4us (  );		//here the time must be more bigger than 4.7us 
	I2CSDA = 0;		//send start condition singal 
	delay_4us (  );		//>4us
	I2CSCL = 0;		//make the I2C bus ready to send or recieve data
	
	return ;
}

/*
*****************************************************************************************
* fuction name: stop_I2C 
*
* Description : this function generates a stop condition in I2C bus.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : 
******************************************************************************************
*/
static void stop_I2C ( void )
{
	I2CSDA = 0;		//send data signal of stop condition
	//delay_4us (  );
	I2CSCL = 1;		//send clock signal of stop condition
	delay_4us (  );		//here the time must be more bigger than 4us 
	I2CSDA = 1;		//send start condition singal 
	delay_4us (  );		//>4us
	
	I2CSCL = 1;
	I2CSDA = 1;
	return ;
}


/*
*****************************************************************************************
* fuction name: ack_I2C 
*
* Description : this function generates a acknowdge signal in I2C bus by CPU.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : 
******************************************************************************************
*/
static void ack_I2C ( void )
{
	I2CSDA = 0;
	delay_4us (  );
	I2CSCL = 1;
	delay_4us (  );
	
	I2CSCL = 0;
	
	return ;
}
	
/*
*****************************************************************************************
* fuction name: noack_I2C 
*
* Description : this function generates a no acknowdge signal in I2C bus by CPU.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : 
******************************************************************************************
*/
static void noack_I2C ( void )
{
	I2CSDA = 1;
	delay_4us (  );
	I2CSCL = 1;
	delay_4us (  );	
	I2CSCL = 0;
	delay_4us (  );
	I2CSDA = 0;
	
	return ;
}
	
/*
*****************************************************************************************
* fuction name: recieve_Byte 
*
* Description : this function receive a byte from I2C bus.
*
* Arguments   : none
*
* returns     : unsigned char, recieved byte from I2C bus.
*
* Notes     : 
******************************************************************************************
*/
static uchar recieve_Byte ( void )
{
	uchar i;
	uchar RecieveByte = 0;
	
	DIRSDA = 1;
	NOP();
	
	for ( i=0; i<8; i++ )
	{
		RecieveByte <<= 1;
		I2CSCL = 0;
		delay_4us (  );		//>4us	
		I2CSCL = 1;
		delay_4us (  );		//>4us	
		if ( I2CSDA )
			RecieveByte += 1;
	}
	
	I2CSCL = 0;
	DIRSDA = 0;
	
	return ( RecieveByte );
}

/*
*****************************************************************************************
* fuction name: I2C_Read
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
uchar I2CRead ( uchar sla, uint16 suba, uchar *s, uchar len, uchar sublen )
{
	unsigned int trytime  = MAX_RETRY;
	unsigned int templen;		//
	uint16 tempsuba;
	unsigned char *p;
	
	//first we check the pointer whether has valid address.
	if ( s == NULL )
		return 1;
	
	//second check sub address numbers
	if ( sublen > 2 )
		return 1;
	
	if ( sublen == 1 )
	{
		sla |= (uchar)((suba>>7)&0x0E); 
	}
				
	while ( trytime )
	{
		p = s;		
		templen = len;
		tempsuba = suba;
		
		if ( templen > 0 )
		{
			start_I2C (  );		//generate start condition
			
			
			if ( transmit_Byte  ( sla ) == 0 )	//device address
				goto readRetry;
			
			if ( sublen == 2 )      //first send high address
			{
			    if ( transmit_Byte  ( (uchar)(tempsuba>>8)&0xff ) == 0 )
				    goto readRetry;
			}
			
			if ( transmit_Byte  ( (uchar)tempsuba&0xff ) == 0 )	//now send low address
				goto readRetry;
				
			start_I2C (  );		//generate start condition
			
			if ( transmit_Byte  ( sla+1 ) == 0 )	//device address
				goto readRetry;
						
			while ( templen > 1 )
			{
				*p = recieve_Byte (  );
				
				p++;
				templen--;
				
				ack_I2C (  );
			}
						
			*p = recieve_Byte (  );
			templen = 0;
			noack_I2C (  );
			
			stop_I2C (  );
			
			return 0;
		}
		
		readRetry:
			trytime --;
			stop_I2C (  );
	}
	
	return 1;
}
		
/*
*****************************************************************************************
* fuction name: transmit_Byte 
*
* Description : this function transmit a byte from I2C bus.
*
* Arguments   : unsigned char, transmit byte in I2C bus.
*
* returns     : bit, acknowdge flag.
*
* Notes     : 
******************************************************************************************
*/
static uchar transmit_Byte  ( uchar TransmitByte )
{
	uchar i;
	
	DIRSDA = 0;
	Ack = 0;
	
	for ( i=0; i<8; i++ )
	{		
		I2CSCL = 0;					
		if (  TransmitByte&0x80  )
			I2CSDA = 1;
		else
			I2CSDA = 0;		
		delay_4us (  );		//>4us		
		I2CSCL = 1;
		delay_4us (  );		//>4us	
		TransmitByte <<= 1;			
	}
	
	I2CSCL = 0;
	I2CSDA = 1;			
	DIRSDA = 1;		//input mode
	I2CSCL = 1;
	delay_4us (  );		//>4us
	if ( I2CSDA ) Ack = 0;
	else Ack = 1;
	
	I2CSCL = 0;
	DIRSDA = 0;
	
	return ( Ack );
}

/*
*****************************************************************************************
* fuction name: I2C_Write
*
* Description : this function process I2C write operation.
*
* Arguments   : 1)sla : The I2C slave id. The bit0 should be set if it is a read access.
*		2)suba: The byte address to be accessed
*		3)*s:   The read/write data buffer in idata
*		4)len:	The amount of bytes to be accessed.
*
* returns     : 1: operation success,
*		0 : occurs error during this operation 
*
* Notes     :	1)Here we just try to write once because there will be unstable when power down. 
*		2)if need write protection, please add related code yourself.
******************************************************************************************
*/
uchar I2CWrite ( uchar sla, uint16 suba, const uchar *s, uchar len, uchar sublen )
{		
	
	
	//first we check the pointer whether has valid address.
	if ( s == NULL )
		return 0;
	
	//second check sub address numbers
	if ( sublen > 3 )
		return 0;
	
	if ( sublen == 1 )
	{
		sla |= (uchar)((suba>>7)&0x0E); 
	}
	
		
	while ( len > 0 )
	{
		start_I2C (  );		//generate start condition
		
		if ( transmit_Byte  ( sla ) == 0 )	//device address
			goto endI2c;
		
		if ( sublen == 2 )      //first send high address
		{
		    if ( transmit_Byte  ( (uchar)(suba>>8)&0xff ) == 0 )	
			    goto endI2c;
		}
		
		if ( sublen < 3 )		//added by Roger 2013/10/27
			if ( transmit_Byte  ( (uchar)suba&0xff ) == 0 )	//now send low address
				goto endI2c;
		
		while ( len > 0 )
		{
			if ( transmit_Byte  ( *s ) == 0 )	//data
				goto endI2c;
			
			s++;
			len--;
			suba++;	
			
			if ( sublen < 3 )
			{
				if( (!(suba&0x0F)) && (len>0) )	//whether page overlap
				{
					if ( !(suba&0xff))
						sla += 0x02;
						
					stop_I2C (  );
					
					if ( sublen < 3 )
						Delay1MsTime ( 5 );
					
					break;
				}	
			}		
		}
	}
	
	stop_I2C (  );
	
	if ( sublen < 3 )
		Delay1MsTime ( 5 );

	return 1;
	
	endI2c:
		stop_I2C (  );
		return 0;
}


				
		
					