/******************************************************************************************
*  Copyright (c) 2010, Bona R&D Department
*  All rights reserved 
*
*  FileName��   	timer.c
*  Author��     	Roger.
*  Date��  			2010-3-8 20:10
*  Revised Date��  	
*  Main Function��  this file provides the timer functions which have relationship with 
*  Version:			BonaV2.0
*  Note�� 			 			
*********************************************************************************************/
#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

#pragma interrupt INTTM00 ISRForTimer0
#pragma interrupt INTTM01 ISRForTimer1

//#pragma interrupt INTTMH1 ISRTimerH1
//#pragma interrupt INTWT ISRClockTimer 

/************************************************************************************************/
#define _TIMER_DEFINE_
#include "include.h"
/************************************************************************************************/



//only used in timer.c file
/************************************************************************************************/
//variables definition
const uchar DAY_OF_MONTH[12]={31, 28, 31, 30, 31, 30,
						31, 31, 30, 31, 30, 31};
static uchar s_halfSec;
static uchar s_twoSec;
//static uchar s_quaterSec;

static uchar s_cardOutCnt;
static uchar s_cardInCnt;

#if(PCB_VERSION_NEW==0)	
	static uchar s_key1DownCnt;
#else
	static uchar s_QDownCnt;
#endif
static uchar s_key2DownCnt;

#if(IC_CARD_MODE==PLC_COMM_USED)
	static uchar s_key1NBBtnCnt;
	static uint16 s_PLCEventCnt;
#endif


#if (IC_CARD_MODE==RF_CARD)
	static uchar s_rfCycleCnt;
#endif
//static uchar s_oneMinute;
//static uint16 s_powerCnt[5];

/************************************************************************************************/

/*
*****************************************************************************************
* fuction name: InitTimerArray
*
* Description : ��ʼ����ʱ������
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/

void InitTimerArray ( void )
{
	ST0 |= 0x000F;
	//����������е�Ԫ�ĸ�λ״̬,����ʼ�ṩʱ��11
	PER0 |= BIT2;	
	NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    	
	// ʹ�ö�ʱ������ 11
    TAU0EN = 1U;    // supplies input clock   11
    
    // ֹͣ��ʱ�� 0 11
    TT0 = (1<<0)+(1<<1)+(1<<4)+(1<<3)+(1<<5);
    // ѡ�񹫹�ʱ��CK0������ֹͣ��Ӧ�Ķ�ʱ�����ܸ��� 11
    // b13b12: CK03, b9b8: CK02, 11
    // b7-b4: CK01, b3-b0: CK00, 11
    // 0b0000 - fCLK ... ... 0b1111 - fCLK/2^15 11
    //CK01 4M/64=62.5kHz
    TPS0 = (0b00<<12)|(0b00<<8)|(0b0110<<4)|(0b0000);  
	
	//��ʱ��0 4ms 11
	////////////////////////////////////////////////////////////
	{	    
    // ֹͣ��ʱ���ж� 11
    TMMK00 = 1U;    // disable INTTM00 interrupt
    TMIF00 = 0U;    // clear INTTM00 interrupt flag

    // �趨�ж����ȼ����� 11
    TMPR100 = 1U;
    TMPR000 = 1U;
    
    // ���ö�ʱ��  11
    // ѡ��CK00-CK03, �����ʱ�� 11
    TMR00 = (0b00<<14)|(0b0000<<0); 

    // ���ü��������ݼ���TDR00 = fTCLK*T - 1 = 4M*1ms-1 11
    TDR00 = 15999; // 4ms@4M

    // �����ʱ�в���� 11
    TOM0 &= ~(1<<0);    // ���ģʽ 11
    TOL0 &= ~(1<<0);    // ��������� 11
    TO0 &= ~(1<<0);     // ֹͣ����ʱ����ĵ�ƽ 11
    TOE0 &= ~(1<<0);    // ��ֹ��ʱ����� 11
    
    TMIF00 = 0U;    // clear INTTM00 interrupt flag 
    TMMK00 = 0U;    // enable INTTM00 interrupt 
    TS0 |= (1<<0);      //������ʱ��0 11
    
    }
	///////////////////////////////////////////////////////////////////////
	
	//��ʱ��1 0.5s 11
	////////////////////////////////////////////////////////////
	{
	    // channels 7  ��ʱ�� 11
        
        // Mask channel 1 interrupt
        TMMK01 = 1U;    // disable INTTM07 interrupt
        TMIF01 = 0U;    // clear INTTM07 interrupt flag 
    
        // Set INTTM01 low priority 
        TMPR101 = 1U;
        TMPR001 = 1U;
    
        // Channel 1 used as interval timer ѡ��CK01 1M; 11
        TMR01 =  (0b10<<14);
        
        TDR01 = 31250; // 500ms@1M
        
        TOM0 &= ~(1<<1);
        TOL0 &= ~(1<<1);
        TO0 &= ~(1<<1);
        TOE0 &= ~(1<<1);
	    
	    TMIF01 = 0U;    // clear INTTM07 interrupt flag 
        TMMK01 = 0U;    // enable INTTM07 interrupt 
        TS0 |= (1<<1);      //������ʱ��7 11
    
	    s_halfSec = 0;
	    s_twoSec = 0;
	
	    g_buttonCnt = 0; 
	#ifdef DEMAND_ENABLE    
	    g_demandSlideCnt = 60; 
	#endif
    }
	////////////////////////////////////////////////////////////
	
	//��ʱ��3 1200bps�������� 833.33us ���պ������� ģ�⴮�� 11 
	////////////////////////////////////////////////////////////
	{
	    // channels 3  ��ʱ�� 11
        
        // Mask channel 3 interrupt
        TMMK03 = 1U;    // disable INTTM03 interrupt
        TMIF03 = 0U;    // clear INTTM03 interrupt flag 
    
        // Set INTTM01 low priority 
        TMPR103 = 0U;
        TMPR003 = 0U;
    
        // Channel 1 used as interval timer ѡ��CK01 4M; 11
        TMR03 =  (0b00<<14);
        
        TDR03 = 3333; // 833.33us@4M 11
        
        TOM0 &= ~(1<<3);
        TOL0 &= ~(1<<3);
        TO0 &= ~(1<<3);
        TOE0 &= ~(1<<3);
	    
	    TMIF03 = 0U;    // clear INTTM03 interrupt flag 
        TMMK03 = 1U;    // disable INTTM03 interrupt 
        TT0 |= (1<<3);      //�ȹرն�ʱ��3 11     
    }
	////////////////////////////////////////////////////////////
	
	//��ʱ��4 38k��� 11
	////////////////////////////////////////////////////////////
	{
        
        TMMK04 = 1U;    // disable INTTM07 interrupt 11
        TMIF04 = 0U;    // clear INTTM07 interrupt flag  11
    
        // Set INTTM07 low priority  11
        TMPR104 = 1U;
        TMPR004 = 1U;
    
        // Channel 7 used as interval timer  11
        TMR04 =  00;
        
        //38k 13.15us 11
        TDR04 = 52; // @4M 11
        
        //�����ʱ�в���� 11
        TO0 &= ~(1<<4);     //ֹͣ����ʱ����͵�ƽ 11 11
        //TOE0 |= (1<<4);��   //������ʱ����� 1111
    }
	/////////////////////////////////////////////////////////////
		
	//��ʱ��5 1200bps�������� 833.33us ��� 11 11
	/////////////////////////////////////////////////////////////
	{               
        
        TMMK05 = 1U;    // disable INTTM07 interrupt 11
        TMIF05 = 0U;    // clear INTTM07 interrupt flag  11
    
        // Set INTTM07 low priority  11
        TMPR105 = 1U;
        TMPR005 = 1U;
    
        // Channel 7 used as interval timer ѡ��CK00 4M; 11
        TMR05 =  (0b00<<14);
        
        TDR05 = 3333; // 833.33us@4M 11
        
        TOM0 &= ~(1<<5);
        TOL0 &= ~(1<<5);
        TO0 &= ~(1<<5);
        TOE0 &= ~(1<<5);
	     
    }
	////////////////////////////////////////////////////////////


}

/*
*****************************************************************************************
* fuction name: Timer4_38k_start
*
* Description : this interrupt service function run in every 4ms.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void Timer4_38k_start(void)
{
    TOE0 |= (1<<4);
    TS0 |= (1<<4);
    
    
}

/*
*****************************************************************************************
* fuction name: Timer4_38k_stop
*
* Description : this interrupt service function run in every 4ms.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void Timer4_38k_stop(void)
{
    TT0 |= (1<<4);
    TOE0 &= ~(1<<4);
}

/*
*****************************************************************************************
* fuction name: Timer5_1200bps_start
*
* Description : this interrupt service function run in every 4ms.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void Timer5_1200bps_start(void)
{
    TS0 |= (1<<5);
    
    TMMK05 = 0U;    // enable INTTM07 interrupt 11
    TMIF05 = 0U;    // clear INTTM07 interrupt flag  11
}

/*
*****************************************************************************************
* fuction name: Timer5_1200bps_stop
*
* Description : this interrupt service function run in every 4ms.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void Timer5_1200bps_stop(void)
{
    TT0 |= (1<<5);
    TMMK05 = 1U;    // disable INTTM07 interrupt 11
}


/*
*****************************************************************************************
* fuction name: ISRForTimer0
*
* Description : this interrupt service function run in every 4ms.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt static void ISRForTimer0( void )
{
	uchar i;
			    
	DI ( );
	
#if(METER_CHIP_MODEL==0x2208)	
	//�����80msʱ����� 11
	if ( g_pulseLedDly > 0 )
	{
		g_pulseLedDly--;		
	}
	if ( g_pulseLedDly == 0 )
		PULSE_OFF();
#endif
				
	//ͨ����ʱ 11
	if ( g_commFor20msDly[0] > 0 )		//IR 11
		g_commFor20msDly[0]--;
	if ( g_commFor20msDly[1] > 0 )		//RS485
		g_commFor20msDly[1]--;
	if ( g_commFor20msDly[2] > 0 )		//PLC
		g_commFor20msDly[2]--;
		
	if ( g_485CommDly > 0 )
    {
        g_485CommDly--;
        if (  g_485CommDly == 0 )
        {   
            InitRS485 ( );
	    	ResetRS485Comm ( );
        }
    }
	if ( g_IRCommDly > 0 )
    {
        g_IRCommDly--;
        if (  g_IRCommDly == 0 )
        {   
            InitIR ( );
	    	ResetIRComm ( );
        }
    }
#if(IC_CARD_MODE==PLC_COMM_USED)	
    if ( g_PLCCommDly > 0 )
    {
        g_PLCCommDly--;
        if (  g_PLCCommDly == 0 )
        {   
            InitPLCComm ( );
	    	ResetPLCComm ( );
        }
    }
#endif       		           
	//card in 
	
#if(METER_CHIP_MODEL==0x2208)
	//RN8209C ���ڽ���������ʱ 11
	if ( g_Uart2Dly > 0 )
		g_Uart2Dly--;
#endif
	
	//�̵���������ʱ 11
	if( g_relayCnt > 0 )
	{
		g_relayCnt--;
		
		if ( g_relayCnt == 0 ) 
		{
			RelayA_L;    
			RelayB_L;   
		}   
	}

#ifdef PRE_PAID_ENABLE
    #if(IC_CARD_MODE==AT24_CARD)
	    //IC���ж� 11
	    //card in 
	    if ( (IC_CHK) && 
	        (g_cardFlag.Bit.ICCardOutUse==0) )       //out
	    {        
	        if ( s_cardOutCnt < 4 )
	            s_cardOutCnt++;
	        else 
	        {            
	            g_cardFlag.Bit.ICCardOutUse = 1; 
	            g_cardFlag.Bit.ICCardInUse = 0;           
	        }  
	        
	        s_cardInCnt = 0;                                                                   
	    }
	    else if (IC_CHK==0)       //in
	    {                 
	        if ( (g_cardFlag.Bit.ICCardInUse==0) )
	        {
	            if ( s_cardInCnt < 4 )
	                s_cardInCnt++;
	            else 
	            {
	                g_cardFlag.Bit.ICCardInUse = 1;
	                g_cardFlag.Bit.ICCardIn = 1; 
	                g_cardFlag.Bit.ICCardOutUse = 0;                
	            } 
	            
	            s_cardOutCnt = 0;                  
	        }
	        
	    } 
	#endif  
#endif

#if(IC_CARD_MODE==PLC_COMM_USED)	
	if ( s_PLCEventCnt > 0 )
	{
		s_PLCEventCnt--;
		PLCEVENT_IO = 0;
		ALARM_ON();
		
		if ( s_PLCEventCnt == 0 )
		{
			PLCEVENT_IO = 1;
			ALARM_OFF();	
		}	
	}
	
	if ( g_PLCRstCnt > 0 )
	{
		g_PLCRstCnt--;
		PLCRST_IO = 0;
		
		ALARM_ON();
		
		if ( g_PLCRstCnt == 0 )
		{
			PLCRST_IO = 1;
			ALARM_OFF();	
		}	
	}	
	
	//if ( PLCSTA_IO == 1 )	
	//	g_PLCInTransCnt = 3;	
#endif

#if(PCB_VERSION_NEW==0)	
	//����1 ���� �� ����������·� up 11
	if ( KEY1 == 0 )		//���� 11
	{
		if ( s_key1DownCnt < 10 )		//40ms
		{
			s_key1DownCnt++;
		
			if ( s_key1DownCnt >= 10 )
			{
				g_cardFlag.Bit.KEY1BtnDown = 1;					
				g_cardFlag.Bit.BtnFreshLCD = 1;	
				g_btnFreezeLCD = BTN_FREEZE_LCD_TIME;
				LCDLED_ON();	
			}			
		}		
	}
	else
	{
		s_key1DownCnt = 0;	
	}
#else		//��PCB�汾 �����������޹��ɼ� 11
	#ifdef INACITVE_POWER_ENABLE
		if ( Q_PULSE == 0 )	//rising edge�����������ʼ,111
		{
			if ( s_QDownCnt < 10 )
			{
				s_QDownCnt++;
				
				if ( s_QDownCnt >= 10 )	
				{
					g_Inactivenergy.InactivePulseCnt++;		
				}
			}					    
		}
		else	
		{
		    s_QDownCnt = 0;	    	    	    	    		
		}
	#endif	
#endif
	
#if(SINGLE_OR_THREE_METER==3)    
    //����2 ���� ����������Ϸ� 11
	if ( KEY2 == 0 )		//���� 11
	{
		if ( s_key2DownCnt < 10 )		//40ms
		{
			s_key2DownCnt++;
		
			if ( s_key2DownCnt >= 10 )
			{
				g_cardFlag.Bit.KEY2BtnDown = 1;	
				g_cardFlag.Bit.BtnFreshLCD = 1;	
				g_btnFreezeLCD = BTN_FREEZE_LCD_TIME;	
				LCDLED_ON();
				
				g_errLCDDispCnt = 0;
			}		
		}			
	}
	else
	{
		s_key2DownCnt = 0;	
	}  
#endif
   
    //0���¼������� 11
    if ( g_zeroSecCnt > 0 )
        g_zeroSecCnt--;

#if(IC_CARD_MODE==PLC_COMM_USED)
	for ( i=0; i<3; i++ )
#else
	for ( i=0; i<2; i++ )
#endif	
   	{	    
	    if ( g_commRxdFrameDly[i] > 0 )
	    {
	    	g_commRxdFrameDly[i]--;
	    	if ( g_commRxdFrameDly[i] == 0 )
	    	{
	    		g_commStatus[i].Bit.RxdFrameDone = 1;
	    	}	    	
	    }
   	}
   	
	EI ( );
}


/*
*****************************************************************************************
* fuction name: Delay1MsTime
*
* Description : this function do nothing just delay time.
*
* Arguments   : uchar n, it is delay time which unit is about 1ms.
*		
* returns     : none
*
* Notes     :	here it is rough time
******************************************************************************************
*/
void Delay1MsTime ( uint16 n )
{
    
    uint16 i ;
    uint16  j ;
    
    for ( i=0; i<n; i++)
    {
        for ( j=0; j<500; j++ );
    }
    
    /*
    n = n*10;
    
    TMHE1 = 1;
	TMMKH1 = 0;	//enable interrupt
	
	while ( n-- )
	{
		g_flag100us = 1;
		while ( g_flag100us );		
	}
	
	TMHE1 = 0;
	TMMKH1 = 1;	//disable interrupt
	*/
}

/*
*****************************************************************************************
* fuction name: delay_4us
*
* Description : this function delay 4us time to match I2C protocol.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : Here the 4us time is rough, you can adapt it if needed.
******************************************************************************************
*/
void delay_4us ( void )
{
	uchar j;
	for ( j=0; j<50; j++ )		//ԭ����30us 11
		NOP ( );
	return;
}

/*
*****************************************************************************************
* fuction name: delay_time
*
* Description : this function delay 4us time to match I2C protocol.
*
* Arguments   : none
*
* returns     : none
*
* Notes     : Here the 4us time is rough, you can adapt it if needed.
******************************************************************************************
*/
void delay_time ( void )
{
	uchar j;
	for ( j=0; j<80; j++ )		//ʵ�ʲ���Ŀǰʱ��Ϊ80->100us 11
	//for ( j=0; j<250; j++ )		//ʵ�ʲ���Ŀǰʱ��Ϊ80->100us 11
	//for ( j=0; j<100; j++ )		//ʵ�ʲ���Ŀǰʱ��Ϊ80->100us 11
		NOP ( );
	return;
}



/*
*****************************************************************************************
* fuction name: ISRForTimer1
*
* Description : this interrupt service function run in every 0.5s
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISRForTimer1( void )
{
	//uint16 tempInt16;
		
	DI ( );
	
	//P0_bit.no0 = ~P0_bit.no0;		//alarm
	//s_quaterSec++;
	//if ( s_quaterSec < 2 )
	//{
	//	return;		
	//}
	//s_quaterSec = 0;

	//NB�����������ϴ� 11
#if(IC_CARD_MODE==PLC_COMM_USED)
	if ( KEY1 == 0 )
	{
		s_key1NBBtnCnt++;
		if ( s_key1NBBtnCnt == 6  )		//3s 11
		{
			s_PLCEventCnt = 500;		//500*4	
		}
		else if ( s_key1NBBtnCnt == 12 )		//发送NB模块复位信号 11
		{
			g_PLCRstCnt = 500;	
			//			
			g_meterStatus3.Bit.BtnKeepRelay = 1;
		}
	}
	else
	{
		s_key1NBBtnCnt = 0;	
	}
	
		
#endif
		

	//ͨ�ſ�֡��������ʱ���� 11
	if ( g_commForOneFrame[0] > 0 )//���� 11
	{
		g_commForOneFrame[0]--;
		if ( g_commForOneFrame[0] == 0)
			g_meterFlag.Bit.IrRcvOvertime = 1;
	}
	if ( g_commForOneFrame[1] > 0 )//485 11
	{
		g_commForOneFrame[1]--;
		if ( g_commForOneFrame[1] == 0)
			g_meterFlag.Bit.RS485RcvOvertime = 1;
	}

#if(IC_CARD_MODE==PLC_COMM_USED)	
	if ( g_PLCInTransCnt > 0 )
		g_PLCInTransCnt--;
#endif
	
	g_meterFlag.Bit.HalfSec = 1;
	    
	s_halfSec++;			
	//�봦��	 11
	if ( s_halfSec >= 2 )
	{
        s_halfSec = 0;
        
        //Һ����ʾ ʱ�� ��˸ 11
        g_lcdFlag.Bit.TimerBlink = ~g_lcdFlag.Bit.TimerBlink;
        
        
        g_meterFlag.Bit.OneSec = 1;
    	
    	//24Сʱ��ʱ�� 11
	#if (FREEZE_DAY_EANBLED==1)
		g_daySeconds++;			
	#endif	
    	        
        //ʵʱʱ�Ӵ��� 11
	#ifdef SOFT_RTC
		//ʱ �� �� ���� 11
		//g_date.sec++;
		_BCDAccOne ( &g_date.sec );
		if( g_date.sec >= 0x60 )	
		{
			g_date.sec = 0;
			
		#ifdef TARIFF_ENABLE
			g_meterFlag.Bit.TariffProcess = 1;
			
		#endif
			
			//g_date.min++;
			_BCDAccOne ( &g_date.min );
			if( g_date.min >= 0x60 )
			{							
				g_date.min = 0;
				
				//g_date.hour++;
				_BCDAccOne ( &g_date.hour );
				if( g_date.hour >= 0x24 )
				{
					g_date.hour = 0;
					
					//g_date.day++;
					_BCDAccOne ( &g_date.day );
					if( g_date.month==0x02 )
					{
					 	if( (g_date.year&0x03) == 0 )
					 	{
					 		if( g_date.day >= 0x29 )
					 		{
					 			g_date.day = 1;
					 			//g_date.month++;
					 			_BCDAccOne ( &g_date.month );
					 			g_energy.CurrentQ = 0;
					 		#ifdef LADDER_ENABLE	
					 			
					 			g_meterFlag.Bit.LadderProcess = 1;
					 			
					 		#endif	
					 		}
					 	}
					 	else
					 	{
					 		if( g_date.day >= 28 )
					 		{
					 			g_date.day = 1;
					 			//g_date.month++;
					 			_BCDAccOne ( &g_date.month );
					 			g_energy.CurrentQ = 0;
					 		#ifdef LADDER_ENABLE
					 			
					 			g_meterFlag.Bit.LadderProcess = 1;
					 		#endif	
					 		}	
					 	}	
					}
					if( g_date.day > DAY_OF_MONTH[(g_date.month>>4)*10+(g_date.month&0x0f)-1] )
					{
						g_date.day = 1;
						//g_date.month++;	
						_BCDAccOne ( &g_date.month );
						g_energy.CurrentQ = 0;	
					#ifdef LADDER_ENABLE	
						
						g_meterFlag.Bit.LadderProcess = 1;
					#endif				
					}
	
					if( g_date.month > 0x12 )
					{
						g_date.month = 1;
						//g_date.year++;	
						_BCDAccOne ( &g_date.year );
					}	
				}				
			}
		}
	#endif
                
        //AD sample interval 
        g_adSampleCnt--;      
        if ( g_adSampleCnt == 0 )
        {
            g_adSampleCnt = AD_SAMPLE_3S;
            
            //ADCS = 1;
			//ADCE = 1;                 
        } 
        
        //ͨѶ��ʾ�� 11
        if ( g_commLCDDisp > 0 )
        	g_commLCDDisp--;
        
        if ( g_initDispCnt > 0 )
        {
        	g_initDispCnt--;
        	if ( g_initDispCnt == 0 )
        	{
        		g_cardDispCnt = LCD_CLEAR_CNT*2;
        	}
        }
        if ( g_cardDispCnt > 0 )   
        	g_cardDispCnt--;
        	
        if ( g_errLCDDispCnt > 0 )
        	g_errLCDDispCnt--;
        
        //����EEPROM��ȡ���� 1���Ӻ�λ���� 11
        if ( g_errStatus.Bit.E2ROM == 1 )
        {
        	g_eepromErrCnt++;
        	if ( g_eepromErrCnt > 60 )
        		ResetMeter();
        }
    
    //����ʹ�� 11
	#ifdef DEMAND_ENABLE
		if ( g_demandSlideCnt > 0 )
			g_demandSlideCnt--;
		if ( g_demandSlideCnt == 0 )	
		{
			g_demandSlideCnt = ((uint16)BCDToHex( (uchar *)&g_demand.DemandSlide, 1 ))*60;	
			g_meterFlag.Bit.DemandProcess = 1;
		}
	#endif
		
		
		
    }
    
    //��λ2���־λ 11
    s_twoSec++;
    if ( s_twoSec >= 5*2 )		//5s
    {
    	s_twoSec = 0;
    	g_meterFlag.Bit.OneLoopTask = 1;
    	g_meterFlag.Bit.Cl2208check = 1;	
    }
    
    //3����ʾ���� 11
    g_LCDSec++;
    if ( g_LCDSec >= g_lcdCycleCnt*2 )
    {
    	g_LCDSec = 0;
    	
    	g_meterFlag.Bit.CycleDisp = 1;
    	
    	//g_1200BPScNT = 0;	
    }
//if ( SINGLE_OR_THREE_METER==1 )    
//
//#else
//	
//		
//#endif
    
    //button delay time
    
    
    //��������ʱ���� 11
	if ( g_BeepDelay > 0 )
	{
	    g_BeepDelay--;
	    
	    if ( g_BeepDelay == 0)
	    {	        	        	            
	        if ( g_BeepCnt == 0 )
	        { 
	            SPEAKER_H(); 
	        } 
	        
	        if ( g_BeepCnt > 0 )
	        {
	            g_BeepCnt--;
	            
	            g_BeepDelay = BEEP_DELAY_1S;
	            SPEAKER_IO = ~SPEAKER_IO;    
	        }   
	            
	          
	    }   	    
	}
        
	//�̵���������ʱ 11
	//if( g_relayCnt > 0 )
    //{
    //    g_relayCnt--;
    //    
    //    if ( g_relayCnt == 0 ) 
    //    {
    //        RelayA_L;    
    //        RelayB_L;   
    //    }   
    //}
    
    if ( g_relayProCnt > 0 )
    	g_relayProCnt--;

	//�̵�����ʱ���� 11
	if ( g_relayChkDly > 0 )
	    g_relayChkDly--;
   
#if (IC_CARD_MODE==RF_CARD)  
   	//��Ƶ�������� 11
	s_rfCycleCnt++;
	if ( s_rfCycleCnt >= 1 )
	{
		g_rfFlag.Bit.Request = 1;
		s_rfCycleCnt = 0;		
	}
	
	if ( g_rfInFieldCnt > 0 )
	{
		g_rfInFieldCnt--;
		if ( g_rfInFieldCnt == 0 )
			g_meterFlag.Bit.RFInFieldFlag = 0;
	}
	
#endif

    //
	EI ( );
	
	return;
}



