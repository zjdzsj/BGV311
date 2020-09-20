/******************************************************************************************
*  Copyright (c) 2013,  R&D Department
*  All rights reserved 
*
*  FileName：   	RN8302.c
*  Author：     	Roger
*  Date：  			2013-3-8 20:10
*  Revised Date：  	
*  Main Function：  this file contains all the functions used in I2C communications.
*  Version:			V1.0
*  Note： 			 			
*********************************************************************************************/
//



#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

#define _RN8302_DEFINE_
#include "include.h"

#if(METER_CHIP_MODEL==0x8302)
//variables statement
/******************************************************************************************/

static const uchar CALI_REG_ORDER[]={
	
	//0x00,			//HFCONST1 02
	//0x02,			//IStart_PS 02
	
	//0x06,			//PRTH1L 2
	//0x07,			//PRTH1H 2
	//0x08,			//PRTH2L 2
	//0x09,			//PRTH2H 2
	
	//0x0c,			//PHSUA	1
	//0x0d,			//PHSUB	1
	//0x0e,			//PHSUC	1
	
	//0x0F,			//PHSIA	3
	//0x10,			//PHSIB	3
	//0x11,			//PHSIC	3
	
	0x13,			//GSUA	2
	0x14,			//GSUB	2
	0x15,			//GSUC	2
	
	0x16,			//GSIA	2
	0x17,			//GSIB	2
	0x18,			//GSIC	2
	
	//0x28,			//GPA 	2
	//0x29,			//GPB	2
	//0x2A,			//GPC	2
	
	0x31,			//PA_PHS	2
	0x32,			//PB_PHS	2
	0x33,			//PC_PHS	2
	
	0x24,			//IA_OS	2
	0x25,			//IB_OS	2
	0x26,			//IC_OS	2
	
};


static uchar s_step;		//读取2208控制字节   11 
/******************************************************************************************/

//function statement
/******************************************************************************************/

/******************************************************************************************/



/*
*****************************************************************************************
* fuction name: SetRN8302_Register
*
* Description : this function initialize the cl2208 chip
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
uchar SetRN8302_Register ( void )
{
	uchar i,ret;
	uchar buf[4];
	uint16 *pint;
	
	FeedWatchdog ( );
	buf[0] = 0x00;
	buf[3] = 0x99;
	//通讯检测 读取DEVICE ID 8FH寄存器 830200 11
	ret = SPIRead_REG ( DEVICE_ID, buf, 3, 0x01 );
	if ( ret != 0 )
	{
		buf[3]=0x01;
		DisplayInputNumber ( buf  );
		for ( i=0; i<6; i++ )
		{	    	    
			Delay1MsTime ( 200 );
		    FeedWatchdog  (  );	    
		}
		
		return 1;
	}
			
	if ( (buf[0]!=0x83) || (buf[1]!=0x02) || (buf[2]!=0x00) )
	{
		buf[3]=0x02;
		DisplayInputNumber ( buf );
		for ( i=0; i<6; i++ )
		{	    	    
			Delay1MsTime ( 200 );
		    FeedWatchdog  (  );	    
		}
		
		return 1;
	}
	
	//写使能 11
	ret = Write_Register_CMD(WREN, WRITE_ENABLE);
	if ( ret != 0 )
		return 1;	
	Delay1MsTime( 2 );
	buf[0] = 0x00;
	//读取是否使能成功 11
	ret = SPIRead_REG ( WREN, buf, 1, 1 );
	if ( ret != 0 )
		return 1;	
	if ( buf[0] != 0x01 )
	{
		buf[3]=0x03;
		DisplayInputNumber ( buf  );
		for ( i=0; i<6; i++ )
		{	    	    
			Delay1MsTime ( 200 );
		    FeedWatchdog  (  );	    
		}
		return 1;
	}
	
	FeedWatchdog ( );
	Delay1MsTime ( 2 );
	//工作模式切换为计量模式emm 11
	do
	{
		ret = Write_Register_CMD(MODE_CHOOSE, 0xa2);
		if ( ret != 0 )
			return 1;	
		
		Delay1MsTime ( 300 );
		
		buf[0] = 0x00;
		ret = SPIRead_REG ( MODE_CHOOSE, buf, 1, 1 ); 	
		if ( ret != 0 )
			return 1;
		Delay1MsTime ( 2 );
				
	}while ( buf[0] != 0x01 ); //判断是否成功切换 11
	
		
	
//	//延时200ms 11
//	Delay1MsTime( 300 );
//	FeedWatchdog ( );
//	//延时200ms 11
	
	
	//读取软件复位是否成功 11
	for( i=0; i<5; i++)
	{
		//软复位 11
//		ret = Write_Register_CMD(SOFTRESET, RESET_CMD);
//		if ( ret != 0 )
//			return 1;
//		FeedWatchdog ( );
//		//延时200ms 11
//		Delay1MsTime( 300 );
//		FeedWatchdog ( );
		//软复位 11
		ret = Write_Register_CMD(SOFTRESET, RESET_CMD);
		if ( ret != 0 )
			return 1;
				
		Delay1MsTime( 300 );
		FeedWatchdog ( );
		Delay1MsTime( 300 );
		FeedWatchdog ( );
		Delay1MsTime( 300 );
		FeedWatchdog ( );
		Delay1MsTime( 300 );
		FeedWatchdog ( );	
//		Delay1MsTime( 300 );
//		FeedWatchdog ( );
//		
//		Delay1MsTime( 300 );
//		FeedWatchdog ( );	
//		Delay1MsTime( 300 );
//		FeedWatchdog ( );
		
//		ret = SPIRead_REG ( SOFTRESET, buf, 1, 1 );
//		if ( ret != 0 )
//			return 1;	
//		if ( ((buf[0]&0x40) == 0x00)  )
//		{		
//			
//		}
			
		ret = SPIRead_REG ( 0x8A, buf, 2, 1 );
		if ( ret != 0 )
			return 1;	
		if ( ((buf[0]&0x40) == 0x40) &&
		        ((buf[1]&0x04) == 0x04) )
		{		
			break;
		}
		
	}
	if ( i == 5 )
	{				
		buf[3]=0x04;
		DisplayInputNumber ( buf  );
		for ( i=0; i<6; i++ )
		{	    	    
			Delay1MsTime ( 200 );
		    FeedWatchdog  (  );	    
		}
		return 1;
	}
	Delay1MsTime ( 2 );
	
//	//关写使能 11
//	ret = Write_Register_CMD(WREN, WRITE_DISABLE);
//	if ( ret != 0 )
//		return 1;	
//	//Delay1MsTime( 2 );
//	buf[0] = 0xff;
//	//读取是否关闭使能 11
//	ret = SPIRead_REG ( WREN, buf, 1, 1 );
//	if ( ret != 0 )
//		return 1;	
//	if ( buf[0] != 0x00 )
//		return 1;	

		 	
	//读取脉冲常数是否成功 11
	//读取软件复位是否成功 11
	for( i=0; i<5; i++)
	{
		//写脉冲常数 11
		buf[0] = HFCONST_H;
		buf[1] = HFCONST_L;
		ret = SPIWrite_REG (HFCONST1,buf, 2, 0x01);
		if ( ret != 0 )
			return 1;
		Delay1MsTime( 3 );
		FeedWatchdog ( );	
//		Delay1MsTime( 300 );
//		FeedWatchdog ( );
		
		buf[0] = 0x00;
		buf[1] = 0x00;
		ret = SPIRead_REG ( HFCONST1, buf, 2, 1 );
		if ( ret != 0 )
			return 1;	
		
		if ( (buf[0]==HFCONST_H) &&
		        (buf[1]==HFCONST_L) )
		{		
			break;
		}
		
		Delay1MsTime ( 200 );
		FeedWatchdog  (  );	
	}
	if ( i== 5 )
	{
		buf[3]=0x05;
		DisplayInputNumber ( buf  );
		for ( i=0; i<6; i++ )
		{	    	    
			Delay1MsTime ( 200 );
		    FeedWatchdog  (  );	    
		}		
			
		return 1;
	}
	
	//三相三线模式设置 11
#if ( CAL_RN8302_MODE == 33 )
	FeedWatchdog ( );
	Delay1MsTime ( 2 );
	//写起动阀值 11
	buf[0] = 0x33;
	ret = SPIWrite_REG (0x86, buf, 1, 0x01);
	if ( ret != 0 )
		return 1;
	//读模式设置 11
	buf[0] = 0x00;
	ret = SPIRead_REG ( 0x86, buf, 1, 1 );
	if ( ret != 0 )
		return 1;	
	if ( (buf[0]!=0x01) )
	{		
		buf[3]=0x09;
		DisplayInputNumber ( buf  );
		for ( i=0; i<6; i++ )
		{	    	    
			Delay1MsTime ( 200 );
		    FeedWatchdog  (  );	    
		}
		
		return 1;
	}
#endif
	
	FeedWatchdog ( );
	Delay1MsTime ( 2 );
	//写起动阀值 11
	buf[0] = ISTART_H;
	buf[1] = ISTART_L;
	ret = SPIWrite_REG (ISTART,buf, 2, 0x01);
	if ( ret != 0 )
		return 1;
	//读取起动阀值 11
	buf[0] = 0x00;
	buf[1] = 0x00;
	ret = SPIRead_REG ( ISTART, buf, 2, 1 );
	if ( ret != 0 )
		return 1;	
	if ( (buf[0]!=ISTART_H) ||
	        (buf[1]!=ISTART_L) )
	{		
		buf[3]=0x06;
		DisplayInputNumber ( buf  );
		for ( i=0; i<6; i++ )
		{	    	    
			Delay1MsTime ( 200 );
		    FeedWatchdog  (  );	    
		}
		
		return 1;
	}
	
	//配置脉冲口 11
	FeedWatchdog ( );
	Delay1MsTime ( 2 );
	//通道使能 11
	buf[0] = 0x77;
	buf[1] = 0x77;
	buf[2] = 0x77;
	ret = SPIWrite_REG (EMUCON,buf, 3, 0x01); 
	if ( ret != 0 )
		return 1;
	//读取起动阀值 11
	buf[0] = 0x00;
	buf[1] = 0x00;
	buf[2] = 0x00;
	ret = SPIRead_REG ( EMUCON, buf, 3, 1 );
	if ( ret != 0 )
		return 1;	
	if ( (buf[0]!=0x77) ||
	        (buf[1]!=0x77) ||
	        (buf[2]!=0x77) )
	{		
		buf[3]=0x07;
		DisplayInputNumber ( buf  );
		for ( i=0; i<6; i++ )
		{	    	    
			Delay1MsTime ( 200 );
		    FeedWatchdog  (  );	    
		}
		
		return 1;
	}
	
	FeedWatchdog ( );   
    //获取校表参数 11
	if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1), 
			(uchar*)&g_rn8302para.gsua,CHECK_CL2208,32 ) != 0 )
	{
		if ( g_rn8302para.sum!=GetSum ( (uchar*)&g_rn8302para.gsua, 27 ) )
		{
			ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1_B), 
				(uchar*)&g_rn8302para.gsua,CHECK_CL2208,32 ); 
			
			if ( g_rn8302para.sum!=GetSum ( (uchar*)&g_rn8302para.gsua, 27 ) )
			{
				g_errStatus.Bit.E2ROM = 1;
			}					
		}				
	}
	
	//写校表参数 11
	pint = (uint16*)&g_rn8302para.gsua;
	for ( i=0; i<12; i++ )
	{
		buf[1] = (uchar)(*pint);
		buf[0] = (uchar)(*pint>>8);
		ret = SPIWrite_REG ( CALI_REG_ORDER[i], buf, 2, 0x01 );
		if ( ret != 0 )
			return 1;	
		
		Delay1MsTime( 2 );
		FeedWatchdog ( );	
		
		//读取起动阀值 11
    	buf[0] = 0x00;
    	buf[1] = 0x00;
    	ret = SPIRead_REG ( CALI_REG_ORDER[i], buf, 2, 1 );
    	if ( ret != 0 )
    		return 1;	
    	if ( (buf[1]!=(uchar)(*pint)) ||
    	        (buf[0]!=(uchar)(*pint>>8)) )
    	{		
			buf[2]=i;
			buf[3]=0x08;
			DisplayInputNumber ( buf  );
			for ( i=0; i<6; i++ )
			{	    	    
				Delay1MsTime ( 200 );
			    FeedWatchdog  (  );	    
			}
    		return 1;
    	}
    	
		pint++;	
		Delay1MsTime ( 2 );					
	}
	
	g_freqency = 5002;	
	
	return 0;
	
}

/*
*****************************************************************************************
* fuction name: CaliGainPara
*
* Description : this function initialize the cl2208 chip
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
uchar CaliGainPara ( void )
{
	uchar i,buf[4];
	uint16 invalue;
	float err,gain;
	uint32 templong;
	uint16 *pint1,*pint2;
	
	pint1 = (uint16*)&g_rn8302para.gsua;	
	pint2 = (uint16*)&g_rn8302para.gsia;		
	for ( i=0; i<3; i++ )
	{
	
	#if ( CAL_RN8302_MODE == 33 )	
		if ( i == 1 )
		{
			*pint1 = 0;
			pint1++;	
			*pint2 = 0;
			pint2++;
			
			continue;
		}
	#endif
		
		FeedWatchdog ( );
		//读取电压有效值寄存器 11
		if ( SPIRead_REG ( UA_RMS+i, buf, 4, 0x00 ) != 0 )
			return 1;
		
		templong = buf[0] & 0xff;
		templong = (templong<<8) | (buf[1] & 0xff);
		templong = (templong<<8) | (buf[2] & 0xff);	
		templong = (templong<<8) | (buf[3] & 0xff);	
		
		if ( templong == 0 )
			return 1;
		
		//获取电压通道误差 11
		err = (float)((float)templong - U_RMS_VALUE)/U_RMS_VALUE;	
		
		gain = (-1.0)*err/(1.0+err);
		
		if ( gain > 0 )
			invalue = (uint16)(32768*gain);
		else
			invalue = (uint16)(65536+32768*gain);
		
		FeedWatchdog ( );
		//写电压通道增益 11
		buf[1] = (uchar)(invalue);
		buf[0] = (uchar)(invalue>>8);
		if ( SPIWrite_REG ( GSUA+i, buf, 2, 0x01 ) != 0 )
			return 1;	
		
		*pint1 = invalue;
		pint1++;
		
		///////////////////////////////////////////////////////////////	
		//读取电流有效值寄存器 11
		if ( SPIRead_REG ( IA_RMS+i, buf, 4, 0x00 ) != 0 )
			return 1;
		
		templong = buf[0] & 0xff;
		templong = (templong<<8) | (buf[1] & 0xff);
		templong = (templong<<8) | (buf[2] & 0xff);	
		templong = (templong<<8) | (buf[3] & 0xff);	
		
		if ( templong == 0 )
			return 1;
			
		//获取电流通道误差 11
		err = ((float)templong - I_RMS_VALUE)/I_RMS_VALUE;	
		
		gain = (-1.0)*err/(1.0+err);
		
		if ( gain > 0 )
			invalue = (uint16)(32768*gain);
		else
			invalue = (uint16)(65536+32768*gain);
		
		FeedWatchdog ( );
		//写电流通道增益 11
		buf[1] = (uchar)(invalue);
		buf[0] = (uchar)(invalue>>8);
		if ( SPIWrite_REG ( GSIA+i, buf, 2, 0x01 ) != 0 )
			return 1;	
			
		*pint2 = invalue;
		pint2++;
		
	}	
	
	FeedWatchdog ( );
	//保存校表数据 11
	if ( StoreCaliPara ( ) != 0 )
		return 1;
								
	return 0;
	
}

/*
*****************************************************************************************
* fuction name: CaliPhasePara
*
* Description : this function initialize the cl2208 chip
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
uchar CaliPhasePara ( void )
{
	uchar i,buf[4];
	uint16 invalue;
	float err,gain;
	uint32 templong;	
	uint16 *pint;
	
	pint = (uint16*)&g_rn8302para.pa_phs;		
	for ( i=0; i<3; i++ )
	{
	#if ( CAL_RN8302_MODE == 33 )	
		if ( i == 1 )
		{
			*pint = 0;
			pint++;				
			continue;
		}
	#endif
		
		//读取功率寄存器 11
		if ( SPIRead_REG ( PA_RMS+i, buf, 4, 0x00 ) != 0 )
			return 1;
		
		templong = buf[0] & 0xff;
		templong = (templong<<8) | (buf[1] & 0xff);
		templong = (templong<<8) | (buf[2] & 0xff);	
		templong = (templong<<8) | (buf[3] & 0xff);	
				
		if ( buf[0] & 0x80 )
		{
			templong = ~templong + 1;					
		}
	
	#if ( CAL_RN8302_MODE == 33 )	
		gain = (float)templong;
		if ( gain < (P_0_5RMS_VALUDE/2) )
		{
			*pint = 0;
			pint++;				
			continue;
		}
	#endif
				
		//获取相位通道误差 11
		err = (float)((float)templong - P_0_5RMS_VALUDE)/P_0_5RMS_VALUDE;	
		
		gain = (-1.0)*err/(1.732);
		
		if ( gain > 0 )
			invalue = (uint16)(32768*gain);
		else
			invalue = (uint16)(65536+32768*gain);
		
		//写相位通道校正值 11
		buf[1] = (uchar)(invalue);
		buf[0] = (uchar)(invalue>>8);
		if ( SPIWrite_REG ( PA_PHS+i, buf, 2, 0x01 ) != 0 )
			return 1;	
			
		*pint = invalue;
		pint++;
		FeedWatchdog ( );
	}
	
	//保存校表数据 11
	if ( StoreCaliPara ( ) != 0 )
		return 1;
		
	return 0;
	
}

/*
*****************************************************************************************
* fuction name: CaliOffsetPara
*
* Description : this function initialize the cl2208 chip
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
uchar CaliOffsetPara ( void )
{
	uchar i,j,buf[4];
	uint16 invalue;
	uint32 templong,templong2;
	uint16 *pint;
	
	pint = (uint16*)&g_rn8302para.ia_os;
	for ( i=0; i<3; i++ )
	{
	
	#if ( CAL_RN8302_MODE == 33 )	
		if ( i == 1 )
		{
			*pint = 0;
			pint++;				
			continue;
		}
	#endif	
		
		templong2 = 0;
		//读取电流寄存器 11
		for ( j=0; j<10; j++ )
		{
			if ( SPIRead_REG ( IA_RMS+i, buf, 4, 0x00 ) != 0 )
				return 1;
		
			templong = buf[0] & 0xff;
			templong = (templong<<8) | (buf[1] & 0xff);
			templong = (templong<<8) | (buf[2] & 0xff);	
			templong = (templong<<8) | (buf[3] & 0xff);	
		
			templong2 += templong;
			
			//延时处理	 11	
			for ( i=0; i<2; i++ )
			{	    	    
				Delay1MsTime ( 200 );
			    FeedWatchdog  (  );	    
			}
		}
		
		templong2 /= 10;
		templong = templong2*templong2;
		
		templong >>= 14;
		
		invalue =(uint16)(~(templong&0xffff)+1);
		invalue |= 0x8000;
				
		//写电流通道offset校正值 11
		buf[1] = (uchar)(invalue);
		buf[0] = (uchar)(invalue>>8);
		if ( SPIWrite_REG ( IA_OS+i, buf, 2, 0x01 ) != 0 )
			return 1;
			
		*pint = invalue;
		pint++;	
		FeedWatchdog ( );
	}
		
	//保存校表数据 11
	if ( StoreCaliPara ( ) != 0 )
		return 1;
			
	return 0;
	
}




		
/*
*****************************************************************************************
* fuction name: GetRegLen
*
* Description : this function initialize the cl2208 chip
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
uchar GetRegLen ( uchar reg, uchar bank )
{
	uchar len;
	
	if ( bank > 1 )
		return 0;
		
	if ( bank == 0 )
	{
		if ( reg <= 0x06 )
		{
			len = 3;
		}
		else if ( reg <= 0x10 )
		{
			len = 4;		
		}
		else if ( reg == 0x13 )
		{
			len = 3;		
		}
		else if ( reg <= 0x1f )
		{
			len = 4;		
		}
		else if ( reg <= 0x58 )
		{
			len = 3;		
		}
		else if ( reg <= 0x69 )
		{
			len = 4;		
		}	
		else if ( reg == 0x95 )
		{
			len = 3;		
		}
		else if ( reg == 0x9c )
		{
			len = 4;		
		}
		else if ( reg == 0x9f )
		{
			len = 3;		
		}
	}
	else if ( bank == 1 )
	{
		if ( reg <= 0x0b )
		{
			len = 2;
		}
		else if ( reg <= 0x0e )
		{
			len = 1;		
		}
		else if ( reg == 0x11 )
		{
			len = 3;		
		}
		else if ( reg <= 0x12 )
		{
			len = 1;		
		}
		else if ( reg <= 0x57 )
		{
			len = 2;		
		}
		else if ( reg <= 0x58 )
		{
			len = 3;		
		}	
		else if ( reg <= 0x5a )
		{
			len = 2;		
		}
		else if ( reg <= 0x62 )
		{
			len = 3;		
		}
		else if ( reg <= 0x63 )
		{
			len = 1;		
		}
		else if ( reg <= 0x67 )
		{
			len = 2;		
		}
		else if ( reg <= 0x68 )
		{
			len = 1;		
		}
		else if ( reg <= 0x69 )
		{
			len = 2;		
		}
		else if ( reg <= 0x6a )
		{
			len = 3;		
		}
		else if ( reg <= 0x71 )
		{
			len = 1;		
		}
		else if ( reg <= 0x75 )
		{
			len = 2;		
		}
		else if ( reg <= 0x82 )
		{
			len = 1;		
		}
		else if ( reg <= 0x83 )
		{
			len = 2;		
		}
		else if ( reg <= 0x86 )
		{
			len = 1;		
		}
		else if ( reg <= 0x8b )
		{
			len = 2;		
		}
		else if ( reg <= 0x8c )
		{
			len = 4;		
		}
		else if ( reg <= 0x8d )
		{
			len = 3;		
		}
		else if ( reg <= 0x8e )
		{
			len = 2;		
		}
		else if ( reg <= 0x8f )
		{
			len = 3;		
		}
	}
	return len;
	
}		

/*
*****************************************************************************************
* fuction name: GetRN8302ParaAddr
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
uint16 GetRN8302ParaAddr ( uchar reg )
{
	uchar i=255;
	
	for ( i=0; i<sizeof(CALI_REG_ORDER)-1; i++ )
	{
		if ( CALI_REG_ORDER[i] == reg )
		{
			break;	
		}
	}
	return i;
}
/*
*****************************************************************************************
* fuction name: StoreCaliPara
*
* Description : this function initialize the cl2208 chip
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
uchar StoreCaliPara ( void )
{
	uchar buf[4],i,buf2[4];
	
	//延时处理	 11	
	for ( i=0; i<4; i++ )
	{	    	    
		Delay1MsTime ( 100 );
	    FeedWatchdog  (  );	    
	}
	
	//获取校验和 11
	if ( GetRN8302CheckSum ( &buf[0] ) != 0 )
	{		
		return 1;	
	}	
	
	Delay1MsTime ( 100 );
	FeedWatchdog  (  );		
	//获取校验和 11
	if ( GetRN8302CheckSum ( &buf2[0] ) != 0 )
	{		
		return 1;	
	}	
	
	if ( (buf[0]!=buf2[0]) ||
			(buf[1]!=buf2[1]) ||
			(buf[2]!=buf2[2]) )
	{
		return 1;	
	}	
		
	g_rn8302para.checksum[0] = buf[0];
	g_rn8302para.checksum[1] = buf[1];
	g_rn8302para.checksum[2] = buf[2];
	
	g_rn8302para.sum = GetSum ( (uchar*)&g_rn8302para.gsua, 27 );
	
	FeedWatchdog ( );
	//保存校表数据 11
	for ( i=0; i<2; i++ )
	{	    	    
		if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1)+16*i,
			(uchar*)&g_rn8302para.gsua+16*i,16 ) != 0 )
			return 1;		    
	}
					
	return 0;
	
}	

/*
*****************************************************************************************
* fuction name: CheckRN8302Reg
*
* Description : this function initialize the cl2208 chip
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
uchar CheckRN8302Reg ( void )
{

	uchar buf[4];
		
	//获取计量数据校验和 11
	//获取校验和 11
	if ( GetRN8302CheckSum ( &buf[0] ) != 0 )
	{
		return 1;	
	}
	
	//
	//判断是否相等 11
	if ( (g_rn8302para.checksum[0] != buf[0]) ||
			(g_rn8302para.checksum[1] != buf[1]) ||
			(g_rn8302para.checksum[2] != buf[2]) )
	{
		//判断RAM中数据是否合法 11
		if ( g_rn8302para.sum!=GetSum ( (uchar*)&g_rn8302para.gsua, 27 ) )
		{			
			//先更新RAM中数据 11
			if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1), 
					(uchar*)&g_rn8302para.gsua,CHECK_CL2208,32 ) != 0 )
			{
				if ( g_rn8302para.sum!=GetSum ( (uchar*)&g_rn8302para.gsua, 27 ) )
				{
					ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1_B), 
					(uchar*)&g_rn8302para.gsua,CHECK_CL2208,32 ); 					
				}				
			}
		
		}
		
		//重新初始化芯片 11
		SetRN8302_Register ( );		
		
		Delay1MsTime ( 5 );
		
		if ( GetRN8302CheckSum ( &buf[0] ) != 0 )
		{
			return 1;	
		}	
		
		if ( (g_rn8302para.checksum[0] != buf[0]) ||
			(g_rn8302para.checksum[1] != buf[1]) ||
			(g_rn8302para.checksum[2] != buf[2]) )
			return 1;
		
	}
			
	return 0;
	
}

/*
*****************************************************************************************
* fuction name: ReadRmsValue
*
* Description : this function initialize the cl2208 chip
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
void ReadRmsValue ( void )
{	
	uchar i,buf[4],ret;
	uint32 tempL;
#if(MODBUS_ENABLED==1)	
	uint32 tempLSum;
	tempLSum = 0;
#endif	
	if ( g_relayCnt > 0 )
	    return;
	
	switch ( s_step )
	{
	//读电压 11
	case 0:
		{
			for ( i=0; i<3; i++ )
			{
				FeedWatchdog ( );
				ret = SPIRead_REG ( UA_REG+i, buf, 4, 0x00 );
				if ( ret == 0 )
				{
    				tempL = ((uint32)buf[0]<<24)+((uint32)buf[1]<<16)+((uint32)buf[2]<<8)+buf[3];
    				
    				tempL = (uint32)(((float)tempL/KV));
    				
    				//20V 
    				if ( tempL < 200 )
    				    tempL = 0;
    				    		 		
    				g_uip[i].URms = Hex32ToBCD( tempL );	
    			}
    			else
    		    {

    		        break;   
    		    }
			}	
			
						
		}
		s_step++;
		
		//获取功率当前放向 11
		FeedWatchdog ( );
		if ( SPIRead_REG ( DIRECTION_REQ, buf, 2, 0x01 ) != 0 )
			return;	
		g_dirFlag.Byte = buf[0];
		
		break;	
	//读电流 11	
	case 1:
		{
			for ( i=0; i<3; i++ )
			{
				FeedWatchdog ( );
				if ( SPIRead_REG ( IA_RMS+i, buf, 4, 0x00 ) != 0 )
					return;	
				
				tempL = ((uint32)buf[0]<<24)+((uint32)buf[1]<<16)+((uint32)buf[2]<<8)+buf[3];
				
				tempL = (uint32)(((float)tempL/KI));	 
				
				//5A 3位小数 11
				//if ( tempL < 15 ) 
				if ( tempL < START_CURRENT ) 		//调整为50mA以下为0 2018/05/17 11      
				    tempL = 0;
			#if(MODBUS_ENABLED==1)		
				tempLSum += tempL;
			#endif	
				g_uip[i].IRms = Hex32ToBCD( tempL );	
			}	 		
		}
	
	#if(MODBUS_ENABLED==1)	
		g_uip[3].IRms = Hex32ToBCD( tempLSum );
	#endif
	
		s_step++;
		break;	
	//读功率 11
	case 2:
		{
			for ( i=0; i<4; i++ )		//修改2015/10/17 11
			{
				FeedWatchdog ( );
				if ( SPIRead_REG ( PA_RMS+i, buf, 4, 0x00 ) != 0 )
					return;	
				
				tempL = ((uint32)buf[0]<<24)+((uint32)buf[1]<<16)+((uint32)buf[2]<<8)+buf[3];
				
				if ( buf[0] & 0x80 )
				{
					tempL = ~tempL + 1;					
				}
				
				tempL = (uint32)(((float)tempL/KP));
				
				//4.4w  2w 11
				//if ( tempL < 20 )
				if ( tempL < START_POWER )		//调整为5W 2018/05/17 11
				{
					tempL = 0;	
				    g_uip[i].IRms = 0;		//电流调整为0  11	
				}	
				    	 		
				g_uip[i].PRms = Hex32ToBCD( tempL );	
			}	 	 			
				
		}
		s_step++;
		break;
		
	//其他 11
	case 3:
		{
		    //功率因数 11
		    for ( i=0; i<4; i++ )		
			{
				FeedWatchdog ( );
				if ( SPIRead_REG ( PFA_RMS+i, buf, 3, 0x00 ) != 0 )
					return;	
				
				tempL = ((uint32)buf[0]<<16)+((uint32)buf[1]<<8)+buf[2];
				
				if ( buf[0] & 0x80 )
				{
					tempL |= 0xff000000;
					tempL = ~tempL + 1;									
				}
				
				tempL >>= 10;				
				tempL *= 1000;
				tempL >>= 13;
													 		
				g_uip[i].Factor = (uint16)Hex32ToBCD( tempL );	
				if ( g_uip[i].PRms < 0x40 )
				    g_uip[i].Factor = 0x1000;  
				if ( g_uip[i].Factor > 0x1000 )
					g_uip[i].Factor = 0x1000;  
			}
		
			//相角计算 11
		#if(MODBUS_ENABLED==1)	
			for ( i=0; i<3; i++ )		
			{
				FeedWatchdog ( );
				if ( SPIRead_REG ( PHASEA_ANGEL+i, buf, 3, 0x00 ) != 0 )
					return;	
				
				tempL = ((uint32)buf[0]<<16)+((uint32)buf[1]<<8)+buf[2];
				
				if ( tempL < 500 )	
					tempL = 0;
				else
				{
					tempL >>= 8;	//2			
					tempL *= 36000;
					tempL >>= 16;		//2位小数 11	
				}			
																 		
				g_uip[i].PhaseAngel = (uint16)Hex32ToBCD( tempL );	 
			}
		
		#endif	
			//恶性负载判断 11	
		#if(MALIGNANT_LOAD_CHK_EANBLED==1)	
			
		#endif
		
			g_2208ErrCnt = 0;	
		}
	
	#ifdef INACITVE_POWER_ENABLE
	    s_step++;
    #else
		s_step = 0;
	#endif
		break;

#ifdef INACITVE_POWER_ENABLE		
	//无功功率11
	case 4:
		{
		    for ( i=0; i<4; i++ )		//修改2015/10/17 11
			{
				FeedWatchdog ( );
				if ( SPIRead_REG ( QA_RMS+i, buf, 4, 0x00 ) != 0 )
					return;	
				
				tempL = ((uint32)buf[0]<<24)+((uint32)buf[1]<<16)+((uint32)buf[2]<<8)+buf[3];
				
				if ( buf[0] & 0x80 )
				{
					tempL = ~tempL + 1;					
				}
				
				tempL = (uint32)(((float)tempL/KQ));
				
				//4.4w  2w 11
				if ( tempL < START_POWER )
				    tempL = 0;		 		
				g_uip[i].QRms = Hex32ToBCD( tempL );	
			}	 	 						
		}
		s_step++;
		break;	
	
	//视在功率11
	case 5:
		{
		    for ( i=0; i<4; i++ )		//修改2015/10/17 11
			{
				FeedWatchdog ( );
				if ( SPIRead_REG ( SA_RMS+i, buf, 4, 0x00 ) != 0 )
					return;	
				
				tempL = ((uint32)buf[0]<<24)+((uint32)buf[1]<<16)+((uint32)buf[2]<<8)+buf[3];
				
				if ( buf[0] & 0x80 )
				{
					tempL = ~tempL + 1;					
				}
				
				tempL = (uint32)(((float)tempL/KS));
				
				//4.4w  2w 11
				if ( tempL < START_POWER )
				    tempL = 0;		 		
				g_uip[i].SRms = Hex32ToBCD( tempL );	
			}	
			
			//电网频率 11
		    FeedWatchdog ( );
			if ( SPIRead_REG ( FREQENCY_REQ, buf, 3, 0x00 ) != 0 )
				return;	
			
			tempL = ((uint32)buf[0]<<16)+((uint32)buf[1]<<8)+buf[2];
			
			
			//g_freqency = 8192000*8/(uint16)
		    tempL = (uint32)(65536000.0*100.0/(tempL*1.0)); 
		    g_freqency = (uint16)Hex32ToBCD( tempL );
		    	 						
		}
		s_step=0;
		break;
		
#endif
		
	default:
		s_step = 0;
		break;		
		
	}	
	return;
}

/*
*****************************************************************************************
* fuction name: GetRN8302CheckSum
*
* Description : this function initialize the cl2208 chip
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
uchar GetRN8302CheckSum ( uchar *p )
{

	//读取校验和寄存器 11
	if ( SPIRead_REG ( 0x6a, p, 3, 0x01 ) != 0 )
	{
		return 0xff;
	}
	
	
	
	return 0;

	
}	

#endif			