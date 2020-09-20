/******************************************************************************************
*  Copyright (c) 2013,  R&D Department
*  All rights reserved 
*
*  FileName：   	CV520.c
*  Author：     	Roger
*  Date：  			2015-6-25 15:39
*  Revised Date：  	
*  Main Function：  this file contains all the functions 
*  Version:			V1.0
*  Note： 			 			
*********************************************************************************************/

	
#pragma sfr

#define _CV520_DEFINE_
#include "include.h"

//variables statement
/******************************************************************************************/

/******************************************************************************************/

//function statement
/******************************************************************************************/
static uchar CV520SelectCommand ( uchar *pSnr );

/******************************************************************************************/
#if (IC_CARD_MODE==RF_CARD)
/*
*****************************************************************************************
* fuction name: ResetCV520
*
* Description : this function process CV520 reset operation.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar ResetCV520 ( void )
{
	HardwareReset ( );	//硬复位 11
	
	RF_SPIWrite_REG ( CommandReg, CV520_RESETPHASE );		//执行软件复位 11
	//DelayTime ( 1 );
	delay_4us ( );
	delay_4us ( );
	delay_4us ( );
	delay_4us ( );
	delay_4us ( );
	
	RF_SPIWrite_REG ( ModeReg, 0x3d );//和Mifare卡通讯，CRC初始值0x6363
	
	RF_SPIWrite_REG ( TReloadRegL, 30 );
	                 
	RF_SPIWrite_REG ( TReloadRegH, 0);
	RF_SPIWrite_REG ( TModeReg, 0x8D);
	RF_SPIWrite_REG ( TPrescalerReg, 0x3E);
	RF_SPIWrite_REG ( TxAskReg, 0x40);
	
	return 0;
}

/*
*****************************************************************************************
* fuction name: AntennaOn
*
* Description : this function set CV520 Antenna on.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar AntennaOn ( void )
{
	
	SetBitMask( TxControlReg, 0x03 );
	//DelayTime ( 1 );
	delay_4us ( );
	delay_4us ( );
	delay_4us ( );
	delay_4us ( );
	delay_4us ( );
		
	return 0;
}

/*
*****************************************************************************************
* fuction name: AntennaOff
*
* Description : this function set CV520 Antenna off.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar AntennaOff ( void )
{
	
	ClearBitMask( TxControlReg, 0x03 );
	//DelayTime ( 1 );
	//DelayShort ( 10 );
		
	return 0;
}

/*
*****************************************************************************************
* fuction name: InitCV520
*
* Description : this function initialize CV520 chip.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar InitCV520 ( void )
{
	ResetCV520 ( );
	
	AntennaOn ( );
	
	g_rfFlag.Byte = 0;
								
	return 0;
}

/*
*****************************************************************************************
* fuction name: GetCV520Version
*
* Description : this function read CV520 chip software version.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : Here we will try to read 5 times if it is false
******************************************************************************************
*/
uchar GetCV520Version ( void )
{
	uchar ret;
	
	ret = RF_SPIRead_REG ( VersionReg );
								
	return ret;
}

/*
*****************************************************************************************
* fuction name: CV520ProcComm
*
* Description : this function let CV520 connet with card according to ISO1443
*
* Arguments   : none
*
* returns     : none
*
* Notes     :  
******************************************************************************************
*/
uchar CV520ProcComm ( uchar Command, uchar *pInData, uchar InLenByte, uchar *pOutData, uint16* pOutLenBit)
{
	uchar status = MI_ERR;
	uchar irqEn   = 0x00;
	uchar waitFor = 0x00;
	uchar lastBits;
	uchar n;
	uint16 i;
	
	FeedWatchdog();
	
	switch ( Command )
	{
	   //
	   case PCD_AUTHENT:
	   //case 0x0e:
	       irqEn   = 0x12;
		   waitFor = 0x10;		//指令命令运行结束11
		   break;
	   case PCD_TRANSCEIVE:
		   irqEn = 0x76;
		   waitFor =0x30;		//指令命令和接收数据结束11
		   break;
	   default:
		   break;
	}
	
	//使能各中断 IRQ引脚状态与Status1Reg寄存器IRQ位状态相反 11
	RF_SPIWrite_REG ( ComIEnReg, irqEn|0x80);	
	//IRQ引脚状态与Status1Reg寄存器IRQ位状态相同 11
	ClearBitMask ( ComIrqReg,0x80);
	
	//520处于空闲状态  11
	RF_SPIWrite_REG ( CommandReg, PCD_IDLE);
	//清空FIFO区数据 11
	SetBitMask (FIFOLevelReg,0x80);
	
	//执行发送FIFO中数据命令  11	
    for (i=0; i<InLenByte; i++)
	    RF_SPIWrite_REG(FIFODataReg, pInData[i]);	//写卡命令	11    
   	RF_SPIWrite_REG (CommandReg, Command);
   	
	if ( Command == CV520_TRANSCEIVE )	
	   SetBitMask (BitFramingReg,0x80); //相当于启动发送STARTSENG
	
	
	//i=2000;
	
	if ( InLenByte > 1 )
		i = 10000;		//10000
	else
		i=20;
	do 
	{
		 n = RF_SPIRead_REG ( ComIrqReg ); //没有使用comIrqReg寄存器中的TimerIRq定时器，所以去掉!(n&0x01) 11
		 i--;
		 FeedWatchdog();
		 
	}
	while ((i!=0) && !(n&waitFor));
	
		 
	ClearBitMask ( BitFramingReg,0x80);//相当于清除发送STARTSENG   11
	
	if (i!=0)
	{	 
		 if( !(RF_SPIRead_REG(ErrorReg)&0x1B) )
		 {
			 status = MI_OK;
			 if (n &irqEn &0x01) 
			     status = MI_NOTAGERR;
			 if ( Command == CV520_TRANSCEIVE )
			 {
				 n = RF_SPIRead_REG(FIFOLevelReg);
				 lastBits = RF_SPIRead_REG(ControlReg) &0x07;
				 if (lastBits)	
				    *pOutLenBit = (n-1)*8 + lastBits;
				 else	
				    *pOutLenBit = n*8;
				 if (n == 0)
				    n = 1;
				 if (n > MAXRLEN)	
				    n = MAXRLEN;  
				 for (i=0; i<n; i++)	
				    pOutData[i] = RF_SPIRead_REG(FIFODataReg);
			 }
		 }
		 else	
		    status = MI_ERR;
   }
   SetBitMask(ControlReg,0x80); 		  // stop timer now
   RF_SPIWrite_REG(CommandReg,PCD_IDLE); 
   return status;
   
}


/*
*****************************************************************************************
* fuction name: CV520RequestCard
*
* Description : this function let CV520 request card.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :  
******************************************************************************************
*/
uchar CV520RequestCard ( uchar reqCode, uchar *pTagType )
{	
	uchar status;  
	uint16 OutLenBit=0;
	
	//cypro加密单元位清0,数据使用标准Mifare标准
	ClearBitMask(Status2Reg, 0x08);	
	
	//？？最后一个字节只发送7位 
	RF_SPIWrite_REG(BitFramingReg, 0x07);
	
	//TX1 TX2引脚持续发送13.56MHz能量载波信号
	SetBitMask(TxControlReg, 0x03);
	
	
	g_rfSendBuf[0] = reqCode;
	status = CV520ProcComm ( PCD_TRANSCEIVE, g_rfSendBuf, 1, g_rfRevBuf, &OutLenBit);
	 
	//if (status ==MI_OK)
	if ( (status ==MI_OK) && (OutLenBit=0x10))
	{	 
		*pTagType =g_rfRevBuf[0];
		*(pTagType+1) =g_rfRevBuf[1];
	}
	else	
	    status = MI_ERR;   
	
								
	return status;
}

/*
*****************************************************************************************
* fuction name: CV520DetectCard
*
* Description : this function CV520 detect whether there are cards.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :  
******************************************************************************************
*/
uchar CV520DetectCard ( void )
{
    uchar status=1;
    
    //射频芯片复位
    InitCV520 ( );
    
    //寻卡 呼叫所有卡片，包括处于休眠(Halt)和休闲状态的卡片11  
    status = CV520RequestCard ( REQALL, g_rftagtype );
    
    //判断卡片在wake-up呼叫后的应答帧
    
    if ( status == MI_OK )
    {
       g_rfFlag.Bit.CurrentRequestOK = 1;
       return TRUE;
   	}
    else
    {
        g_rfFlag.Bit.CurrentRequestOK = 0;
        
        AntennaOff ( );  //CV520天线关闭
        HardwareHalt ( );
        
        return FALSE;
    }
}

/*
*****************************************************************************************
* fuction name: CV520Anticoll
*
* Description : this function CV520 anti-collite when there are more than one card.
*
* Arguments   : none
*
* returns     : card serial number
*
* Notes     :  
******************************************************************************************
*/
uchar CV520Anticoll ( uchar *pSnr )
{
    uchar status;
    uchar i,snr_check=0;
    uint16 OutLenBit=0;
    
    FeedWatchdog( );
    
    ClearBitMask ( Status2Reg,0x08 );
    RF_SPIWrite_REG ( BitFramingReg,0x00 );
    ClearBitMask ( CollReg,0x80 );
    
    
    g_rfSendBuf[0] = ANTICOLL1;
    g_rfSendBuf[1] = 0x20;
    status = CV520ProcComm ( PCD_TRANSCEIVE, g_rfSendBuf, 2, g_rfRevBuf, &OutLenBit);
    if ( status == MI_OK )
    {
    	 for ( i=0;i<4;i++ )
         {   
            *(pSnr+i)  = g_rfRevBuf[i];
            snr_check ^= g_rfRevBuf[i];
         }
         if (snr_check != g_rfRevBuf[i])  
            status = MI_ERR;    
    }
    
    SetBitMask(CollReg,0x80);
    
    return status;
}

/*
*****************************************************************************************
* fuction name: CV520SelectCard
*
* Description : this function CV520 anti-collite when there are more than one card.
*
* Arguments   : none
*
* returns     : card serial number
*
* Notes     :  
******************************************************************************************
*/
uchar CV520SelectCard ( void )
{
	uchar status=1;
	
	if ( (g_rfFlag.Bit.CurrentRequestOK==0) || (g_rfFlag.Bit.LastRequestOK==1)) 	  
		return FALSE;	
		
	g_rfFlag.Bit.LastRequestOK = g_rfFlag.Bit.CurrentRequestOK;	
	status = CV520Anticoll ( g_rfUID );				
		
	if( status == MI_OK )
		status = CV520SelectCommand ( g_rfUID );
		
	if(status ==MI_OK)
		return TRUE;
	else
	{	
	    AntennaOff();  //CV520?~{(,~}????~{!@~}? 
	    HardwareHalt();
	    
	    return FALSE;
	}
}

/*
*****************************************************************************************
* fuction name: CV520SelectCommand
*
* Description : this function CV520 proecss select card command.
*
* Arguments   : none
*
* returns     : card serial number
*
* Notes     :  
******************************************************************************************
*/
static uchar CV520SelectCommand ( uchar *pSnr )
{
    uchar status=1;
    uchar i;
    uint16 OutLenBit=0;
    
    FeedWatchdog();
    
    g_rfSendBuf[0] = ANTICOLL1;
    g_rfSendBuf[1] = 0x70;
    g_rfSendBuf[6] = 0;
    for ( i=0; i<4; i++ )
    {
	    g_rfSendBuf[i+2] = *(pSnr+i);
	    g_rfSendBuf[6]  ^= *(pSnr+i);
    }
    
    CalulateCRC ( g_rfSendBuf, 7, g_rfSendBuf+7 );    
    ClearBitMask( Status2Reg, 0x08 );
    
    status = CV520ProcComm ( PCD_TRANSCEIVE, g_rfSendBuf, 9, g_rfRevBuf, &OutLenBit );
    //if ( status ==MI_OK ) 
   	if ( (status ==MI_OK) && (OutLenBit=0x18))
        status = MI_OK;  
    else  
        status = MI_ERR; 
          
    return status;
}

/*
*****************************************************************************************
* fuction name: CV520MFAuthent
*
* Description : this function process MF authent command.
*
* Arguments   : none
*
* returns     : TRUEN or FALSE
*
* Notes     :  
******************************************************************************************
*/
uchar CV520MFAuthent ( uchar *pkey, uchar *pSnr, uchar block, uchar modeByte )
{
    uchar status=1;
    uchar i;
    uint16 OutLenBit=0;
    
    //block3 7 11等存储密码、及存储控制11
    if ( (block&0x03) != 0x03 )
    	return FALSE;
    	   	
    FeedWatchdog();
        
    g_rfSendBuf[0] = modeByte;
    g_rfSendBuf[1] = block;
    //KEYA KEYB
    for ( i=0; i<6; i++ )
    {
	    g_rfSendBuf[i+2] = *(pkey+i);	    
    }
    //4字节卡序列号11
    for ( i=0; i<4; i++ )
    {
	    g_rfSendBuf[i+8] = *(pSnr+i);	    
    }
        
    status = CV520ProcComm ( PCD_AUTHENT,g_rfSendBuf,12,g_rfRevBuf, &OutLenBit );
    if ( status ==MI_OK )  
    {
        if ( RF_SPIRead_REG(Status2Reg)&0x08 )      
        	return TRUE;
        else
        	return FALSE;	
    }
    else  
        return FALSE;
}	

/*
*****************************************************************************************
* fuction name: CV520ReadData
*
* Description : this function process read command.
*
* Arguments   : none
*
* returns     : TRUEN or FALSE
*
* Notes     :  
******************************************************************************************
*/
uchar CV520ReadData ( uchar block )
{
    uchar status=1;
    uchar i;
    uint16 OutLenBit=0;
       	   	
    FeedWatchdog();
        
    g_rfSendBuf[0] = READ;
    g_rfSendBuf[1] = block;
    
    CalulateCRC ( g_rfSendBuf, 2, g_rfSendBuf+2 ); 
           
    status = CV520ProcComm ( PCD_TRANSCEIVE,g_rfSendBuf,4,g_rfRevBuf, &OutLenBit );
    //if ( status ==MI_OK )  
    if ( (status ==MI_OK) && (OutLenBit=0x90))
        return TRUE;
    else  
        return FALSE;
}

/*
*****************************************************************************************
* fuction name: CV520WriteData
*
* Description : this function process write command.
*
* Arguments   : none
*
* returns     : TRUEN or FALSE
*
* Notes     :  
******************************************************************************************
*/
uchar CV520WriteData ( uchar block, uchar *pbuf )
{
    uchar status=1;
    uchar i;
    uint16 OutLenBit=0;
       	   	
    FeedWatchdog();
        
    g_rfSendBuf[0] = 0xa0;
    g_rfSendBuf[1] = block;
    
    CalulateCRC ( g_rfSendBuf, 2, g_rfSendBuf+2 ); 
           
    status = CV520ProcComm ( PCD_TRANSCEIVE,g_rfSendBuf,4,g_rfRevBuf, &OutLenBit );       
    if ((status != MI_OK) || (OutLenBit != 4) || ((g_rfRevBuf[0] & 0x0F) != 0x0A))
        return FALSE;
        
    //写入数据11
    if ( status == MI_OK )
    {
     	memcpy ( &g_rfSendBuf[0], pbuf, 16 );   
        CalulateCRC ( g_rfSendBuf, 16, g_rfSendBuf+16 ); 
        status = CV520ProcComm ( PCD_TRANSCEIVE,g_rfSendBuf,18,g_rfRevBuf, &OutLenBit );       
    	if ((status != MI_OK) || (OutLenBit != 4) || ((g_rfRevBuf[0] & 0x0F) != 0x0A))
        	return FALSE;
        	
        return TRUE;
    }
    
    return FALSE;
}

/*
*****************************************************************************************
* fuction name: CV520Halt
*
* Description : this function let CV520 into halt status.
*
* Arguments   : none
*
* returns     : TRUEN or FALSE
*
* Notes     :  
******************************************************************************************
*/
uchar CV520Halt ( void )
{
    uint16 OutLenBit=0;
       	   	
    FeedWatchdog();
        
    g_rfSendBuf[0] = HALT;
    g_rfSendBuf[1] = 0;
    
    CalulateCRC ( g_rfSendBuf, 2, g_rfSendBuf+2 ); 
           
    CV520ProcComm ( PCD_TRANSCEIVE,g_rfSendBuf,4,g_rfRevBuf, &OutLenBit );       
            	
	return TRUE;  
}
	
/*
*****************************************************************************************
* fuction name: CV520ProcessValue
*
* Description : this function value add or dec.
*
* Arguments   : none
*
* returns     : TRUEN or FALSE
*
* Notes     :  
******************************************************************************************
*/
uchar CV520ProcessValue ( uchar mode, uchar block, uchar* pValue )
{	
	uchar status;
	uint16 OutLenBit=0;
       	   	
    FeedWatchdog();
        
    g_rfSendBuf[0] = mode;
    g_rfSendBuf[1] = block;
    
    CalulateCRC ( g_rfSendBuf, 2, g_rfSendBuf+2 ); 
           
    status = CV520ProcComm ( PCD_TRANSCEIVE,g_rfSendBuf,4,g_rfRevBuf, &OutLenBit );       
    if ((status != MI_OK) || (OutLenBit != 4) || ((g_rfRevBuf[0] & 0x0F) != 0x0A))
    {   
    	status = MI_ERR;   
    }
        
    if (status == MI_OK )
    {
        memcpy(g_rfSendBuf, pValue, 4);
        CalulateCRC ( g_rfSendBuf, 4, g_rfSendBuf+4 ); 
        OutLenBit = 0;
        status = CV520ProcComm ( PCD_TRANSCEIVE,g_rfSendBuf,6,g_rfRevBuf, &OutLenBit );
    }
    
    if (status == MI_OK )
    {
        g_rfSendBuf[0] = PICC_TRANSFER;
    	g_rfSendBuf[1] = block;
    
    	CalulateCRC ( g_rfSendBuf, 2, g_rfSendBuf+2 ); 
        OutLenBit = 0;   
    	status = CV520ProcComm ( PCD_TRANSCEIVE,g_rfSendBuf,4,g_rfRevBuf, &OutLenBit ); 

        if ((status != MI_OK) || (OutLenBit != 4) || ((g_rfRevBuf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    
    if ( status == MI_OK )
    	return TRUE;
    else
    	return FALSE;

}

#endif
