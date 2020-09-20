/******************************************************************************************
*  Copyright (c) 2013,  R&D Department
*  All rights reserved 
*
*  FileName��   	buyprocess.c
*  Author��     	Roger
*  Date��  			2010-3-8 20:10
*  Revised Date��  	
*  Main Function��  this file contains all the functions used in I2C communications.
*  Version:			V1.0
*  Note�� 			 			
*********************************************************************************************/
#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

#define _BUY_PROCESS_DEFINE_
#include "include.h"



//variables statement
/******************************************************************************************/
const uchar SELFKEY[16]={0x84,0x71,0xb5,0xbc,0xde,0x4d,0x4b,0xbe,
				0xfb,0x1d,0x45,0xa8,0x56,0xef,0x85,0xc8};


//��ʼ������ 11
//��ʼ������ 11
const uchar INIT_PRICE_TABLE[]={
	//���ʵ����Ϣ	8	 11
	0x00,0x00,		//���ʵ��1  ���ֽ���ǰ ���ֽ��ں� 2�ֽ� 11
	0x00,0x00,		//���ʵ��2  2�ֽ� 11
	0x00,0x00,	    //���ʵ��3  2�ֽ� 11
	0x00,0x00,		//���ʵ��4  2�ֽ� 11
	0x00,0x00,		//���ʵ��5  2�ֽ� 11
	0x00,0x00,		//���ʵ��6  2�ֽ� 11	
	0x00,0x00,0x00,	//����3�ֽ� 	 11							
	//0x00			//16	
};

const uchar INIT_PASSWORD_TABLE[]={
	0x02,               //ͨѶ���� 4 11	
	0x00,0x00,0x00,
	//0x02		        //У����	   16
};

const uchar INIT_YEAR_ZONE_TABLE[]={
	0x01,		//��ʱ���� 11
	0x01,		//ʱ�α��� 11
	0x08,		//��ʱ�α��� 11
	0x01,0x01,0x01,		//��1��ʱ�� 11
	0x01,0x01,0x01,			//��2��ʱ�� 11
	0x01,0x01,0x01,			//��3��ʱ�� 11
	0x01,0x01,0x01,			//��4��ʱ�� 11
	//0x10		    //У���	 11
};

const uchar INIT_TARIFF_TBL1[]={
	//��1ʱ�α� ����ʱ�α� 12 13 14 
	0x04,0x00,0x00,		//��1ʱ�α� 11
	0x04,0x00,0x08,		//��2ʱ�α� 11
	0x04,0x00,0x08,		//��3ʱ�α� 11
	0x04,0x00,0x08,	    //��4ʱ�α� 11
	0x04,0x00,0x08,		//��5ʱ�α�	11
	0x04,0x00,0x08,		//��6ʱ�α� 11
	0x04,0x00,0x08,		//��7ʱ�α� 11
	0x04,0x00,0x08,		//��8ʱ�α�	11  
	0x04,0x00,0x08,		//��9ʱ�α� 11
	0x04,0x00,0x08,		//��10ʱ�α� 11
	0x04,0x00,0x08,	    //��11ʱ�α� 11
	0x04,0x00,0x08,		//��12ʱ�α� 11
	0x04,0x00,0x08,		//��13ʱ�α� 11
	0x04,0x00,0x08,		//��14ʱ�α� 11	  					
	//unsigned char Checksum10;		// 43	
};


const uchar INIT_LADDER_VALUE_TBL[]={
	//����ֵ��Ϣ	36
	0x00,0x00,		//����ֵ1  2�ֽ� 11
	0x00,0x00,		//����ֵ2  2�ֽ� 11
	0x00,0x00,		//����ֵ3  2�ֽ� 11
	0x00,0x00,		//����ֵ4  2�ֽ� 11
	0x00,0x00,		//����ֵ5  2�ֽ� 11	
	0x00,0x00,		//����ֵ6  2�ֽ� 11
	0x00,0x00,		//����ֵ7  2�ֽ� 11								
	//unsigned char Checksum6;		//11
};

const uchar INIT_LADDER_PRICE_TBL[]={
	//���ݵ����Ϣ	37
	0x00,0x00,		//0x50,0x14, Ĭ��5200 0.52 ���ݵ��1  2�ֽ� 11
	0x00,0x00,		//���ݵ��2  2�ֽ� 11
	0x00,0x00,		//���ݵ��3  2�ֽ� 11
	0x00,0x00,	    //���ݵ��4  2�ֽ� 11
	0x00,0x00,		//���ݵ��5  2�ֽ� 11
	0x00,0x00,		//���ݵ��6  2�ֽ�	11	
	0x00,0x00,		//���ݵ��6  2�ֽ�	11
	0x00,0x00,		//���ݵ��6  2�ֽ�	11							
	//unsigned char Checksum7;		//12+1=13
};

//��ʼ������ 11
const char INIT_SOFTWARE_VERSION[8]={	' ',' ',
										VERSION_UPDATE_2+0X30,
										VERSION_UPDATE_1+0X30,
										'.',
										VERSION_HEAD_2+0x30,
										VERSION_HEAD_1+0x30,
										'V'																																		
										};  
/******************************************************************************************/

#ifdef PRE_PAID_ENABLE
//function statement
/******************************************************************************************/
static void StoreBuyHistroy ( uchar* buf );
static uchar OneBCDToHex( uchar p );

#if(PROTOCOL_ADD_ORDERS==1)
	
	static uchar CaculateLessDays ( uchar* pbuf1, uchar* pbuf2 );
	static uchar CheckOrdersValid ( uchar *pbuf, uchar len );
	static void StoreOrdersHistroy ( uchar* buf );
#endif

/******************************************************************************************/

#endif

#ifdef PRE_PAID_ENABLE
/*
*****************************************************************************************
* fuction name: ProcessUserBuyQuan
*
* Description : this function process user buy
*
* Arguments   : none
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
uchar ProcessUserBuyQuan ( uchar *pbuf, uchar mode, uchar buyMode )
{           
    uchar i,eFlag,usedFlag;
    uint16 buycnt;
    uint32 buyQuan,tempBalance,tempCrossZero,temp32;
#if(PROTOCOL_ADD_ORDERS==1)
	uchar ret;
#endif       
    eFlag = 0;      //Ĭ�ϲ�����EEPRM�����¼ 11
    usedFlag = 0;
                     
    //��ȡ��������빺���� 11
    if ( buyMode == 1 )		//������ 11
    {
    	buycnt = ((uint16)pbuf[17]<<8) + pbuf[16];
    	buyQuan = ((uint32)pbuf[15]<<24) + ((uint32)pbuf[14]<<16) + ((uint32)pbuf[13]<<8)+ pbuf[12];
    
    #if(IC_CARD_MODE==AT24_CARD)
    	CardI2CRead ( CARD_ADDR, 0x90, (uchar*)&usedFlag, 1 );
    #endif
    
    #if(IC_CARD_MODE==RF_CARD)
    	if ( ReadBlockData ( 05, (uchar*)g_buf ) == FALSE ) 
	    {
	    	return RF_READ_TYPE_ERR;	
	    } 
	    usedFlag = g_buf[0];
	#endif	
    }
    else
    {
    	if ( pbuf[16]==0xff && pbuf[17]==0xff )
    		buycnt = 0xffff;
    	else	
    		buycnt = (uint16)BCDToHex( &pbuf[16], 2 );
    	buyQuan = BCDToHex( &pbuf[12], 4 )*100; 	
    }  
    
    if ( buyQuan > 3000000000 )
    {
        return COMM_OVER_BUY_QUAN_ERR;    
    } 
    
    //�ж϶������Ƿ���� 11
#if(PROTOCOL_ADD_ORDERS==1)
	if ( buyMode > 1 )
	{
		ret = CheckOrdersValid ( pbuf, buyMode );	
		if ( ret > 0 )
			return ret;
		//�����Ǻ����� ���й��������ֵ���� 11
		buycnt = g_buyPara.BuyCount + 1;			
	}
#endif
                 
    //�û��������ʹ��� 11
    if ( (mode == NORMAL_MODE) ||     //�����۵� 11
            ( mode == BUY_RETURN_MODE )  )    //�۵��˷� 11    
    {
        if ( g_buyPara.Status == BACK_MONEY_STATUS )    //�����˷�ģʽ���� 11
        {
            return HAVE_IN_RETURN_MODE;   
        }
        
        //�۵��˷� ��������������1 11
        if ( mode == BUY_RETURN_MODE )
        {
            if ( buycnt < 1 )    
                return COMM_BUY_CNT_ERR; 
        }    
        
        //�������Ϊ0 Ϊ������ 11
        if ( buycnt == 0 )
        {
            //���������Ϊ 0 
            if ( buyQuan > 0 )
                return OPEN_CARD_INFO_ERR;                        
        }
        
        //��������ж�:ֻ�������������������� 11
        //1)���������ͬ��˵���ǲ��� �����²忨 11
        //2)���ڹ�������ȿ��Ϲ��������1�� �����۵� 11
        if ( (buycnt != g_buyPara.BuyCount) &&
                ((buycnt-1) != g_buyPara.BuyCount) )
            return COMM_BUY_CNT_ERR;
            
        //���ƿ����жϣ����Ϲ������Ϊ0,���ƹ������ҲΪ0 ���� 11
        //���Ϲ������Ϊ1�����ƹ������Ϊ0 11              
        if ( (buycnt<2) && (g_buyPara.BuyCount==0) )
        {            
            //�û���Ű� 11
            for ( i=0; i<6; i++ )       
                g_addr.CardNum[i] = pbuf[6+i]; 
                
            g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
	        StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
		        (uchar*)&g_addr.Zone,Meter_Addr_Len );               
        }
                        
        //�������� 11
        //ֻ�� �������²��� 11   
        if ( (g_buyPara.BuyCount == 0) && (buycnt==0) &&
        			(usedFlag==0) )
        {             
            tempCrossZero = g_energy.Overdraft;    
            tempBalance = g_energy.Balance;                 
        }   
        else if ( (((buycnt-1) == g_buyPara.BuyCount)) &&
        			(usedFlag==0) )     //�������� 11
        {            
            //����2019/01/08 11
            temp32 = g_energy.Balance+buyQuan;
            if ( temp32 < buyQuan )
            {
            	return COMM_OVERMAX_ERR;	
            }
            
            //�ж϶ڻ���� 11
//            if ( (buycnt==1) )
//            {
//            	temp32 = g_energy.Balance+buyQuan-g_energy.PrePaid;
//            	if ( temp32 >= 
//	                (((uint32)g_para.MaximBalance)*10000) ) //4λС�� 11
//	            {
//	            	return COMM_OVERLIMIT_ERR;       
//	            } 
//            }
//            else
//            {
//            	if ( (g_energy.Balance+buyQuan) >= 
//	                (((uint32)g_para.MaximBalance)*10000) ) //4λС�� 11
//	            {
//	            	return COMM_OVERLIMIT_ERR;       
//	            }  	
//            }
            
                        
            
            if ( mode == NORMAL_MODE )    //�������� 11
            {                                                       
                temp32 = buyQuan;                
                //����Ԥ�ý�� 11
                if ( buycnt == 1 )
                {
                	if ( g_para.CurrentRatio == 65535 )		//Ϊ�˼�����ϵͳ ϵͳ�۳�Ԥ�ý�� 11
                	{
                		//������� ����������� 11
		            	if ( g_energy.Overdraft > temp32 )
		            	{
		            		tempCrossZero = g_energy.Overdraft-temp32;	
		            		tempBalance = 0;	
		            	}
		            	else
		            	{
		                	tempBalance = g_energy.Balance+(temp32-g_energy.Overdraft);
		                	tempCrossZero = 0;
		                }	
                	}
                	else
                	{
                		temp32 = g_energy.PrePaid; 
	                	if ( temp32 > buyQuan )		//������С��Ԥ�ý�� 11
	                	{
	                		//return COMM_FIRST_BUY_LESS_ERR;	
	                		//����Ӧʣ���� 11
	                		if ( g_energy.Balance > 0 )
	                		{
	                			temp32 = g_energy.PrePaid - g_energy.Balance;	//��ʹ�ý�� 11                				
	                			if ( buyQuan > temp32 )
	                			{
	                				tempBalance = buyQuan-temp32;
	                				tempCrossZero = 0;		
	                			}
	                			else
	                			{
	                				tempCrossZero = temp32 - buyQuan;
	                				tempBalance = 0;	
	                			}
	                		}
	                		else
	                		{
	                			temp32 = g_energy.PrePaid + g_energy.Overdraft;	//��ʹ�ý�� 11	
	                			
	                			tempCrossZero = temp32 - buyQuan;
	                			tempBalance = 0;
	                		}	                		                		
	                	}
	                	else
	                	{
	                		temp32 = buyQuan - temp32;	//
	                		
	                		//������� ����������� 11
			            	if ( g_energy.Overdraft > temp32 )
			            	{
			            		tempCrossZero = g_energy.Overdraft-temp32;	
			            		tempBalance = 0;	
			            	}
			            	else
			            	{
			                	tempBalance = g_energy.Balance+(temp32-g_energy.Overdraft);
			                	tempCrossZero = 0;
			                }		
	                	} 	
                	}
                	               			
                		
                }
                else
                {
                	//������� ����������� 11
	            	if ( g_energy.Overdraft > temp32 )
	            	{
	            		tempCrossZero = g_energy.Overdraft-temp32;	
	            		tempBalance = 0;	
	            	}
	            	else
	            	{
	                	tempBalance = g_energy.Balance+(temp32-g_energy.Overdraft);
	                	tempCrossZero = 0;
	                }	
                }	
                                                                                         
            }
            else if ( mode == BUY_RETURN_MODE )   //�����˷� 11
            {           	
                temp32 = buyQuan;
                
                //������� ����������� 11
            	if ( g_energy.Balance >= temp32 )
            	{
            		tempBalance = g_energy.Balance-temp32;
                	tempCrossZero = 0;               		
            	}
            	else
            	{
                	if ( g_energy.Balance > 0 )
                	{
                		tempCrossZero = temp32-g_energy.Balance;		
                	}
                	else
                	{
                		tempCrossZero = g_energy.Overdraft + temp32-g_energy.Balance;	
                	}
                	tempBalance = 0;
                }                                  
            }
                                  
            eFlag = 1;      //��Ҫ����11 
                                                                  
        }        
        else        //���������ͬ ֻ���·�д������ 11
        {
            tempCrossZero = g_energy.Overdraft;    
            tempBalance = g_energy.Balance;
            
        }         
        //��д������ ͳһ���� ������ 11
                
    }
    else if ( mode == CANCEL_RETURN_MODE )        //�����˷� 11
    {
        if ( g_buyPara.Status != BACK_MONEY_STATUS )		//�ж��˷�״̬ 11
        {
        #if(IC_CARD_MODE==PLC_COMM_USED)
        	//������������ж� 11
	        if ( buycnt != 0xffff )
	        {
	        	//��Ҫ��дʣ���� ���˷ѽ�� ��͸֧��� 11    
		        buyQuan = g_energy.Balance;
		        
		        tempCrossZero = g_energy.Overdraft;    
		        tempBalance = g_energy.Balance;
		        
		        eFlag = 1;	
	        }	                      
        #else
        	//�������������ͬ 11
	        if ( buycnt != g_buyPara.BuyCount )
	            return COMM_BUY_CNT_ERR;
	            
	     	//��Ҫ��дʣ���� ���˷ѽ�� ��͸֧��� 11    
	        buyQuan = g_energy.Balance;
	        
	        tempCrossZero = g_energy.Overdraft;    
	        tempBalance = g_energy.Balance;
	        
	        eFlag = 1;	
	    #endif
	            
	        
        }
        
    }
    
    //������ �ȸ��¿����������� 11   
    if ( buyMode == 1 )
    {
    	//�Ƚ��з�д����   11     
	    //���⣬����ʹ�ñ�־�ͷ�д�� 11
	    g_buf[0] = 0xa6;
	    //������� 11
	    g_buf[1] = (uchar)((buycnt>>8)&0xff);
	    g_buf[2] = (uchar)((buycnt>>0)&0xff);
	    //���ź�2λ�� 11
	    pbuf = (uchar*)&g_commBuf[0].Data[0+0x10]; 
	    g_buf[3] = pbuf[7];
	    g_buf[4] = pbuf[8]; 
	    //���ι�����  11
	    g_buf[5] = (uchar)((buyQuan>>24)&0xff);
	    g_buf[6] = (uchar)((buyQuan>>16)&0xff);   
	    g_buf[7] = (uchar)((buyQuan>>8)&0xff);
	    g_buf[8] = (uchar)((buyQuan>>0)&0xff);
	    //�ۼ��õ��� 11  
	    g_buf[9] = (uchar)((g_energy.Accumulate>>24)&0xff);
	    g_buf[10] = (uchar)((g_energy.Accumulate>>16)&0xff);   
	    g_buf[11] = (uchar)((g_energy.Accumulate>>8)&0xff);
	    g_buf[12] = (uchar)((g_energy.Accumulate)&0xff);
	    //���״̬ 11
	    g_buf[13] = g_errStatus.Byte;
	    g_buf[14] = 0; 
	    for ( i=0; i<14; i++ )
	        g_buf[14] += g_buf[i];
	    g_buf[15] = 0x3a; 
	    Encrypt ( g_buf, 0, 16 ); 
	
	#if(IC_CARD_MODE==AT24_CARD)     
	    if ( CardI2CWrite ( CARD_ADDR, 0x80, (uchar*)&g_buf[0], 16 ) != 1 )
	    {
	        return WRITE_BACK_ERR;         
	    }
	#endif
	
	#if(IC_CARD_MODE==RF_CARD)     
	    if ( WriteBlockData ( 4, (uchar*)&g_buf[0] ) == FALSE ) 
	    {
	    	return WRITE_BACK_ERR;	
	    }	
	#endif
	    
	    //��ʹ�ñ�ʶ 11
	     g_buf[0] = 0x01;
	    //�ϴβ忨�����ɹ�ʱ�� 11
	    g_buf[1] = g_date.year;
	    g_buf[2] = g_date.month;    
	    g_buf[3] = g_date.day;
	    g_buf[4] = g_date.hour;
	    g_buf[5] = g_date.min;
	    g_buf[6] = g_date.sec; 
	    for ( i=7; i<15; i++ )		//added by Roger 2015/07/24
	        g_buf[i] = 0;
	#if(IC_CARD_MODE==AT24_CARD) 
	    if ( CardI2CWrite ( CARD_ADDR, 0x90, (uchar*)&g_buf[0], 7 ) != 1 )
	    {
	        return WRITE_BACK_ERR;  
	    }
	#endif 
	
	#if(IC_CARD_MODE==RF_CARD)     
	    if ( WriteBlockData ( 5, (uchar*)&g_buf[0] ) == FALSE ) 
	    {
	    	return WRITE_BACK_ERR;	
	    }	
	#endif
	   
	    //��д������ 11
	    g_buf[0] = 0xa6;
	    //�޵繦�� 11
	    g_buf[1] = (uchar)((g_para.LimitPower>>8)&0xff);
	    g_buf[2] = (uchar)((g_para.LimitPower>>0)&0xff);
	    //�ڻ������ֵ 11
	    g_buf[3] = (uchar)((g_para.MaximBalance>>8)&0xff);
	    g_buf[4] = (uchar)((g_para.MaximBalance>>0)&0xff);
	    //͸֧�����ֵ 2λС�� 11
	    temp32 = g_para.CreditLimit/100;
	    g_buf[5] = (uchar)((temp32>>8)&0xff);
	    g_buf[6] = (uchar)((temp32>>0)&0xff);
	    //������б� 11  
	    g_buf[7] = (uchar)((g_para.CurrentRatio>>8)&0xff);
	    g_buf[8] = (uchar)((g_para.CurrentRatio)&0xff);
	    //��� 11
	    g_buf[9] = (uchar)((g_price>>24)&0xff);
	    g_buf[10] = (uchar)((g_price>>16)&0xff);   
	    g_buf[11] = (uchar)((g_price>>8)&0xff);
	    g_buf[12] = (uchar)((g_price>>0)&0xff);
	    
	    g_buf[13] = 0;
	    g_buf[14] = 0;
	    for ( i=0; i<14; i++ )
	        g_buf[14] += g_buf[i];
	    g_buf[15] = 0x3a; 
	    Encrypt ( g_buf, 0, 16 ); 
	
	#if(IC_CARD_MODE==AT24_CARD)       
	    if ( CardI2CWrite ( CARD_ADDR, 0xa0, (uchar*)&g_buf[0], 16 ) != 1 )
	    {
	        return WRITE_BACK_ERR;         
	    }	
	#endif
	
	#if(IC_CARD_MODE==RF_CARD)     
	    if ( WriteBlockData ( 6, (uchar*)&g_buf[0] ) == FALSE ) 
	    {
	    	return WRITE_BACK_ERR;	
	    }	
	#endif
		
    }
      
    //���¹����¼ EEPROM 11
    if ( eFlag == 1 )
    {               
        //���ι�����  ���1�γ��˷ѽ�� �����˷ѽ�� 11
      	temp32 = buyQuan/100;    
        temp32 = Hex32ToBCD( temp32 );   
        g_buf[0] = (uchar)((temp32>>0)&0xff);
        g_buf[1] = (uchar)((temp32>>8)&0xff);
        g_buf[2] = (uchar)((temp32>>16)&0xff); 
        g_buf[3] = (uchar)((temp32>>24)&0xff);   
        if ( mode == BUY_RETURN_MODE )   //�����˷� 11
        {
            //������˷� �����˷ѽ���¼ 11
            EEPROM_NOWP();
    	    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_ReturnQuan), 
    		    &g_buf[0], 4, 1  );
		    EEPROM_WP();
            
            g_buf[3] =  0x80;       //���λ��1 11   
        }           
        //������� 2 11
		if ( (mode == CANCEL_RETURN_MODE)  )
			temp32 = Hex16ToBCD( g_buyPara.BuyCount );
		else
    		temp32 = Hex16ToBCD( buycnt );
        g_buf[4] = (uchar)(temp32 & 0xff);
        g_buf[5] = (uchar)((temp32>>8) & 0xff);
        
        //����ǰʣ���� 11
        if ( buycnt == 1 )  //��һ�ι��� 11
        {
            temp32 = 0;
        }
        else
            temp32 = g_energy.Balance/100;
        temp32 = Hex32ToBCD( temp32 ); 
        g_buf[6] = (uchar)((temp32>>0)&0xff);
        g_buf[7] = (uchar)((temp32>>8)&0xff);
        g_buf[8] = (uchar)((temp32>>16)&0xff); 
        g_buf[9] = (uchar)((temp32>>24)&0xff);  
        
        //�����ʣ���� 11
        temp32 = tempBalance/100;
        temp32 = Hex32ToBCD( temp32 );  
        g_buf[10] = (uchar)((temp32>>0)&0xff);
        g_buf[11] = (uchar)((temp32>>8)&0xff);
        g_buf[12] = (uchar)((temp32>>16)&0xff); 
        g_buf[13] = (uchar)((temp32>>24)&0xff);
        
        //������ۼƹ����� 11
        I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_BuyQuan), 
 					&g_buf[20], 18, 1  ) ;
        temp32 = BCDToHex( &g_buf[34], 4 ); 
        if ( mode == BUY_RETURN_MODE )   //�����˷� 11
            temp32 -= buyQuan/100;       
        else
        {
            temp32 += buyQuan/100;                           
        }
        temp32 = Hex32ToBCD( temp32 );      
        g_buf[14] = (uchar)((temp32>>0)&0xff);
        g_buf[15] = (uchar)((temp32>>8)&0xff);
        g_buf[16] = (uchar)((temp32>>16)&0xff); 
        g_buf[17] = (uchar)((temp32>>24)&0xff);
        
        //���汾�ι����¼ 11
        StoreBuyHistroy ( g_buf );
		       		
		g_energy.Balance = tempBalance;
        g_energy.Overdraft = tempCrossZero;
        
      	g_buyPara.BuyQuan = buyQuan/100;
        if ( mode != CANCEL_RETURN_MODE ) 
			g_buyPara.BuyCount++;       //��������ۼ� 11
               		
        //����EEPROM 11
        g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
        StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
    			(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );    	
    	
    	SaveEnergy ( NORMAL_SAVE ); 
    	
    	//���涩���� 11
    #if(PROTOCOL_ADD_ORDERS==1)
    	if ( buyMode > 1 )	
    	{
    		StoreOrdersHistroy ( pbuf+buyMode-8 );	
    	}
    #endif	
    	
    }
    
    //������д�ɹ��� �л�״̬ 11
    if ( mode == CANCEL_RETURN_MODE )
    {
        //g_buyPara.Alarm1 = 0;
        //g_buyPara.Alarm2 = 0;	
        
        //�������״̬ 2020/08/11 11
        if ( buycnt == 0xffff )
        {
        	if ( g_buyPara.Status == BACK_MONEY_STATUS )		//�ж��˷�״̬ 11
        		g_buyPara.Status = USER_STATUS;	
        	g_meterStatus3.Bit.StatusMeter = 0;		//�˳��˷�״̬ 11	
        }
        else
        {
        	g_buyPara.Status = BACK_MONEY_STATUS; 	
        	//�˷�״̬�� ������բ 11      
        	g_relayFlag.Bit.RelayOn = 1;
        	g_meterStatus3.Bit.StatusMeter = 1;		//�˷�״̬ 11
        }	   
                                
        
        //����EEPROM 11
        g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
        StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
    			(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );
    }
                      
    
#ifdef TARIFF_ENABLE        
    g_meterFlag.Bit.TariffProcess = 1;
#endif 
                    
    
    
    if ( g_energy.Balance > 0 )
    {   	
    	//������բ���� 11
    	g_relayFlag.Bit.RelayAlarmHappened = 0;
    	g_relayFlag.Bit.BalanceZeroHappened = 0;
    	g_relayFlag.Bit.BalanceZero5MinFlg = 0;
    	g_relayFlag.Bit.RelayAlarmOn = 0;
    	
    	ProcessRelayStatus ( );
    	ProcessRelay ( );
    	
    	g_meterStatus3.Bit.NoMoney = 0;
    }
        
    return 0;
      
}

/*
*****************************************************************************************
* fuction name: CheckOrdersValid
*
* Description : this function process user buy
*
* Arguments   : none
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
#if(PROTOCOL_ADD_ORDERS==1)
static uchar CheckOrdersValid ( uchar *pbuf, uchar len )
{           
    
    uchar i,j,eFlag,usedFlag,diff;
    uint32 order1,order2,tempOrder1,tempOrder2;
    uint16 order3,tempOrder3;
    uchar *ptmepBuf;
        
    usedFlag = 0;
      
    //��ȡ���綩���� 11
    ptmepBuf = pbuf + len -8;
    for( i=0; i<8; i++ )
   	{
   		if ( CheckBCD(ptmepBuf[i]) == 1 )
   			break;
   	}
   	if ( i < 8 )
   		return COMM_ORDER_NUM_ERR;       	 
    //yymmdd
    order1 = (((uint32)ptmepBuf[7])<<16) + 
    			(((uint32)ptmepBuf[6])<<8) + 
    			((uint32)ptmepBuf[5]);
    //hhmmss
    order2 = (((uint32)ptmepBuf[4])<<16) + 
    			(((uint32)ptmepBuf[3])<<8) + 
    			((uint32)ptmepBuf[2]);
    //counter
    order3 =  (((uint16)ptmepBuf[1])<<8) + 
    			((uint16)ptmepBuf[0]);
    
    //���ڴ洢�����űȶ� 11
    eFlag = 0;		//Ĭ�ϱ��ڴ洢������ȷ 11

    for( i=0; i<10; i++ )
    {
    	I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_Order)+8*i,
				(uchar*)&g_buf[0], 8, 1  );	
    	
    	for( j=0; j<8; j++ )
	   	{
	   		if ( CheckBCD(g_buf[j]) == 1 )
	   		{	   			
	   			break;
	   		}	   		
	   	}
	   	if ( j < 8 )
	   	{
	   		eFlag++;
	   		continue;
	   	}
	   	
    	//yymmdd
	    tempOrder1 = (((uint32)g_buf[7])<<16) + 
	    			(((uint32)g_buf[6])<<8) + 
	    			(((uint32)g_buf[5]));
	    //hhmmss
	    tempOrder2 = (((uint32)g_buf[4])<<16) + 
	    			(((uint32)g_buf[3])<<8) + 
	    			((uint32)g_buf[2]);
	    //counter
	    tempOrder3 = (((uint16)g_buf[1])<<8) + 
	    			((uint16)g_buf[0]);
	    			
    	//�����1�ζ����űȽ�ʱ�� ��ʾ���� 11
    	if ( i == 0 )
    	{
    		if ( (order1 < tempOrder1) ||
    			((order1==tempOrder1)&&(order2<tempOrder2)) )
    		{
    			diff = CaculateLessDays ( ptmepBuf, g_buf );
    			if ( diff > 30 )	
    				return COMM_ORDER_OVER_ERR;
    		}	
    	}
    	    	   		   		   		   	
	   	//�ж��Ƿ���� 11
	   	if ( (order1 == tempOrder1) &&
	   			( order2 == tempOrder2 ) &&
	   			( order3 == tempOrder3 ) )
	   	{
	   		usedFlag = 1;
	   		break;	
	   	}	   			  	    	
    }
    if ( eFlag > 0 )  
    {
    	
    } 
                   
    //����˵�������ſ����� ����������ݳ�ֵ 11
    if ( usedFlag == 0 )                 
    	return 0;
    else
    	return COMM_ORDER_USED_ERR;
      
}
#endif
/*
*****************************************************************************************
* fuction name: CaculateLessDays
*
* Description : ����ȴ洢��ʱ��С������
*
* Arguments   : uchar* buf, uchar len
*		
* returns     : uchar checksum
*
* Notes     :	 
******************************************************************************************
*/
#if(PROTOCOL_ADD_4_SERIALS==1)
static uchar CaculateLessDays ( uchar* pbuf1, uchar* pbuf2 )
{
	uchar diff;
	
	diff = 0;
	//pbuf2�Ǵ洢ʱ�� pbuf1Ϊ��ǰʱ�� 11
	if ( pbuf2[7] >= pbuf1[7] )		//year
	{
		diff = OneBCDToHex( pbuf2[7] ) - OneBCDToHex( pbuf1[7] );
		if ( diff >= 2 )
			return 0xff;
		else if ( diff == 1 )		//���� 11
		{
			if ( pbuf1[6] >= pbuf2[6] )	//month
				return 0xff;
			
			diff = 12 + OneBCDToHex( pbuf1[6] ) - OneBCDToHex( pbuf2[6] );
			if ( diff >= 2 )
				return 0xff;
			
			diff = 31 + OneBCDToHex( pbuf1[5] ) - OneBCDToHex( pbuf2[5] );	
		}
		else //ͬ�� 11
		{					
			if ( pbuf2[6] >= pbuf1[6] )	//month
			{
				diff = OneBCDToHex( pbuf1[6] ) - OneBCDToHex( pbuf2[6] );
				if ( diff >= 2 )		//����1���� 11
					return 0xff;
				else if ( diff == 1 )		//���� 11
				{
					diff = 30 + OneBCDToHex( pbuf1[5] ) - OneBCDToHex( pbuf2[5] );	
				}
				else		//ͬ�� 11
				{														
					if ( pbuf2[5] > pbuf1[5] )	//day
					{
						diff = pbuf2[5] - pbuf1[5];
					}	
				}
			}	
			
		}			
			
	}	
	
	
	return diff;
}
#endif
/*
*****************************************************************************************
* fuction name: OneBCDToHex
*
* Description : This function visualize the request item according to the diffrent condition.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/

static uchar OneBCDToHex( uchar p )
{
    uchar ret; 
   
    ret = (p>>4)*10+(p&0x0f);		
                                
    return( ret );
    
}


/*
*****************************************************************************************
* fuction name: StoreOrdersHistroy
*
* Description : �洢���10�ζ����ż�¼ 
*
* Arguments   : uchar* buf, uchar len
*		
* returns     : uchar checksum
*
* Notes     :	 
******************************************************************************************
*/
#if(PROTOCOL_ADD_ORDERS==1)
static void StoreOrdersHistroy ( uchar* buf )
{
    uchar i;
    uchar temp[8];
    
   	//ת���9�ζ������� 11
    for ( i=0; i<9; i++ )
    {
        I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last9_Order)-8*i, 
				&temp[0], 8, 1  ) ;
        
        EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last10_Order)-8*i, 
    		    &temp[0], 8, 1  );
		EEPROM_WP();
		
		Delay1MsTime ( 2 );
		
		FeedWatchdog ( );							
    }
    
    //�洢���1�ζ�����¼���� 11    
    EEPROM_NOWP();
	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_Order), 
		    &buf[0], 8, 1  );
	EEPROM_WP();
	FeedWatchdog ( );
		
    return;
}
#endif
/*
*****************************************************************************************
* fuction name: StoreBuyHistroy
*
* Description : �洢�����¼ 
*
* Arguments   : uchar* buf, uchar len
*		
* returns     : uchar checksum
*
* Notes     :	 
******************************************************************************************
*/
static void StoreBuyHistroy ( uchar* buf )
{
    uchar i;
    uchar temp[18];
    
   	//ת���9�ι������� 11
    for ( i=0; i<9; i++ )
    {
        I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last9_BuyQuan)-18*i, 
				&temp[0], 18, 1  ) ;
        
        EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last10_BuyQuan)-18*i, 
    		    &temp[0], 18, 1  );
		EEPROM_WP();
		
		Delay1MsTime ( 5 );
		
		FeedWatchdog ( );							
    }
    
    //�洢���1�ι����¼���� 11    
    EEPROM_NOWP();
	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1_BuyQuan), 
		    &buf[0], 18, 1  );
	EEPROM_WP();
	FeedWatchdog ( );
		
    return;
}

#endif
/*
*****************************************************************************************
* fuction name: Decrypt
*
* Description : this function decrypt the output data in IC card.
*
* Arguments   : 1)uchar *buf : the data need to decrypt
*		2)uchar pos: the location in IC card
*		3)uchar len: the lenght of data need to decrypt
*		4)uchar fixedChar: the key data
*
* returns     : null 
*
* Notes       :
******************************************************************************************
*/

void Decrypt ( uchar *buf, uchar pos, uchar len, uchar fixedChar )
{
    uchar randnum;
    uchar i;
    uchar addr;
    
    addr = (pos + POSKEY)&0x0f;
    randnum = (buf[0] - SELFKEY[addr])^fixedChar;
    buf[0] = fixedChar;
        
    pos++;      
    for ( i=1; i<len; i++ )
    {
        addr = (pos + POSKEY)&0x0f;
        buf[i] -= SELFKEY[addr]; 
        buf[i] ^= randnum;  
        pos++;         
    }
    
    return;
}
/*
*****************************************************************************************
* fuction name: Encrypt
*
* Description : this function encrypt the output data in IC card.
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

void Encrypt ( uchar *buf, uchar pos, uchar len )
{                
    uchar randnum;
    uchar i;
    uchar tmp;
    uchar addr;
    
    //randnum = ((uchar)rand()&0xff); 11
    randnum = SELFKEY[g_randCnt&0x0f];
          
    for ( i=0; i<len; i++ )
    {
        tmp = buf[i] ^ randnum;
        addr = (pos + POSKEY)&0x0f;
        buf[i] = tmp + SELFKEY[addr]; 
        pos++;               
    }
    
    return;
}
				
/*
*****************************************************************************************
* fuction name: ClearAllData
*
* Description : this function process clear all data in meter
*
* Arguments   : none
*		
* returns     : none
*
* Notes     :	 
******************************************************************************************
*/
void ClearAllData ( uint32 preBuy, uint16 price )
{        
    uchar *pchar;
    uchar i,len,len1;
    uint32 tempL;
    
    //�����¼ 11
	I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,TotalMeterClearCnt), 
 			&g_buf[0], 3, 1  ) ;
  	tempL = BCDToHex( &g_buf[0], 3 );
  	tempL++;
  	tempL = Hex32ToBCD( tempL );
	g_buf[0] = (uchar)tempL&0xff;
	g_buf[1] = (uchar)(tempL>>8)&0xff;
	g_buf[2] = (uchar)(tempL>>16)&0xff;			
	EEPROM_NOWP();
	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,TotalMeterClearCnt), 
 			&g_buf[0], 3, 1  ) ;
	EEPROM_WP();
		
	for ( i=0; i<9; i++ )
	{
		I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last9MeterClear)-18*i, 
				&g_buf[0], 18, 1  ) ;
		
		EEPROM_NOWP();
    	I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last10MeterClear)-18*i, 
    		&g_buf[0], 18, 1  );
		EEPROM_WP();
		
		Delay1MsTime ( 5 );
		
		FeedWatchdog ( );
	}
	g_buf[0] = g_date.year;
	g_buf[1] = g_date.month;
	g_buf[2] = g_date.day;
	g_buf[3] = g_date.hour;
	g_buf[4] = g_date.min;
	g_buf[5] = g_date.sec;
	g_buf[6] = 0x78;
	g_buf[7] = 0x56;
	g_buf[8] = 0x34;
	g_buf[9] = 0x12;
	tempL = Hex32ToBCD( g_energy.PosAccumulate );				
	g_buf[10] = (uchar)tempL&0xff;
	g_buf[11] = (uchar)(tempL>>8)&0xff;
	g_buf[12] = (uchar)(tempL>>16)&0xff;			
	g_buf[13] = (uchar)(tempL>>24)&0xff;
	tempL = Hex32ToBCD( g_energy.NegAccumulate );				
	g_buf[14] = (uchar)tempL&0xff;
	g_buf[15] = (uchar)(tempL>>8)&0xff;
	g_buf[16] = (uchar)(tempL>>16)&0xff;			
	g_buf[17] = (uchar)(tempL>>24)&0xff;
	EEPROM_NOWP();
    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,Last1MeterClear), 
    		&g_buf[0], 18, 1  );
	EEPROM_WP();
	Delay1MsTime ( 5 );
	       
    for ( i=0; i<48; i++ )
    { 
        g_buf[i] = 0;
    }
    
    //EEPROM�������� 11
    //////////////////////////////////////////////////////////////       
    //�������ݱ��������� 11
    len = (uchar)(offsetof(EEPROM_DATA,PSTART1)&0xff);
    len >>= 4;
    for( i=0; i<len; i++ )
    {
        EEPROM_NOWP();
        I2CWrite ( EEPROM_ADDR, 16*((uint16)i), g_buf, 16 ,1 );
        EEPROM_WP(); 
        Delay1MsTime ( 5 );
        FeedWatchdog  (  );        
    }
    
    //�����¼��У������������ 11
    len = (uchar)((offsetof(EEPROM_DATA,Zone)&0xff)>>4);
    len1 = (uchar)(offsetof(EEPROM_DATA,TotalMeterClearCnt)>>4);
    for( i=len; i<len1; i++ )
    {
        EEPROM_NOWP();
        I2CWrite ( EEPROM_ADDR, 16*((uint16)i), g_buf, 16 ,1 );
        EEPROM_WP(); 
        Delay1MsTime ( 5 );
        FeedWatchdog  (  );        
    }
    
    //
    len = (uchar)(offsetof(EEPROM_DATA,Zone_B)>>4);        
    for( i=len; i<EEPROM_PAGES; i++ )
    {
        EEPROM_NOWP();
        I2CWrite ( EEPROM_ADDR, 16*((uint16)i), g_buf, 16 ,1 );
        EEPROM_WP(); 
        Delay1MsTime ( 5 );
        FeedWatchdog  (  );        
    }
    
    //////////////////////////////////////////////////////////////
    
    //ͨѶ��ַ����  11
    pchar = (uchar*)&g_addr.Zone;
    for ( i=0; i<8; i++ )
    { 
        *pchar++ = 0;
    }
    for ( i=0; i<6; i++ )
        if ( ((g_addr.MeterAddr[i]&0x0f) == 0x0f) ||
              ((g_addr.MeterAddr[i]&0xf0) == 0xf0)  )
            break;
    if ( i < 6 )
    {
        for ( i=0; i<8; i++ )
        {
            g_addr.MeterAddr[i] = 0x11; 
        }               
    }
    //����ַ���� 11
    FeedWatchdog ( );
	g_addr.Checksum = GetSum ( (uchar*)&g_addr.Zone, Meter_Addr_Len-1 );
	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Zone),
		(uchar*)&g_addr.Zone,Meter_Addr_Len );
    
    
    //����ȫ������ 11
    pchar = (uchar*)&g_buyPara.BuyCount;
    for ( i=0; i<16; i++ )
    { 
        *pchar++ = 0;
    }    
    g_errStatus.Byte = 0;             
    //�������1 2 11
    g_buyPara.Alarm1 = ALARM1_DEFAULT;
    g_buyPara.Alarm2 = ALARM2_DEFAULT;			    
    //Ԥ�ý�� 11
    g_buyPara.BuyQuan = 0;
    //����������ֵ 11
    g_buyPara.FactorLimit = 1000;
    //����������Ӧ������ֵ 11
	g_buyPara.FactorPower = 0;
    g_buyPara.Checksum = GetSum ( (uchar*)&g_buyPara.BuyCount, Meter_Buy_PARA_Len-1 );
	FeedWatchdog ( );
	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,BuyCount),
		(uchar*)&g_buyPara.BuyCount,Meter_Buy_PARA_Len );
					
    
    //����Ԥ�� 11
    g_para.LimitPower = MAX_POWER_LIMIT;
    g_para.MaximBalance = MAX_BALANCE_LIMIT;
    g_para.CreditLimit = CREDIT_LIMIT;
    g_para.CurrentRatio = CURRENT_RATIO;   
    g_para.Price = price;
    g_para.BonusMoneyNum = 1;
    g_para.MeterTpye = 0; //���ñ����۵�ģʽ ������11
    g_meterStatus3.Bit.KeepRelay = 0; 
    g_meterStatus3.Bit.RelayCmdStatus = 0;
    g_meterStatus3.Bit.StatusMeter = 0;		//�˷�״̬ 11
    g_para.Checksum = GetSum ( (uchar*)&g_para.LimitPower, Meter_Para_Len-1 );
	FeedWatchdog ( );
	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LimitPower),
		(uchar*)&g_para.LimitPower,Meter_Para_Len );
           
    //������� 11
    pchar = (uchar*)&g_energy.Accumulate;
    for ( i=0; i<Meter_EQuan_Len; i++ ) 
        *pchar++ = 0;
    g_price = price;
    g_energy.Balance = preBuy;	 
    g_energy.PrePaid = preBuy;  
    
#ifdef INACITVE_POWER_ENABLE 
	pchar = (uchar*)&g_Inactivenergy.InactiveQuan;
    for ( i=0; i<Meter_InactiveQuan_Len; i++ ) 
        *pchar++ = 0;
#endif
 
    SaveEnergy( NORMAL_SAVE );
    
    //�����ʼ��Ϊ02 000000
	FeedWatchdog ( );
    for( i=0; i<sizeof(INIT_PASSWORD_TABLE); i++ )
    	g_buf[i] = INIT_PASSWORD_TABLE[i];
    g_buf[i] = 	GetSum ( (uchar*)&g_buf[0], sizeof(INIT_PASSWORD_TABLE) );
    StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,PASSWORD),
		(uchar*)&g_buf[0],i+1 );
	
	//��ʼ�������汾�� 11
	FeedWatchdog ( );
    for( i=0; i<sizeof(INIT_SOFTWARE_VERSION); i++ )
    	g_buf[i] = INIT_SOFTWARE_VERSION[i];
    EEPROM_NOWP();
    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,SoftVersion), g_buf, 
    			sizeof(INIT_SOFTWARE_VERSION) ,1 );
    EEPROM_WP(); 
			         
#ifdef TARIFF_ENABLE
	//���ʵ����Ϣ
	FeedWatchdog ( );
    for( i=0; i<sizeof(INIT_PRICE_TABLE); i++ )
    	g_buf[i] = INIT_PRICE_TABLE[i];
    g_buf[i] = 	GetSum ( (uchar*)&g_buf[0], sizeof(INIT_PRICE_TABLE) );
    StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,TariffPrice1),
		(uchar*)&g_buf[0],i+1 );
  		    
	//ʱ�� ���ʳ�ʼ�� 11
    //��ʱ���� 11
    FeedWatchdog ( );
	for( i=0; i<sizeof(INIT_YEAR_ZONE_TABLE); i++ )
    	g_buf[i] = INIT_YEAR_ZONE_TABLE[i];
    g_buf[i] = 	GetSum ( (uchar*)&g_buf[0], sizeof(INIT_YEAR_ZONE_TABLE) );

    EEPROM_NOWP();
    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,YearZoneNum), 
                    g_buf, i+1, 1  );
	EEPROM_WP();
    
    
	//��1ʱ�α� ����ʱ�α� 12 13 14 
	FeedWatchdog ( );
	for( i=0; i<sizeof(INIT_TARIFF_TBL1); i++ )
    	g_buf[i] = INIT_TARIFF_TBL1[i];
    g_buf[i] = 	GetSum ( (uchar*)&g_buf[0], sizeof(INIT_TARIFF_TBL1) );

    EEPROM_NOWP();
    I2CWrite ( EEPROM_ADDR, offsetof(EEPROM_DATA,HHMMNN1), 
                    g_buf, i+1, 1  );
	EEPROM_WP();	
#endif



#ifdef LADDER_ENABLE	
	//����ֵ��Ϣ
	FeedWatchdog ( );
	for( i=0; i<sizeof(INIT_LADDER_VALUE_TBL); i++ )
    	g_buf[i] = INIT_LADDER_VALUE_TBL[i];
    g_buf[i] = 	GetSum ( (uchar*)&g_buf[0], sizeof(INIT_LADDER_VALUE_TBL) );
	
	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,Ladder1),
 					 (uchar*)&g_buf[0],i+1 );	
 					 	
	//���ݵ����Ϣ
	FeedWatchdog ( );
	for( i=0; i<sizeof(INIT_LADDER_PRICE_TBL); i++ )
    	g_buf[i] = INIT_LADDER_PRICE_TBL[i];
    g_buf[i] = 	GetSum ( (uchar*)&g_buf[0], sizeof(INIT_LADDER_PRICE_TBL) );
	
	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,LadderPrice1),
 					 (uchar*)&g_buf[0],i+1 );	
 	
 	//�������� Ĭ�ϰ��� 11
 	g_buf[0] = 0x00;
 	g_buf[1] = 0x01;		//�� 11
 	g_buf[2] = 0x18;
 	g_buf[3] = 0x05;		//�� 11
 	g_buf[4] = 0x01;		//�� 11	
 	g_buf[5] = 0x18;
 	g_buf[6] = 0x05;		//�� 11
 	g_buf[7] = 0x01;		//�� 11
 	for( i=8; i<15; i++ )
    	g_buf[i] = 0x00;
 	//У��� 11
	g_buf[15] = GetSum ( &g_buf[0], 15 );
	StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,YYMM),
		 (uchar*)&g_buf[0],16 );			 		
#endif
	
	//����ʣ�������� 11
	g_relayFlag.Bit.RelayNeedToOperate = 1;   
	if ( g_energy.Balance > 0 )		
	    g_relayFlag.Bit.RelayOn = 0;	
	else
	    g_relayFlag.Bit.RelayOn = 1;	
	
		
	            
    return;   
	           	      
}
		
				