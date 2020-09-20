/******************************************************************************************
*  Copyright (c) 2007, Bona RD Department 
*  All rights reserved 
*
*  FileName：   	Energy.c
*  Author：     	Roger Zou
*  Date：  			2010-3-8 20:11
*  Revised Date：  	
*  Function：       this file contains functions are used to measure active energy.
*  Version:			BonaV2.0
*  Note：        			
*********************************************************************************************/
#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

#include "macro.h"
#pragma interrupt INTP0	ISRForEnergy

#if(PCB_VERSION_NEW_4==0)
	#if(PCB_VERSION_NEW==0)
		#ifdef INACITVE_POWER_ENABLE
			#pragma interrupt INTP7	ISRForInactiveEnergy
		#endif
	#endif
#endif

//#pragma interrupt INTTM000 ISRForEnergy
//#pragma interrupt INTP2 ISRForEnergy

#define _ENERGY_DEFINE_
#include "include.h"

/*****************************************************************************************/
//需量使能 11
#ifdef DEMAND_ENABLE
	static uint32 s_lastActivePower[15]; 
	//static uint32 s_lastInactivePower[15]; 
	static PHASE_POS_FLAG s_dir;
#endif

static uchar s_pulseCnt;

#if(CALI_CHANGE_CARD_ENABLED==1)	
	static uchar s_coefCnt;
#endif
/*****************************************************************************************/

/*****************************************************************************************/



/*****************************************************************************************/

/*
*****************************************************************************************
* fuction name: InitEnergyPin
*
* Description : this function initializes the enegy pulse pin and direction pin
*
* Arguments   : none
*	
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void InitEnergyPin ( void )
{
//需量使能 11
#ifdef DEMAND_ENABLE
	uchar i;
#endif

#ifdef DEMAND_ENABLE	
	for ( i=0; i<15; i++ )
	{
		s_lastActivePower[i] = 0;
		//s_lastInactivePower[i] = 0;	
	}
#endif	

#if(CALI_CHANGE_CARD_ENABLED==1)	
	s_coefCnt = 0;	
#endif
			
	return;
}

/*
*****************************************************************************************
* fuction name: ISRForEnergy
*
* Description : this interrupt service function get the energy pulse
*
* Arguments   : none
*	
* returns     : none
*
* Notes     :
******************************************************************************************
*/
__interrupt void ISRForEnergy ( void )
{		
	DI ( );
	
	
	if ( PULSE == 1 )	//rising edge，脉冲计数开始,111
	{
	#if(METER_CHIP_MODEL==0x2208)
		PULSE_OFF();
	#endif
				
		g_energy.PulseCnt++;	
	
	#if(CALI_CHANGE_CARD_ENABLED==1)	
		if ( (g_RunCoef>0) && (g_RunCoef<=COEF_MAX) )
		{
			s_coefCnt += g_RunCoef;
			if ( s_coefCnt >= 100 )
			{
				s_coefCnt = 0;
				g_energy.PulseCnt++;
			}	
		}
	#endif
	   
	}
	else	//falling edge
	{
	#if(METER_CHIP_MODEL==0x2208)
	   //脉冲灯控制 11
	    PULSE_ON();
	    
	    g_pulseLedDly = PULSE_LED_WIDTH;
	   
	#endif
	    
	    
	    	    	    		
	}
			
	EI ( );
}
	
/*
*****************************************************************************************
* fuction name: ISRForInactiveEnergy
*
* Description : this interrupt service function get the energy pulse
*
* Arguments   : none
*	
* returns     : none
*
* Notes     :
******************************************************************************************
*/
#if(PCB_VERSION_NEW_4==0)
	#if(PCB_VERSION_NEW==0)
	__interrupt void ISRForInactiveEnergy ( void )
	{		
		DI ( );
		
	//无功电量计量 11
	#ifdef INACITVE_POWER_ENABLE	
		if ( Q_PULSE == 1 )	//rising edge，脉冲计数开始,111
		{
			
					
			g_Inactivenergy.InactivePulseCnt++;	
		    
		}
		else	//falling edge
		{
		    //脉冲灯控制 11
		    

		    	    	    		
		}
	#endif
				
		EI ( );
	}
	#endif
#endif
		
/*
*****************************************************************************************
* fuction name: ManageEnergy
*
* Description : this function manage this meter all enegy data
*
* Arguments   : none
*	
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ManageEnergy ( void )
{				
	uint32 tempL,tempL2;
	
	if ( (g_relayProCnt > 0) )
		return;

#if(KUAISU_TEST==1)	
	if ( g_energy.PulseCnt >= 1 )
	{
	    g_energy.PulseCnt -= 1;
	    
	    s_pulseCnt += 25;
	    g_energy.Accumulate += 25;
	    
	#ifdef TARIFF_ENABLE
	    if ( g_energy.TariffNum == 1 ) 
	    	g_energy.TariffQ1+= 25;
	    else if ( g_energy.TariffNum == 2 ) 
	    	g_energy.TariffQ2+= 25;
		else if ( g_energy.TariffNum == 3 ) 
	    	g_energy.TariffQ3+= 25;
	    else if ( g_energy.TariffNum == 4 ) 
	    	g_energy.TariffQ4+= 25;
	#endif    
	    g_energy.CurrentQ+= 25;
#else		 
	if ( g_energy.PulseCnt >= CONSTANT_FRACTION )
	{
	    g_energy.PulseCnt -= CONSTANT_FRACTION;
	    s_pulseCnt++;
	    g_energy.Accumulate++;
    
	    	    	    
	#ifdef TARIFF_ENABLE
	    if ( g_energy.TariffNum == 1 ) 
	    	g_energy.TariffQ1++;
	    else if ( g_energy.TariffNum == 2 ) 
	    	g_energy.TariffQ2++;
		else if ( g_energy.TariffNum == 3 ) 
	    	g_energy.TariffQ3++;
	    else if ( g_energy.TariffNum == 4 ) 
	    	g_energy.TariffQ4++;
	#endif    
	    g_energy.CurrentQ++;
#endif	
		
		//正负电能累计 11
		if ( g_dirFlag.Bit.RevPFlag )
	    {
	        g_energy.NegAccumulate++;    
	    }
	    else
	    {
	        g_energy.PosAccumulate++;    
	    }	
		
		//每度电保存一次 0.01 11
	    if ( s_pulseCnt >= 100 )
	    {	        	        
	        s_pulseCnt -= 100;
	       	SaveEnergy( NORMAL_SAVE );			        
	    }
				
    #ifdef PRE_PAID_ENABLE		
		//剩余金额先按4位小数扣费 11

		//考虑电压和电流变比 11
		if ( (g_para.CurrentRatio == 0) ||
			(g_para.CurrentRatio > 20000) )
		{
			tempL = 1;	
		}
		else
			tempL = (uint32)g_para.CurrentRatio;
		    
	    tempL2 = g_price * tempL; 
   	    
   	    tempL = tempL2 / 100; 
   	
   	#if(KUAISU_TEST==1)	
   		tempL *= 25;
   	#endif
   	
   	#ifdef BONUS_PADI_ENABLE    
   	    //先扣除赠送金额 11
   	    if ( g_energy.BonusBalance >= tempL )
   		{
   			g_energy.BonusBalance -= tempL;	   			
   		} 
   		else
   		{
   			//赠送金额大于0 11
   			if ( g_energy.BonusBalance > 0 )
	   		{
	   			tempL -= g_energy.BonusBalance;	
	   			g_energy.BonusBalance = 0;	   			
	   		}  
   	#endif
   			
   			//扣除购电剩余金额 11
   			if( g_energy.Balance >= tempL )
		    {
		        g_energy.Balance -= tempL;
		    }
		    else
		    {	        
		        if ( g_energy.Balance > 0 )
		        {	        	
		        	g_energy.Overdraft = tempL - g_energy.Balance;	
		        	g_energy.Balance = 0;
		        }
		        else
		        {
		        	g_energy.Overdraft += tempL;		
		        }
		    } 
	#ifdef BONUS_PADI_ENABLE   			
   		}   
   	#endif
   		  	     			    
		//判断电价是2位小数还是3位 4位小数 11
		if ( (tempL2%100) != 0 )	//3或4位小数 11 11
		{			
			g_energy.Fraction5And6 += (uchar)(tempL2%100);
			if ( g_energy.Fraction5And6 >= 100)
			{
				g_energy.Fraction5And6 -= 100;
			
			#ifdef BONUS_PADI_ENABLE	
				//先扣除赠送金额 11
		   	    if ( g_energy.BonusBalance > 0 )
		   		{
		   			g_energy.BonusBalance -= 1;	   			
		   		} 
				else
				{
			#endif
			
					if( g_energy.Balance > 0 )
					{
						g_energy.Balance -= 1;
					}
					else
					{
						g_energy.Overdraft += 1;	
					}	
			#ifdef BONUS_PADI_ENABLE					
				}	
			#endif
										
			}
		}					
	#endif
				    
    #ifdef LADDER_ENABLE	
		if ( (g_energy.CurrentQ%100) == 0 )
		{								
			g_meterFlag.Bit.LadderProcess = 1;
		}
	#endif	
	
		//增加判断继电器状态 11
		if ( (g_relayFlag.Bit.RelayOn == 0) &&
			(g_relayFlag.Bit.RelayNeedToOperate==0) &&
			(g_relayCnt==0) )
	   	{
	   		g_relayFlag.Bit.RelayOn = 1;
	   		g_errStatus.Bit.RelayErr = 1;	
	   	}
	   	else
	   	{
	   		g_errStatus.Bit.RelayErr = 0;		
	   	}          	
	}	
    
#ifdef INACITVE_POWER_ENABLE   
    //无功计量 11
    if ( g_Inactivenergy.InactivePulseCnt >= CONSTANT_FRACTION )	
    {
        g_Inactivenergy.InactivePulseCnt -= CONSTANT_FRACTION;    
        
        g_Inactivenergy.InactiveQuan++;
        
        //正负电能累计 11
		if ( g_dirFlag.Bit.RevQFlag )
	    {
	        g_Inactivenergy.NegInactiveAcc++;   
	#ifdef TARIFF_ENABLE
	    if ( g_energy.TariffNum == 1 ) 
	    	g_Inactivenergy.NegInactiveAcc1T1++;
	    else if ( g_energy.TariffNum == 2 ) 
	    	g_Inactivenergy.NegInactiveAcc1T2++;
		else if ( g_energy.TariffNum == 3 ) 
	    	g_Inactivenergy.NegInactiveAcc1T3++;
	    else if ( g_energy.TariffNum == 4 ) 
	    	g_Inactivenergy.NegInactiveAcc1T4++;
	#endif     
	    }
	    else
	    {
	        g_Inactivenergy.PosInactiveAcc++;  
	#ifdef TARIFF_ENABLE
	    if ( g_energy.TariffNum == 1 ) 
	    	g_Inactivenergy.PosInactiveAccT1++;
	    else if ( g_energy.TariffNum == 2 ) 
	    	g_Inactivenergy.PosInactiveAccT2++;
		else if ( g_energy.TariffNum == 3 ) 
	    	g_Inactivenergy.PosInactiveAccT3++;
	    else if ( g_energy.TariffNum == 4 ) 
	    	g_Inactivenergy.PosInactiveAccT4++;
	#endif       
	    }	
          
    }
#endif
    
    
             		
	return;
}

//需量使能 11
#ifdef DEMAND_ENABLE
/*
*****************************************************************************************
* fuction name: ManageDemand
*
* Description : this function manage this meter all demand data
*
* Arguments   : none
*	
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ManageDemand ( void )
{	
	uchar i,cnt,flag,tempChar;
	uint32 tempL,tempL2;
	uint16 addr;
		
	
	if ( g_meterFlag.Bit.DemandProcess == 0 )	
		return;
	g_meterFlag.Bit.DemandProcess = 0;
	
	if ( (g_demand.DemandSlide==0) ||
		(g_demand.DemandCycle==0) )
	{				
		g_demand.DemandCycle = 0x15;
		g_demand.DemandSlide = 1;
	}
	
	flag = 0;	
	cnt = (uchar)BCDToHex( (uchar *)&g_demand.DemandCycle, 1 );
	tempChar = (uchar)BCDToHex( (uchar *)&g_demand.DemandSlide, 1 );
	cnt /= tempChar;
	
	//有功需量处理 11
	if ( s_dir.Bit.ActiveDir != g_dirFlag.Bit.RevPFlag )
	{
		for ( i=0; i<cnt-1; i++ )
			s_lastActivePower[i] = 0;	
	}
	else
	{
		//先减去之前的滑差数据 11
		for ( i=0; i<cnt-1; i++ )
			s_lastActivePower[i] = s_lastActivePower[i+1];	
	}

#if(SINGLE_OR_THREE_METER==1)	
	s_lastActivePower[cnt-1] = BCDToHex( (uchar*)&g_uip.PRms, 4 );
#else
	s_lastActivePower[cnt-1] = BCDToHex( (uchar*)&g_uip[3].PRms, 4 );
#endif
	
	s_dir.Bit.ActiveDir = g_dirFlag.Bit.RevPFlag;
	tempL = 0;
	for ( i=0; i<cnt; i++ )
		tempL += s_lastActivePower[i];
	
	tempL /= cnt;		
	//反向有功需量 11	
	tempL2 = 0;
	if ( g_dirFlag.Bit.RevPFlag )
	{			
		g_demand.NegActive = tempL;	
		
		I2CRead ( EEPROM_ADDR,offsetof(EEPROM_DATA,CurDemand)+8, 
			(uchar*)&tempL2,3, 1 );
		tempL = BCDToHex( (uchar*)&tempL2, 3 );
		if ( g_demand.NegActive > tempL )
		{									
			flag = 2;	
		}
	}
	else	//正向有功需量 11	
	{
		g_demand.PosActive = tempL;	
		
		I2CRead ( EEPROM_ADDR,offsetof(EEPROM_DATA,CurDemand), 
			(uchar*)&tempL2,3, 1 );	
		tempL = BCDToHex( (uchar*)&tempL2, 3 );
		if ( g_demand.PosActive > tempL )
		{			
			flag = 1;	
		}
	}
		
	
#ifdef INACITVE_POWER_ENABLE
	//无功需量 11
	
	for ( i=0; i<cnt-1; i++ )
		s_lastInactivePower[i] = s_lastInactivePower[i+1];	
	
	if ( (g_dirFlag.Bit.RevQFlag==0) && (g_dirFlag.Bit.RevQFlag==0) )		//1phase
	{		
		if ( s_dir.Bit.InactiveDir != 1 )
		{
			s_dir.Bit.InactiveDir = 1;
			for ( i=0; i<cnt-1; i++ )
				s_lastInactivePower[i] = 0;	
		}			
	}
	else if ( (g_dirFlag.Bit.RevQFlag==1) && (g_dirFlag.Bit.RevQFlag==0) )		//2phase
	{		
		if ( s_dir.Bit.InactiveDir != 2 )
		{
			s_dir.Bit.InactiveDir = 2;
			for ( i=0; i<cnt-1; i++ )
				s_lastInactivePower[i] = 0;	
		}			
	}
	else if ( (g_dirFlag.Bit.RevQFlag==1) && (g_dirFlag.Bit.RevQFlag==1) )		//3phase
	{		
		if ( s_dir.Bit.InactiveDir != 3 )
		{
			s_dir.Bit.InactiveDir = 3;
			for ( i=0; i<cnt-1; i++ )
				s_lastInactivePower[i] = 0;	
		}			
	}
	else if ( (g_dirFlag.Bit.RevQFlag==0) && (g_dirFlag.Bit.RevQFlag==1) )		//4phase
	{		
		if ( s_dir.Bit.InactiveDir != 4 )
		{
			s_dir.Bit.InactiveDir = 4;
			for ( i=0; i<cnt-1; i++ )
				s_lastInactivePower[i] = 0;	
		}			
	}	
	s_lastInactivePower[cnt-1] = g_uip[3].QRms;
		
	tempL = 0;
	for ( i=0; i<cnt; i++ )
		tempL += s_lastInactivePower[i];
	
	tempL /= cnt;		
	
	if ( s_dir.Bit.InactiveDir == 1 )
	{
		g_demand.Q1Inactive = tempL;	
		I2CRead ( EEPROM_ADDR,offsetof(EEPROM_DATA,CurDemand)+8, 
			(uchar*)&tempL2, 4, 1 );
		tempL = BCDToHex( (uchar*)&tempL2, 4 );
		if ( g_demand.Q1Inactive > tempL)
		{
			flag = 3;	
		}	
	}
	else if ( s_dir.Bit.InactiveDir == 2 )
	{
		g_demand.Q2Inactive = tempL;	
		I2CRead ( EEPROM_ADDR,offsetof(EEPROM_DATA,CurDemand)+12, 
			(uchar*)&tempL2, 4, 1 );
		tempL = BCDToHex( (uchar*)&tempL2, 4 );
		if ( g_demand.Q2Inactive > tempL)
		{
			flag = 4;	
		}	
	}
	else if ( s_dir.Bit.InactiveDir == 3 )
	{
		g_demand.Q3Inactive = tempL;	
		I2CRead ( EEPROM_ADDR,offsetof(EEPROM_DATA,CurDemand)+16, 
			(uchar*)&tempL2, 4, 1 );
		tempL = BCDToHex( (uchar*)&tempL2, 4 );
		if ( g_demand.Q3Inactive > tempL)
		{
			flag = 5;	
		}	
	}
	else if ( s_dir.Bit.InactiveDir == 4 )
	{
		g_demand.Q4Inactive = tempL;	
		I2CRead ( EEPROM_ADDR,offsetof(EEPROM_DATA,CurDemand)+20, 
			(uchar*)&tempL2, 4, 1 );
		tempL = BCDToHex( (uchar*)&tempL2, 4 );
		if ( g_demand.Q4Inactive > tempL)
		{
			flag = 6;	
		}	
	}
#endif
	
	//保存数据 11
	if ( flag > 0 )
	{						
//		g_demand.Checksum = GetSum ( (uchar*)&g_demand.PosActive, Meter_Demand_Len-1 );
//		EEPROM_NOWP();
//	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,DemandPara),
//			(uchar*)&g_demand.PosActive,Meter_Demand_Len, 1  );
//		EEPROM_WP();
		
		addr = offsetof(EEPROM_DATA,CurDemand);
		if ( flag == 1 )
		{
			tempL = Hex32ToBCD( g_demand.PosActive );								
		}
		else if ( flag == 2 )
		{
			tempL = Hex32ToBCD( g_demand.NegActive );
						
			addr += 8;
		}
		
		g_buf[0] =  (uchar)(tempL&0xff);
		g_buf[1] =  (uchar)((tempL>>8)&0xff);
		g_buf[2] =  (uchar)((tempL>>16)&0xff);
		
		g_buf[3] = g_date.min;
		g_buf[4] = g_date.hour;
		g_buf[5] = g_date.day;
		g_buf[6] = g_date.month;
		g_buf[7] = g_date.year;
		
		EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, addr, (uchar*)&g_buf[0],8, 1  );
		EEPROM_WP();	
	}	
	
	return;		
}
#endif

/*
*****************************************************************************************
* fuction name: SaveEnergy
*
* Description : this function save  all enegy data
*
* Arguments   : none
*	
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void SaveEnergy ( uchar mode )
{
	if ( g_errStatus.Bit.E2ROM == 1 )
		return;
		
    DI();
    
    g_energy.Checksum = GetSum ( (uchar*)&g_energy.Accumulate, Meter_EQuan_Len-1 );     
    
    if ( mode == LOW_POWER_SAVE )
    {
    	//
	    EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Accumulate_Low),
			(uchar*)&g_energy.Accumulate,Meter_EQuan_Len, 1  );
		EEPROM_WP();
		
    #ifdef RTC_8025T	
		EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,DropDateTime),
			(uchar*)&g_date.sec, 7, 1  );
		EEPROM_WP();	
	#endif	
	
	#ifdef INACITVE_POWER_ENABLE	
		g_Inactivenergy.Checksum = GetSum ( (uchar*)&g_Inactivenergy.InactiveQuan, Meter_InactiveQuan_Len-1 );  
		EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,InactiveQuan_Low),
			(uchar*)&g_Inactivenergy.InactiveQuan, Meter_InactiveQuan_Len, 1  );
		EEPROM_WP();	
	#endif	
		
		g_meterFlag.Bit.PowerDown = 1;
    }
    else
    {
    	//
	    EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Accumulate),
			(uchar*)&g_energy.Accumulate,Meter_EQuan_Len, 1  );
		EEPROM_WP();
		
		//备份区数据 11
		EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Accumulate_B),
			(uchar*)&g_energy.Accumulate,Meter_EQuan_Len, 1  );
		EEPROM_WP();
	
	#ifdef INACITVE_POWER_ENABLE	
		g_Inactivenergy.Checksum = GetSum ( (uchar*)&g_Inactivenergy.InactiveQuan, Meter_InactiveQuan_Len-1 );  
		EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,InactiveQuan),
			(uchar*)&g_Inactivenergy.InactiveQuan, Meter_InactiveQuan_Len, 1  );
		EEPROM_WP();	
		
		//备份区数据 11
		EEPROM_NOWP();
	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,InactiveQuan_B),
			(uchar*)&g_Inactivenergy.InactiveQuan, Meter_InactiveQuan_Len, 1  );
		EEPROM_WP();
	#endif	
			
    }
    
	EI();

	return;
}

/*
*****************************************************************************************
* fuction name: ManageTariff
*
* Description : this function decide tariff every minute
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ManageTariff ( void )
{				
	uchar i,num,tariffTblNum;
	uint16 tempInt;

#ifdef TARIFF_ENABLE	
	
	
	if ( g_meterFlag.Bit.TariffProcess )	//judge every minute
	{								
		g_meterFlag.Bit.TariffProcess = 0;
		
		g_lcdFlag.Bit.TariffDisp = 0;
			
		//获取年时区 判断年时区 11
	    I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,YearZoneNum), 
	    	(uchar*)&g_buf[0], 16, 1  );        	
		if ( g_buf[15] != GetSum ( &g_buf[0], 15 ) )
	    {
			g_buf[0] = 1;
			g_buf[3] = 1;		//采用的时段表号 11
		}	    
		else
		{
			//年时区数判断 11
			if ( g_buf[0] == 0 )
				return;
				
			if ( (g_buf[0] > 4)  )
			{
				g_buf[0] = 1;		
			}	
			
			//时段表数判断 11
			if ( g_buf[1] == 0 )
				return;
			if ( (g_buf[1] > 8) )
			{
				g_buf[1] = 1;		
			}
			
			//日时段表数判断 11
			tariffTblNum = ((g_buf[2]>>4)&0x0f)*10+g_buf[2]&0x0f;		//BCD码 11
			if ( tariffTblNum == 0 )
				return;
			if ( (tariffTblNum > 14) )
			{
				tariffTblNum = 1;		
			}	
		}
				
		//根年时区表 获取 使用8个时段表的哪个 11
		for ( i=0; i<g_buf[0]; i++ )
		{
		    if ( ( (g_buf[3*i+5]==g_date.month) && (g_buf[3*i+4]>g_date.day) ) 
		            || (g_buf[3*i+5] > g_date.month) )
			{
				break;
			}	    		    		    
		}
		if ( i == 0 )
			i = g_buf[0];
		i -= 1;
		num = g_buf[3*i+3];     //采用的时段表号 11
		if ( (num > g_buf[1]) || (num==0) )
		{
			//g_energy.TariffNum = 4;				
			//return;	
			num = 1;		
		}			
		tempInt = ((uint16)(num-1))*48;     //偏移量 11		
		g_tariffOffset = tempInt;
				
		//获取费率时段表 11
		I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,HHMMNN1)+tempInt, 
	    	(uchar*)&g_buf[0], 48, 1  ); 
        //校验和判断 11
							
		for( i=0; i<tariffTblNum; i++ )
		{
			
			if ( ( g_buf[3*i+2] == g_date.hour && g_buf[3*i+1] > g_date.min) || 
				(g_buf[3*i+2] > g_date.hour) )
			{
				break;
			}						
		}
		
		if ( i == 0 )
			i = tariffTblNum;
		i -= 1;
				
		if ( g_energy.TariffNum != g_buf[3*i] )
		{
			g_energy.TariffNum = g_buf[3*i];
		}
		
		if ( (g_energy.TariffNum > 4) || (g_energy.TariffNum== 0) )	//4 tariffs
		{
		 	g_energy.TariffNum = 4;	
		 	//return;
		}				
	
		
		//获取当前单价 11
		if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,TariffPrice1),
			&g_buf[0],CHECK_ADDMOD,16 ) != 0 )
		{						
			return;			
		}
		
		//获取电价 11
		tempInt = ((uint16)g_buf[(g_energy.TariffNum-1)*2+1]<<8)| g_buf[(g_energy.TariffNum-1)*2];
	
		if ( (tempInt<MIN_PRICE) || (tempInt>MAX_PRICE) )
	    {	        	
            g_price = g_para.Price;
			return;
		}
			
		g_price = ((uint32)tempInt);
	
	    g_para.Price = (uint16)g_price;      //防止在processPrice函数中被改回来 11
	
			
	#ifdef LADDER_ENABLE	
		g_meterFlag.Bit.LadderProcess = 1;
		      
	#endif	
		
		g_lcdFlag.Bit.TariffDisp = 1;
	}	

#endif

	return;
}	

/*
*****************************************************************************************
* fuction name: ManageLadder
*
* Description : this function decide ladder price every energy update
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ManageLadder ( void )
{	
	uchar i;
	uint16 tempInt;
	

#ifdef LADDER_ENABLE		
	
	if ( g_meterFlag.Bit.LadderProcess == 0 )
		return;
	g_meterFlag.Bit.LadderProcess = 0;
	
	g_lcdFlag.Bit.LadderDisp = 0;
	
	//获取阶梯值表 11
	I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Ladder1), 
	    	(uchar*)&g_buf[0], 16, 1  );
	if ( g_buf[10] != GetSum ( &g_buf[0], 10) )
    {			
		return;			
	}	
									
	for( i=0; i<LADDER_VALUE_NUMER; i++ )
	{
		tempInt = ((uint16)g_buf[2*i+1])<<8 | g_buf[2*i]; 
		if ( g_energy.CurrentQ < (tempInt*100) )
		{
			 break;
		}						
	}
	
	//根据阶梯值位置，获取阶梯电价	 11				
	I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,LadderPrice1), 
	    	(uchar*)&g_buf[0], 16, 1  );
	if ( g_buf[12] != GetSum ( &g_buf[0], 12) )
    {			
		return;			
	}	
	tempInt = ((uint16)g_buf[2*i+1])<<8 | g_buf[2*i];
	
	if ( tempInt < MIN_PRICE )
		return; 
		
	if ( tempInt > MAX_PRICE || tempInt < MIN_PRICE )
		return; 
	
	tempInt += g_para.Price;
	if ( tempInt > MAX_PRICE || tempInt < MIN_PRICE )
		return;
		
	g_price = ((uint32)tempInt);
	g_lcdFlag.Bit.LadderDisp = 1;
#endif

	return;
}


