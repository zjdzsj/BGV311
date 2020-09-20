/*
  -this file contains declarations of functions and variables used in this project.
*/

#ifndef _EXTERN_DECLARE_H

#define _EXTERN_DECLARE_H


//#include <stddef.h>

#define KUAISU_TEST		(0)		//快速走字 --启用；0--关闭 11
#define ENCRYPT_ENABLED	(0)		//加密算法 1--启用；0--关闭 11
//新抄表卡 0---老的不带费率电量 1--返回带费率电量 11
#define NEW_READ_IC_CARD_ENABLED	(1)		


#define PCB_VERSION_NEW		(1)		//0--老版本  1--新版本 11
#define PCB_VERSION_NEW_4	(0)		//0--老版本  1---4新版本  更改串口P0.4 P0.3与0.1 0.2对换11

//2019年10月18日 修改增加 因为老板子焊接了新的屏 所以增加这个宏来控制 11
#define PCB_LCD_NEW			(1)		//0--老版本  1---4新版本 11

//2020/04/24 修改液晶显示 新液晶屏 11


#define RF_CARD				0
#define AT24_CARD			1
#define PLC_COMM_USED		2
#define IC_CARD_MODE		AT24_CARD	//
//#define IC_CARD_MODE		RF_CARD		//
//#define IC_CARD_MODE		PLC_COMM_USED	//NB


//协议中增加了4字节序列号 宏定义区分 2020/04/07 11
#define PROTOCOL_ADD_4_SERIALS		(1)		//1--启用		0--不启用 11
#define PROTOCOL_ADD_ORDERS			(1)		//1--启用		0--不启用 11

//编译相关 start
/////////////////////////////////////////////////////////////////////////////////

//#define NORMAL_METER			//普通表 11
//#define NORMAL_TARIFF_METER		//普通多费率表 11
//#define PREPAID_METER			//预付费表 11
#define PREPAID_TARIFF_METER	//预付费多费率表 11
//#define PREPAID_METER_MODBUS			//预付费表-带MODBUS 11

//佳测测试版 购电次数限制50次 11
#define JIA_CE_TEST		(0)		//50--启用；0--关闭 11

//1--使能新协议 0--使用17年含之前程序 11
#define NEW_PROTOCOL_2018		(1)			

//#define SINGLE_OR_THREE_METER	(1)		//1--单相 11
#define SINGLE_OR_THREE_METER	(3)		//3--三相 11



//普通表 11
#ifdef NORMAL_METER
	//都不定义 11
	#define MODBUS_ENABLED		(0)			//modbus协议使能:0--disable; 1--enabled 11
#endif

//普通多费率表 11
#ifdef NORMAL_TARIFF_METER
	#define RTC_8025T
	//#define SOFT_RTC	
	#define MODBUS_ENABLED		(0)			//modbus协议使能:0--disable; 1--enabled 11
#endif

//预付费表 11
#ifdef PREPAID_METER
	#define PRE_PAID_ENABLE     //预付费使能 11
	//#define BONUS_PADI_ENABLE	//赠费功能使能 11
	#define MODBUS_ENABLED		(0)			//modbus协议使能:0--disable; 1--enabled 11

#endif

//预付费多费率表 11
#ifdef PREPAID_TARIFF_METER
	#define PRE_PAID_ENABLE     //预付费使能 11
	//#define BONUS_PADI_ENABLE	//赠费功能使能 11

	#define RTC_8025T
	//#define SOFT_RTC	
	#define MODBUS_ENABLED		(0)			//modbus协议使能:0--disable; 1--enabled 11
#endif

//预付费表-MODBUS 11
#ifdef PREPAID_METER_MODBUS
	#define PRE_PAID_ENABLE     //预付费使能 11
	//mobus使能 11
	#define MODBUS_ENABLED		(1)			//modbus协议使能:0--disable; 1--enabled 11
#endif

//计量芯片型号 决定单相还是三相 11
#if(SINGLE_OR_THREE_METER==3)
	#define METER_CHIP_MODEL		(0x8302)
#else
	#define METER_CHIP_MODEL		(0x2208)
#endif

//#define INACITVE_POWER_ENABLE   //无功计量使能 11
//#define DEMAND_ENABLE			//需量使能 11 常州 2019/12/26


#ifdef RTC_8025T
	#define EEPROM_PAGES			(128)			//24c16
	
	#define TARIFF_ENABLE			//多费率使能 11
	//#define LADDER_ENABLE			//阶梯使能 11
#else
	#define EEPROM_PAGES			(64)			//24c08
#endif

#define TARIFF_TABLE_NUMBER		(8)		//最大费率数 11
#define LADDER_VALUE_NUMER		(5)		//最大阶梯数 5 11

//#define TIMER_RELAY_USED		(0)		//0-时间段控制关闭；1-时间段控制使能1 11
//冻结电量相关 11
#define FREEZE_DAY_EANBLED		(0)		//0-disabled; 1--enabled 11
#define FREEZE_HOUR_EANBLED		(0)		//0-disabled; 1--enabled 11
//恶性负载使能 11
#define MALIGNANT_LOAD_CHK_EANBLED		(0)		//0-disabled; 1--enabled 11
//断相事件记录使能 11
#define LOSS_PHASE_EANBLED		(0)		//0-disabled; 1--enabled 11
//负载越限事件记录使能 11
#define OVER_POWER_EANBLED		(0)		//0-disabled; 1--enabled 11
//负载跳闸使能 11
#define OVER_POWER_RELAY_OFF_EANBLED	(1)		//0-disabled; 1--enabled 11

//误差改变卡 使能 11
#define CALI_CHANGE_CARD_ENABLED		(0)

#define IR_CHANNEL_ENABLE
#define RS485_CHANNEL_ENABLE

#if(SINGLE_OR_THREE_METER==3)
	//液晶型号 1  11
	//#define LCD_TYPE_DEFINE     (0x0822)
	//#define LCD_TYPE_DEFINE     (0x0823)
	#define LCD_TYPE_DEFINE     (0x0806)
#endif

//电流规格 11
#if(SINGLE_OR_THREE_METER==3)
	#define IB_20A_RELAY_OUT
	//#define IB_20A
	//#define IB_10A
	//#define IB_5A
	//#define IB_1_5A
	
	//#define CAL_RN8302_MODE		(33)		//三相三线 11
	#define CAL_RN8302_MODE		(34)		//三相四线 11
	
	#if (CAL_RN8302_MODE==33)		//三相三线 11
		//#define U_VOL_220_OR_57		(380)
		#define U_VOL_220_OR_57		(100)	
	#endif
	
	#if (CAL_RN8302_MODE==34)		//三相四线 11
		#define U_VOL_220_OR_57		(220)
		//#define U_VOL_220_OR_57		(57)	
	#endif
	
#endif

#ifdef IB_20A_RELAY_OUT
	#define RELAY_OUT_CONTROL
#endif

#ifdef IB_1_5A
	#define RELAY_OUT_CONTROL
#endif

//单相版本 11
#if (SINGLE_OR_THREE_METER==1)
	#define VERSION_HEAD_1		(1)
	#ifdef PRE_PAID_ENABLE					
	 	#ifdef TARIFF_ENABLE
		 	#define VERSION_HEAD_2		(6)
		#else
			#define VERSION_HEAD_2		(7)
		#endif
	#else
		
		#ifdef PREPAID_METER_MODBUS
			#define VERSION_HEAD_2		(5)		
		#else
			#ifdef TARIFF_ENABLE
		 		#define VERSION_HEAD_2		(8)
			#else
				#define VERSION_HEAD_2		(9)
			#endif
		#endif
	#endif
	
	#define VERSION_UPDATE_1		(1)		//软件更新版本 11
	
	#if (PCB_VERSION_NEW_4==1)
		#if(IC_CARD_MODE==PLC_COMM_USED)
			#define VERSION_UPDATE_2		(8)		//软件更新版本 11
		#else
			#define VERSION_UPDATE_2		(6)		//软件更新版本 11
		#endif
	#else
		#define VERSION_UPDATE_2		(7)		//软件更新版本 11
	#endif
#endif

//三相版本 11
#if (SINGLE_OR_THREE_METER==3)
	
	#if ( CAL_RN8302_MODE == 33 )
		#define VERSION_HEAD_1		(5)
	#else
		#define VERSION_HEAD_1		(3)
	#endif
	
	#ifdef PRE_PAID_ENABLE					
	 	#ifdef TARIFF_ENABLE
		 	#define VERSION_HEAD_2		(6)
		#else
			#define VERSION_HEAD_2		(7)
		#endif
	#else
		#ifdef PREPAID_METER_MODBUS
			#define VERSION_HEAD_2		(5)		
		#else
			#ifdef TARIFF_ENABLE
		 		#define VERSION_HEAD_2		(8)
			#else
				#define VERSION_HEAD_2		(9)
			#endif
		#endif		
	#endif
	
		
	#ifdef IB_20A_RELAY_OUT
		#define VERSION_UPDATE_1		(5)		//软件更新版本 11
	#endif
	#ifdef IB_20A
		#define VERSION_UPDATE_1		(4)		//软件更新版本 11
	#endif
	#ifdef IB_10A
		#define VERSION_UPDATE_1		(3)		//软件更新版本 11
	#endif
	#ifdef IB_5A
		#define VERSION_UPDATE_1		(2)		//软件更新版本 11
	#endif
	#ifdef IB_1_5A
		#define VERSION_UPDATE_1		(1)		//软件更新版本 11
	#endif
	
	#if (PCB_VERSION_NEW_4==1)
		#if( IC_CARD_MODE==RF_CARD )
			#define VERSION_UPDATE_2		(7)		//软件更新版本 11
		#else
			#define VERSION_UPDATE_2		(6)		//软件更新版本 11
		#endif
	#else 
		#if (PCB_VERSION_NEW==1)
			#define VERSION_UPDATE_2		(5)		//软件更新版本 11
		#else
			#if( IC_CARD_MODE==RF_CARD )
				#define VERSION_UPDATE_2		(4)		//软件更新版本 11
			#else		//ic card 2019/10/27
				#if( PCB_LCD_NEW==1 )
					#define VERSION_UPDATE_2		(2)	
				#else
					#define VERSION_UPDATE_2		(3)		//软件更新版本 11
				#endif
			#endif
		#endif
	#endif
#endif

/////////////////////////////////////////////////////////////////////////////////
//编译相关 end

#define COMM_LEN_ERR		(0X84)				//数据长度不符 11
#define COMM_METER_NUM_ERR		(0X31)			//表地址不符 11
#define COMM_PRICE_ERR		(0X83)			//电价超范围 11
#define COMM_CARD_NUM_ERR	(0X12)			//用户编号错 11
#define COMM_BUY_CNT_ERR	(0X13)			//购电次数错 11
#define COMM_OVER_BUY_QUAN_ERR       (0x42)          //超出最大购电值 11
#define HAVE_IN_RETURN_MODE		(0x38)		//退费模式无法售电 11
#define OPEN_CARD_INFO_ERR	(0x36)		//开户信息错 11
#define COMM_OVERLIMIT_ERR		(0X14)	//超囤积量 11
#define COMM_FIRST_BUY_LESS_ERR		(0X37)	//第一次购电金额小于预置金额 11
#define COMM_JIA_CE_TEST_ERR		(0x99)		//佳测限值到期 11
#define COMM_OVERMAX_ERR		(0X15)	//超最大购电金额  11
#define COMM_ORDER_NUM_ERR       (0x43)          //订单号格式错误 11
#define COMM_ORDER_OVER_ERR       (0x44)          //订单号时间已经超出内部存储数据 11
#define COMM_ORDER_USED_ERR       (0x45)          //订单号已使用过 11
#define WRITE_BACK_ERR 			    (0x58)		//返写卡出错 11

#define NORMAL_SAVE			(0)			//正常电压情况下保存 11
#define LOW_POWER_SAVE		(1)			//掉电时保存 11

//购电模式 11
#define NORMAL_MODE			(1)		//正常购电 11
#define BUY_RETURN_MODE			(3)		//售电退费 11
#define CANCEL_RETURN_MODE			(2)		//销户退费 11
#define BUY_WITH_ORDER_NUM_MODE			(4)		//带订单号购电 11

//#define MAX_LIMIT_POWER  (62) 
//#define MAX_LIMIT_POWER  (82)   ////¸Ä20AµÄ±í revised by Roger 2012/09/23
//#define MAX_LIMIT_POWER  (4000)   //À©Õ¹Îª2Î»Ð¡Êý revised by Roger 2013/09/07
//#define MIN_LIMIT_POWER  (20)   //2Î»Ð¡Êý added by Roger 2013/09/07

#define MAX_PRICE		(50000)		//5元  11
#define MIN_PRICE		(100)		//0.01元  11
#define DEFAULT_PRICE	(13000)		//默认电价1.3元 11

#define LCD_CYCLE_TIME		(5)
#define LCD_CLEAR_CNT		(3)

#define COEF_MAX			(100)		//最大误差20% 11
//meter constant definition
//////////////////////////////////////////////////////////////////////////////////////////


#define MAX_BALANCE_LIMIT     (10000)     //最大囤积量 10000 11
#define CREDIT_LIMIT            (0)         //赊欠限额 默认为0 11
#define CURRENT_RATIO            (1)         //电流互感比 11
#define TARIFF_NUMBER            (1)         //费率个数 11
#define ALARM1_DEFAULT            (10)         //报警金额1 11
#define ALARM2_DEFAULT            (0)         //报警金额2 11
#define ALARM_SET_DEFAULT            (0x03)     //报警启用标识 11
#define PRE_SET_BUY_QUAN            (500000)     //预置金额 11

//meter constant definition
//////////////////////////////////////////////////////////////////////////////////////////
#if(SINGLE_OR_THREE_METER==1)
	#define METER_CONSTANT      (1200)
	#define RELAY_DELAY_CNT         (40)		//继电器拉闸延时 150/4=35 11
	#define MAX_POWER_LIMIT     (65500)     // 65kw 11
	
	#define START_CURRENT		(20)			//20mA
	#define START_POWER			(30)			//3.0W
#endif

#ifdef IB_20A_RELAY_OUT
	#define METER_CONSTANT      (200)
	#define RELAY_DELAY_CNT         (40)		//继电器拉闸延时 150/4=35 11
	#define MAX_POWER_LIMIT     (65500)     //¼«ÏÞ¹¦ÂÊÖµ 65kw 11
	
	#define START_CURRENT		(70)			//70mA
	#define START_POWER			(60)			//6.0W
#endif

#ifdef IB_20A
	#define METER_CONSTANT      (200)
	#define RELAY_DELAY_CNT         (40)		//继电器拉闸延时 150/4=35 11
	#define MAX_POWER_LIMIT     (65500)     //¼«ÏÞ¹¦ÂÊÖµ 65kw 11
	
	#define START_CURRENT		(70)			//70mA
	#define START_POWER			(60)			//6.0W
#endif

#ifdef IB_10A
	#define METER_CONSTANT      (400)
	#define RELAY_DELAY_CNT         (40)		//继电器拉闸延时 150/4=35 11
	#define MAX_POWER_LIMIT     (65500)     //65kw 11
	
	#define START_CURRENT		(40)			//70mA
	#define START_POWER			(50)			//6.0W
#endif

#ifdef IB_5A
	#define METER_CONSTANT      (800)
	#define RELAY_DELAY_CNT     (50)		//继电器拉闸延时150/4=35 11
	#define MAX_POWER_LIMIT     (65500)     //65kw 11
	
	#define START_CURRENT		(20)			//70mA
	#define START_POWER			(30)			//6.0W
#endif	

#ifdef IB_1_5A
	#define METER_CONSTANT      (3200)
	#define RELAY_DELAY_CNT      (0)		//继电器拉闸延时 11
	#define MAX_POWER_LIMIT     (65500)     //65kw 11
		
	#define START_CURRENT		(3)			//3mA
	#define START_POWER			(6)			//0.6W
#endif


#define CONSTANT_FRACTION   (METER_CONSTANT/(100))
#define CONSTANT_0_01FRACTION   (METER_CONSTANT/(100))
#define MAX_POWER_CNT   	(METER_CONSTANT/(12))
//#define CONSTANT_FRACTION   (16)

//#define VALIDPULSEWIDTH_L   (0x4000)
//#define VALIDPULSEWIDTH_H   (0x8000)


#define PULSE_LED_WIDTH			(20)
#define VALIDPULSEWIDTH_L	 0x753      //60MS  ÐÞÕýÂö¿í¿ØÖÆÎª2012/12/27 11
//#define VALIDPULSEWIDTH_L	 0x9c4      //80MS 11
#define VALIDPULSEWIDTH_H	 0xc35		//100ms Ôö¼Ó×¢ÊÍ2012/12/27	 11


//#define VALIDPULSEWIDTH_L	 0x1e84     //270MS 11
//#define VALIDPULSEWIDTH_H	 0x2ab9

//#define VALIDPULSEWIDTH_L	 0
//#define VALIDPULSEWIDTH_H	 0xffff
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////

#define CHECK_CL2208		(0)		//2208Ð£ÑéÄ£Ê½ 11
#define CHECK_ADDMOD		(1)		//ÀÛ¼ÓºÍÐ£ÑéÄ£Ê½ 11
//////////////////////////////////////////////////////////////////////////////////////////////////


//#define SIZE_OF(TYPE, MEMBER) sizeof(((TYPE *)0)->MEMBER)
//#define NULL    ((void*)0)        

//typedef  void (*pFunction) ( void );
//#define ResetMeter     ((pResetMeter)0)( )
#define ResetMeter()     while(1)

#define uchar	unsigned char
#define uint16	unsigned int
#define uint32  unsigned long int

#define BUF_MAX_LEN (100)
#define FRAME_DELAY_CNT             (10)          //4*10
#define FRAME_SEND_CNT             (200)          //4*10
#define FeedWatchdog()        (WDTE = 0xac )   //clear watchdog counter

#define CheckBCD(inputData)	((((inputData&0x0f)>0x09)||((inputData&0xF0)>0x90))?1:0)
#define CheckMinAndSec(inputData)	((((inputData&0x0f)>0x09)||(inputData>0x59))?1:0)
#define CheckHour(inputData)	((((inputData&0x0f)>0x09)||(inputData>0x23))?1:0)
#define CheckWeek(inputData)	(inputData>7?1:0)
#define CheckDay30(inputData)		((((inputData&0x0f)>0x09)||(inputData>0x30)||(inputData==0))?1:0)
#define CheckDay31(inputData)		((((inputData&0x0f)>0x09)||(inputData>0x31)||(inputData==0))?1:0)
#define CheckDay28(inputData)		((((inputData&0x0f)>0x09)||(inputData>0x28)||(inputData==0))?1:0)
#define CheckDay29(inputData)		((((inputData&0x0f)>0x09)||(inputData>0x29)||(inputData==0))?1:0)	
#define CheckMonth(inputData)	((((inputData&0x0f)>0x09)||(inputData>0x12)||(inputData==0))?1:0)
    
    
#define CheckManufacture(inputData)	((((inputData&0x0f)>0x09)||(inputData>0x99))?1:0)

#define INIT_STATUS             (0)		
#define MANUFACTURE_STATUS      (0)
#define OUT_STATUS              (1)
#define USER_STATUS             (2)
#define BACK_MONEY_STATUS       (3)		//销户退费模式 added by Roger 2014/07/24

#define METER_NULL      		(0x00)
#define METER_1317      		(0xAA)
#define METER_7              	(0x55)
#define METER_9              	(0x99)


#define FIVE_MININUTES_CNT      (300)
#define ONE_MININUTES_CNT      (60)

#define AD_SAMPLE_3S                (3)           
#define AD_SAMPLE_5MIN              (300)

#define BTN_FREEZE_LCD_TIME			(6)
//meter error status byte
/////////////////////////////////////////////////////////////////////////////////
#define EEPROM_ERR              (0x01)
#define RELAY_ERR               (0x02)
#define OVERPOWER_ERR           (0x04)
#define CROSSZERO_ERR           (0x08)

/////////////////////////////////////////////////////////////////////////////////


//meter flag byte
/////////////////////////////////////////////////////////////////////////////////
#define CARD_RIGHT_IN           (0x01)
#define CARD_IN                 (0x02)
#define CARD_IN_USE             (0x04)
#define HALF_SECOND             (0x08)

/////////////////////////////////////////////////////////////////////////////////

#define POSKEY 						(0x56)
#define DECRYPT_KEY                 (0xA6)

//I2C bus device address
#define EEPROM_ADDR			        0xa0
#define CARD_ADDR			        0xa0
#define EEPROM_MODE                 1
#define MANUFACTURE_ADDR_START      0x10

#define RX8025T_ADDR				(0x64)
#define START_ADDR					(00)
#define EXTENTION_REG				(0x0D) 		//00
#define FLAG_REG					(0x0E)		//00
#define CONTROL_REG					(0x0F)  	//Ê¹ÄÜÃëÂö³å0x60,¹Ø±ÕÃëÂö³å0x40 11

//ti
//#define RX8025T_ADDR				(0xD0)
//#define START_ADDR					(00)
//#define EXTENTION_REG				(0x0D) 		//00
//#define FLAG_REG					(0x0E)		//00
//#define CONTROL_REG					(0x0F)  	//使能秒脉冲0x60,关闭秒脉冲0x40


//LCD driver device address
#define LCD_ADDR			        0x7c
#define IC_SET						(0x80+0x6A)	//software reset
#define MODE_SET_ON					(0x80+0x48)	//display on
#define MODE_SET_OFF				(0x80+0x40)	//display off
#define ADDR_SET					(0x00+0x00)	//start from 0
#define DIS_CTL						(0x80+0x22)	//display control
#define BLK_CTL						(0x80+0x70)	//blink off
#define AP_CTL						(0x80+0x7c)	//all pixel on


#define CYCLE_DISP_TIME_P 	3			//2s	
#define CYCLE_DISP_TIME 	5			//5s

#define NORMAL_DISP_TIME_P 	2			//2s	
#define NORMAL_DISP_TIME 	12			//10s

#define BEEP_DELAY_1S           1
#define BEEP_DELAY_2S           4
#define BEEP_DELAY_5MIN         1200

#define KEY_DOWN_COUNTER        3       //about 3s

#define EEPROM_BACKUP_OFFSET    (0x1000)      //4K



#define BIT0			0x01
#define BIT1			0x02
#define BIT2			0x04
#define BIT3			0x08
#define BIT4			0x10
#define BIT5			0x20
#define BIT6			0x40
#define BIT7			0x80

//PIN macro definitions
///////////////////////////////////////////////////////////////////////////////////////////
//EEPROM P6.0->SCL, P6.1->SDA, P6.2->WP
#define EEPROM_SCL_INPUT		(PM6.0=1)
#define EEPROM_SCL_OUTPUT		(PM6.0=0)
#define EEPROM_SCL_H			(P6.0=1)
#define EEPROM_SCL_L			(P6.0=0)

#define EEPROM_SDA_INPUT		(PM6.1=1)
#define EEPROM_SDA_OUTPUT		(PM6.1=0)
#define EEPROM_SDA_H			(P6.1=1)
#define EEPROM_SDA_L			(P6.1=0)
#define EEPROM_SDA				(P6.1)

#define EEPROM_WP_INPUT		        (PM2.1=1)
#define EEPROM_WP_OUTPUT	        (PM2.1=0)
#define EEPROM_WP()					(P2.1=1)
#define EEPROM_NOWP()				(P2.1=0)

//IC card pin P3.1->IC_CHK, P3.2->IC_SCL(MCUCLK)
// P3.0->IC_SDA(IO_CARD), P7.0->IC_VCC; P1.6->IC_RST;
#define IC_CHK_INPUT			(PM1.3=1)
#define IC_CHK_OUTPUT			(PM1.3=0)
#define IC_CHK						(P1.3)

#define IC_SCL_INPUT		(PM0.2=1)
#define IC_SCL_OUTPUT		(PM0.2=0)
#define IC_SCL_H				(P0.2=1)
#define IC_SCL_L				(P0.2=0)

#define IC_SDA_INPUT		(PM1.2=1)
#define IC_SDA_OUTPUT		(PM1.2=0)
#define IC_SDA_H				(P1.2=1)
#define IC_SDA_L				(P1.2=0)
#define IC_SDA					(P1.2)

#define IC_POWER_INPUT		(PM0.0=1)
#define IC_POWER_OUTPUT		(PM0.0=0)
#define IC_POWER_OFF		(P0.0=1)
#define IC_POWER_ON			(P0.0=0)

#if(PCB_VERSION_NEW==0)
	//Relay control
	#define RelayA_OUTPUT		(PM1.6=0)
	#define RelayA_H				(P1.6=1)
	#define RelayA_L				(P1.6=0)
	#define RelayB_OUTPUT		(PM1.5=0)
	#define RelayB_H				(P1.5=1)
	#define RelayB_L				(P1.5=0)
#else
		//Relay control
	#define RelayA_OUTPUT		(PM1.6=0)
	#define RelayA_H				(P1.6=1)
	#define RelayA_L				(P1.6=0)
	#define RelayB_OUTPUT		(PM1.5=0)
	#define RelayB_H				(P1.5=1)
	#define RelayB_L				(P1.5=0)
#endif

#if(IC_CARD_MODE==PLC_COMM_USED)
	#define PLCEVENT_IO		(P1.3)
	#define PLCSTA_IO		(P1.2)
	#define PLCRST_IO		(P0.2)
#endif

//speaker
#define SPEAKER_OUTPUT		(PM2.0=0)
#define SPEAKER_H()					P2.0=1
#define SPEAKER_L()					P2.0=0
#define SPEAKER_IO              P2.0

// lcd led 1
#define LCDLED_OUTPUT		(PM4.5=0)
#define LCDLED_ON()					P4.5=1
#define LCDLED_OFF()					P4.5=0


// led 1
#if(METER_CHIP_MODEL==0x2208)
	//PULSE
	#define PULSE_OUTPUT		(PM4.2=0)
	#define PULSE_ON()					P4.2=0
	#define PULSE_OFF()					P4.2=1
	
	// led 2
	#define LED2_OUTPUT		(PM4.0=0)
	#define LED2_ON()					P4.0=0
	#define LED2_OFF()					P4.0=1
	#define LED2                    P4.0

	// alarm
	#define ALARM_OUTPUT		(PM4.4=0)
	#define ALARM_ON()					P4.4=0
	#define ALARM_OFF()					P4.4=1
	#define ALARM                    P4.4	
		

#else if (METER_CHIP_MODEL==0x8302)
	#define ALARM_OUTPUT		(PM4.4=0)
	#define ALARM_ON()		(P4.4=0)			
	#define ALARM_OFF()		(P4.4=1)
	
	#define ALARM                    P4.4	
#endif		




//RS485 control
#define RS485CON_OUTPUT				(PM0.5=0)		//X2
#define RS485CON_TXD()					P0.5=0
#define RS485CON_RXD()					P0.5=1

#if(SINGLE_OR_THREE_METER==3)
	#if(PCB_VERSION_NEW==1)
		//Key detect
		#define KEY1_INPUT					(PM4.3=1)		//up
		#define KEY1							(P4.3)
	#else
		//Key detect
		#define KEY1_INPUT					(PM12.2=1)		//up
		#define KEY1							(P12.2)
	#endif
	
	#define KEY2_INPUT					(PM12.1=1)		//down
	#define KEY2							(P12.1)
#else
	//Key detect
	#define KEY1_INPUT					(PM4.3=1)
	#define KEY1							(P4.3)

#endif

#if(SINGLE_OR_THREE_METER==3)
	#if(PCB_VERSION_NEW==0)	
		#define PROG_KEY_INPUT					(PM12.3=1)		//PROG
		#define PROG_KEY2						(P12.3)
		
		#define Q_PULSE_INPUT					(PM4.3=1)
		#define Q_PULSE							(P4.3)
	#else
		#define Q_PULSE_INPUT					(PM12.2=1)
		#define Q_PULSE							(P12.2)
		
		#define PROG_KEY_INPUT					(PM12.3=1)		//PROG
		#define PROG_KEY2						(P12.3)
	#endif
		
#else
	#define PROG_KEY_INPUT					(PM12.2=1)		//PROG
	#define PROG_KEY2						(P12.2)

#endif



//pulse detect 
#define PULSE_INPUT					()
#define PULSE								(P13.7)



///////////////////////////////////////////////////////////////////


//SPI bus pin
#define SPI_CS_OUTPUT		(PM0.4=0)
#define SPI_CS_H			(P0.4=1)
#define SPI_CS_L			(P0.4=0)

#define SPI_MCLK_OUTPUT		(PM0.3=0)
#define SPI_MCLK_H			(P0.3=1)
#define SPI_MCLK_L			(P0.3=0)

#define SPI_MISO_INPUT		(PM12.4=0)
#define SPI_MISO_IO			(P12.4)

#define SPI_MOSI_OUTPUT		(PM4.2=0)
#define SPI_MOSI_H			(P4.2=1)
#define SPI_MOSI_L			(P4.2=0)

#define NOP_4				{NOP ( );NOP ( );\
							NOP ( );NOP ( );\
							NOP ( );NOP ( );\
							NOP ( );NOP ( );\
							NOP ( );NOP ( );}
							
#define NOP_8				{NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;\
								NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;\
								NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;\
								NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;\
								NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;NOP_4;}							
//HT1621ÃüÁîÂë 11
///////////////////////////////////////////////////////////////////////////////
#define OPEN_LCD			(0x03)		//´ò¿ªLCDÆ«Ñ¹·¢ÉúÆ÷ 11
#define OPEN_SYS_OSC		(0x01)		//´ò¿ªÏµÍ³Õñµ´Æ÷ 11
#define BIAS1_3_4COM		(0x29)		//1/3Æ«Ñ¹ 4COM 11
#define DISABLE_WDT			(0x05)		//WDTÒç³ö±êÖ¾Êä³öÊ§Ð§ 11

#define RC_256				(0x18)		//ÏµÍ³Ê±ÖÓÔ´ Æ¬ÄÚRCÕñµ´Æ÷ 11
#define DISABLE_BUZZ		(0x08)		//¹Ø±ÕÉùÒôÊä³ö 11
#define DISABLE_IRQ			(0x80)		//IRTÊ§Ð§ 11


#define NO_Trans			(0)
#define BCDTransL4			(1)
#define BCDTransL2			(2)
#define BCDTransI0			(3)
#define BCDTransI4			(4)
#define BCD1ToHEX1			(5)
#define BCDToCHAR			(6)
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////

#define MAXHANDLEBYTES		(BUF_MAX_LEN-12)
#define COMM_ONEFRAME_TIMEOUT	(10)		//10/2=5秒 11
#define COMM_DELAY_20MS		(6)		//20ms 11

//IR transmit and recieve P1.4->38K P0.7->RXD 11
#define IR_38K_INPUT		(PM1.4=1)
#define IR_38K_OUTPUT		(PM1.4=0)
#define IR_38K_H()		    (P1.4=1)
#define IR_38K_L()			(P1.4=0)
#define IR_38K_IO			(P1.4)

#define IR_RXD_INPUT		()
#define IR_RXD_OUTPUT		()
#define IR_RXD_H()		    ()
#define IR_RXD_L()			()

#define UART_TXD		(P0.7)

#define IR_RXD			(P1.4)
#define IR_TXD_STATE	(0x00)
#define IR_RXD_STATE	(0x01)

/////////////////////////////////////////////////////////////////////////
//¹¦ÄÜÂë 11
#define READ_DATA_97		(0x01)		//¹ã²¥Ð£Ê±  11
#define BROADCAST_TIMESET	(0x08)		//¹ã²¥Ð£Ê±  11
#define READ_DATA			(0x11)		//¶ÁÊý¾Ý 11
#define READ_NEXT_DATA		(0x12)		//¶ÁºóÐøÊý¾Ý 11
#define READ_ADDRESS		(0x13)		//¶ÁÍ¨ÐÅµØÖ· 11
#define WRITE_DATA			(0x14)		//Ð´Êý¾Ý 11
#define WRITE_ADDRESS		(0x15)		//Ð´Í¨ÐÅµØÖ· 11
#define FREEZE_COMMAND		(0x16)		//¶³½áÃüÁî 11
#define CHANGE_BAUDRATE		(0x17)		//¸ü¸ÄÍ¨ÐÅËÙÂÊ 11
#define CHANGE_PASSWORD		(0x18)		//ÐÞ¸ÄÃÜÂë 11
#define CLEAR_DEMMAND		(0x19)		//×î´óÐèÁ¿ÇåÁã 11
#define CLEAR_METER_QUAN	(0x1A)		//µçÁ¿ÇåÁã 11
#define CLEAR_EVENT			(0x1B)		//ÊÂ¼þÇåÁã 11

#define ID_AUTHENTICATE		(0x03)		//Éí·ÝÈÏÖ¤ 11
#define REMOTE_CONTROL		(0x1C)		//Ô¶³Ì¿ØÖÆ 11
#define MULTI_OUT_SET		(0x1D)		//¶à¹¦ÄÜ¿Ú¿ØÖÆ 11

/////////////////////////////////////////////////////////////////////////

#if(METER_CHIP_MODEL==0x2208)
	/////////////////////////////////////////////////////////////////////////
	#define CALIREG_NUM	7			//使用校表寄存器数量6+1(reserved) 11
	#define CALICOUNT	5			//误差计算采样次数 11

	#define U_FACTOR				0x2243		//电压默认系数 11
	#define IL_FACTOR				0x724b		//火线电流默认系数 11
	#define IN_FACTOR				0x7253		//零线电流默认系数 11
	#define P_FACTOR				0xe84c00	//功率默认系数 11


	#define EMUSTATUS 	0x2d		//计量状态及校验和寄存器 11
	#define RDATA 		0x44		//上次读出寄存器值 11
	#define WDATA 		0x45		//上次写寄存器值 11

	#define PFCNT 		0x20		//快速有功脉冲计数 11
	#define IARMS 		0x22		//电流有效值 11
	#define URMS 		0x24		//电压有效值 11
	#define UFREQ 		0x25		//频率值 11
	#define POWERP 		0x26		//有功功率有效值 11

	#define HFCONST		0x02		//高频常数 11
	#define GPQA		0x05		//功率增益校验 11
	#define PHASE		0x07		//相位校验 11
	#define APOR		0x0A		//偏移校验 11
	#define IARMSOS		0x0E		//
	#define PSTART		0x03		//启动寄存器 11

	#define FREQ_K      (long int)(447443)

	//////////////////////////////////////////////////////////////////////////////////

#else if (METER_CHIP_MODEL==0x8302)
	
	#ifdef IB_20A_RELAY_OUT
		#define U_RMS_VALUE		(33000000.0)
		#define I_RMS_VALUE		(20000000.0)
		#define	P_0_5RMS_VALUDE	(39339065.0)
		
		#define KV				(15000.0)		//µçÑ¹ÏµÊý 1Î»Ð¡Êý 11
		#define KI				(1000.0)		//µçÁ÷ÏµÊý 3Î»Ð¡Êý 11
		#define KP				(1788.0)		//ÓÐ¹¦¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
		#define KQ				(1788.0)		//ÎÞ¹¦¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
		#define KS				(1788.0)		//ÊÓÔÚ¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
	#endif
	
	#ifdef IB_20A
		#if ( CAL_RN8302_MODE == 33 )	
			#if ( U_VOL_220_OR_57 == 380 )			
				#define U_RMS_VALUE		(57000000.0)
				#define I_RMS_VALUE		(20000000.0)
				#define	P_0_5RMS_VALUDE	(67949295.0)
			#endif								
		#else
			#if ( U_VOL_220_OR_57 == 220 )	
				#define U_RMS_VALUE		(33000000.0)
				#define I_RMS_VALUE		(20000000.0)
				#define	P_0_5RMS_VALUDE	(39339065.0)
			#endif						
		#endif
		
	
		#define KV				(15000.0)		//µçÑ¹ÏµÊý 1Î»Ð¡Êý 11
		#define KI				(1000.0)		//µçÁ÷ÏµÊý 3Î»Ð¡Êý 11
		#define KP				(1788.0)		//ÓÐ¹¦¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
		#define KQ				(1788.0)		//ÎÞ¹¦¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
		#define KS				(1788.0)		//ÊÓÔÚ¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
	#endif
	
	#ifdef IB_10A
		#define U_RMS_VALUE		(33000000.0)
		#define I_RMS_VALUE		(10000000.0)
		#define	P_0_5RMS_VALUDE	(19669532.0)
		
		#define KV				(15000.0)		//µçÑ¹ÏµÊý 1Î»Ð¡Êý 11
		#define KI				(1000.0)		//µçÁ÷ÏµÊý 3Î»Ð¡Êý 11
		#define KP				(1788.0)		//¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
		#define KQ				(1788.0)		//¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
		#define KS				(1788.0)		//ÊÓÔÚ¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
	#endif

	#ifdef IB_5A
		#if ( CAL_RN8302_MODE == 33 )	
			#if ( U_VOL_220_OR_57 == 380 )			
				#define U_RMS_VALUE		(57000000.0)
				#define I_RMS_VALUE		(5000000.0)
				#define	P_0_5RMS_VALUDE	(16987323.0)
			#endif		
		#else
			#if ( U_VOL_220_OR_57 == 220 )	
				#define U_RMS_VALUE		(33000000.0)
				#define I_RMS_VALUE		(5000000.0)
				#define	P_0_5RMS_VALUDE	(9834766.0)
			#endif
		#endif
					
		#define KV				(15000.0)		//µçÑ¹ÏµÊý 1Î»Ð¡Êý 11
		#define KI				(1000.0)		//µçÁ÷ÏµÊý 3Î»Ð¡Êý 11
		#define KP				(1788.0)		//¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
		#define KQ				(1788.0)		//¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
		#define KS				(1788.0)		//ÊÓÔÚ¹¦ÂÊÏµÊý 4Î»Ð¡Êý 11
	#endif


	#ifdef IB_1_5A
		#if ( CAL_RN8302_MODE == 33 )	
			#if (U_VOL_220_OR_57 == 100) 
				#define U_RMS_VALUE		(15000000.0)
				#define I_RMS_VALUE		(9000000.0)
				#define	P_0_5RMS_VALUDE	(8046627.0)
				
				#define KV				(15000.0)		// 11
				#define KI				(6000.0)		// 11
				#define KP				(10728.8)		// 11
				#define KQ				(10728.8)		// 11
				#define KS				(10728.8)		// 11
			#endif
			
			#if (U_VOL_220_OR_57 == 380) 
			
			#endif
		#else		//三相四线 11
			#if (U_VOL_220_OR_57 == 57) 
				#define U_RMS_VALUE		(8655000.0)
				#define I_RMS_VALUE		(9000000.0)
				#define	P_0_5RMS_VALUDE	(4642903.8)
				
				#define KV				(15000.0)		// 11
				#define KI				(6000.0)		// 11
				#define KP				(10728.4)		// 11
				#define KQ				(10728.4)		// 11
				#define KS				(10728.4)		// 11
			#endif
			
			#if (U_VOL_220_OR_57 == 220) 
				#define U_RMS_VALUE		(33000000.0)
				#define I_RMS_VALUE		(9000000.0)
				#define	P_0_5RMS_VALUDE	(17702579.0)
				
				#define KV				(15000.0)		// 11
				#define KI				(6000.0)		// 11
				#define KP				(10728.4)		// 11
				#define KQ				(10728.4)		// 11
				#define KS				(10728.4)		// 11
			#endif
		#endif
		 				
		
	#endif

	#define UA_RMS		0x07		//AÏàµçÑ¹ÓÐÐ§Öµ 11
	#define UB_RMS		0x08		//BÏàµçÑ¹ÓÐÐ§Öµ 11
	#define UC_RMS		0x09		//CÏàµçÑ¹ÓÐÐ§Öµ 11

	#define IA_RMS		0x0B		//AÏàµçÁ÷ÓÐÐ§Öµ 11
	#define IB_RMS		0x0C		//BÏàµçÁ÷ÓÐÐ§Öµ 11
	#define IC_RMS		0x0D		//CÏàµçÁ÷ÓÐÐ§Öµ 11

	#define PA_RMS		0x14		//AÏà¹¦ÂÊ 11
	#define PB_RMS		0x15		//BÏà¹¦ÂÊ 11
	#define PC_RMS		0x16		//CÏà¹¦ÂÊ 11
	#define PT_RMS		0x17		//ºÏÏà¹¦ÂÊ 11

	#define QA_RMS		0x18		//AÏàÎÞ¹¦¹¦ÂÊ 11
	#define QB_RMS		0x19		//BÏàÎÞ¹¦¹¦ÂÊ 11
	#define QC_RMS		0x1A		//CÏàÎÞ¹¦¹¦ÂÊ 11
	#define QT_RMS		0x1B		//ºÏÏàÎÞ¹¦¹¦ÂÊ 11

	#define SA_RMS		0x1C		//AÏàÊÓÔÚ¹¦ÂÊ 11
	#define SB_RMS		0x1D		//BÏàÊÓÔÚ¹¦ÂÊ 11
	#define SC_RMS		0x1E		//CÏàÊÓÔÚ¹¦ÂÊ 11
	#define ST_RMS		0x1F		//ºÏÏàÊÓÔÚ¹¦ÂÊ 11

	#define PFA_RMS		0x20		//AÏà¹¦ÂÊÒòÊý 11
	#define PFB_RMS		0x21		//BÏà¹¦ÂÊÒòÊý 11
	#define PFC_RMS		0x22		//CÏà¹¦ÂÊÒòÊý 11
	#define PFT_RMS		0x23		//ºÏÏà¹¦ÂÊÒòÊý 11

	#define GSUA		0x13		//µçÑ¹Í¨µÀAÔöÒæ 11
	#define GSUB		0x14		//µçÑ¹Í¨µÀBÔöÒæ 11
	#define GSUC		0x15		//µçÑ¹Í¨µÀCÔöÒæ 11

	#define GSIA		0x16		//µçÁ÷Í¨µÀAÔöÒæ 11
	#define GSIB		0x17		//µçÁ÷Í¨µÀBÔöÒæ 11
	#define GSIC		0x18		//µçÁ÷Í¨µÀCÔöÒæ 11

	#define PA_PHS		0x31		//AÏàÏàÎ»Ð£Õý¼Ä´æÆ÷ 11
	#define PB_PHS		0x32		//BÏàÏàÎ»Ð£Õý¼Ä´æÆ÷ 11
	#define PC_PHS		0x33		//CÏàÏàÎ»Ð£Õý¼Ä´æÆ÷ 11

	#define IA_OS		0x24		//AÏàµçÁ÷ÓÐÐ§ÖµOFFSET 11
	#define IB_OS		0x25		//BÏàµçÁ÷ÓÐÐ§ÖµOFFSET 11
	#define IC_OS		0x26		//CÏàµçÁ÷ÓÐÐ§ÖµOFFSET 11
	
	#define PHASEA_ANGEL		0x50		//A相相角 11
	#define PHASEB_ANGEL		0x51		//B相相角 11
	#define PHASEC_ANGEL		0x52		//C相相角 11
	
	#define FREQENCY_REQ    0x57        //频率 11
	#define DIRECTION_REQ    0x66        //功率方向寄存器 11

	//////////////////////////////////////////////////////////////////////////////////
	#define DEVICE_ID	0x8f
	#define MODE_CHOOSE	0x81
	#define WREN		0x80		//Ð´Ê¹ÄÜ¼Ä´æÆ÷ 11
		#define WRITE_ENABLE	0xe5
		#define WRITE_DISABLE	0xDC
	#define SOFTRESET	0x82
		#define RESET_CMD		0xfa
	
	#ifdef IB_20A_RELAY_OUT
		#define HFCONST1	0x00
			#define HFCONST_H	0x95
			#define HFCONST_L	0xe1
		#define ISTART		0x02
			#define ISTART_H	0x05
			#define ISTART_L	0xdc
	#endif
	
	#ifdef IB_20A
		#define HFCONST1	0x00
			#define HFCONST_H	0x95
			#define HFCONST_L	0xe1
		#define ISTART		0x02
			#define ISTART_H	0x05
			#define ISTART_L	0xdc
	#endif
	
	#ifdef IB_10A
		#define HFCONST1	0x00
			#define HFCONST_H	0x4A
			#define HFCONST_L	0xF0
		#define ISTART		0x02
			#define ISTART_H	0x02
			#define ISTART_L	0xEE
	#endif

	#ifdef IB_5A
		#define HFCONST1	0x00
			#define HFCONST_H	0x25
			#define HFCONST_L	0x78
			
		#define ISTART		0x02
			#define ISTART_H	0x01
			#define ISTART_L	0x77

	#endif

	#ifdef IB_1_5A
		#define HFCONST1	0x00
		#define ISTART		0x02
		
		#if ( CAL_RN8302_MODE == 33 )	//三相三线 11
			#if (U_VOL_220_OR_57 == 100) 
				#define HFCONST_VAL	(14388)
				#define ISTART_VAL	(677)
				
				#define HFCONST_H	((HFCONST_VAL>>8)&0xff)
				#define HFCONST_L	((HFCONST_VAL)&0xff)
				
				#define ISTART_H	((ISTART_VAL>>8)&0xff)
				#define ISTART_L	((ISTART_VAL)&0xff)
			#endif
			
		#else		//三相四线 11
			#if (U_VOL_220_OR_57 == 57) 
				#define HFCONST_VAL	(14388)
				#define ISTART_VAL	(677)
				
				#define HFCONST_H	((HFCONST_VAL>>8)&0xff)
				#define HFCONST_L	((HFCONST_VAL)&0xff)
				
				#define ISTART_H	((ISTART_VAL>>8)&0xff)
				#define ISTART_L	((ISTART_VAL)&0xff)
			#endif
			
			#if (U_VOL_220_OR_57 == 220) 
				#define HFCONST_H	0x38
				#define HFCONST_L	0x34
				
				#define ISTART_H	0x02
				#define ISTART_L	0xA3
			#endif
		
		#endif		
	#endif
		
	#define EMUCON		0x62
		
	#define UA_REG		0x07
	#define UB_REG		0x08
	#define UC_REG		0x09
#endif
//////////////////////////////////////////////////////////////////////////////////
//Í¨Ñ¶±í¸ñÊ¹ÓÃºê¶¨Òå 11
#define BCDType		0
#define HexType		1
#define Hex2To4 	2
#define Hex4To4		3		//±íÄÚ2×Ö½ÚÍ¨Ñ¶À©Õ¹Îª4×Ö½Ú 11
#define Hex2To3 	4
#define Hex2To3_1 	5
#define Hex2To2 	6
#define Hex2To4_0 	7
#define Hex2To4_10	8

#define WR_Enable	0
#define W_Enable	1
#define R_Enable 	2

#define L_RAM			0
#define L_Constant		1
#define L_2208_R 		2
#define L_2208_W 		3
#define L_RAM_E2P		4
#define L_EEPROM		5
#define L_RX8025		6
#define L_FLASH			7
#define L_EEPROM_H		8
#define L_EEPROM_T		9

#define BCDCheck	0
#define BCDCheck_N	1
#define BCDCheck_D	2	//week check:0~6,day check:1~31,month check:1~12,year check:07~99
#define BCDCheck_ECS	3
#define BCDCheck_E	4

#define ENERGY_DATA_LEN		(4)		//µçÁ¿Êý¾Ý³¤¶È 11
#define TARIFF_NUM			(4)		//·ÑÂÊ¸öÊý 11

#define KEY_NOT				(0)
#define KEY_YES				(1)
#define KEY_H_YES			(2)
//////////////////////////////////////////////////////////////////////////////////

#define SINGLE_PRICE		(0x55)
#define MULTI_PRICE			(0x5A)
#define LADDER_PRICE		(0xA5)
#define MULTI_LADDER_PRICE	(0xAA)

////////////////////////////////////////////////////////
#define TRUE			(0)
#define FALSE			(1)
////////////////////////////////////////////////////////



//modbus表格宏定义11
//////////////////////////////////////////////////////////
#define DOT0	(0)
#define DOT1	(1)
#define DOT2	(2)
#define DOT3	(3)
#define DOT4	(4)

#define P_CHAR	(0)
#define P_INT	(1)
#define P_LONG	(2)
#define P_VOID	(3)
#define P_LONG_2	(4)
//////////////////////////////////////////////////////////

#define EVEN_CHK		(0)
#define ODD_CHK		(1)
#define NONE_CHK		(2)

#define NC_MODE		(0)
#define NO_MODE		(1)

/////////////////////////////////////////////////////////////////////
//CV520寄存器定义
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAskReg              0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		          0x3F


/////////////////////////////////////////////////////////////////////
//Mifare_One卡片命令字11
/////////////////////////////////////////////////////////////////////
#define REQIDL           0x26               //寻天线区内未进入休眠状态 11
#define REQALL           0x52               //寻天线区内全部卡 11
#define ANTICOLL1        0x93               //防冲撞  卡号4字节 11
#define ANTICOLL2        0x95               //防冲撞   卡号7字节的第2层 11
#define AUTHENT1A        0x60               //验证A密钥 11
#define AUTHENT1B        0x61               //验证B密钥 11
#define READ             0x30               //读块11
#define PICC_WRITE       0xA0               //写块11
#define DECREMENT        0xC0               //扣款 11
#define INCREMENT        0xC1               //充值 11
#define RESTORE          0xC2               //调块数据到缓冲区11
#define PICC_TRANSFER    0xB0               //保存缓冲区中数据11
#define HALT             0x50               //休眠 11

//CV520命令字
/////////////////////////////////////////////////////////////////////
#define CV520_IDLE              0x00               //取消当前命令 11
#define CV520_AUTHENT           0x0E               //验证密钥 11
#define CV520_RECEIVE           0x08               //接收数据 11
#define CV520_TRANSMIT          0x04               //发送数据 11
#define CV520_TRANSCEIVE        0x0C               //发送并接收数据 11
#define CV520_RESETPHASE        0x0F               //复位 11
#define CV520_CALCCRC           0x03               //CRC计算 11


#define MI_OK                          0
#define MI_NOTAGERR                    1
#define MI_ERR                         2

/////////////////////////////////////////////////////////////////////
//CV520命令字
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //取消当前命令 11
#define PCD_AUTHENT           0x0E               //验证密钥 11
#define PCD_RECEIVE           0x08               //接收数据 11
#define PCD_TRANSMIT          0x04               //发送数据 11
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据  11
#define PCD_RESETPHASE        0x0F               //复位  11
#define PCD_CALCCRC           0x03               //CRC计算 11

#define MAXRLEN 64

#endif 
