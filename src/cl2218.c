/******************************************************************************************
*  Copyright (c) 2013,  R&D Department
*  All rights reserved 
*
*  FileName：   	cl2208.c
*  Author：     	Roger
*  Date：  			2013-3-8 20:10
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

#define _CL2208_DEFINE_
#include "include.h"

#if(METER_CHIP_MODEL==0x2208)

//variables statement
/******************************************************************************************/
static const uchar CALI_REG_ORDER[CALIREG_NUM]={
	//0x00,			//SYSCON 00
	//0x01,			//EMUCON 01	
	0x03,			//PSTART 03
	
	//0x04,			//QSTART 04
	0x05,			//GPQA 05
	0x07,			//PHASE 07
	
	0x0A,			//APOR 0A
	0x0E,			//IARMSOS 0E	
	0x02,			//HFCONST 02
	0x00,			//reserved
};

/*
//2208读取寄存器
static const CL2208_READ_REG CL2208_READ_REG_TABLE[]={
	0x24,	(uint32 *)&g_factor.UFactor,(uint32 *)&g_uip.URms,//电压有效值 11
	0x22, 	(uint32 *)&g_factor.IFactor,(uint32 *)&g_uip.IRms,	//通道A电流有效值 11	
	0x26,	(uint32 *)&g_factor.PFactor,(uint32 *)&g_uip.PRms,	//功率有效值 11
};
*/
static uchar s_step;		//读取2208控制字节  11  
static uint32 s_SPower,s_Power;			//
/******************************************************************************************/

//function statement
/******************************************************************************************/
static float GetErrorData ( uchar *p,uint32* pPower,uchar cnt );
static uchar StoreCaliPara ( void );

/******************************************************************************************/



/*
*****************************************************************************************
* fuction name: CalibrateCl2208
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
uchar CalibrateCl2208 ( void )
{
	uchar i;
	uchar buf[2];
	uchar *p;
	
	p = (uchar*)&g_cl2208para.pstart;	
	for ( i=0; i<CALIREG_NUM-1; i++ )
	{
		buf[0] = p[2*i+1];
		buf[1] = p[2*i];
		if ( CALI_REG_ORDER[i] == 0x07 )		//相位调整寄存器长度为1 11
		{
			SPIWrite_REG (CALI_REG_ORDER[i],&buf[1], 1);	
		}
		else
		{
			SPIWrite_REG (CALI_REG_ORDER[i],buf, 2);	
		}
			 
	}	
	
	//延时1ms,等待校验和更新，校验和更新11.2us 11
	Delay1MsTime( 1 );
	
	if ( StoreCaliPara ( )!= 0 )
		return 1;
	
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
uchar CaliGainPara ( uchar *p )
{
	uchar i;
	uint32 templong,inL;
	float err;
	
	for ( i=0; i<4; i++ )
	{
		g_buf[i] = i+1;
	}
	templong = 0x98765432;
	
	//读取高频常数 11
	if ( SPIRead_REG ( HFCONST, g_buf, 2 ) != 0 )
		return 1;		
	
	templong = ((((uint32)g_buf[0])<<8)|(uint32)g_buf[1])&0x0000ffff;
	
	//功率校正系数 11
	g_factor.PFactor = (uint32)((float)templong*METER_CONSTANT*1.3330128);
	
	//获取误差 11
	err = GetErrorData ( &p[8],&templong,3 );	
	if ( err > 100 )
		return 1;
	
	err = -err/(1+err);
		
	if(err >= 0) templong = (uint32)(32768*err);
	else templong = 0x10000 + (uint32)(32768*err);
	
	g_buf[0] = (uchar)(templong>>8);
	g_buf[1] = (uchar)(templong&0xff);
	
	if ( SPIWrite_REG ( GPQA,g_buf, 2 ) != 0 )
		return 1;
	
	
	g_cl2208para.gpqa = (uint16)templong;
	//电压电流系数校正	 11				
	//电压系数 11
	if ( SPIRead_REG ( URMS, g_buf, 3 ) != 0 )
		return 1;
	templong = ((uint32)g_buf[0]) & 0xff;
	templong = (templong<<8) | ((uint32)g_buf[1] & 0xff);
	templong = (templong<<8) | ((uint32)g_buf[2] & 0xff);	
	for ( i=0; i<3; i++ )
		p[i] = p[i+1];	
	p[3] = 0;
	inL = BCDToHex(&p[0], 4);		//标准电压转化成HEX	 11
	g_factor.UFactor = templong*1000/inL;
	
	//电流系数 11
	if ( SPIRead_REG ( IARMS, g_buf, 3 ) != 0 )
		return 1;
	templong = ((uint32)g_buf[0]) & 0xff;		
	templong = (templong<<8) | ((uint32)g_buf[1] & 0xff);
	templong = (templong<<8) | ((uint32)g_buf[2] & 0xff);
	for ( i=4; i<7; i++ )
		p[i] = p[i+1];	
	p[7] = 0;
	inL = BCDToHex(&p[4], 4);		//标准电压转化成HEX	 11
	g_factor.IFactor = templong*1000/inL;
	
	//读取当前功率，计算启动阀值 11
	if ( SPIRead_REG ( POWERP, g_buf, 4 ) != 0 )
		return 1;	
	templong = (uint32)g_buf[0] & 0xFf;		//revised by Roger 2014/11/10
	templong = (templong<<8) | ((uint32)g_buf[1] & 0xff);
	templong = (templong<<8) | ((uint32)g_buf[2] & 0xff);
	templong = (templong<<8) | ((uint32)g_buf[3] & 0xff);	
	if( templong & 0x80000000 )
	{
		templong = (~templong) + 1;
	} 
	
	templong /= 500;		//0.2%启动阀值	 11
	templong >>= 8;
	g_buf[0] = (uchar)((templong>>8)&0xff);
	g_buf[1] = (uchar)(templong&0xff);
	if ( SPIWrite_REG ( PSTART, g_buf, 2 ) != 0 )
		return 1;
	g_cl2208para.pstart = (uint16)(templong&0xffff);
		
	//校验和比对成功后，存入EEPROM 11
	if ( StoreCaliPara ( )!= 0 )
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
uchar CaliPhasePara ( uchar *p )
{
	uint32 templong;
	float err;
	
	//获取误差 11
	err = GetErrorData ( &p[8],&templong,5 );
	err /= 1.732;
	err = (-err)*180/3.1415926;
	
	templong = 256 + (uint32)(err/0.02);
	
	g_buf[0] = (uchar)templong&0xff;

	if ( SPIWrite_REG ( PHASE,(uchar *)&g_buf[0], 1 ) != 0 )
		return 1;	
	g_cl2208para.phase = (uint16)g_buf[0]&0x00ff;
	

	//校验和比对成功后，存入EEPROM 11
	if ( StoreCaliPara ( )!= 0 )
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
uchar CaliOffsetPara ( uchar *p )
{
	uchar buf[4];
	uint32 templong;
	float err,k;
		
	if ( SPIRead_REG ( GPQA, (uchar *)&buf[0], 2 ) != 0 )
		return 1;
	templong = (uint32)buf[0];
	templong = (templong<<8) | (uint32)buf[1];
		
	if( templong & 0x8000 )//有功增益系数为负值 11
	{
		k = 1+((float)templong-65536)/32768;
	}
	else
	{
		k = 1+(float)templong/32768;
	}
	
	//获取误差 11
	err = GetErrorData ( &p[8],&templong,10 );	
	
	//if ( err > 0 )
	//	temp = (uint32)(err * 10000);
	//else
	//	temp = (uint32)(err *(-1) * 10000);	
	//DisplayInputNumber ( (uchar*)&temp );	
	
	k = templong/k;
				
	if( err >= 0 ) 
	{
		templong = (uint32)(k*(err/(err+1)));
		templong = (~templong) +1;//误差为正 11
	}
	else 
		templong = (uint32)(k*(-err/(err+1)));
			
	buf[0] = (uchar)((templong>>8)&0xff);
	buf[1] = (uchar)(templong&0xff);
	//DisplayInputNumber ( buf  );
	if ( SPIWrite_REG ( APOR,(uchar *)buf, 2 ) != 0 )
		return 1;	
	g_cl2208para.apor = (uint16)templong;
	
	//g_cl2208para.checksum = GetCl2208CheckSum ( (uint16 *)&g_cl2208para.pstart,CALIREG_NUM );
	//if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1),
	//			(uchar *)&g_cl2208para.pstart,CALIREG_NUM*2+2 ) != 0 )
	//	return 1;
	
	//校验和比对成功后，存入EEPROM 11
	if ( StoreCaliPara ( )!= 0 )
		return 1;
			
	return 0;
	
}

/*
*****************************************************************************************
* fuction name: GetErrorData
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
static float GetErrorData ( uchar *p, uint32* pPower,uchar cnt )
{
	uchar i,buf[4];
	uint32 tempR,tempW,tempL;
	float err;
		
	tempW = 0;
	for(i=0; i<cnt; i++)
	{
		
		//读取当前功率值 11
		if ( SPIRead_REG ( POWERP, (uchar*)&buf[0], 4 ) != 0 )
			return 200;
		
		//tempInt = (uint32)(buf[0]<<24)|(buf[1]<<16)|(buf[2]<<8)|(buf[3]<<0);
		tempL = buf[0] & 0xff;
		tempL = (tempL<<8) | (buf[1] & 0xff);
		tempL = (tempL<<8) | (buf[2] & 0xff);
		tempL = (tempL<<8) | (buf[3] & 0xff);
		
		if( tempL & 0x80000000 )
		{
			tempL = (~tempL) + 1;
		}
	
		tempW += tempL;
		
		Delay1MsTime ( 350 ); //延时350ms 11
		FeedWatchdog ( );	//
		
		
	}
	tempL = tempW/cnt;
	*pPower = tempL;
	
	
	tempW = (uint32)(((float)tempL/g_factor.PFactor)*1000000);//计算出实际功率值 11

	tempR = BCDToHex(p, 4);		//功率转化成HEX	 11
	
	
	
	err = ((float)tempW - tempR)/tempR;	
		
	return err;
	
}

/*
*****************************************************************************************
* fuction name: CaliIARMSPara
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
uchar CaliIARMSPara ( uchar *p )
{
	uchar i,n,buf[2];
	uint16 tempint;
	uint32 templong,temp;
	
	n = 0;
	templong = 0;
	for(i=0; i<10; i++)
	{
		if ( SPIRead_REG ( IARMS, (uchar *)&buf[0], 2 ) != 0 )		//电流有效值 11
			return 1;
		
		tempint = (uint16)buf[0];
		tempint = (tempint<<8) | (uint16)buf[1];			
		if( tempint & 0x8000 ) //最高位为1做零处理 11
		{
			tempint = 0;
		}
		else 
			n++;
			
		templong +=  (uint32)tempint;		
	}
	if ( n > 0 )
		templong /= n;
	
	
	templong >>= 8;
	temp = templong*templong;//平方 11

	temp = ~temp+1;
	buf[0] = (uchar)(temp >>= 8)&0xff;//取中间4位 11
	buf[1] = (uchar)temp &0xff;
	
	if ( SPIWrite_REG ( IARMSOS,(uchar *)&buf[0], 2 ) != 0 )
		return 1;	
	g_cl2208para.iarmsos = (uint16)(temp>>8);
	
	//g_cl2208para.checksum = GetCl2208CheckSum ( (uint16 *)&g_cl2208para.pstart,CALIREG_NUM );
	//if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1),
	//			(uchar *)&g_cl2208para.pstart,CALIREG_NUM*2+2 ) != 0 )
	//	return 1;
	//校验和比对成功后，存入EEPROM 11
	if ( StoreCaliPara ( )!= 0 )
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
uchar GetRegLen ( uchar reg )
{
	uchar len;
	
	if ( reg <= 0x21 )
	{
		len = 2;
		
		if ( reg == 0x07 || reg==0x08 )
			len = 1;	
	}
	else if ( reg <= 0x24 )
	{
		len = 3;		
	}
	else if ( reg == 0x25 )
	{
		len = 2;		
	}
	else if ( reg <= 0x28 )
	{
		len = 4;		
	}
	else if ( reg <= 0x2d )
	{
		len = 3;		
	}
	else if ( reg <= 0x43 )
	{
		len = 1;		
	}	
	else if ( reg == 0x44 )
	{
		len = 4;		
	}
	else if ( reg == 0x45 )
	{
		len = 2;		
	}
	else if ( reg == 0x46 )
	{
		len = 3;		
	}
	
	return len;
	
}		

/*
*****************************************************************************************
* fuction name: GetCl2208ParaAddr
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
uint16 GetCl2208ParaAddr ( uchar reg )
{
	uchar i;
	
	for ( i=0; i<CALIREG_NUM-1; i++ )
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
static uchar StoreCaliPara ( void )
{
	uchar buf[4];
	uint16 tempInt;
	
	//获取计量数据校验和 11
	do{
		if ( SPIRead_REG ( EMUSTATUS, buf, 3 )!= 0 )
			return 1;	
		delay_time ( );	
		FeedWatchdog  (  );	
	}while(buf[0]&BIT0);		//0完成；1未完成 11
	
	//计算RAM中校表数据的checksum 11
	g_cl2208para.checksum = 
		GetCl2208CheckSum ( (uint16 *)&g_cl2208para.pstart,CALIREG_NUM );

	//判断是否相等 11
	tempInt = (uint16)buf[1]&0xff;
	tempInt =  (tempInt<<8) | (buf[2]&0xff);	
	if ( tempInt != g_cl2208para.checksum )
	{
		return 1;	
	}
		
	//保存到内存及eeprom中 11
	if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1),
				(uchar*)&g_cl2208para.pstart,CALIREG_NUM*2+2 ) != 0 )
		return 1;		
	
	//有效值系数 11
	g_factor.checksum = GetSum ( (uchar*)&g_factor.UFactor, 15 ); 
	if ( StoreToE2ROMAndBackup ( offsetof(EEPROM_DATA,UFactor),
				(uchar*)&g_factor.UFactor,16 ) != 0 )
		return 1;		
	
	return 0;
	
}	

/*
*****************************************************************************************
* fuction name: CheckCl2208Reg
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
uchar CheckCl2208Reg ( void )
{
	uchar i,checksum;
	uchar buf[8];
	uint16 tempInt;
	uchar *p;
	
	//校验RAM中参数校验和 11
 	tempInt = GetCl2208CheckSum ( (uint16 *)&g_cl2208para.pstart,CALIREG_NUM );
	if ( g_cl2208para.checksum != tempInt )
	{
		
		if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,PSTART1), 
				(uchar*)&g_cl2208para.pstart,CHECK_CL2208,16 ) != 0 )
		{			
			
			//if ( g_buyPara.Status != MANUFACTURE_STATUS )
			{
				//while(1); 	//电量信息不正确，重启，让表不正常 11
				g_errStatus.Bit.E2ROM = 1;
			}
			return 1;	
		}	
				
	}
	
	//获取计量数据校验和 11
	do{
		if ( SPIRead_REG ( EMUSTATUS, buf, 3 )!= 0 )
			return 1;	
		delay_time ( );	
		FeedWatchdog  (  );	
	}while(buf[0]&BIT0);		//0完成；1未完成 11
	
	//判断是否相等 11
	tempInt = (uint16)buf[1]&0xff;
	tempInt =  (tempInt<<8) | (buf[2]&0xff);
	if ( tempInt != g_cl2208para.checksum )
	{		
		//先进行软复位 11
		SoftResetCl2208 ( );
		FeedWatchdog  (  );	
		//g_buf[0]=buf[0];
		//g_buf[1]=buf[1];
		//g_buf[2]=buf[2];
		//g_buf[3]=0x55;
	    //DisplayInputNumber ( g_buf );
		Delay1MsTime ( 300 );
		FeedWatchdog  (  );	
			
		//校验数据 11
		p = (uchar*)&g_cl2208para.pstart;	
		for ( i=0; i<CALIREG_NUM-1; i++ )
		{
			buf[0] = p[2*i+1];
			buf[1] = p[2*i];
			if ( CALI_REG_ORDER[i] == 0x07 )		//相位调整寄存器长度为1 11
			{
				SPIWrite_REG (CALI_REG_ORDER[i],&buf[1], 1);	
			}
			else
			{
				SPIWrite_REG (CALI_REG_ORDER[i],buf, 2);	
			}
				 
		}		
	}	
	
	//校验RAM中有效值系数校验和 11
	checksum = GetSum ( (uchar*)&g_factor.UFactor, 15 );
	if ( (g_factor.checksum != checksum) ||
		(g_factor.UFactor==0 && checksum==0) )
	{			
		
		if ( ReloadFromE2ROMAndBackup ( offsetof(EEPROM_DATA,UFactor), 
				(uchar*)&g_factor.UFactor,CHECK_ADDMOD,16 ) != 0 )
		{			
			//if ( g_buyPara.Status == USER_STATUS )
		    {
				//while(1); 	//电量信息不正确，重启，让表不正常 11
				g_errStatus.Bit.E2ROM = 1;
			}
			return 1;	
		}	
				
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
	uchar i;
	uint32 tempL;
	uint16 temp16;
	
	switch ( s_step )
	{
	//读电压 11
	case 0:
		{
			if ( SPIRead_REG ( 0x24, (uchar*)&g_buf[0], 3 ) != 0 )
				return;	
			
			tempL = ((uint32)g_buf[0]<<16)+((uint32)g_buf[1]<<8)+((uint32)g_buf[2]<<0);
			//DisplayInputNumber ( (uchar*)&tempL  );
			//for ( i=0; i<15; i++ )
			//{	    	    
			//	Delay1MsTime ( 200 );
			//    FeedWatchdog  (  );	    
			//}	
			if ( g_buf[0] & BIT7 )
				tempL = 0;	
			tempL = (uint32)(((float)tempL/g_factor.UFactor)*10);	//1位小数 11
			s_SPower = tempL;
				 		
			g_uip.URms = Hex32ToBCD( tempL );	
		
		}
		s_step++;
		break;	
	//读电流 11	
	case 1:
		{
			if ( SPIRead_REG ( 0x22, (uchar*)&g_buf[0], 3 ) != 0 )
				return;	
			tempL = ((uint32)g_buf[0]<<16)+((uint32)g_buf[1]<<8)+((uint32)g_buf[2]<<0);
			if ( g_buf[0] & BIT7 )
				tempL = 0;	
			tempL = (uint32)(((float)tempL/g_factor.IFactor)*1000);			//3位小数 11
			if(tempL<START_CURRENT)		//20mA 
			    tempL = 0;
			
			s_SPower *= tempL; 
			s_SPower /= 1000;		//kw
			       	 		
			g_uip.IRms = Hex32ToBCD( tempL );	

		}
		s_step++;
		break;	
	//读功率 11
	case 2:
		{
			if ( SPIRead_REG ( 0x26, (uchar*)&g_buf[0], 4 ) != 0 )
				return;	
			tempL = ((uint32)g_buf[0]<<24)+((uint32)g_buf[1]<<16)+((uint32)g_buf[2]<<8)+g_buf[3];
			if( tempL & 0x80000000 )
			{
				tempL = (~tempL) + 1;
			}
			tempL = (uint32)(((float)tempL/g_factor.PFactor)*10000);
			if(tempL<START_POWER)
			    tempL = 0;
			
			s_Power = tempL;
				
			/*
			//限电功率判断 11
			if ( g_para.LimitPower != 0 )
			{
				if ( tempL >  g_para.LimitPower*10 )
				{
					g_OverPowerFlagCnt++;
					if ( g_OverPowerFlagCnt > 1 )
						g_errStatus.Bit.OverPower = 1;	
				}
				else
					g_OverPowerFlagCnt = 0;	
			}
			*/		 		
			g_uip.PRms = Hex32ToBCD( tempL );	
				
		}
		s_step++;
		break;
	
	case 3:
		{
			//计算视在功率 U*I=0.1V*0.001A 单位W 4位小数 11
		    if ( s_SPower == 0 )
		    {
		        g_uip.Factor = 0x1000;  
		    }
		    else
		    {
		    	//3位小数
			    tempL = (s_Power*1000)/s_SPower;
			    if ( tempL > 1000 )
			        tempL = 1000;    	
				g_uip.Factor = (uint16)Hex32ToBCD( tempL );	
		    }
		//恶性负载使能 11
		#if(MALIGNANT_LOAD_CHK_EANBLED==1) 		    
		    //数组移位 11
			for ( i=1; i<8; i++ )
			{
				g_curRLoad[i-1].SRms = g_curRLoad[i].SRms;
				g_curRLoad[i-1].PRms = g_curRLoad[i].PRms;
			}    			    		    
		    g_curRLoad[7].PRms = s_Power;		
			g_curRLoad[7].SRms = s_SPower;	
			g_meterFlag.Bit.MaglentFlag = 1;
		#endif		
		}
		s_step++;
		break;
			
	
	//其他 11
	case 4:
		{
		    //获取计量数据计量状态 11
    		if ( SPIRead_REG ( EMUSTATUS, g_buf, 3 )!= 0 )
    			return;	
        		
        	if ( (g_buf[0]&BIT1) == BIT1 )		//0正向；1反向 11    	
			{
			    g_dirFlag.Bit.RevPFlag = 1;   			    
			}
			else
			{
				g_dirFlag.Bit.RevPFlag = 0; 
			}
			    
			    
		    //频率获取 11
		    if ( SPIRead_REG ( UFREQ, g_buf, 2 )!= 0 )
    			return;
    		tempL = ((uint32)g_buf[0]<<8)+((uint32)g_buf[1]<<0);	
    		temp16 = (uint16)(447443/tempL);
    		//temp16 = (uint16)(tempL&0x0000ffff);
    	    g_uip.Freq = (uint16)Hex16ToBCD( temp16 );	

		}
	
		s_step = 0;	
				
		break;	
			
	default:
		s_step = 0;
		break;		
		
	}	
	return;
}


#endif					