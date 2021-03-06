/******************************************************************************************
*  Copyright (c) 2010, Bona Design Department(Tai Yuan)
*  All rights reserved 
*
*  FileName：   	ICCard.c
*  Author：     	Bona Meter
*  Date：  			2010-3-8 20:10
*  Revised Date：  	
*  Main Function：  this file contains all the functions processing IC card.
*  Version:			BonaV2.0
*  Note： 			 			
*********************************************************************************************/


#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

//#pragma interrupt INTP4 ISRForCardIn


#define _ICCard_DEFINE_
#include "include.h"

//
//card type
/////////////////////////////////////////////////////////////////////////////////
#define CLEAR_ALL_CARD              (0x51)     //总清卡 11
#define MANUFATURE_SUPER_CARD       (0x52)     //生产设置卡 11
#define OUT_SUPER_CARD              (0x53)     //出厂设置卡 11
#define CHECK_RELAY_CARD            (0x54)     //继电器检测卡 11
#define READ_BACK_CARD              (0x55)     //抄表卡 11
#define USER_CARD                   (0x56)     //用户购电卡 11
#define CLEAR_ZONE_CARD             (0x57)     //分清卡 11
#define DATE_SET_CARD             	(0x58)     //时间设置卡 11
#define CLEAR_CALIDATA_CARD        	(0x59)     //校表参数初始化卡11 
#define RETURN_MONEY_CARD        	(0x5a)     //退费卡11
#define METER_SET_CARD        		(0x5b)     //表号设置卡11
#define FORCE_RELAY_CARD        	(0x5D)     //强制跳闸和闸卡 11
#define SINGLE_CALI_CARD        	(0x5C)     //单点校正卡 11
#define MONEY_GIVEN_CARD        	(0x5E)     //补助充值卡 11
#define CALI_CHANGE_CARD        	(0x5F)     //误差快慢修正卡 11
/////////////////////////////////////////////////////////////////////////////////

//卡处理出错报警
//////////////////////////////////////////////////////////////////////////////////
//card process error status byte
#define CARD_NOT_CONSISTENT 		(0x01)		//不是合法卡 11
#define CARD_FRAME_ERR 				(0x02)		//卡上数据格式错 11
#define CARD_ZONE_ERR 				(0x04)		//分区号错 11
#define CARD_BUYCNT_ERR 			(0x08)		//购电次数不符 11

#define CARD_OVERLIMIT_ERR 			(0x10)		//超过囤积量 11
#define CARD_CARDID_ERR 			(0x20)		//卡号不符 11
#define CARD_USED_ERR 				(0x40)		//卡已使用过 11
#define CARD_STATUS_ERR 			(0x80)		//状态不符 11

#define CARD_CARDTYPE_ERR 			(0x81)		//不存在此卡类型 11
#define READ_CARD_ERR 				(0x82)		//读卡上数据失败 11

#define BUY_CARD_OPEN_ERR 			(0x51)		//购电卡开户卡状态错 11
#define BUY_CARD_NUM_ERR 			(0x52)		//购电卡购电量错 11

#define UPDATE_EEPROM_ERR 			(0x53)		//更新EEPOM出错 11
#define OVER_READNUM_ERR 			(0x54)		//超出最大抄表数量 11

//added by Roger 2015/07/24
#define CALIBRATE_INIT_ERR 			(0x55)		//校表参数卡出错 11
#define UPDATE_MOMONEY_ERR 			(0x56)		//退费卡写卡出错 11

#define RESET_CALI_CHIP_ERR 			(0x57)		//初始化计量芯片卡出错 11
//#define WRITE_BACK_ERR 			    (0x58)		//返写卡出错 11

#define FORCE_CARD_STATUS_ERR		(0x61)		//强制跳闸卡状态不符 11
#define UNAUTH_CARD_ERR				(0x62)		//非法卡 11

#define CALI_PRO_ERR				(0x71)		//校表失败 11
#define COEF_ERR					(0x72)

#define JIA_CE_TEST_ERR		(0x99)		//佳测限值到期 11
//////////////////////////////////////////////////////////////////////////////////
#if(IC_CARD_MODE==AT24_CARD)
/******************************************************************************************/




//added by Roger 2015/07/24
/////////////////////////////////////////////////////////////////
//static uchar s_cardInFlag;		//1字节，售电卡正确插入标识 11
static uint16 s_cardInCnt;		//卡插入时间 11
////////////////////////////////////////////////////////////////

#if(METER_CHIP_MODEL==0x2208)
	extern const uint16 CALIBRATE_TABLE_value[];	
#endif
/******************************************************************************************/

//function statement
/******************************************************************************************/





static uchar CheckDataFrame ( uchar *buf );


static uchar SetParaFromCard ( void );
static uchar ProcessReadbackCard ( void );


static uchar ProcessRetrunBack ( void );  
#ifdef BONUS_PADI_ENABLE 
	static uchar ProcessBonusRetrunBack ( uint32 temp32, uint16 temp16 );  
#endif 


                     
/******************************************************************************************/
/*
*****************************************************************************************
* fuction name: InitICCarVariables
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
#ifdef PRE_PAID_ENABLE
void InitICCarVariables ( void )
{
    //卡状态判断 11
	if ( IC_CHK )
    {
        g_cardFlag.Bit.ICCardOutUse = 1; 
        g_cardFlag.Bit.ICCardInUse = 0;   
        g_LCDFrame = 9;       
    }
	else
    {
        g_cardFlag.Bit.ICCardOutUse = 0; 
        g_cardFlag.Bit.ICCardInUse = 1;     
        g_cardFlag.Bit.ICCardIn = 1; 
    }
    
        
    s_cardInCnt = 0;
}
#endif	



/*
*****************************************************************************************
* fuction name: CheckDataFrame
*
* Description : 
*
* Arguments   : 1)uchar *buf : the data need to encrypt
*		2)uchar pos: the location in IC card
*		3)uchar len: the lenght of data need to encrypt
*
* returns     : null 
*
* Notes       :
******************************************************************************************
*/

static uchar CheckDataFrame ( uchar *buf )
{                
    uchar temp;
    
    if ( (buf[0] != 0xa6) || (buf[15] != 0x3a) )
    {
    	return 1; 	
    }
    
    temp = GetSum ( buf, 14 );
    if ( temp != buf[14] )
    	return 1;
    
    return 0;
}

/*
*****************************************************************************************
* fuction name: ManageICCard
*
* Description : this function process IC card when detect IC card plug in
*
* Arguments   : none
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/

void ManageICCard ( void )
{						
       
    uint16 tempInt16;
    uint32 tempInt32,tmp32_2;
    uchar i,cardType,meterMode,addr[6],judgeFlag;
    uchar *pbuf;
        				
	if ( g_cardFlag.Bit.ICCardIn == 0 )
	    return;
	g_cardFlag.Bit.ICCardIn = 0;
	s_cardInCnt = 0;
	
	g_initDispCnt = LCD_CLEAR_CNT;
	DisplayCardIn ( );
		
	FeedWatchdog ( );	    
	
	IC_POWER_ON; 
	Delay1MsTime(1);
	
	//LED display set	
	//g_meterFlag.Bit.CardAndPowerupDisp = 1;	
	g_meterFlag.Bit.CycleDisp = 1;
	g_LCDSec = 0;
	g_LCDFrame = 0;		//插卡显示 11
	
	for ( i=0; i<16; i++ )
		g_buf[i] = 0;	
	//卡错误状态置0 11
    g_cardStatus = 0;
    
    //获取卡上128字节数据 11
    pbuf = (uchar*)&g_commBuf[0].Data[0];  
    CardI2CRead ( CARD_ADDR, 0X00, (uchar*)pbuf, 64 );
    //pbuf = (uchar*)&g_commBuf[1].Data[0];
    //CardI2CRead ( CARD_ADDR, 0X40, (uchar*)pbuf, 64 );
    
    
    //判断0x00处，判断卡类型 11	
   	pbuf = (uchar*)&g_commBuf[0].Data[0];   
	Decrypt ( pbuf, 0x00, 16, DECRYPT_KEY );   
    if ( CheckDataFrame (pbuf) != 0 )
    {
    	//g_cardStatus = CARD_NOT_CONSISTENT;		//非法卡 11
    	g_cardStatus = UNAUTH_CARD_ERR;
    	goto  end_ic_process;		
    }  
    cardType = pbuf[1];
    
    
    
	//首先判断0x10处，是否为本地区发行卡、卡号是否相符、电表模式 11		
	//CardI2CRead ( CARD_ADDR, 0X10, (uchar*)g_buf, 16 ); 
	pbuf = (uchar*)&g_commBuf[0].Data[0+0x10];    
	Decrypt ( pbuf, 0x10, 16, DECRYPT_KEY );    
    if ( CheckDataFrame (pbuf) != 0 )
    {
    	//g_cardStatus = CARD_NOT_CONSISTENT;		//非法卡 11
    	g_cardStatus = 0x61;
    	goto  end_ic_process;		
    }
    
    //功能卡判断  11
    if ( (cardType==CLEAR_ALL_CARD) || 		//总清卡 11
    	(cardType==MANUFATURE_SUPER_CARD) ||  //生产超级卡 11
    	(cardType==OUT_SUPER_CARD) || 		//出厂超级卡 11
    	(cardType==CHECK_RELAY_CARD) ||		//继电器测试卡 11
    	(cardType==DATE_SET_CARD) )		//时间设置卡 revised by Roger 2015/07/24
    {
	    //分区号判断 11
	    tempInt16 = ((uint16)pbuf[1]<<8) + pbuf[2];
	    if ( tempInt16 != 0x9999 )
	    {
	    	g_cardStatus = CARD_ZONE_ERR;		//分区号不符 11
	    	goto  end_ic_process;			
	    }   
	    //卡号判断 11
	    for ( i=0; i<6; i++ )
	    {
	    	if (  pbuf[8-i] != 0x99 )
	    	{
		    	g_cardStatus = CARD_CARDID_ERR;		//卡号不符 11
		    	goto  end_ic_process;		
		    }
		}	
    }
    
       
    //购电卡和退费卡需要判断卡号 11
   	if ( 	(cardType==USER_CARD) ||		//购电卡 11
   				(cardType==RETURN_MONEY_CARD) )		//退费卡 11
   	{
   		if ( (g_buyPara.BuyCount>1) ||
   			(g_buyPara.Status==USER_STATUS) ) 
        {
        	//表中卡号判断 11
	        for ( i=0; i<6; i++ )
	        {
	        	if ( g_addr.CardNum[i] != pbuf[3+i] )
	        	{
	        		g_cardStatus = CARD_CARDID_ERR;		//卡号不符 11
		    		goto  end_ic_process;		
	        	}
	        	
	        }	
        }  		   
   	}  
    
    //判断分区号 11
    //只有购电卡和分清卡需判断电表分区号 11 
    //退费卡也需判断分区号 11
    if ( (cardType==USER_CARD) ||		//购电卡 11
    		(cardType==CLEAR_ZONE_CARD) || 	//分清卡 11
    		(cardType==RETURN_MONEY_CARD) || 	//退费卡 11
    		(cardType==FORCE_RELAY_CARD) ||		//强制跳闸卡 11
    	 	(cardType==CALI_CHANGE_CARD) )		//误差调校卡 11
   { 		
    	judgeFlag = 1;
    	if ( (cardType==USER_CARD) ||		//购电卡 11    		
    			(cardType==RETURN_MONEY_CARD) ) 	//退费卡 11
    	{
    		//需增加判断购电次数<1的情况不判断 11
    		if ( g_buyPara.BuyCount < 1 )	
    			judgeFlag = 0;
    	}
    	
    	if ( judgeFlag > 0 )
    	{
    		//判断表分区号 11
	        pbuf = (uchar*)&g_commBuf[0].Data[0+0x10];
	        tempInt16 = (((uint16)pbuf[1]<<8)|(uint16)pbuf[2]);
	    	if ( g_addr.Zone != tempInt16 )
	    	{
	            g_cardStatus = CARD_ZONE_ERR;  
	            goto  end_ic_process;                    
	    	}	
    	}    	
    }
    
     
    switch ( cardType )
    {
    case CLEAR_ALL_CARD:    //总清卡  11 
    {        
        DisplayClear ( ); 
        g_initDispCnt = LCD_CLEAR_CNT;
        
        pbuf = (uchar*)&g_commBuf[0].Data[0]; 
        tmp32_2 = ((uint32)pbuf[2]<<24) + ((uint32)pbuf[3]<<16) + ((uint32)pbuf[4]<<8)+ pbuf[5];                
        ClearAllData ( tmp32_2, 10000 );																									
	#ifdef PRE_PAID_ENABLE		
		ProcessPrice( );
	#endif	
        break;
    }
    
    case MANUFATURE_SUPER_CARD: //生产超级卡 11
    {
        if ( g_buyPara.Status == MANUFACTURE_STATUS )      
        {
            g_cardStatus = SetParaFromCard ( );
            pbuf = (uchar*)&g_energy.Accumulate;
	    	for ( i=0; i<Meter_EQuan_Len; i++ ) 
	        	*pbuf++ = 0;
            pbuf = (uchar*)&g_commBuf[0].Data[0]; 
            tmp32_2 = ((uint32)pbuf[10]<<24) + ((uint32)pbuf[11]<<16) + ((uint32)pbuf[12]<<8)+ pbuf[13];  	
            g_energy.Balance = tmp32_2; 
            
            //added by Roger 2015/07/24
            g_para.MeterTpye &= ~BIT0; 	  //设置本地售电模式 保电解除 11
            
       	#ifdef PRE_PAID_ENABLE		
			ProcessPrice( );
		#endif	 
            SaveEnergy ( NORMAL_SAVE );        
        }
        else
        	g_cardStatus = CARD_STATUS_ERR;
        break;
    }
    case OUT_SUPER_CARD:    //出厂超级卡 11
    {
        //if ( g_buyPara.Status != MANUFACTURE_STATUS )
        if ( g_buyPara.Status == USER_STATUS )  //revised by Roger 2015/07/22
        {
        	g_cardStatus = CARD_STATUS_ERR;
        	break;
        }   
        if ( g_buyPara.Status == BACK_MONEY_STATUS )  //revised by Roger 2015/07/24
        {
        	g_cardStatus = CARD_STATUS_ERR;
        	break;
        }              		
        g_cardStatus = SetParaFromCard ( ); 
         
        pbuf = (uchar*)&g_energy.Accumulate;
    	for ( i=0; i<Meter_EQuan_Len; i++ ) 
        	*pbuf++ = 0;
        
        pbuf = (uchar*)&g_commBuf[0].Data[0]; 
        tmp32_2 = ((uint32)pbuf[10]<<24) + ((uint32)pbuf[11]<<16) + ((uint32)pbuf[12]<<8)+ pbuf[13];  	
        g_energy.Balance = tmp32_2; 
        g_energy.PrePaid = tmp32_2;
        
        //added by Roger 2015/07/24
      	g_para.MeterTpye &= ~BIT0; //设置本地售电模式 保电解除 11
             
    #ifdef PRE_PAID_ENABLE		
		ProcessPrice( );
	#endif	
        SaveEnergy ( NORMAL_SAVE );                                         
        break;
   	}
    case CHECK_RELAY_CARD:  //继电器检测卡  11  
    {                                  
        //g_relayFlag.Bit.RelayInTest = 1;
        g_relayFlag.Bit.RelayInTest = ~g_relayFlag.Bit.RelayInTest;
        if ( g_relayFlag.Bit.RelayInTest )        
            g_5minutesCnt = FIVE_MININUTES_CNT; 
        else
            g_5minutesCnt = 0;       
        g_relayFlag.Bit.RelayNeedToOperate = 1;                             
        break;
   	}
    case READ_BACK_CARD:    //抄表卡 11
        ProcessReadbackCard (  );
        break; 
    case USER_CARD:         //用户卡 11 
    {           
        //表状态判断 11
        if ( g_buyPara.Status == MANUFACTURE_STATUS )
        {
            g_cardStatus = CARD_STATUS_ERR;
        	break;    
        }  
        if ( g_buyPara.Status == BACK_MONEY_STATUS )  //revised by Roger 2015/07/24
        {
        	g_cardStatus = CARD_STATUS_ERR;
        	break;
        } 
        FeedWatchdog  (  );   
                     
        //g_cardStatus = ProcessUserCard (  ); 
        
        //0x00处参数  11
        //户号 11
        for ( i=0; i<6; i++ )
        {
        	g_commBuf[1].Data[6+i] =  g_commBuf[0].Data[0x13+i];
        }  
        //购电金额 11
        g_commBuf[1].Data[12] = g_commBuf[0].Data[13]; 
        g_commBuf[1].Data[13] = g_commBuf[0].Data[12]; 
        g_commBuf[1].Data[14] = g_commBuf[0].Data[11]; 
        g_commBuf[1].Data[15] = g_commBuf[0].Data[10]; 
       	//购电次数 11         
		g_commBuf[1].Data[16] = g_commBuf[0].Data[3]; 
		g_commBuf[1].Data[17] = g_commBuf[0].Data[2];   
		
		pbuf = &g_commBuf[1].Data[0]; 
		if ( (g_commBuf[0].Data[4]&0x80) == 0x80 )
			meterMode = BUY_RETURN_MODE;
		else
			meterMode = NORMAL_MODE;	
    	//更新参数 11
    	FeedWatchdog  (  );
		g_cardStatus = SetParaFromCard ( );
    
    #ifdef PRE_PAID_ENABLE		
    	g_cardStatus |= ProcessUserBuyQuan ( pbuf, meterMode, 1 ); 
                				
    	
		ProcessPrice( );
	#endif	         
        break;
   	}
   	
    case CLEAR_ZONE_CARD:   //分清卡 11 
   	{        
        
    	
    #ifdef TARIFF_ENABLE
    	pbuf = (uchar*)&g_energy.Accumulate; 
    	for( i=0; i<24; i++ )
    		g_commBuf[0].Data[32+i]	= pbuf[i]; 
    #else
    	tempInt32 = g_energy.Accumulate;
    #endif
    
    	tempInt16 = g_addr.Zone;
    	for( i=0; i<6; i++ )
    		addr[i] = g_addr.MeterAddr[i];
    	
    	DisplayClear ( ); 
        g_initDispCnt = LCD_CLEAR_CNT;
    	pbuf = (uchar*)&g_commBuf[0].Data[0]; 
        tmp32_2 = ((uint32)pbuf[2]<<24) + ((uint32)pbuf[3]<<16) + ((uint32)pbuf[4]<<8)+ pbuf[5];                
        ClearAllData ( tmp32_2, 10000 );
        	        
        FeedWatchdog  (  );
        g_buyPara.Status = OUT_STATUS;
    
    #ifdef TARIFF_ENABLE
    	pbuf = (uchar*)&g_energy.Accumulate; 
    	for( i=0; i<24; i++ )
    		pbuf[i] = g_commBuf[0].Data[32+i]; 
    	g_energy.PosAccumulate = g_energy.Accumulate;
    #else
    	g_energy.Accumulate = tempInt32;
        g_energy.PosAccumulate = tempInt32;
    #endif   
                
        g_energy.PrePaid = tmp32_2;
        g_addr.Zone = tempInt16;
        for( i=0; i<6; i++ )
    		g_addr.MeterAddr[i] = addr[i];
        
        //added by Roger 2015/07/24
        g_para.MeterTpye &= ~BIT0;  //设置本地售电模式 保电解除 11
        
        SaveEnergy ( NORMAL_SAVE ); 
          
    	g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
	    FeedWatchdog  (  );
	    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
				(uchar*)&g_addr.Zone,Meter_Addr_Len ) != 0 )
			g_cardStatus = UPDATE_EEPROM_ERR;	
			
		g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
	    FeedWatchdog  (  );
	    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
				(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len ) != 0 )
			g_cardStatus = UPDATE_EEPROM_ERR;
			
	#ifdef PRE_PAID_ENABLE		
		ProcessPrice( );
	#endif	
		
        break; 
    }
    
    case DATE_SET_CARD:   //时间设置卡 11
    {
    #ifdef RTC_8025T 
    	pbuf = (uchar*)&g_commBuf[0].Data[0];
    	
    	SetTime ( pbuf );
    	
    	g_date.year = pbuf[2];
    	g_date.month = pbuf[3];
    	g_date.day = pbuf[4];
    	
    	g_date.hour = pbuf[5];
    	g_date.min = pbuf[6];
    	g_date.sec = pbuf[7];
    #endif	
    	
    	break;
	}
	
	#if(METER_CHIP_MODEL==0x2208)
	case CLEAR_CALIDATA_CARD:	//校表参数初始化卡 11
	{
    	if ( g_buyPara.Status == USER_STATUS ||
        	g_buyPara.Status == RETURN_MONEY_CARD )
    	{
            g_cardStatus = CARD_STATUS_ERR;
        	break;    
        }
    	
    	g_initDispCnt = LCD_CLEAR_CNT;
		DisplayCali ( 0 );
			
    	CopyRamData ( (uchar*)&g_cl2208para.pstart, (uchar*)&CALIBRATE_TABLE_value[0],CALIREG_NUM*2 );
					
		g_cl2208para.Reserved = 0;
		g_factor.UFactor = U_FACTOR;
		g_factor.IFactor = IL_FACTOR;
		g_factor.PFactor = P_FACTOR;
		g_factor.Reserved[0] = 0;
		g_factor.Reserved[1] = 0;
		g_factor.Reserved[2] = 0;
		
		
		//FeedWatchdog (  );	
		SoftResetCl2208 ( );
		FeedWatchdog (  );
		CalibrateCl2208 ( );
    	break;	
    }
    	//校正卡 11
    case SINGLE_CALI_CARD:
    {
    	g_commBuf[0].Data[1] = 0x00;
    	g_commBuf[0].Data[2] = 0x22;
    	g_commBuf[0].Data[3] = 0x00;
    	g_commBuf[0].Data[4] = 0x00;		//220.000V
    	
    	g_commBuf[0].Data[5] = 0x00;
    	g_commBuf[0].Data[6] = 0x00;
    	g_commBuf[0].Data[7] = 0x50;
    	g_commBuf[0].Data[8] = 0x00;		//005.000A
    	    	   	
    	if ( g_commBuf[0].Data[0] == 1 )	//100%Ib，1.0(L) 11
    	{
    		g_initDispCnt = LCD_CLEAR_CNT;
			DisplayCali ( 1 );
			
			g_commBuf[0].Data[9] = 0x01;
	    	g_commBuf[0].Data[10] = 0x10;
	    	g_commBuf[0].Data[11] = 0x00;
	    	g_commBuf[0].Data[12] = 0x00;		//01.100000	KW					
			if ( CaliGainPara ( &g_commBuf[0].Data[1] ) != 0 )
			{
				g_cardStatus = CALI_PRO_ERR;
        		break;
			}	
    	}
    	else if ( g_commBuf[0].Data[0] == 2 ) //100%Ib，0.5L(L) 111
    	{
    		g_initDispCnt = LCD_CLEAR_CNT;
			DisplayCali ( 2 );
			
			g_commBuf[0].Data[9] = 0x00;
	    	g_commBuf[0].Data[10] = 0x55;
	    	g_commBuf[0].Data[11] = 0x00;
	    	g_commBuf[0].Data[12] = 0x00;		//00.550000	KW	
	    	
			if ( CaliPhasePara ( &g_commBuf[0].Data[1] ) != 0 )
			{
				g_cardStatus = CALI_PRO_ERR;
        		break;
			}	
    	}
		else if ( g_commBuf[0].Data[0] == 3 ) //5%Ib，1.0(L) 11
		{
			g_initDispCnt = LCD_CLEAR_CNT;
			DisplayCali ( 3 );
			
			g_commBuf[0].Data[5] = 0x00;
    		g_commBuf[0].Data[6] = 0x00;
    		g_commBuf[0].Data[7] = 0x02;
    		g_commBuf[0].Data[8] = 0x50;		//000.250A
    	
			g_commBuf[0].Data[9] = 0x00;
	    	g_commBuf[0].Data[10] = 0x05;
	    	g_commBuf[0].Data[11] = 0x50;
	    	g_commBuf[0].Data[12] = 0x00;		//00.055000	KW	
	    	
			if ( CaliOffsetPara ( &g_commBuf[0].Data[1] ) != 0 )
			{
				g_cardStatus = CALI_PRO_ERR;
        		break;
			}	
		}
    	break;
    }
	#else if (METER_CHIP_MODEL==0x8302)    
    case CLEAR_CALIDATA_CARD:	//校表参数初始化卡 11
    {
    	//表状态判断 11
        if ( g_buyPara.Status == USER_STATUS ||
        	g_buyPara.Status == RETURN_MONEY_CARD )
        {
            g_cardStatus = CARD_STATUS_ERR;
        	break;    
        }
    	
    	g_initDispCnt = LCD_CLEAR_CNT;
		DisplayCali ( 0 );
			
    	//CopyRamData ( (uchar*)&g_cl2208para.pstart, (uchar*)&CALIBRATE_TABLE_value[0],sizeof(CALI_REG_ORDER)*2 );
		pbuf = (uchar*)&g_rn8302para.gsua;
		for ( i=0; i<32; i++ )
		{
			*pbuf++ = 0;		
		}	
		//软复位 11
		if ( Write_Register_CMD(SOFTRESET, RESET_CMD) != 0 )
		{
            g_cardStatus = RESET_CALI_CHIP_ERR;
        	break;    
        }
		FeedWatchdog ( );
		//延时20ms 11
		Delay1MsTime( 20 );
		FeedWatchdog ( );
		
		if ( StoreCaliPara ( ) != 0 )
		{
			g_cardStatus = CALIBRATE_INIT_ERR;	//revised by Roger 2015/07/24
			break;
		}
										
		FeedWatchdog (  );
		SetRN8302_Register ( );
    	break;	
    }	
    	//校正卡 11
    case SINGLE_CALI_CARD:
    {
    	if ( g_commBuf[0].Data[0] == 1 )	//100%Ib，1.0(L) 11
    	{
    		g_initDispCnt = LCD_CLEAR_CNT;
			DisplayCali ( 1 );
		
			if ( CaliGainPara ( ) != 0 )
			{
				g_cardStatus = CALI_PRO_ERR;
        		break;
			}	
    	}
    	else if ( g_commBuf[0].Data[0] == 2 ) //100%Ib，0.5L(L) 111
    	{
    		g_initDispCnt = LCD_CLEAR_CNT;
			DisplayCali ( 2 );
			
			if ( CaliPhasePara ( ) != 0 )
			{
				g_cardStatus = CALI_PRO_ERR;
        		break;
			}	
    	}
		else if ( g_commBuf[0].Data[0] == 3 ) //5%Ib，1.0(L) 11
		{
			g_initDispCnt = LCD_CLEAR_CNT;
			DisplayCali ( 3 );
			
			if ( CaliOffsetPara ( ) != 0 )
			{
				g_cardStatus = CALI_PRO_ERR;
        		break;
			}	
		}
    	break;
    }
	#endif
    
    //added by Roger 2015/07/24	
    case RETURN_MONEY_CARD:	//退费卡 11
    {
    	//表状态判断 11
        if ( g_buyPara.Status != USER_STATUS )
        {
            g_cardStatus = CARD_STATUS_ERR;
        	break;    
        }
    	
    	
    	 
	    if ( g_cardStatus != 0 ) 
	    	break;
	    
	        
    	//返写退费返写信息A0~AF 11
	    if ( ProcessRetrunBack ( ) != 0 )
	    {
	        g_cardStatus = UPDATE_MOMONEY_ERR;	
	        break;
	    }
	    
	    //处理表内剩余金额及状态 跳闸、报警灯亮、液晶常显--TF--标识 11
	    //表计处于退费状态，可以插卡插卡退费前剩余金额 11
	    //表计此时只认分清卡、总清卡，重新开户才能用,不认购电卡、生产、出厂超级卡11
	    g_buyPara.Status = BACK_MONEY_STATUS;
	    //更新EEPROM 11
    	g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
    	if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
			(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len ) != 0 )
		{
			g_cardStatus = UPDATE_MOMONEY_ERR;	
	        break;	
		}
	    
    	break;
    }
    
    //added by Roger 2015/10/17	
    case METER_SET_CARD:	//表号设置卡 11   
    {
		pbuf = (uchar*)&g_commBuf[0].Data[0];   
		
		
		for ( i=0; i<6; i++ )
			g_addr.MeterAddr[i] = pbuf[7-i];	
		
		g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
		StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
				(uchar*)&g_addr.Zone,Meter_Addr_Len );
					    	
    	break;
    }	
    
    //强制跳闸和闸卡 11
    case FORCE_RELAY_CARD:
    {
    	pbuf = (uchar*)&g_commBuf[0].Data[0];  
    	if ( pbuf[2] == 0x1B ) //强制跳闸 11
		{
			if ( (g_para.MeterTpye&BIT0) == BIT0 )		//保电模式 11
			{
				g_cardStatus = FORCE_CARD_STATUS_ERR;
				break;	
			}	
			
			g_relayFlag.Bit.RelayOn = 1;
           	g_relayFlag.Bit.RelayNeedToOperate = 1;
           		           		
           	g_para.MeterTpye |= BIT1;  //设置远程拉闸 11
	      	g_meterStatus3.Bit.RelayCmdStatus = 1;
	           	
		}
    	else if( pbuf[2] == 0x2B ) //跳闸取消 11
		{		 
           	g_para.MeterTpye &= ~BIT1;  //取消远程拉闸 11	
				
           	g_meterStatus3.Bit.RelayCmdStatus = 0;
		}
    	else if( pbuf[2] == 0x1A ) //保电 11
		{
			g_relayFlag.Bit.RelayOn = 0;				
           	g_relayFlag.Bit.RelayNeedToOperate = 1;  //执行合闸动作 11
           		
           	g_para.MeterTpye |= BIT0;  //设置保电模式 11 
           	
	      	g_meterStatus3.Bit.KeepRelay = 1; 
	           	
	      	g_para.MeterTpye &= ~BIT1;  //取消远程拉闸 11	
	       	g_meterStatus3.Bit.RelayCmdStatus = 0;          	          	
		}
		else if( pbuf[2] == 0x2A ) //保电解除 11
		{
			g_para.MeterTpye &= ~BIT0; 	
			g_meterStatus3.Bit.KeepRelay = 0;			
		}
		    	
    	g_para.Checksum = GetSum ( (uchar*)&g_para.LimitPower, Meter_Para_Len-1 );
       	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LimitPower),
			(uchar*)&g_para.LimitPower,Meter_Para_Len );
				
    	break;
    }
    
    //补助金额充值卡 11
    case MONEY_GIVEN_CARD:
    {
    	if ( g_buyPara.Status != USER_STATUS ) 
    	{
    		g_cardStatus = CARD_STATUS_ERR;
    		break;	
    	}
    		
    	pbuf = (uchar*)&g_commBuf[0].Data[0];
    	//用户编号判断 11
		if ( g_buyPara.BuyCount > 0 ) //已发生过购电 11
		{						
			judgeFlag = BIT0+BIT1;
			for ( i=0; i<6; i++ )
	        {
	        	if ( pbuf[2+i] != 0x99 )
	        	{
	        		judgeFlag &= ~BIT0;	
	        	}
	        	
	        	if ( (g_addr.CardNum[i] != pbuf[6+i]) )
	        	{
	        		judgeFlag &= ~BIT1;		        				
	        	} 	 	
	        }	
	        
	        if ( judgeFlag == 0 ) 
	        {
	        	g_cardStatus = CARD_CARDID_ERR;
				break;	
			}		       	        	  				
		}
		
		//补助金额处理 11
		//获取补助次数 11
    	if ( pbuf[13] > g_energy.BonusBuyCount )
    	{
    		//获取补助金额 11
    		tempInt32 = pbuf[8]; 
			tempInt32 = (tempInt32<<8) | pbuf[9]; 
			tempInt32 = (tempInt32<<8) | pbuf[10]; 
			tempInt32 = (tempInt32<<8) | pbuf[11];	
				
    		if ( (pbuf[12]&BIT0) == BIT0 )	//结算日累计 11
    		{   			
    			tempInt32 += g_energy.BonusBalance;		
    		}
    		
			//补助次数 11
			tempInt16 = pbuf[13];	
    	}
    	else
    	{
    		//获取补助金额 11
    		tempInt32 = g_energy.BonusBalance; 
			//补助次数 11
			tempInt16 = g_energy.BonusBuyCount;	
    	}
    
    #ifdef BONUS_PADI_ENABLE	
		g_cardStatus = ProcessBonusRetrunBack ( tempInt32,  tempInt16 ); 
		if ( g_cardStatus != 0 )
			break;	
    #endif
    			
    	g_energy.BonusBuyCount = pbuf[13];    		   		    	
    	g_energy.BonusMode = pbuf[12];  			
    	g_energy.BonusBalance = tempInt32;		

    	if ( (pbuf[12]&BIT0) == BIT0 )	//结算日累计 11
    		g_energy.BonusBuyQuan = tempInt32-g_energy.BonusBalance;			
    		
    		
    	SaveEnergy ( NORMAL_SAVE ); 
    
    #ifdef BONUS_PADI_ENABLE		
    	StoreBonusBuyHistroy ( );
    #endif
    					
    	break;
    }	

#if(CALI_CHANGE_CARD_ENABLED==1)	    
    //误差校正卡 11
    case CALI_CHANGE_CARD:
    {
    	pbuf = (uchar*)&g_commBuf[0].Data[0];  
    	if ( pbuf[2] <= COEF_MAX )  
		{				
			g_RunCoef = pbuf[2];							
		}
		else
		{
			g_cardStatus = COEF_ERR;
		}	
    	break;	
    }	
#endif
    	    	
    default:
        g_cardStatus = CARD_CARDTYPE_ERR;
        break;   
    }
    
    if ( g_cardStatus != 0 )
        goto end_ic_process;    
                          
    
end_ic_process:
    //关闭电源  11
    IC_POWER_OFF;
	FeedWatchdog (  );	
	
	//g_initDispCnt = 0;
	
    if ( g_cardStatus != 0 )
    {
        g_cardFlag.Bit.ICCardRightIn = 0;
        
        //蜂鸣器处理  11   
	    g_BeepDelay = BEEP_DELAY_2S;
	    g_BeepCnt = 0;
	    SPEAKER_L();
    }
    else
    {
        g_cardFlag.Bit.ICCardRightIn = 1;
        
        //蜂鸣器处理  11 
        g_BeepDelay = BEEP_DELAY_1S;        
	    g_BeepCnt = 3;
    }
    
    
    
    //如果之前是报警电量断开继电器，此时插入任何卡均可闭合继电器 11
    if( g_relayFlag.Bit.RelayAlarmOn )
    {
        g_relayFlag.Bit.RelayAlarmOn = 0;
        if ( g_energy.Balance > 0 )
            if( g_relayFlag.Bit.RelayOn == 0 )
            {
                g_relayFlag.Bit.RelayNeedToOperate = 1;
                
                //ALARM_OFF();		//报警灯灭  11 
            }
   	}
   	
   	//赊欠跳闸 到零处理 11
   	if ( g_relayFlag.Bit.BalanceZeroHappened )
   	{  	    
   	    g_relayFlag.Bit.BalanceZero5MinFlg = 1;
   	}
   	
   	//如果之前是过压保护断开继电器，此时插入任何卡均可闭合继电器 11
   	if ( g_errStatus.Bit.OverVoltage )
   	{
   	    g_adSampleCnt = AD_SAMPLE_3S;
   	    if ( g_energy.Balance > 0 )
            if( g_relayFlag.Bit.RelayOn == 0 )
                g_relayFlag.Bit.RelayNeedToOperate = 1;       	    
   	}

//负载跳闸使能 11
#if(OVER_POWER_RELAY_OFF_EANBLED==1)   	
   	//如果之前是过载保护断开继电器，此时插入任何卡均可闭合继电器 11
   	if ( g_errStatus.Bit.OverPower )
   	{
   	    g_errStatus.Bit.OverPower = 0;
   	    g_OverPowerFlagCnt = 0;		//added by Roger 2013/09/07
   	    g_meterStatus3.Bit.OverPower = 0;   
   	    
   	    if ( g_energy.Balance > 0 )
            if( g_relayFlag.Bit.RelayOn == 0 )
                g_relayFlag.Bit.RelayNeedToOperate = 1;       	    
   	}
#endif

	//恶性负载使能 11
#if(MALIGNANT_LOAD_CHK_EANBLED==1)	
	//如果之前是过载保护断开继电器，1分钟后可闭合继电器 11
	if(g_errStatus.Bit.RLoadHappened)
	{
		g_errStatus.Bit.RLoadHappened = 0;
		for ( i=0; i<8; i++ )
    	{
    	    g_curRLoad[i].PRms = 0;
    	    g_curRLoad[i].SRms = 0;    
    	} 	
	}
#endif
		   		
	//由于使用通讯缓冲区，将通讯通道重置 11
#ifdef IR_CHANNEL_ENABLE
	InitIR();
	ResetIRComm();
#endif	
	
#ifdef RS485_CHANNEL_ENABLE
	InitRS485();
	ResetRS485Comm();
#endif	
   	
#ifdef LADDER_ENABLE								
	g_meterFlag.Bit.LadderProcess = 1;
#endif	
	
	
	
	return;		
}


				

/*
*****************************************************************************************
* fuction name: SetParaFromCard
*
* Description : this function process parameter set of super card
*
* Arguments   : uchar *buf
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
static uchar SetParaFromCard ( void )
{       
    
    uchar updateFlag,i,cnt,j;
    uchar *pbuf;
   
    //0X00和0x10处数据已解密 11
    
    //0x00 处参数 11
    //CardI2CRead ( CARD_ADDR, 0X00, (uchar*)g_buf, 16 ); 
 	pbuf = (uchar*)&g_commBuf[0].Data[0];      
     
    //购电次数02H~03H 2字节11
    //g_buyPara.BuyCount = ((uint16)pbuf[2]<<8) + pbuf[3];
    //更新标志 04H 1字节11
    updateFlag = pbuf[4]; 
    //报警启用标识11
    //g_buyPara.AlarmSet = pbuf[5]; 
    //报警金额1 06H~07H 2字节11
    g_buyPara.Alarm1 = ((uint16)pbuf[6]<<8) + pbuf[7];
    //报警金额2 08H~09H 2字节11
    g_buyPara.Alarm2 = ((uint16)pbuf[8]<<8) + pbuf[9];
    //预置或购电金额 4字节 4位小数11
    //g_buyPara.BuyQuan = ((uint32)pbuf[10]<<24) + ((uint32)pbuf[11]<<16) + ((uint32)pbuf[12]<<8)+ pbuf[13];
   	
    
    //表状态：生产、出厂、用户11
    if ( pbuf[1]==MANUFATURE_SUPER_CARD )
    	g_buyPara.Status = 	MANUFACTURE_STATUS;
    else if ( pbuf[1]==OUT_SUPER_CARD )
    	g_buyPara.Status = 	OUT_STATUS;
   	else if ( pbuf[1]==USER_CARD )
    	g_buyPara.Status = 	USER_STATUS;
    
    FeedWatchdog (  );			
    //更新EEPROM 11
    g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
			(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len ) != 0 )
		return UPDATE_EEPROM_ERR;
		
   
    //0x10处参数 分区号、卡号、模式字、表号 11
    if ( (g_buyPara.Status != USER_STATUS) || 
    	((g_buyPara.Status == USER_STATUS)&&(g_buyPara.BuyCount==0)))
    {
	    //CardI2CRead ( CARD_ADDR, 0X10, (uchar*)g_buf, 16 ); 
	    pbuf = (uchar*)&g_commBuf[0].Data[0+0x10];     
	    g_addr.Zone = ((uint16)pbuf[1]<<8) + pbuf[2];
	    for ( i=0; i<6; i++ )
	    	g_addr.CardNum[5-i] = pbuf[8-i];
	    //g_addr.BuyMode = pbuf[9];
	    
	    //revised by Roger 2015/07/22	    
	    //for ( i=0; i<4; i++ )
	    //	g_addr.MeterAddr[3-i] = pbuf[10+i];	    
	    //g_addr.MeterAddr[4] = 0x00;
	    //g_addr.MeterAddr[5] = 0x00;
	    
	    FeedWatchdog (  );
	    //更新EEPROM 11
	    g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
	    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
				(uchar*)&g_addr.Zone,Meter_Addr_Len ) != 0 )
			return UPDATE_EEPROM_ERR;	
	}
      	
    //0x20处参数 11
    //if ( updateFlag & BIT0 )
    if ( ( updateFlag & BIT0 ) || 
    	(g_buyPara.BuyCount==1) )		//第1次购电参数更新问题 revised 2015/11/19 11
    {
	    //CardI2CRead ( CARD_ADDR, 0X20, (uchar*)g_buf, 16 );  
	    pbuf = (uchar*)&g_commBuf[0].Data[0+0x20];   
		Decrypt ( pbuf, 0x20, 16, DECRYPT_KEY );    
	    if ( CheckDataFrame (pbuf) != 0 )
	    {
	    	//return  CARD_NOT_CONSISTENT;		//非法卡 11	
	    	return 0x63;
	    }
	    //限电功率 hex 11
	    g_para.LimitPower = ((uint16)pbuf[1]<<8) + pbuf[2];
	    //最大囤积量 BCD 11
	    g_para.MaximBalance = (((uint16)pbuf[3]<<8) + pbuf[4]);
	    //赊欠限额 11
	    g_para.CreditLimit =  (((uint32)pbuf[5]<<8)+ pbuf[6])*100;
	    //电流互感比 11
	    g_para.CurrentRatio = (((uint16)pbuf[7]<<8)+ pbuf[8]);
	    //当前电价 11
	    g_para.Price = (uint16)(((uint32)pbuf[9]<<24) + ((uint32)pbuf[10]<<16) + ((uint32)pbuf[11]<<8)+ pbuf[12]);
	    
	    FeedWatchdog (  );	
	    //更新EEPROM 11
	    g_para.Checksum = GetSum ( (uchar*)&g_para.LimitPower, Meter_Para_Len-1 );
	    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LimitPower),
				(uchar*)&g_para.LimitPower,Meter_Para_Len ) != 0 )
			return UPDATE_EEPROM_ERR;		
    }
    

#ifdef TARIFF_ENABLE		
    //0x30处参数 费率电价 11
    if ( (updateFlag & BIT1) || 
    	(g_buyPara.BuyCount==1) )
    {
	    //CardI2CRead ( CARD_ADDR, 0X30, (uchar*)g_buf, 16 );  
	    pbuf = (uchar*)&g_commBuf[0].Data[0+0x30];    
		Decrypt ( pbuf, 0x30, 16, DECRYPT_KEY );    
	    if ( CheckDataFrame (pbuf) != 0 )
	    {
	    	//return  CARD_NOT_CONSISTENT;		//非法卡 11	
	    	return 0x64;
	    }
	    
	    //更新到EEPROM中 11
	    //高低位换位 11
	    for ( i=0; i<6; i++ )
	   	{
	    	pbuf[2*i] = pbuf[2*i+2];
	    }
	    for ( i=12; i<15; i++ )
	    	pbuf[i] = 0;	
	    pbuf[15] = GetSum ( pbuf, 15 );
	    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,TariffPrice1),
				(uchar*)pbuf,16 ) != 0 )
			return UPDATE_EEPROM_ERR;	    
    }
#endif

#ifdef LADDER_ENABLE   
    //0x40处参数 阶梯值 11
    if ( (updateFlag & BIT2) || 
    	(g_buyPara.BuyCount==1) )
    {
	    CardI2CRead ( CARD_ADDR, 0X40, (uchar*)g_buf, 16 );  
	    //pbuf = (uchar*)&g_commBuf[1].Data[0];    
		Decrypt ( g_buf, 0, 16, DECRYPT_KEY );    
	    if ( CheckDataFrame (g_buf) != 0 )
	    {
	    	//return  CARD_NOT_CONSISTENT;		//非法卡 11	
	    	return 0x65;
	    }
	    
	    //更新到EEPROM中 11
	    //高低位换位 11
	    for ( i=0; i<5; i++ )
	    {
	    	g_buf[2*i] = g_buf[2*i+2];
	    }	
	    g_buf[10] = GetSum ( g_buf, 10 );
	    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Ladder1),
				(uchar*)g_buf,11 ) != 0 )
			return UPDATE_EEPROM_ERR;	    
    }

   	
   	//0x50处参数 阶梯电价 11
    if ( (updateFlag & BIT2) || 
    	(g_buyPara.BuyCount==1) )
    {
	    CardI2CRead ( CARD_ADDR, 0X50, (uchar*)g_buf, 16 );  
	    //pbuf = (uchar*)&g_commBuf[1].Data[10];    
		Decrypt ( g_buf, 0, 16, DECRYPT_KEY );    
	    if ( CheckDataFrame (g_buf) != 0 )
	    {
	    	//return  CARD_NOT_CONSISTENT;		//非法卡 11	
	    	return 0x66;
	    }
	    
	    //更新到EEPROM中 11
	    //高低位换位 11
	    for ( i=0; i<6; i++ )
	   	{
	    	g_buf[2*i] = g_buf[2*i+2];
	    }	
	    g_buf[12] = GetSum ( g_buf, 12 );
	    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LadderPrice1),
				(uchar*)g_buf,13 ) != 0 )
			return UPDATE_EEPROM_ERR;	    
    }
		
    //0x1E0处参数 阶梯累计周期 11
    if ( (updateFlag & BIT2) || 
    	(g_buyPara.BuyCount==1) )
    {
	    CardI2CRead ( CARD_ADDR, 0X1E0, (uchar*)g_buf, 16 );    
		Decrypt ( g_buf, 0, 16, DECRYPT_KEY );    
	    if ( CheckDataFrame (g_buf) != 0 )
	    {
	    	//return  CARD_NOT_CONSISTENT;		//非法卡 11	
	    	return 0x69;
	    }
	    
	    //更新到EEPROM中 11
	    //高低位换位 11
	    for ( i=0; i<5; i++ )
	   	{
	    	g_buf[i] = g_buf[i+1];
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
				return UPDATE_EEPROM_ERR;	
			
			g_dateLadder = 0x99; 	//不相等就进行切换 11  	
	    }
	     
    }
#endif

#ifdef TARIFF_ENABLE    
    //0x60 0x70处参数 时段表 11
//    if ( (updateFlag & BIT1) || 
//    	(g_buyPara.BuyCount==1) )
//    {
//	    //CardI2CRead ( CARD_ADDR, 0X60, (uchar*)g_buf, 16 );  
//	    pbuf = (uchar*)&g_commBuf[1].Data[0+0x20];    
//		Decrypt ( pbuf, 0x60, 16, DECRYPT_KEY );    
//	    if ( CheckDataFrame (pbuf) != 0 )
//	    {
//	    	return  CARD_NOT_CONSISTENT;		//非法卡 11	
//	    }
//	    //高低位换位 11
//	    for ( i=0; i<4; i++ )
//	   	{
//	    	g_buf[3*i] = pbuf[3*i+3];	
//	    	g_buf[3*i+1] = pbuf[3*i+2];	
//	    	g_buf[3*i+2] = pbuf[3*i+1];	
//	    }
//	    	
//	    //CardI2CRead ( CARD_ADDR, 0X70, (uchar*)g_buf, 16 );  
//	    pbuf = (uchar*)&g_commBuf[1].Data[0+0x30];    
//		Decrypt ( pbuf, 0x70, 16, DECRYPT_KEY );    
//	    if ( CheckDataFrame (pbuf) != 0 )
//	    {
//	    	return  CARD_NOT_CONSISTENT;		//非法卡 11	
//	    }
//	    //高低位换位 11
//	    for ( i=0; i<4; i++ )
//	    {
//	    	g_buf[12+3*i] = pbuf[3*i+3];	
//	    	g_buf[12+3*i+1] = pbuf[3*i+2];	
//	    	g_buf[12+3*i+2] = pbuf[3*i+1];	
//	    }	
//	    	
//	    //更新到EEPROM中 11
//	    //计算校验和 11	
//	    g_buf[24] = GetSum ( g_buf, 24 );
//	    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,HHMMNN1),
//				(uchar*)g_buf,25 ) != 0 )
//			return UPDATE_EEPROM_ERR;	    
//    }
    
    //0x100~0x1F0 时段表 11
    if ( (updateFlag & BIT1) || 
    	(g_buyPara.BuyCount==1) )
    {
	    CardI2CRead ( CARD_ADDR, 0X100, (uchar*)g_buf, 32 );    
		Decrypt ( g_buf, 0x00, 16, DECRYPT_KEY );    
	    if ( CheckDataFrame (g_buf) != 0 )
	    {
	    	return  CARD_NOT_CONSISTENT;		//非法卡 11	
	    }
	    //年时区个数 g_buf[1] 11
	    //年时区数判断 11
		if ( (g_buf[1] > 4)  )
		{
			return  CARD_NOT_CONSISTENT;		//非法卡 11			
		}					
	    //时段表数 g_buf[2] 11
	    if ( (g_buf[2] > 8)  )
		{
			return  CARD_NOT_CONSISTENT;		//非法卡 11		
		}
	    //时段数 g_buf[3] 11
	    g_buf[0] = ((g_buf[3]>>4)&0x0f)*10+g_buf[3]&0x0f;
		if ( (g_buf[0] > 14)  )
		{
			return  CARD_NOT_CONSISTENT;		//非法卡 11			
		}
	    g_buf[3] = g_buf[0];
	    g_buf[0] = g_buf[1];      	
		g_buf[1] = g_buf[2]; 
		g_buf[2] = g_buf[3]; 
		   
		Decrypt ( (uchar*)&g_buf[16], 0x10, 16, DECRYPT_KEY );    
	    if ( CheckDataFrame ((uchar*)&g_buf[16]) != 0 )
	    {
	    	return  CARD_NOT_CONSISTENT;		//非法卡 11	
	    }	
	    //高低位换位 11
	    for ( i=0; i<4; i++ ) 
	   	{
	   		g_buf[3+3*i] = g_buf[19+3*i];			//NN
	   		g_buf[4+3*i] = g_buf[18+3*i];			//MM
	   		g_buf[5+3*i] = g_buf[17+3*i];			//YY
	   	}   	
	    //更新到EEPROM中 11
	    g_buf[15] = GetSum ( g_buf, 15 );
	    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,YearZoneNum),
				(uchar*)g_buf, 16 ) != 0 )
			return UPDATE_EEPROM_ERR;	
		
		//	
		cnt = g_buf[1];		
		for ( i=0; i<cnt; i++ )   
		{
			pbuf = (uchar*)&g_commBuf[0].Data[40];
			CardI2CRead ( CARD_ADDR, 0X120+0x30*i, pbuf, 48 );    
			Decrypt ( pbuf, 0x20+0x30*i, 16, DECRYPT_KEY );
			if ( CheckDataFrame (pbuf) != 0 )
		    {
		    	return  CARD_NOT_CONSISTENT;		//非法卡 11	
		    }	
		    //高低位换位 11
		    for ( j=0; j<4; j++ ) 
		   	{
		   		g_buf[0+3*j] = pbuf[3+3*j];			//NN
		   		g_buf[1+3*j] = pbuf[2+3*j];			//MM
		   		g_buf[2+3*j] = pbuf[1+3*j];			//HH
		   	}   
		    
			pbuf += 16;
			Decrypt ( pbuf, 0x30+0x30*i, 16, DECRYPT_KEY );
			if ( CheckDataFrame (pbuf) != 0 )
		    {
		    	return  CARD_NOT_CONSISTENT;		//非法卡 11	
		    }			    
		    //高低位换位 11
		    for ( j=0; j<4; j++ ) 
		   	{
		   		g_buf[12+3*j] = pbuf[3+3*j];			//NN
		   		g_buf[13+3*j] = pbuf[2+3*j];			//MM
		   		g_buf[14+3*j] = pbuf[1+3*j];			//HH
		   	}   	
		    
		    pbuf += 16;
			Decrypt ( pbuf, 0x40+0x30*i, 16, DECRYPT_KEY );
			if ( CheckDataFrame (pbuf) != 0 )
		    {
		    	return  CARD_NOT_CONSISTENT;		//非法卡 11	
		    }			    
		    //高低位换位 11
		    for ( j=0; j<4; j++ ) 
		   	{
		   		g_buf[24+3*j] = pbuf[3+3*j];			//NN
		   		g_buf[25+3*j] = pbuf[2+3*j];			//MM
		   		g_buf[26+3*j] = pbuf[1+3*j];			//HH
		   	}
		   	
		    for(j=36;j<42;j++)
		    	g_buf[j] = 0;
		    
		    //更新到EEPROM中 11
		    g_buf[42] = GetSum ( g_buf, 42 );
		    if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,HHMMNN1)+48*(uint16)i,
					(uchar*)g_buf, 43 ) != 0 )
				return UPDATE_EEPROM_ERR;			    
		} 
    }
    
#endif
    
    return 0;   
}


/*
*****************************************************************************************
* fuction name: ProcessReadbackCard
*
* Description : this function process out super card initializing
*
* Arguments   : none
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/

static uchar ProcessReadbackCard ( void )
{          
    uchar i;
    uchar *pbuf;
    uint32 temp32;
    
    //抄表数量 不加密的 11
    //CardI2CRead ( CARD_ADDR, 0X20, (uchar*)g_buf, 16 );  
    pbuf = (uchar*)&g_commBuf[0].Data[0+0x20];    
	    
    if ( pbuf[1] > pbuf[0] )
    	return OVER_READNUM_ERR;
    
    //分区号 2     11
    g_buf[0] = (uchar)((g_addr.Zone>>8) & 0xff);
    g_buf[1] = (uchar)(g_addr.Zone & 0xff);
    
    //卡号 6 11
    if ( g_para.CurrentRatio == 65535 )		//为了兼容老系统 11
   	{
   		for ( i=0; i<6; i++ )
    		g_buf[2+i] = g_addr.CardNum[i];
   	}
   	else
   	{
   		for ( i=0; i<6; i++ )
    		g_buf[2+i] = g_addr.CardNum[5-i];
   	}
    
    		
       	    	
   	//表号 6 11
    for ( i=0; i<6; i++ )
    	g_buf[8+i] = g_addr.MeterAddr[5-i];
    //购电次数 2 11
    g_buf[14] = (uchar)((g_buyPara.BuyCount>>8) & 0xff);
    g_buf[15] = (uchar)(g_buyPara.BuyCount & 0xff);
    //最近1次购电量/购电金额 4 11
    temp32 = g_buyPara.BuyQuan*100;
    g_buf[16] = (uchar)((temp32>>24) & 0xff);
    g_buf[17] = (uchar)((temp32>>16) & 0xff);
    g_buf[18] = (uchar)((temp32>>8) & 0xff);
    g_buf[19] = (uchar)(temp32 & 0xff);
    //累计用电量	4  11
    g_buf[20] = (uchar)((g_energy.Accumulate>>24) & 0xff);
    g_buf[21] = (uchar)((g_energy.Accumulate>>16) & 0xff);
    g_buf[22] = (uchar)((g_energy.Accumulate>>8) & 0xff);
    g_buf[23] = (uchar)(g_energy.Accumulate & 0xff);
   	//透支电量/金额 4 11
   	g_buf[24] = (uchar)((g_energy.Overdraft>>24) & 0xff);
    g_buf[25] = (uchar)((g_energy.Overdraft>>16) & 0xff);
    g_buf[26] = (uchar)((g_energy.Overdraft>>8) & 0xff);
    g_buf[27] = (uchar)(g_energy.Overdraft & 0xff);
   	//当前剩余金额 4 11
   	g_buf[28] = (uchar)((g_energy.Balance>>24) & 0xff);
    g_buf[29] = (uchar)((g_energy.Balance>>16) & 0xff);
    g_buf[30] = (uchar)((g_energy.Balance>>8) & 0xff);
    g_buf[31] = (uchar)(g_energy.Balance & 0xff);
    //限电功率 added by Roger 2018/02/07 
    g_buf[32] = (uchar)((g_para.LimitPower>>8) & 0xff);
    g_buf[33] = (uchar)(g_para.LimitPower & 0xff);
    //最大囤积量 added by Roger 2018/02/07 
    g_buf[34] = (uchar)((g_para.MaximBalance>>8) & 0xff);
    g_buf[35] = (uchar)(g_para.MaximBalance & 0xff);
    //赊欠金额限值 added by Roger 2018/02/07 
    temp32 = g_para.CreditLimit/100;
    g_buf[36] = (uchar)((temp32>>8) & 0xff);
    g_buf[37] = (uchar)(temp32 & 0xff);
    //电表变比 added by Roger 2018/02/07 
    g_buf[38] = (uchar)((g_para.CurrentRatio>>8) & 0xff);
    g_buf[39] = (uchar)(g_para.CurrentRatio & 0xff);
    //电价	4  11
    temp32 = g_price;
    g_buf[40] = (uchar)((temp32>>24) & 0xff);
    g_buf[41] = (uchar)((temp32>>16) & 0xff);
    g_buf[42] = (uchar)((temp32>>8) & 0xff);
    g_buf[43] = (uchar)(temp32 & 0xff);
    
   	if ( g_para.CurrentRatio == 65535 )		//为了兼容老系统 11
   	{
   		CardI2CWrite ( CARD_ADDR, 0x30+0x20*pbuf[1], g_buf, 32 );	
   	}
   	else
   	{
   	#if(NEW_READ_IC_CARD_ENABLED==0)	
   		CardI2CWrite ( CARD_ADDR, 0x30+0x30*pbuf[1], g_buf, 44 );	
   	#else if (NEW_READ_IC_CARD_ENABLED==1)	
   		CardI2CWrite ( CARD_ADDR, 0x30+0x40*pbuf[1], g_buf, 44 );		
   	#endif
   	}
    
    #if (NEW_READ_IC_CARD_ENABLED==1)	
    	//if ( pbuf[0] == 10)
    		CardI2CWrite ( CARD_ADDR, 0x60+0x40*pbuf[1], (uchar*)&g_energy.TariffQ1, 16 );   	
    #endif
   	        
    pbuf[1]++;
    CardI2CWrite ( CARD_ADDR, 0x21, &pbuf[1], 1 );
                 
    return 0;   
}



//added by Roger 2015/07/24
/*
*****************************************************************************************
* fuction name: ProcessRetrunBack
*
* Description : this function process out super card initializing
*
* Arguments   : none
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
static uchar ProcessRetrunBack ( void )
{
	uchar i;
	uint32 tempL;
		
	//先进行返写操作 11       
    g_buf[0] = 0xa6; 
    //剩余金额  11
    if ( g_energy.Overdraft > 0 )
    	tempL = g_energy.Overdraft+0x80000000;
    else
    	tempL = g_energy.Balance;	
    g_buf[1] = (uchar)((tempL>>24)&0xff);
    g_buf[2] = (uchar)((tempL>>16)&0xff);   
    g_buf[3] = (uchar)((tempL>>8)&0xff);
    g_buf[4] = (uchar)((tempL>>0)&0xff);
    //卡号 6 11
    for ( i=0; i<6; i++ )
		g_buf[5+i] = g_addr.CardNum[5-i];
    //电表状态 11
    g_buf[11] = g_errStatus.Byte;
    //购电次数 11
    g_buf[12] = (uchar)((g_buyPara.BuyCount>>8)&0xff);
    g_buf[13] = (uchar)((g_buyPara.BuyCount>>0)&0xff);
    
    g_buf[14] = 0; 
    for ( i=0; i<14; i++ )
        g_buf[14] += g_buf[i];
 	g_buf[15] = 0x3a; 
    Encrypt ( g_buf, 0, 16 ); 
      
    if ( CardI2CWrite ( CARD_ADDR, 0xb0, (uchar*)&g_buf[0], 16 ) != 1 )
    {
        return WRITE_BACK_ERR;         
    }	
	
	return 0;
}	

#ifdef BONUS_PADI_ENABLE
/*
*****************************************************************************************
* fuction name: ProcessBonusRetrunBack
*
* Description : this function process out super card initializing
*
* Arguments   : none
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
static uchar ProcessBonusRetrunBack ( uint32 temp32, uint16 temp16 )
{
	uchar i;
		
	//先进行返写操作 11       
    g_buf[0] = 0xa6; 
    //剩余金额  11
    g_buf[1] = (uchar)((g_energy.Balance>>24)&0xff);
    g_buf[2] = (uchar)((g_energy.Balance>>16)&0xff);   
    g_buf[3] = (uchar)((g_energy.Balance>>8)&0xff);
    g_buf[4] = (uchar)((g_energy.Balance>>0)&0xff);
    //购电次数 11
    g_buf[5] = (uchar)((g_buyPara.BuyCount>>8)&0xff);
    g_buf[6] = (uchar)((g_buyPara.BuyCount>>0)&0xff);
    //补助剩余金额  11
    g_buf[7] = (uchar)((temp32>>24)&0xff);
    g_buf[8] = (uchar)((temp32>>16)&0xff);   
    g_buf[9] = (uchar)((temp32>>8)&0xff);
    g_buf[10] = (uchar)((temp32>>0)&0xff);
    //购电次数 11
    g_buf[11] = (uchar)((temp16>>8)&0xff);
    g_buf[12] = (uchar)((temp16>>0)&0xff);
    g_buf[13] = 0;
    g_buf[14] = 0; 
    for ( i=0; i<14; i++ )
        g_buf[14] += g_buf[i];
 	g_buf[15] = 0x3a; 
      
    if ( CardI2CWrite ( CARD_ADDR, 0x80, (uchar*)&g_buf[0], 16 ) != 1 )
    {
        return WRITE_BACK_ERR;         
    }	
	
	return 0;
}
#endif

#endif		