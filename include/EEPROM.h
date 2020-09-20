/*
  -this file contains declarations of functions and variables used in protocol.c.
*/

#ifndef _EEPROM_H

#define _EEPROM_H

//EEPROM data map 
//24c04=0.5k bytes=32 pages(16 bytes)
//24c08=1k bytes=64 pages(16 bytes)
//24c16=2k bytes=128 pages(16 bytes)
//24c32=4k bytes=128 pages(32 bytes)
//24c64=8k bytes=256 pages(32 bytes)
//24c128=16k bytes=256 pages(64 bytes)
//24c256=32k bytes=512 pages(64 bytes)
//24c512=64k bytes=512 pages(128 bytes) 11
typedef struct EEPROM_DATA_tag
{  	
	//
	//掉电数据保存区 64字节 4*16 11
  	unsigned char Accumulate_Low[64];		//掉电时保存 有功电量 4字节 11
  	 
  	
  	////////////////////////////////////////////////////////////////////////
	//计量相关32字节 --有备份 2*16 11
	
	unsigned char PSTART1[2];		//2字节 11
	unsigned char GPQA1[2];			//2字节 11
	unsigned char PHASEREG[2];		//2字节	 11
	unsigned char APORREG[2];			//2字节	 11							
	unsigned char IARMSOSREG[2];		//2字节 11
	unsigned char HFCONST11[2];		//2字节	 11
	unsigned char Reserved8[2];		//1字节	2 11
	unsigned char CRC161[2];		//2字节  16 11	
	
	//有效值数据 11
	unsigned char 	UFactor[4];				//电压系数  4 11
	unsigned char	IFactor[4];				//电流1系数 4 11
	unsigned char   PFactor[4];				//功率系数  4 11
	unsigned char  	Reserved9[3];			//3字节   11  
	unsigned char 	Checksum9;				//校验码	   16 1 11	
	  	
  	//4+2+
  	//参数存储数据区1 --有备份 8*16 1  
  	//PAGE 6  	  	
  	//地址参数信息  11
	unsigned int Zone;			//电表分区号 2字节 11	
	unsigned char CardNum[6];	//对应卡号 6字节 11
	unsigned char MeterAddr[6];	//对应表号 6字节 11	
	unsigned char BuyMode;		//购电模式，电量or价格	1字节	 11	
	unsigned char Checksum1;			//16	
    
    //PAGE 7 
	//购电信息  11
	unsigned int BuyCount;		//购电次数 2字节 11
	unsigned long BuyQuan;		//购电量或购电金额 4字节 11	
	unsigned int Alarm1;		//报警电量1 2字节 11
	unsigned int Alarm2;		//报警电量2 2字节 11
	unsigned int FactorLimit;		//功率因数限值 2 11	
	unsigned int FactorPower;		//功率因数对应功率限值 2 11		
	unsigned char Status;		//表计状态 11
	unsigned char Checksum2;		//16	
	
	//PAGE 8 
	//参数信息		11
	unsigned int LimitPower;			//限电功率 2字节 11
	unsigned int MaximBalance;			//最大囤积量 2字节	 11
	unsigned long CreditLimit;			//电压互感比 4字节 11
	unsigned int CurrentRatio;			//电流互感比 2 11
	unsigned char BonusMoneyNum;			// 补助金额有效序号模式字 11
	unsigned char RelayFlag;			// 1 11
	unsigned int Price;					//基础电价 2 11
	unsigned char MeterTpye;			//表型判断 11						
	unsigned char Checksum3;			//16
	
	//PAGE 9~12 
	//电量信息	64字节	5~7
	unsigned long Accumulate;		//累计电量 11
	unsigned long TariffQ1;			//·ÑÂÊµçÁ¿1 4×Ö½Ú 11
	unsigned long TariffQ2;			//·ÑÂÊµçÁ¿2 4×Ö½Ú  11
	unsigned long TariffQ3;			//·ÑÂÊµçÁ¿3 4×Ö½Ú		16	
	unsigned long TariffQ4;			//·ÑÂÊµçÁ¿4 4×Ö½Ú 11	
	unsigned long CurrentQ;			//当前月电量 4	 11
	unsigned long Balance;			//剩余金额 4 11
	unsigned long Overdraft;		//过零金额 4 11	
	unsigned long PrePaid;				//预置金额 4 11
	unsigned long BonusPrePaid;			//补助预置金额	4 16		
	unsigned long PosAccumulate;		//正向有功总电量 11	
	unsigned long NegAccumulate;		//反向有功总电量 11					
	unsigned long BonusBalance;			//赠送的剩余金额  4	 11	
	unsigned long BonusBuyQuan;			//赠送金额 4 11
	unsigned int BonusBuyCount;			//赠送金额次数 2 11	
	unsigned char BonusDate;			//赠送日 1 11	
	unsigned char BonusMode;			//赠送模式 1 11		
	unsigned char Fraction5And6;		//剩余金额计算 11	
	unsigned char TariffNum;			//费率号 11			
	unsigned char PulseCnt;			//脉冲个数 11						
	unsigned char Checksum4;			//64
	
	//PAGE 13 
	//通讯密码 0xd0 11
	unsigned char PASSWORD[4];		//通讯密码 4	
	unsigned char Checksum11;				//校验码	   16
	unsigned char DropDateTime[7];			//掉电时间7  11
	unsigned int baudrate;		//波特率 2字节 11
	unsigned char checkType;	//校验方式 1字节 11
	unsigned char checksum_baudrate;		//校验方式 1字节 11  
	
	
	//PAGE 14~25 
	//4+2+8			
	////////////////////////////////////////////////////////////////////////
	//电量清零记录 近10次  3+18*10+9=192=12*16 11
  	///////////////////////////////////////////////////////////////////////////////
  	unsigned char TotalMeterClearCnt[3];
  	unsigned char MeterClearReserved[9];
  	unsigned char Last1MeterClear[18];
  	unsigned char Last2MeterClear[18];
  	unsigned char Last3MeterClear[18];
  	unsigned char Last4MeterClear[18];
  	unsigned char Last5MeterClear[18];
  	unsigned char Last6MeterClear[18];
  	unsigned char Last7MeterClear[18];
  	unsigned char Last8MeterClear[18];
  	unsigned char Last9MeterClear[18];
  	unsigned char Last10MeterClear[18];
  	///////////////////////////////////////////////////////////////////////////////
  	
  	//4+2+8+12						
  	///////////////////////////////////////////////////////////////////////////////
  	//参数存储备份区1  10*16 11
  	
  	////PAGE 26 
  	//计量校表数据 38 11
	//计量校表数据 0
	unsigned char PSTART1_B[2];		//2字节 11
	unsigned char GPQA1_B[2];			//2字节 11
	unsigned char PHASEREG_B[2];		//2字节	 11
	unsigned char APORREG_B[2];			//2字节	 11							
	unsigned char IARMSOSREG_B[2];		//2字节 11
	unsigned char HFCONST11_B[2];		//2字节	 11
	unsigned char Reserved8_B[2];		//1字节	2 11
	unsigned char CRC161_B[2];		//2字节  16 11	
			
	//有效值数据 39 11
	unsigned char 	UFactor_B[4];				//电压系数  4 11
	unsigned char	IFactor_B[4];				//电流1系数 4 11
	unsigned char   PFactor_B[4];				//功率系数  4 11	
	unsigned char  	Reserved9_B[3];				//3字节    16 11
	unsigned char 	Checksum9_B;				//校验码	   1  11 					
  	///////////////////////////////////////////////////////////////////////////////
  	
  	//PAGE 28 0X1C0
  	//地址参数信息 40
	unsigned int Zone_B;			//电表分区号 2字节 11
	unsigned char CardNum_B[6];	//对应卡号 6字节 11
	unsigned char MeterAddr_B[6];	//对应表号 6字节 11	
	unsigned char BuyMode_B;		//购电模式，电量or价格	1字节	 11
	unsigned char Checksum1_B;			//16	
	
	
	//购电信息 41
	unsigned int BuyCount_B;		//购电次数 2字节 11
	unsigned long BuyQuan_B;		//购电量或购电金额 4字节	11
	unsigned int Alarm1_B;		//报警电量1 2字节 11
	unsigned int Alarm2_B;		//报警电量2 2字节 11
	unsigned int FactorLimit_B;		//功率因数限值 2 11	
	unsigned int FactorPower_B;		//功率因数对应功率限值 2 11	
	unsigned char Status_B;		//表状态：生产、出厂、用户态 1字节 11
	unsigned char Checksum2_B;		//16	

	//参数信息	42
	unsigned int LimitPower_B;			//限电功率 2字节 11
	unsigned int MaximBalance_B;		//最大囤积量 2字节	11
	unsigned long CreditLimit_B;		//电压互感比 2字节 11
	unsigned int CurrentRatio_B;		//电流互感比 2字节	 11
	unsigned char BonusMoneyNum_B;			// 补助金额有效序号模式字 11
	unsigned char RelayFlag_B;			// 1 11
	unsigned int Price_B;				//基础电价  2字节 11									
	unsigned char MeterTpye_B;			//表型判断 11	
	unsigned char Checksum3_B;			//16

	//电量信息	64字节	43 44 45	
	unsigned long Accumulate_B;		//ÀÛ¼ÆÓÃµçÁ¿ 4×Ö½Ú 11
	unsigned long TariffQ1_B;			//·ÑÂÊµçÁ¿1 4×Ö½Ú 11
	unsigned long TariffQ2_B;			//·ÑÂÊµçÁ¿2 4×Ö½Ú  11
	unsigned long TariffQ3_B;			//·ÑÂÊµçÁ¿3 4×Ö½Ú		16	
	unsigned long TariffQ4_B;			//·ÑÂÊµçÁ¿4 4×Ö½Ú 11	
	unsigned long CurrentQ_B;			//当前月电量 4	 11
	unsigned long Balance_B;			//剩余金额 4 11
	unsigned long Overdraft_B;		//过零金额 4 11	
	unsigned long PrePaid_B;				//预置金额 4 11
	unsigned long BonusPrePaid_B;			//补助预置金额	4 16		
	unsigned long PosAccumulate_B;		//正向有功总电量 11	
	unsigned long NegAccumulate_B;		//反向有功总电量 11					
	unsigned long BonusBalance_B;			//赠送的剩余金额  4	 11	
	unsigned long BonusBuyQuan_B;			//赠送金额 4 11
	unsigned int BonusBuyCount_B;			//赠送金额次数 2 11	
	unsigned char BonusDate_B;			//赠送日 1 11	
	unsigned char BonusMode_B;			//赠送模式 1 11			
	unsigned char Fraction5And6_B;		//剩余金额计算 11	
	unsigned char TariffNum_B;			//费率号 11			
	unsigned char PulseCnt_B;			//脉冲个数 11
	unsigned char Checksum4_B;			//32+4+4
	
	//通讯密码+ 11
	unsigned char PASSWORD_B[4];		//通讯密码 4 11	
	unsigned char 	Checksum11_B;				//校验码	   16
	unsigned char DropDateTime_B[7];			//掉电时间7  11
	unsigned int baudrate_B;		//波特率 2字节 11
	unsigned char checkType_B;	//校验方式 1字节 11
	unsigned char checksum_baudrate_B;		//校验方式 1字节 11
	
	
	//4+2+8+12+10
	
	//0X240
	//最近10次购电成功订单号 5pages 11
	///////////////////////////////////////////////////////////////////////////////// 
	unsigned char Last1_Order[8];	            //最近1次购电订单号 8字节 11
	unsigned char Last2_Order[8];	            //最近2次购电订单号 8字节 11
	
	unsigned char Last3_Order[8];	            //最近3次购电订单号 8字节 11
	unsigned char Last4_Order[8];	            //最近4次购电订单号 8字节 11
	
	unsigned char Last5_Order[8];	            //最近5次购电订单号 8字节 11
	unsigned char Last6_Order[8];	            //最近6次购电订单号 8字节 11
	
	unsigned char Last7_Order[8];	            //最近7次购电订单号 8字节 11
	unsigned char Last8_Order[8];	            //最近8次购电订单号 8字节 11
	
	unsigned char Last9_Order[8];	            //最近9次购电订单号 8字节 11
	unsigned char Last10_Order[8];	            //最近10次购电订单号 8字节 11
	///////////////////////////////////////////////////////////////////////////////// 
	//end
		
	//最近10次历史购电记录 10*18+12=192=12*16 11
  	/////////////////////////////////////////////////////////////////////////////////  	
  	unsigned char Last1_BuyQuan[4];	            //最近1次购电金额 4字节 11
  	unsigned char Last1_BuyCounter[2];			//最近1次购电次数 2字节 11
  	unsigned char Last1_BuyPreQuan[4];	        //最近1次购电前金额量 4字节 11
  	unsigned char Last1_BuyAfterQuan[4];	    //最近1次购电后金额 4字节 11
  	unsigned char Last1_TotalBuyQuan[4];	     //最近1次累计购电金额 4字节 11
  	
  	unsigned char Last2_BuyQuan[4];	            //最近2次购电量 2字节 11
  	unsigned char Last2_BuyCounter[2];			//最近2次购电次数 2字节 11
  	unsigned char Last2_BuyPreQuan[4];	        //最近2次购电前金额量 2字节 11
  	unsigned char Last2_BuyAfterQuan[4];	    //最近2次购电后金额 2字节 11
  	unsigned char Last2_TotalBuyQuan[4];	     //最近2次累计购电金额 4字节 11
  	 	
  	unsigned char Last3_BuyQuan[4];	            //最近3次购电量 2字节 11
  	unsigned char Last3_BuyCounter[2];			//最近3次购电次数 2字节 11
  	unsigned char Last3_BuyPreQuan[4];	        //最近3次购电前金额量 2字节 11
  	unsigned char Last3_BuyAfterQuan[4];	    //最近3次购电后金额 2字节 11
  	unsigned char Last3_TotalBuyQuan[4];	     //最近3次累计购电金额 4字节 11
  		
  	unsigned char Last4_BuyQuan[4];	            //最近4次购电量 2字节 11
  	unsigned char Last4_BuyCounter[2];			//最近4次购电次数 2字节 11
  	unsigned char Last4_BuyPreQuan[4];	        //最近4次购电前金额量 2字节 11
  	unsigned char Last4_BuyAfterQuan[4];	    //最近4次购电后金额 2字节 11
  	unsigned char Last4_TotalBuyQuan[4];	     //最近4次累计购电金额 4字节 11
  	 	
  	unsigned char Last5_BuyQuan[4];	            //最近5次购电量 2字节 11
  	unsigned char Last5_BuyCounter[2];			//最近5次购电次数 2字节 11
  	unsigned char Last5_BuyPreQuan[4];	        //最近5次购电前金额量 2字节 11
  	unsigned char Last5_BuyAfterQuan[4];	    //最近5次购电后金额 2字节 11
  	unsigned char Last5_TotalBuyQuan[4];	     //最近5次累计购电金额 4字节 11
  	  	
  	unsigned char Last6_BuyQuan[4];	            //最近6次购电量 2字节 11
  	unsigned char Last6_BuyCounter[2];			//最近6次购电次数 2字节 11
  	unsigned char Last6_BuyPreQuan[4];	        //最近6次购电前金额量 2字节 11
  	unsigned char Last6_BuyAfterQuan[4];	    //最近6次购电后金额 2字节 11
  	unsigned char Last6_TotalBuyQuan[4];	     //最近6次累计购电金额 4字节 11
  	 	
  	unsigned char Last7_BuyQuan[4];	            //最近7次购电量 2字节 11
  	unsigned char Last7_BuyCounter[2];			//最近7次购电次数 2字节 11  	
  	unsigned char Last7_BuyPreQuan[4];	        //最近7次购电前金额量 2字节 11
  	unsigned char Last7_BuyAfterQuan[4];	    //最近7次购电后金额 2字节 11
  	unsigned char Last7_TotalBuyQuan[4];	     //最近7次累计购电金额 4字节 11
  	
  	unsigned char Last8_BuyQuan[4];	            //最近8次购电量 2字节 11
  	unsigned char Last8_BuyCounter[2];			//最近8次购电次数 2字节 11  	
  	unsigned char Last8_BuyPreQuan[4];	        //最近8次购电前金额量 2字节 11
  	unsigned char Last8_BuyAfterQuan[4];	    //最近8次购电后金额 2字节 11
  	unsigned char Last8_TotalBuyQuan[4];	     //最近8次累计购电金额 4字节 11
  	
  	unsigned char Last9_BuyQuan[4];	            //最近9次购电量 2字节 11
  	unsigned char Last9_BuyCounter[2];			//最近9次购电次数 2字节 11  	
  	unsigned char Last9_BuyPreQuan[4];	        //最近9次购电前金额量 2字节 11
  	unsigned char Last9_BuyAfterQuan[4];	    //最近9次购电后金额 2字节 11
  	unsigned char Last9_TotalBuyQuan[4];	     //最近9次累计购电金额 4字节 11
  	
  	unsigned char Last10_BuyQuan[4];	         //最近10次购电量 2字节 11
  	unsigned char Last10_BuyCounter[2];			//最近10次购电次数 2字节 11
  	unsigned char Last10_BuyPreQuan[4];	        //最近10次购电前金额量 2字节 11
  	unsigned char Last10_BuyAfterQuan[4];	    //最近10次购电后金额 2字节 11
  	unsigned char Last10_TotalBuyQuan[4];	     //最近10次累计购电金额 4字节 11
	
	unsigned char Last1_ReturnQuan[4];	     //最近1次退费金额 4字节 11
  	unsigned char SoftVersion[8];            //软件版本号 11	  	
  	///////////////////////////////////////////////////////////////////////////////
  	//4+2+8+12+10+12
  	
  	//最近10次历史赠费记录 10*6+4=64=4*16 11
  	/////////////////////////////////////////////////////////////////////////////////  	
  	unsigned char Last1_BonusQuan[4];	            //最近1次赠费金额 4字节 11
  	unsigned char Last1_BonusCounter[2];			//最近1次赠费次数 2字节 11
	
	unsigned char Last2_BonusQuan[4];	            //最近2次赠费金额 4字节 11
  	unsigned char Last2_BonusCounter[2];			//最近2次赠费次数 2字节 11
  	
  	unsigned char Last3_BonusQuan[4];	            //最近3次赠费金额 4字节 11
  	unsigned char Last3_BonusCounter[2];			//最近3次赠费次数 2字节 11
  	unsigned char Last4_BonusQuan[4];	            //最近4次赠费金额 4字节 11
  	unsigned char Last4_BonusCounter[2];			//最近4次赠费次数 2字节 11
  	unsigned char Last5_BonusQuan[4];	            //最近5次赠费金额 4字节 11
  	unsigned char Last5_BonusCounter[2];			//最近5次赠费次数 2字节 11
  	unsigned char Last6_BonusQuan[4];	            //最近6次赠费金额 4字节 11
  	unsigned char Last6_BonusCounter[2];			//最近6次赠费次数 2字节 11
  	unsigned char Last7_BonusQuan[4];	            //最近7次赠费金额 4字节 11
  	unsigned char Last7_BonusCounter[2];			//最近7次赠费次数 2字节 11
  	unsigned char Last8_BonusQuan[4];	            //最近8次赠费金额 4字节 11
  	unsigned char Last8_BonusCounter[2];			//最近8次赠费次数 2字节 11
  	unsigned char Last9_BonusQuan[4];	            //最近9次赠费金额 4字节 11
  	unsigned char Last9_BonusCounter[2];			//最近9次赠费次数 2字节 11
  	unsigned char Last10_BonusQuan[4];	            //最近10次赠费金额 4字节 11
  	unsigned char Last10_BonusCounter[2];			//最近10次赠费次数 2字节 11
	
	unsigned char Last1_ReturnBonusQuan[4];	     //最近1次退费金额 4字节 11
  	///////////////////////////////////////////////////////////////////////////////
  	//4+2+8+12+10+4
  	
  	//掉电记录 近10次 3+12*10+5=128=8*16 11
  	///////////////////////////////////////////////////////////////////////////////
  	unsigned char TotalDropPowerCnt[3];	
  	unsigned char Last1DropPower[12];
  	unsigned char Last2DropPower[12];
  	unsigned char Last3DropPower[12];
  	unsigned char Last4DropPower[12];
  	unsigned char Last5DropPower[12];
  	unsigned char Last6DropPower[12];
  	unsigned char Last7DropPower[12];
  	unsigned char Last8DropPower[12];
  	unsigned char Last9DropPower[12];
  	unsigned char Last10DropPower[12];
  	unsigned char DropPowerReserved[5];
  	/////////////////////////////////////////////////////////////////////////////// 	
  	//4+2+8+12+10+4+8
  	
  	//不需要备份区数据 48
  	unsigned char Last1BuyDate[5];		//上1次购电日期 5字节 11
  	unsigned char WEEK;			//week
  	unsigned char DAY;			//day
  	unsigned char MONTH;		//month
  	unsigned char YEAR;			//year
  	unsigned char SECOND;		//second
  	unsigned char MINUTE;		//minute
  	unsigned char HOUR;			//hour
  	unsigned char PriceErrCnt;	//1字节 11  
  	unsigned char Reserved14[3];			//3字节  16 11
  	//////////////////////////////////////////////////////////////////////
  	//4+2+8+12+10+4+8+1=49
  	
  	//以上是普通预付费表存储空间 4+10+10+12+4+12+8+1=61*16 字节数 11

#ifdef RTC_8025T
  	 //0X420	
  	//15*16  15页  11
  	/////////////////////////////////////////////////////////////////////////////////
  	unsigned long Last1_A1;			//累计用电量 4字节 11
	unsigned long Last1_T1;			//费率电量1 4字节 11
	unsigned long Last1_T2;			//费率电量2 4字节 11
	unsigned long Last1_T3;			//费率电量3 4字节  11		16	
	
	unsigned long Last1_T4;			//费率电量4 4字节 11
	unsigned long Last2_A1;			//累计用电量 4字节  11
	unsigned long Last2_T1;			//费率电量1 4字节 11
	unsigned long Last2_T2;			//费率电量2 4字节       16  11
	
	unsigned long Last2_T3;			//费率电量3 4字节  11				
	unsigned long Last2_T4;			//费率电量4 4字节  11
	unsigned long Last3_A1;			//累计用电量 4字节 11
	unsigned long Last3_T1;			//费率电量1 4字节      16 11
	
	unsigned long Last3_T2;			//费率电量2 4字节  11       	
	unsigned long Last3_T3;			//费率电量3 4字节  11				
	unsigned long Last3_T4;			//费率电量4 4字节  11
	unsigned long Last4_A1;			//累计用电量 4字节     16 11
	
	unsigned long Last4_T1;			//费率电量1 4字节 11
	unsigned long Last4_T2;			//费率电量2 4字节  11      	
	unsigned long Last4_T3;			//费率电量3 4字节	11			
	unsigned long Last4_T4;			//费率电量4 4字节       16 11
	
	unsigned long Last5_A1;			//累计用电量 4字节 11
	unsigned long Last5_T1;			//费率电量1 4字节  11
	unsigned long Last5_T2;			//费率电量2 4字节   11
	unsigned long Last5_T3;			//费率电量3 4字节		16	 11
	
	unsigned long Last5_T4;			//费率电量4 4字节  11
	unsigned long Last6_A1;			//累计用电量 4字节  11
	unsigned long Last6_T1;			//费率电量1 4字节  11
	unsigned long Last6_T2;			//费率电量2 4字节       16 11
	
	unsigned long Last6_T3;			//费率电量3 4字节	11			
	unsigned long Last6_T4;			//费率电量4 4字节   11
	unsigned long Last7_A1;			//累计用电量 4字节   11
	unsigned long Last7_T1;			//费率电量1 4字节      16  11
	
	unsigned long Last7_T2;			//费率电量2 4字节  11       	
	unsigned long Last7_T3;			//费率电量3 4字节  11				
	unsigned long Last7_T4;			//费率电量4 4字节  11
	unsigned long Last8_A1;			//累计用电量 4字节     16  11
	
	unsigned long Last8_T1;			//费率电量1 4字节  11
	unsigned long Last8_T2;			//费率电量2 4字节  11      	
	unsigned long Last8_T3;			//费率电量3 4字节  11				
	unsigned long Last8_T4;			//费率电量4 4字节       16 11
	
	unsigned long Last9_A1;			//累计用电量 4字节  11
	unsigned long Last9_T1;			//费率电量1 4字节  11
	unsigned long Last9_T2;			//费率电量2 4字节  11
	unsigned long Last9_T3;			//费率电量3 4字节		16	 11
	
	unsigned long Last9_T4;			//费率电量4 4字节  11
	unsigned long Last10_A1;			//累计用电量 4字节  11
	unsigned long Last10_T1;			//费率电量1 4字节  11
	unsigned long Last10_T2;			//费率电量2 4字节       16 11
	
	unsigned long Last10_T3;			//费率电量3 4字节	11			
	unsigned long Last10_T4;			//费率电量4 4字节   11
	unsigned long Last11_A1;			//累计用电量 4字节   11
	unsigned long Last11_T1;			//费率电量1 4字节      16  11
	
	unsigned long Last11_T2;			//费率电量2 4字节  11     	
	unsigned long Last11_T3;			//费率电量3 4字节	11			
	unsigned long Last11_T4;			//费率电量4 4字节   11
	unsigned long Last12_A1;			//累计用电量 4字节     16 11
	
	unsigned long Last12_T1;			//费率电量1 4字节  11
	unsigned long Last12_T2;			//费率电量2 4字节  11      	
	unsigned long Last12_T3;			//费率电量3 4字节  11				
	unsigned long Last12_T4;			//费率电量4 4字节       16 11
	
	//////////////////////////////////////////////////////////////////
#endif	
	
	//累计15+60=75 pages

	//多费率使能 27*16 11
	///////////////////////////////////////////////////////////////////////////////
#ifdef TARIFF_ENABLE 
	//0X510
	//费率电价信息		 11
	unsigned int TariffPrice1;		//费率电价1  2字节 11
	unsigned int TariffPrice2;		//费率电价2  2字节 11
	unsigned int TariffPrice3;		//费率电价3  2字节 11
	unsigned int TariffPrice4;		//费率电价4  2字节 11
	unsigned int TariffPrice5;		//费率电价5  2字节 11
	unsigned int TariffPrice6;		//费率电价6  2字节 11	
	unsigned char Reserved5[3];		//3字节	 11							
	unsigned char Checksum5;			//16
	
	//年时区数  11 
	unsigned char YearZoneNum;		//年时区数 11
	unsigned char TimeZoneNum;		//时段表数 11
	unsigned char DayTalbeNum;		//日时段表数 11
	unsigned char MMDDY1[3];		//第1年时区 11
	unsigned char MMDDY2[3];		//第2年时区 11
	unsigned char MMDDY3[3];		//第3年时区 11
	unsigned char MMDDY4[3];		//第4年时区 11
	unsigned char CSYearZone;		//校验和	 11
	
	//第1时段表 费率时段表 11 
	unsigned char HHMMNN1[3];		//第1时段表 11
	unsigned char HHMMNN2[3];		//第2时段表 11
	unsigned char HHMMNN3[3];		//第3时段表 11
	unsigned char HHMMNN4[3];		//第4时段表 11
	unsigned char HHMMNN5[3];		//第5时段表	11
	unsigned char HHMMNN6[3];		//第6时段表 11
	unsigned char HHMMNN7[3];		//第7时段表 11
	unsigned char HHMMNN8[3];		//第8时段表	11  
	unsigned char HHMMNN9[3];		//第9时段表 11
	unsigned char HHMMNN10[3];		//第10时段表 11
	unsigned char HHMMNN11[3];		//第11时段表 11
	unsigned char HHMMNN12[3];		//第12时段表 11
	unsigned char HHMMNN13[3];		//第13时段表 11
	unsigned char HHMMNN14[3];		//第14时段表 11	  					
	unsigned char Checksum10;		// 43	
	unsigned char Reserved10[5];	//5字节  48
	
	//21 page 14+21=35 0x04e4
	unsigned char HHMMNN22_88[7*48];		//第2~8时段表 11	
	
	//0X6B0
	//费率电价信息	11
	unsigned int TariffPrice1_B;		//费率电价1  2字节 11
	unsigned int TariffPrice2_B;		//费率电价2  2字节 11
	unsigned int TariffPrice3_B;		//费率电价3  2字节 11
	unsigned int TariffPrice4_B;		//费率电价4  2字节 11
	unsigned int TariffPrice5_B;		//费率电价5  2字节 11
	unsigned int TariffPrice6_B;		//费率电价6  2字节	 11
	unsigned char Reserved5_B[3];		//1字节	11							
	unsigned char Checksum5_B;			//16	
		
#endif
	///////////////////////////////////////////////////////////////////////////////
	
	//累计27+75=102 pages
	
	//阶梯使能 5*16 11
	///////////////////////////////////////////////////////////////////////////////
#ifdef LADDER_ENABLE 
					
	//阶梯值信息	36
	unsigned int Ladder1;		//阶梯值1  2字节 11
	unsigned int Ladder2;		//阶梯值2  2字节 11
	unsigned int Ladder3;		//阶梯值3  2字节 11
	unsigned int Ladder4;		//阶梯值4  2字节 11
	unsigned int Ladder5;		//阶梯值5  2字节 11	
	unsigned int Ladder6;		//阶梯值6  2字节 11
	unsigned int Ladder7;		//阶梯值7  2字节 11								
	unsigned char Checksum6;		//11
	unsigned char Reserved6;		//5字节	16

	//阶梯电价信息	37
	unsigned int LadderPrice1;		//阶梯电价1  2字节 11
	unsigned int LadderPrice2;		//阶梯电价2  2字节 11
	unsigned int LadderPrice3;		//阶梯电价3  2字节 11
	unsigned int LadderPrice4;		//阶梯电价4  2字节 11
	unsigned int LadderPrice5;		//阶梯电价5  2字节 11
	unsigned int LadderPrice6;		//阶梯电价6  2字节	11	
	unsigned int LadderPrice7;		//阶梯电价7  2字节	11
	unsigned int LadderPrice8;		//阶梯电价8  2字节	11	
	
	//阶梯值切换时间	36
	unsigned char YYMM[2];		//  2字节 11
	unsigned char LADDER_BASE_YYMMDD[3];		//基准切换年月日  3字节 11
	unsigned char LADDER_YYMMDD[3];		//下次切换年月日  3字节 11
	unsigned char LADDER_Reserved7[7];		//1字节	 11	
	unsigned char LADDER_Checksum;			//16
	
	//阶梯值信息	23		
	unsigned int Ladder1_B;		//阶梯值1  2字节 11
	unsigned int Ladder2_B;		//阶梯值2  2字节 11
	unsigned int Ladder3_B;		//阶梯值3  2字节 11
	unsigned int Ladder4_B;		//阶梯值4  2字节 11
	unsigned int Ladder5_B;		//阶梯值5  2字节 11									
	unsigned char Checksum6_B;			//11
	unsigned char Reserved6_B[5];		//5字节	16
	
	//阶梯电价信息	 24	
	unsigned int LadderPrice1_B;		//阶梯电价1  2字节 11
	unsigned int LadderPrice2_B;		//阶梯电价2  2字节 11
	unsigned int LadderPrice3_B;		//阶梯电价3  2字节 11
	unsigned int LadderPrice4_B;		//阶梯电价4  2字节 11
	unsigned int LadderPrice5_B;		//阶梯电价5  2字节 11
	unsigned int LadderPrice6_B;		//阶梯电价6  2字节	11
	unsigned char Reserved7_B[3];		//1字节	 11							
	unsigned char Checksum7_B;			//16						
			
	//阶梯值切换时间	36
	unsigned char YYMM_B[2];		//  2字节 11
	unsigned char LADDER_BASE_YYMMDD_B[3];		//基准切换年月日  3字节 11
	unsigned char LADDER_YYMMDD_B[3];		//下次切换年月日  3字节 11
	unsigned char LADDER_Reserved7_B[7];		//1字节	 11	
	unsigned char LADDER_Checksum_B;			//16
#endif

//无功电量参数 11
#ifdef INACITVE_POWER_ENABLE	
	///////////////////////////////////////////////////////////////////////////////
	unsigned char InactiveQuan_Low[48];		//掉电时保存 无功电量 4字节 11
		
	//无功电量数据 8~10  预留48字节 11
	///////////////////////////////////////////////////////////////////////////////
	unsigned long InactiveQuan;			//无功电量  4	 11	
	unsigned long PosInactiveAcc;		//当前正向无功电能 4 11	
	unsigned long PosInactiveAccT1;
	unsigned long PosInactiveAccT2;
	unsigned long PosInactiveAccT3;
	unsigned long PosInactiveAccT4;
	unsigned long NegInactiveAcc;		//当前反向无功电能 4 11	
	unsigned long NegInactiveAcc1T1;
	unsigned long NegInactiveAcc1T2;
	unsigned long NegInactiveAcc1T3;
	unsigned long NegInactiveAcc1T4;
	unsigned char InactivePulseCnt;		//无功电能脉冲数  11
	unsigned char Checksum8;			//1  11
	unsigned char Reserved25[2];		//3字节	 11	
			      	  		  	
	
	//无功电量数据 8~10  预留48字节 11
	unsigned long InactiveQuan_B;			//无功电量  4	 11	
	unsigned long PosInactiveAcc_B;		//当前正向无功电能 4 11	
	unsigned long NegInactiveAcc_B;		//当前反向无功电能 4 11	
	unsigned char InactivePulseCnt_B;		//无功电能脉冲数  11
	unsigned char Checksum8_B;			//1  11
	unsigned char Reserved25_B[34];		//3字节	 11


	
	//////////////////////////////////////////////////////////////////////////////////
#endif

#if( FREEZE_DAY_EANBLED== 1)
	//0x720  多费率不带需量 11
	unsigned char LastDay1FreezeActive[4];			//（上1天）整点冻结正向有功总电能 4	 11
	unsigned char LastDay2FreezeActive[4];			//（上2天）整点冻结正向有功总电能 4	 11
	unsigned char LastDay3FreezeActive[4];			//（上3天）整点冻结正向有功总电能 4	 11
	unsigned char LastDay4FreezeActive[4];			//（上4天）整点冻结正向有功总电能 4	 11	
#endif

#if(FREEZE_HOUR_EANBLED==1)	
	//1+9*100+11=912=57*16
	/////////////////////////////////////////////////////////////////////////////	
	//整点冻结电量数据 目前只支持有功总电量 11
	unsigned char FreezePoint;
	unsigned char Last1FreezeTime[5];			//（上1次）整点冻结时间 5	 11
	unsigned char Last1FreezeActive[4];			//（上1次）整点冻结正向有功总电能 4	 11
	unsigned char LastFreezeTime[9*99];			//（上n次）整点冻结时间 9*99	 11
   	unsigned char ReservedFreeze[11];		//11字节	 11
	/////////////////////////////////////////////////////////////////////////////	
#endif
	
#if(LOSS_PHASE_EANBLED==1)		
	//(6+14*10)*3+10=448=28*16
	/////////////////////////////////////////////////////////////////////////////	
	//断相记录 11
	
	
	unsigned char PhaseALossCnt[3];			//A相断相总次数 11
	unsigned char PhaseALossMinute[3];			//A相断相总累计时间	 11
	unsigned char PhaseALossLast1Record[14];			//（上1次）A相断相记录 11	 11
   	unsigned char PhaseALossLast2Record[14];			//（上2次）A相断相记录 11
   	unsigned char PhaseALossLast3Record[14];			//（上3次）A相断相记录 11
   	unsigned char PhaseALossLast4Record[14];			//（上4次）A相断相记录 11
   	unsigned char PhaseALossLast5Record[14];			//（上5次）A相断相记录 11
   	unsigned char PhaseALossLast6Record[14];			//（上6次）A相断相记录 11
   	unsigned char PhaseALossLast7Record[14];			//（上7次）A相断相记录 11
   	unsigned char PhaseALossLast8Record[14];			//（上8次）A相断相记录 11
   	unsigned char PhaseALossLast9Record[14];			//（上9次）A相断相记录 11
   	unsigned char PhaseALossLast10Record[14];			//（上10次）A相断相记录 11 	
   	
   	//B断相记录 11
	unsigned char PhaseBLossCnt[3];			//B相断相总次数 11
	unsigned char PhaseBLossMinute[3];			//B相断相总累计时间	 11
	unsigned char PhaseBLossLast1Record[14];			//（上1次）B相断相记录 11	 11
   	unsigned char PhaseBLossLast2Record[14];			//（上2次）B相断相记录 11
   	unsigned char PhaseBLossLast3Record[14];			//（上3次）B相断相记录 11
   	unsigned char PhaseBLossLast4Record[14];			//（上4次）B相断相记录 11
   	unsigned char PhaseBLossLast5Record[14];			//（上5次）B相断相记录 11
   	unsigned char PhaseBLossLast6Record[14];			//（上6次）B相断相记录 11
   	unsigned char PhaseBLossLast7Record[14];			//（上7次）B相断相记录 11
   	unsigned char PhaseBLossLast8Record[14];			//（上8次）B相断相记录 11
   	unsigned char PhaseBLossLast9Record[14];			//（上9次）B相断相记录 11
   	unsigned char PhaseBLossLast10Record[14];			//（上10次）B相断相记录 11
   	
   	//C断相记录 11
	unsigned char PhaseCLossCnt[3];			//C相断相总次数 11
	unsigned char PhaseCLossMinute[3];			//C相断相总累计时间	 11
	unsigned char PhaseCLossLast1Record[14];			//（上1次）C相断相记录 11	 11
   	unsigned char PhaseCLossLast2Record[14];			//（上2次）C相断相记录 11
   	unsigned char PhaseCLossLast3Record[14];			//（上3次）C相断相记录 11
   	unsigned char PhaseCLossLast4Record[14];			//（上4次）C相断相记录 11
   	unsigned char PhaseCLossLast5Record[14];			//（上5次）C相断相记录 11
   	unsigned char PhaseCLossLast6Record[14];			//（上6次）C相断相记录 11
   	unsigned char PhaseCLossLast7Record[14];			//（上7次）C相断相记录 11
   	unsigned char PhaseCLossLast8Record[14];			//（上8次）C相断相记录 11
   	unsigned char PhaseCLossLast9Record[14];			//（上9次）C相断相记录 11
   	unsigned char PhaseCLossLast10Record[14];			//（上10次）C相断相记录 11
   	
   	unsigned char PhaseCLossReserved[10];
	/////////////////////////////////////////////////////////////////////////////		
#endif

	//负载越限 11
#if(OVER_POWER_EANBLED==1)		
	//6+14*10+14=160=10*16
	/////////////////////////////////////////////////////////////////////////////	
	//负载越限 11
	unsigned char OverPowerCnt[3];			//总次数 11
	unsigned char OverPowerMinute[3];	
	unsigned char OverPowerLast1[20];
	unsigned char OverPowerLast2[20];
	unsigned char OverPowerLast3[20];
	unsigned char OverPowerLast4[20];
	unsigned char OverPowerLast5[20];
	unsigned char OverPowerLast6[20];
	unsigned char OverPowerLast7[20];
	unsigned char OverPowerLast8[20];
	unsigned char OverPowerLast9[20];
	unsigned char OverPowerLast10[20];
	unsigned char OverPowerReserved[2];
#endif	

	
	//不需要备份区数据 11
	////////////////////////////////////////////////////////////////////////
	
	
	//三相需量 11
#ifdef DEMAND_ENABLE	
	//1+12= 14 pages
	unsigned char ReservedDemandPara[16];			//A相断相总次数 11
	
	unsigned char DemandPara[11];		//当前月需量数据 24字节 11
	unsigned char ReservedDemand[5];
	unsigned char CurDemand[16];		//上1月需量数据 24字节 11
	unsigned char Last1Demand[16];		//上1月需量数据 24字节 11
	unsigned char Last2Demand[16];		//上2月需量数据 24字节 11
	unsigned char Last3Demand[16];		//上3月需量数据 24字节 11
	unsigned char Last4Demand[16];		//上4月需量数据 24字节 11
	unsigned char Last5Demand[16];		//上5月需量数据 24字节 11
	unsigned char Last6Demand[16];		//上6月需量数据 24字节 11
	unsigned char Last7Demand[16];		//上7月需量数据 24字节 11
	unsigned char Last8Demand[16];		//上8月需量数据 24字节 11
	unsigned char Last9Demand[16];		//上9月需量数据 24字节 11
	unsigned char Last10Demand[16];		//上10月需量数据 24字节 11
	unsigned char Last11Demand[16];		//上11月需量数据 24字节 11
	unsigned char Last12Demand[16];		//上12月需量数据 24字节 11	
#endif	
		
	//事件记录 11
	
	
	
}EEPROM_DATA;

			
#endif
