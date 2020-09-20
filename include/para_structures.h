/*
  -this file contains declarations of functions and variables used in protocol.c.
*/

#ifndef _PARA_STRUCTUTRES_H

#define _PARA_STRUCTUTRES_H

//地址参数相关 11
typedef struct MeterAddr_tag
{
	unsigned int Zone;			//分区号 2 11
	unsigned char CardNum[6];	//用户编号 卡号 11
	unsigned char MeterAddr[6];	//表地址 11
	
	unsigned char BuyMode;		//单电价、多费率等购电模式  11	
	unsigned char Checksum;		//16	
}Meter_Addr;
#define Meter_Addr_Len		sizeof(Meter_Addr)     //16

//运行参数相关 11
typedef struct MeterBuyPara_tag
{
	unsigned int BuyCount;		//购电次数 11
	unsigned long BuyQuan;		//购电金额 11
	
	unsigned int Alarm1;		//报警金额1 11
	unsigned int Alarm2;		//报警金额2 11
	
	unsigned int FactorLimit;		//功率因数限值 2 11	
	unsigned int FactorPower;		//功率因数对应功率限值 2 11	
	
	unsigned char Status;		//表计状态 11
	unsigned char Checksum;		//16	
}Meter_Buy_PARA;
#define Meter_Buy_PARA_Len		sizeof(Meter_Buy_PARA)     //16

//电量与金额相关 11
typedef struct MeterPara_tag
{			
	unsigned int LimitPower;		//功率限值 11
	unsigned int MaximBalance;		//囤积限额 11
	
	unsigned long CreditLimit;			//透支限额 4 11
	unsigned int CurrentRatio;			//电流互感比 2 11
	
	unsigned char BonusMoneyNum;			// 补助金额有效序号模式字 11
	unsigned char RelayFlag;			// 1 11
	
	unsigned int Price;					//基础电价 2 11
	unsigned char MeterTpye;			//表型判断 11					
	unsigned char Checksum;			//16
}Meter_Para;
#define Meter_Para_Len		sizeof(Meter_Para)     //16

//bit0---0 保电解除 1 保电 11
//bit1---0 远程拉闸解除 1 远程拉闸 11
//bit5---外置继电器控制模式：0--脉冲式；1--电平式 11
//bit6---0 不启用时间段控制 1 启用时间段控制 11
//bit7---0 赠送月清零 1 赠送累计 11

//电量相关  64 11
typedef struct MeterEQuan_tag
{			
	unsigned long Accumulate;		//累计电量 4 11
	unsigned long TariffQ1;			//费率1 11
	unsigned long TariffQ2;			//费率2 11
	unsigned long TariffQ3;			//费率3 		16
	
	unsigned long TariffQ4;			//费率4 11	
	unsigned long CurrentQ;			//当前月电量 4	 11
	unsigned long Balance;			//剩余金额 4 11
	unsigned long Overdraft;		//过零金额 4 11		16
	
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
	unsigned char Checksum;			//1  64						
	
}Meter_EQuan;

#define Meter_EQuan_Len		sizeof(Meter_EQuan)     //64

//无功电量相关  48 11
typedef struct MeterInactivQuan_tag
{				
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
	unsigned char Checksum;			//1  48							
	
}Meter_InactiveQuan;

#define Meter_InactiveQuan_Len		sizeof(Meter_InactiveQuan)     //48



//需量参数  48 11
typedef struct MeterDemand_tag
{				
	unsigned long PosActive;		//正向有功总最大需量 11		2019/12/26 11
	unsigned long NegActive;		//反向有功总最大需量11		2019/12/26 11
	
//	unsigned long Q1Inactive;			//第1象限无功总最大需量 4 11
//	unsigned long Q2Inactive;			//第2象限无功总最大需量 4 11
//	unsigned long Q3Inactive;			//第3象限无功总最大需量 4 11
//	unsigned long Q4Inactive;			//第4象限无功总最大需量 4 11
	
	unsigned char DemandCycle;		//最大需量周期 11
	unsigned char DemandSlide;		//滑差时间	
	unsigned char reserved;		//滑差时间	
	unsigned char Checksum;			//1  					
}Meter_Demand;
#define Meter_Demand_Len		sizeof(Meter_Demand)     //21

//时间结构体 11
typedef struct DateTime_tag
{			
	unsigned char sec;		//秒 11
	unsigned char min;		//分 11
	unsigned char hour;		//时 11
	unsigned char week;		//星期 11
	unsigned char day;		//日 111
	unsigned char month;	//月 11
	unsigned char year;		//年 11
	unsigned char reserved;	//  1×Ö½Ú 11
	//8
}Date_Time;



typedef union Card_STATUS_tag
{
	unsigned char Byte;
	struct
	{
		unsigned char CardNotConsistent:1;
		unsigned char FrameErr:1;
		unsigned char ZoneWrong:1;
		unsigned char BuyCntErr:1;
		unsigned char OverLimit:1;
		unsigned char CardIDErr:1;
		unsigned char CardUsed:1;
		unsigned char StatusErr:1;
	}Bit;
}Card_STATUS;

//flags used in processing
////////////////////////////////////////////////////////////////////////////////////
typedef union METER_FLAG_tag
{
	unsigned int Byte;
	struct
	{		
		unsigned char HalfSec:1;
		unsigned char Cl2208check:1;		
		unsigned char LadderProcess:1;
		unsigned char PLCRcvOvertime:1; 
	    		
		unsigned char OneSec:1;
		unsigned char PowerDown:1;    
		unsigned char IrRcvOvertime:1;
		unsigned char RS485RcvOvertime:1;	
			
		unsigned char DemandProcess:1;
		unsigned char RFInFieldFlag:1;		
		unsigned char ZeroSecondFlag:1;
        unsigned char RevPFlag:1;       //µçÄÜ·´Ïò±êÊ¶ 11
			
		unsigned char OneLoopTask:1;		
		unsigned char TariffProcess:1;
		unsigned char CycleDisp:1;    
		unsigned char MaglentFlag:1;	
		    	    	    
				  
	}Bit;
}METER_FLAG;

//flags used in processing
////////////////////////////////////////////////////////////////////////////////////
typedef union POWER_DIR_FLAG_tag
{
	unsigned char Byte;
	struct
	{									
		unsigned char RevPFlagA:1;
		unsigned char RevPFlagB:1;	
		unsigned char RevPFlagC:1;			
		unsigned char RevPFlag:1;    
	    	    	   	    						
		unsigned char RevQFlagA:1;
		unsigned char RevQFlagB:1;	
		unsigned char RevQFlagC:1;		
		unsigned char RevQFlag:1;    
	    	    	    					    	    	    				  
	}Bit;
}POWER_DIR_FLAG;

typedef union PHASE_POS_FLAG_tag
{
	unsigned char Byte;
	struct
	{									
		unsigned char ActiveDir:1;    
	    unsigned char InactiveDir:3;
	    						
		unsigned char :1;    
	    unsigned char :1;
	    unsigned char :1;	
	    unsigned char :1;
					    	    	    				  
	}Bit;
}PHASE_POS_FLAG;

typedef union LCD_DISP_FLAG_tag
{
	unsigned char Byte;
	struct
	{									
		unsigned char :1;    
	    unsigned char :3;
	    						
		unsigned char LadderDisp:1;    
	    unsigned char TariffDisp:1;
	    unsigned char TimerBlink:1;	
	    unsigned char :1;
					    	    	    				  
	}Bit;
}LCD_DISP_FLAG;

//this meter status byte should redefine according to different province's protocol.
typedef union METER_ErrSTATUS_tag
{
	unsigned char Byte;
	struct
	{
		unsigned char E2ROM:1;
		unsigned char RelayErr:1;
		unsigned char OverPower:1;
		unsigned char OverVoltage:1;
		unsigned char CrossZero:1;		    
		unsigned char CardDataErr:1;
		unsigned char RLoadHappened:1; 
		unsigned char EMUErr:1; 	
	}Bit;
}METER_ErrSTATUS;

typedef union RELAY_FLAG_tag
{
	unsigned char Byte;
	struct
	{		
		unsigned char RelayAlarmOn:1;    
	    unsigned char RelayAlarmHappened:1;
		
		unsigned char RelayInTest:1;    
		unsigned char RelayOn:1;    
		unsigned char RelayNeedToOperate:1;	
		    
		unsigned char StatusToSave:1;
		unsigned char BalanceZero5MinFlg:1;		
				
		unsigned char BalanceZeroHappened:1;  
		      	    
			  
	}Bit;
}RELAY_FLAG;

typedef union CARD_FLAG_tag
{
	unsigned char Byte;
	struct
	{		
		unsigned char ICCardRightIn:1;
		unsigned char ICCardOutUse:1;		
		unsigned char ICCardIn:1;
		unsigned char ICCardInUse:1;
			    
		unsigned char KEY1BtnDown:1;			    
		unsigned char KEY2BtnDown:1;
		unsigned char BtnFreshLCD:1;				
		unsigned char :1;    	    
			  
	}Bit;
}CARD_FLAG;

typedef struct CL2208_SF_tag
{ 
	unsigned long 	PRms;				//有功功率  4	
	unsigned long 	SRms;				//无功功率  4
	
}CL2208_SF;

////////////////////////////////////////////////////////////////////////////////////


//IR channel communication buffer according to my protocol
typedef struct COMM_BUFFER_tag
{
	//unsigned char  Address[6];
	unsigned char  Command;
	unsigned char  Len;
  	unsigned char  Data[BUF_MAX_LEN-12];
    	
}COMM_BUFFER;

//Í¨ÐÅÊ¹ÓÃ±êÖ¾Î» 11
typedef union COMM_STATUS_tag
{ 	
	unsigned int Byte;
	struct
	{   	
	unsigned char Active:1;
	unsigned char ByteAvailable:1;  				
	unsigned char DataLoss:1; 		  	  			
	unsigned char ParityError:1;
				
	unsigned char MessageComplete:1;
	unsigned char MessageReady:1;
	unsigned char IsBroadcast:1;	
	unsigned char IsSuperAddress:1;
			
	unsigned char MessageIsForMe:1;			
	unsigned char IsDL645Protocol:1;									   		   			
	unsigned char TransmitReady:1;				
	unsigned char MessageFormatetterComplete:1;	
	
	unsigned char TransmitComplete:1;
	unsigned char ModbusSendData:1;
	unsigned char RxdFrameDone:1;			
	unsigned char IsModbusProtocol:1;
	}Bit;	
}COMM_STATUS;

typedef union GLOBLE_COMM_STATUS_tag
{ 	
	unsigned int Byte;
	struct
	{   	
	unsigned char :1;
	unsigned char :1;
	unsigned char :1; 		  	  			
	unsigned char :1;
					
	unsigned char :1;
	unsigned char :1;
	unsigned char RS485RxdFrameDone:1;			
	unsigned char IRRxdFrameDone:1;
	}Bit;	
}GLOBLE_COMM_STATUS;

typedef union COMM_ERROR_tag
{
	unsigned char Byte;
	struct{
	unsigned char otherErr :1;
	unsigned char noRequestData:1;	
	unsigned char badPassword:1;
	unsigned char baudRateErr:1;
			
	unsigned char overYearPeriods:1;
	unsigned char overDayPeriods:1;		
	unsigned char overTarriffs:1;						
	unsigned char balanceZero:1;
	}Bit;						
}COMM_ERROR;

////ºìÍâÍ¨ÐÅÊ¹ÓÃ²ÎÊý 11
typedef struct COMM_PARA_tag
{
//	unsigned char 	*pHead;
//	unsigned char 	*pEnd;
//	unsigned char  FIFO[BUFF_LEN];
	
	unsigned char RxdHead;		//1
	unsigned char RxdEnd;		//1
	
	unsigned char RxdByte;		//1
	unsigned char ErrorByte;	//1	
	
	unsigned char TxdByte;		//1	
		
	unsigned char MCState;		//1	
	unsigned char MCcs;			//1	
	unsigned char MBState;		//1	
	unsigned char MBcs;			//1	
	unsigned char MBPtr;		//1	
	
    	
}COMM_PARA;

//计量相关参数结构体定义 start
///////////////////////////////////////////////////////////////////
#if(METER_CHIP_MODEL==0x2208)
	typedef struct CL2208_FACTOR_tag
	{ 
		unsigned long 	UFactor;				//电压系数  4 11
		unsigned long	IFactor;				//电流1系数 4 11
		unsigned long   PFactor;				//功率系数  4 11
		unsigned char 	Reserved[3];			//3字节	 11
		unsigned char 	checksum;				//校验码	   1  16 11	
	}CL2208_FACTOR;	
	
	typedef struct CL2208_UIP_tag
	{ 
		unsigned long 	URms;					//电压有效值  4 11
		unsigned long	IRms;					//电流1有效值 4 11
		unsigned long   PRms;					//功率有效值  4 11
		unsigned int    Freq;					//频率值  2 11
		unsigned int 	Factor;			        //功率因数 2字节	 11

	}CL2208_UIP;
	
	typedef struct CL2208_Para_tag
	{ 
		unsigned int   	pstart;				//2
		unsigned int 	gpqa;				//2	
		unsigned int 	phase;				//2
		unsigned int 	apor;				//2
		unsigned int 	iarmsos;			//2
		unsigned int 	hfconst;			//2
		unsigned int 	Reserved;			//2字节	 11
		unsigned int 	checksum;			//2	14
	}CL2208_Para;
	
#else if (METER_CHIP_MODEL==0x8302)
	
	typedef struct RN8302_UIP_tag
	{ 
		unsigned long 	URms;					//电压有效值  4 11
		unsigned long	IRms;					//电流1有效值 4 11
		unsigned long   PRms;					//功率有效值  4 11
		unsigned long   QRms;					//无功功率有效值  4 11
		unsigned long   SRms;					//视在功率有效值  4 11
		unsigned int 	Factor;			        //功率因数 2字节	 11	
	#if(MODBUS_ENABLED==1)	
		unsigned int 	PhaseAngel;			        //相角 2字节	 11	
	#endif		    
	}RN8302_UIP;
	
	typedef struct RN8302_Para_tag
	{ 	
		unsigned int 	gsua;				//2
		unsigned int 	gsub;				//2		
		unsigned int 	gsuc;				//2
		
		unsigned int 	gsia;				//2
		unsigned int 	gsib;				//2
		unsigned int 	gsic;				//2
			
		unsigned int 	pa_phs;				//2		
		unsigned int 	pb_phs;				//2			
		unsigned int 	pc_phs;				//2
		
		unsigned int 	ia_os;				//2 11
		unsigned int 	ib_os;				//2 11
		unsigned int 	ic_os;				//2 11	

		unsigned char 	checksum[3];			//2	
		unsigned char	sum;
		unsigned char 	reserved1[4];		//4		32
		
	}RN8302_Para;
	
#endif

///////////////////////////////////////////////////////////////////
//计量相关参数结构体定义 end

//radio frequecy IC card flag
/*****************************************************************************************/
typedef union RF_FLAG_tag
{
    unsigned char Byte;
    struct{        
        unsigned char CurrentRequestOK:1;
        unsigned char LastRequestOK:1;       //
        unsigned char Request:1;
        unsigned char :1;        
        
        unsigned char :1;     //                
        unsigned char :1;     //        
        unsigned char :1;
        unsigned char :1;        		
    }Bit;            
}RF_STATUS_FLAG;


union MESSAGE_BOARD
{ 	
	unsigned int Byte;
	struct
	{   	
	unsigned char Uart6Rxd:1;  				
	unsigned char Uart6Txd:1; 		  	  	
	unsigned char IRRxd:1;	
	unsigned char IRTxd:1;		
	unsigned char :1;	
	unsigned char :1;	
	unsigned char :1;		
	unsigned char :1;
		   	
	unsigned char OneSec:1;	
	unsigned char HalfSec:1;	
	unsigned char :1;   			
	unsigned char :1;				
	unsigned char :1;	
	unsigned char :3;
	}Bit;	
};
////////////////////////////////////////////////////////////////////////////////////

//运行状态字3 定义 11
typedef union METER_STATUS3_tag
{
	unsigned int Byte;
	struct
	{		
		unsigned char NoMoney:1;
		unsigned char OverPower:1;		
		unsigned char Alarm2Status:1;
		unsigned char MagliStatus:1;   
	    		
		unsigned char RelayStatus:1;
		unsigned char :1;    
		unsigned char RelayCmdStatus:1;
		unsigned char PreOffStatus:1;	
			
		unsigned char :1;
		unsigned char :1;		
		unsigned char StatusMeter:1;
		unsigned char :1;      
			
		unsigned char KeepRelay:1;		
		unsigned char :1;
		unsigned char :1;    
		unsigned char :1;	
				  
	}Bit;
}METER_STATUS3;

//modbus一类参数 11
//Model bus结构体 11
typedef struct ModBus_RTU_tag
{			
	unsigned int Reg;		//寄存器地址 2字节 11
	unsigned char RegLen:4;	//寄存器长度 1字节 11
	unsigned char Type:4;	    //数据类型 1字节 11
	unsigned char DotPos:4;	    //小数点位置 1字节 11	
	unsigned char PointerType:4;	    //指针类型 字节 11
	unsigned int RamAddr;	//数据指针地址 2字节 11
	
}ModBus_RTU_ARRAY;


//波特率结构体 11
typedef struct BAUDRATE_PARA_tag
{			
	unsigned int baudrate;		//波特率 2字节 11
	unsigned char checkType:4;	//校验方式 1字节 11
	unsigned char relayMode:4;	//外置继电器模式字 1字节 11
	unsigned char checksum;		//校验方式 1字节 11
}BAUDRATE_PARA;

#endif
