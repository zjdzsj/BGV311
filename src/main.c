/******************************************************************************************
*  Copyright (c) 2010, Bona R&D Department
*  All rights reserved 
*
*  FileName：   	main.c
*  Author：     	Roger
*  Date：  			2010-3-8 20:10
*  Revised Date：  	
*  Main Function：  this file contians the main function
*  Version:			BonaV2.0
*  Note： 			 			
*********************************************************************************************/
#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

//#pragma interrupt INTTM000 ISRTimer000

#define _MAIN_DEFINE_

#include "include.h"

typedef  void (*pFunction) ( void );

//#pragma location="SECUID"
//__root const uchar securityID_bytes[]={0xFf,0xFF,0xFF,0xFF,0xFf,0xFF,0xFF,0xFF,0xFf,0xFF} ;


/*****************************************************************************************/

/*****************************************************************************************/

static void EndOfTask ( void );
static void TopLoopTask ( void );
static void InitSystemStep1 ( void );
static void InitSystemStep2 ( void );
static void EndOfHalfSecondTask ( void );	
/*****************************************************************************************/

unsigned char s_counterFunc=0;      //1 秒函数处理状态字 11
static unsigned char s_counterFuncHalfSec=0;

static const pFunction RegularTaskTable[] = {

		(pFunction)ProcessSecTaskFlags, 
		(pFunction)ProcessRelayStatus,
		(pFunction)ManageLCDDisplay,
    
    
    #ifdef PRE_PAID_ENABLE 
    	#if(IC_CARD_MODE==AT24_CARD)
			(pFunction)ManageICCard,
		#endif
	#endif	
	
		(pFunction)ProcessRelay,
		
	#ifdef TARIFF_ENABLE
		(pFunction)ManageTariff,
	#endif
	#ifdef LADDER_ENABLE		
		(pFunction)ManageLadder,
	#endif
		(pFunction)ReadRmsValue,
		
	#ifdef DEMAND_ENABLE
		(pFunction)ManageDemand,
	#endif	
		
	 	(pFunction)EndOfTask 
};

static const pFunction HalfSecondTaskTable[] = {
	#ifdef RTC_8025T
		(pFunction)GetTime,
	#endif
	(pFunction)ProcessHalfSecTaskFlags,
	(pFunction)EndOfHalfSecondTask 
};

/*****************************************************************************************/

/*
*****************************************************************************************
* fuction name: main
*
* Description : this function contorl meter process.
*
* Arguments   : none
*		
* returns     : int, but never return actual value
*
* Notes     :
******************************************************************************************
*/
int main ( void )
{			
	void (* pTaskFunc) ( void );
	void (* pHalfSecTaskFunc) ( void );
	uchar i;
	uint16 addr;
				
	InitSystemStep1 (  );	        
	
	FeedWatchdog ( );
		
    ALARM_ON();
    LCDLED_ON();
    for ( i=0; i<2; i++ )
	{	    	    
		Delay1MsTime ( 200 );
	    FeedWatchdog  (  );	    
	}
    
	DisplayFull( );
	for ( i=0; i<6; i++ )
	{	    	    
		Delay1MsTime ( 200 );
	    FeedWatchdog  (  );	    
	}
		
	DisplayVersion ( );
	for ( i=0; i<2; i++ )
	{	    	    
		Delay1MsTime ( 200 );
	    FeedWatchdog  (  );	    
	}	
	
//	for ( i=0; i<16; i++ )
//		g_buf[i] = 16+i;
//	//Page 1
//	addr = offsetof(EEPROM_DATA,CurDemand);
//    EEPROM_NOWP();
//    I2CWrite ( EEPROM_ADDR, addr, &g_buf[0], 16, 1  );
//	EEPROM_WP();
//	
//	I2CRead ( EEPROM_ADDR, addr, &g_buf[0], 16, 1  ) ;	
//	
//	for ( i=0; i<16; i++ )
//		g_buf[i] = 32+i;
//	//Page 1
//	addr = 0x0e;
//    EEPROM_NOWP();
//    I2CWrite ( EEPROM_ADDR, addr, &g_buf[0], 16, 1  );
//	EEPROM_WP();
//	
//	I2CRead ( EEPROM_ADDR, addr, &g_buf[0], 16, 1  ) ;	
//	
   	/*
   	g_buf[0] = 0x00;
	g_buf[1] = 0x00;
	g_buf[2] = 0x60;
   	I2CWrite ( RX8025T_ADDR, EXTENTION_REG,buftes, 3, 1  );
   	*/
   	
//   	g_buf[6] = 0x18;
//	g_buf[5] = 0x10;
//	g_buf[4] = 0x28;
//	g_buf[3] = 0x00;
//	g_buf[2] = 0x14;
//	g_buf[1] = 0x05;
//	g_buf[0] = 0x00;
//	SetTime ( g_buf );

#if (IC_CARD_MODE==RF_CARD)  	
   	InitCV520 ( );
   	HardwareHalt ( );
#endif
   	  	
    ALARM_OFF();
    LCDLED_OFF();
    
   	//GetTime ( );
    		
	InitSystemStep2 (  );	
	
#if(METER_CHIP_MODEL==0x8302)	

	if ( SetRN8302_Register( ) != 0 )
    {
        if ( SetRN8302_Register( ) != 0 )
            while ( 1 );               
    }
    
#endif
    
#if(METER_CHIP_MODEL==0x2208)	
	InitUart2 ( );
	SoftResetCl2208 ( );
	FeedWatchdog  (  );	
	Delay1MsTime ( 150 );
	FeedWatchdog  (  );	
	Delay1MsTime ( 150 );
	FeedWatchdog  (  );	
	CheckCl2208Reg ( );
#endif
	
	
//	while(1)
//	{
//		ALARM_ON();
//		RelayA_L;    
//        RelayB_H;
//	    for ( i=0; i<1; i++ )
//		{	    	    
//			Delay1MsTime ( 50 );
//		    FeedWatchdog  (  );	    
//		}	
//		RelayA_L;    
//        RelayB_L;
//        
//        for ( i=0; i<6; i++ )
//		{	    	    
//			Delay1MsTime ( 200 );
//		    FeedWatchdog  (  );	    
//		}
//        
//        ALARM_OFF();
//        RelayA_H;    
//        RelayB_L;
//	    for ( i=0; i<1; i++ )
//		{	    	    
//			Delay1MsTime ( 50 );
//		    FeedWatchdog  (  );	    
//		}	
//		RelayA_L;    
//        RelayB_L;
//		
//	}
	
	    		
	while ( 1 )
	{					
		FeedWatchdog ( );
			    		
		//while ( (g_relayCnt==0) && CompareEXLVI() == 1 )
		while ( (g_relayProCnt==0) && 
		#if(IC_CARD_MODE==PLC_COMM_USED)
				(g_PLCInTransCnt==0) &&
		#endif
				(LVIF==1) )
		{	
								
			DI ( );
			
			
			ALARM_OFF();
			if ( g_meterFlag.Bit.PowerDown == 0 )
				SaveEnergy ( LOW_POWER_SAVE );			
			ALARM_ON();
			EI();			
			
			FeedWatchdog (  );	
			
		#ifdef RTC_8025T						    
		    //低功耗处理 11
		    ProcessLowPower ( );
		#endif
		}
			
		TopLoopTask ( );
		
		if ( g_meterFlag.Bit.HalfSec )
		{			
   		    if ( g_relayCnt == 0 )
	        {
	    
   		        pHalfSecTaskFunc = HalfSecondTaskTable[s_counterFuncHalfSec++];	
   			    (*pHalfSecTaskFunc) ( );   	
   		    }			    		
  		}
  				
		//here perform the regular tasks which perform frequency is 0.5s or more longer
		if ( g_meterFlag.Bit.OneSec )
		{			
   		    
   		    if ( g_relayCnt == 0 )
	        {
    	        //DI ( );
       		    pTaskFunc = RegularTaskTable[s_counterFunc++];	
       			(*pTaskFunc) ( );  
       			//EI( ); 
       		}		    		
   		}
   		
   		if ( g_meterFlag.Bit.OneLoopTask )
   		{
   		
   		}
   		   								   				   				
	}	
		
	return 1;
        
}

/*
*****************************************************************************************
* fuction name: EndOfTask
*
* Description : Here this function just clear one flag s_flagHalfSec
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/	
static void EndOfTask ( void )
{
	//uchar buftes[16];
	
	
    //P12_bit.no0 = 0;
    //I2CRead ( RX8025T_ADDR, 0x00,buftes, 7, 1  );
   	//DisplayInputNumber ( buftes  );	
   	//P12_bit.no0 = 1;
	g_meterFlag.Bit.OneSec = 0;
	
   	s_counterFunc = 0; 
   	
   	g_meterFlag.Bit.PowerDown = 0;
   			
	return;
}

/*
*****************************************************************************************
* fuction name: EndOfHalfSecondTask
*
* Description : Here this function just clear one flag s_flagHalfSec
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/	
static void EndOfHalfSecondTask ( void )
{
		
	g_meterFlag.Bit.HalfSec = 0;
	
   	s_counterFuncHalfSec = 0; 
   	
   			
	return;
}

/*
*****************************************************************************************
* fuction name: TopLoopTask
*
* Description : This task which process in this function are need to be done every loop
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void TopLoopTask ( void )
{
	uchar i;
	
	if ( g_relayCnt > 0 )
	    return;
	    
	//处理电量累加和递减 11
	ManageEnergy ( );

	
	//通信处理发送 11
#if(IC_CARD_MODE==PLC_COMM_USED)
	for ( i=0; i<3; i++ )
#else
	for ( i=0; i<2; i++ )
#endif	
	{
		if ( g_meterFlag.Bit.RS485RcvOvertime )
		{
			ResetRS485Comm ( );	
		}
		if ( g_meterFlag.Bit.IrRcvOvertime )
		{
			ResetIRComm ( );	
		}
	#if(IC_CARD_MODE==PLC_COMM_USED)
		if ( g_meterFlag.Bit.PLCRcvOvertime )
		{
			ResetPLCComm ( );	
		}
	#endif	
		ProcessCommRxdOrTxd ( i );
		//CheckCommMessage ( i );
		//DecodeIRMessage ( i );
		//BuildIRMessage ( i );
	}
	
    //发生分钟事件 11
    if ( g_date.sec == 0 )
    {      
        if ( g_zeroSecCnt==0 )
        {
            g_zeroSecCnt = 300;    
            g_meterFlag.Bit.ZeroSecondFlag = 1;   
        }	
    }
	
	if (　g_cardFlag.Bit.BtnFreshLCD )
	{
		g_cardFlag.Bit.BtnFreshLCD = 0;	
		ManageLCDDisplay ( );
	}

#ifdef PRE_PAID_ENABLE 
	#if (IC_CARD_MODE==RF_CARD)
	if ( g_rfFlag.Bit.Request || (g_rfTryCnt>0) )
    {               	   	       	
    	if ( CV520DetectCard ( ) == TRUE )
    	{        		        		
    		if ( g_meterFlag.Bit.RFInFieldFlag == 0 )    		
    			ManageRF_ICCard( );         				        		
        }
    	else
    	{
    		g_meterFlag.Bit.RFInFieldFlag = 0;
    		g_rfInFieldCnt = 0;
    		g_rfTryCnt = 0;
    	}
    	
    	g_rfFlag.Bit.Request = 0; 
    	if (g_rfTryCnt>0)
    		g_rfTryCnt--;	    	        	
    }	
    #endif
#endif
			
	return;
}

/*
*****************************************************************************************
* fuction name: InitSystemStep1
*
* Description : Here this function just initialize system
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/	
static void InitSystemStep1 ( void )
{
	DI ( );
	
	InitHardware ( );
			    

		                       
	EI( ); 
   			
	return;
}

/*
*****************************************************************************************
* fuction name: InitSystemStep1
*
* Description : Here this function just initialize system
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/	
static void InitSystemStep2 ( void )
{	
	InitEnergyPin (  );
    InitTimerArray ();
	
	InitGlobalVariable (  );

#ifdef IR_CHANNEL_ENABLE
	InitIR();
	ResetIRComm();
#endif	
	
#ifdef RS485_CHANNEL_ENABLE
	InitRS485();
	ResetRS485Comm();
#endif		

#if(IC_CARD_MODE==PLC_COMM_USED)
	InitPLCComm ( );
	ResetPLCComm ( );
#endif	
	   			
	return;
}	
