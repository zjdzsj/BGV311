/******************************************************************************************
*  Copyright (c) 2011, *** R&D Department
*  All rights reserved 
*
*  FileName��   	LCD.c
*  Author��     	Roger.
*  Date��  			2011/11/15 14:54
*  Revised Date��  	
*  Main Function��  this file manages LCD display of meter
*  Version:			
*  Note�� 			 			
*********************************************************************************************/

/******************************************************************************************/
#pragma sfr
#pragma di
#pragma ei
#pragma NOP
#pragma HALT
#pragma STOP

#include "macro.h"
//#pragma interrupt INTTMH0 ISRTimerH0
//#pragma interrupt INTTM000 ISTimer000

//#pragma interrupt INTKR ISRForKeyButton

#if(PCB_VERSION_NEW==1)
	#pragma interrupt INTP7	ISRForKey1
#endif

#define _LCD_DEFINE_
#include "include.h"
/******************************************************************************************/

#define LCDSEG_         	(0xF0400)  /* LCD Memory */
#define LCDSEG              ((char*) LCDSEG_) 


#define LCD_NUM						(17)
#define LCD_OFFSET                  (0)

#define SegA        0x80     
#define SegB        0x40
#define SegC        0x08
#define SegD        0x02
#define SegE        0x04
#define SegF        0x20
#define SegG        0x10

#define SegA2        0x01     
#define SegB2        0x02
#define SegC2        0x04
#define SegD2        0x08
#define SegE2        0x04
#define SegF2        0x01
#define SegG2        0x02
     
//AUX1 SEG
#define SEG_S1        0X80   //����1 11
#define SEG_S2        0X40   //����2 11
#define SEG_S4        0X20   //����4 11
#define SEG_S3        0X10   //����3 11
#define SEG_S7        0X08   //�� 11
#define SEG_S8        0X04   //�� 11
#if(PCB_LCD_NEW==0)
	#define SEG_S9        0X02   //������1 11
	#define SEG_S10       0X01   //���� 11
#else
	#define SEG_a        0X02   //A 11
	#define SEG_b       0X01   //b 11
#endif

//AUX2
#define SEG_S18        0X80   //���2 11
#define SEG_S17        0X40   //���1 11
#define SEG_S16        0X20   //2��ʱ�� 11
#define SEG_S15        0X10   //1��ʱ�� 11
#define SEG_S14        0X08   //���ʷ��� 11
#define SEG_S11        0X04   //�� 11
#define SEG_S12        0X02   //�� 11
#if(PCB_LCD_NEW==0)
	#define SEG_S13        0X01   ///������2 11
#else
	#define SEG_c       0X01   //c 11
#endif


//AUX3
#define SEG_S19        0X80   //��̼� 11
#define SEG_S20        0X40   //�� 11
#define SEG_S21        0X20   //�ź�ǿ�� 11
#define SEG_S22        0X10   //��Կ״̬ 11
#define SEG_DK        0X08   //���� 11
#define SEG_IN        0X04   //�� 11
#define SEG_NO        0X02   //�� 11
#define SEG_CG        0X01   //�ɹ� 11

//AUX4
#define SEG_A        0X80   //A 11
#define SEG_YUAN      0X40   //Ԫ 11
#define SEG_Hz        0X20   //Hz 11
#define SEG_k        0X10   //k 11
#define SEG_V        0X08   //V 11
#define SEG_vv        0X04   //vv 11
#define SEG_ar        0X02   //ar 11
#define SEG_TJ        0X01   //�ڻ� 11

//AUX5
#if(PCB_LCD_NEW==0)
	
#else
	#define SEG_ACTIVE    0X40   //�й� 11
	#define SEG_S9        0X20   //������1 11
	#define SEG_S10       0X10   //���� 11
	#define SEG_S13       0X08   ///������2 11
#endif
#define SEG_h        0X01   //h 11

//AUX6
#define SEG_MONTH        0X80   //�� 11
#define SEG_QUAN        0X40   //�� 11
#define SEG_PRICE        0X20   //�� 11
#define SEG_TIME1        0X10   //ʱ 11
#define SEG_TIME2        0X08   //�� 11
#define SEG_DUAN        0X04   //�� 11
#define SEG_JINE        0X02   //��� 11
#define SEG_METERID      0X01   //��� 11

//AUX7
#define SEG_ZONG        0X80   //�� 11
#define SEG_T1        0X40   //�� 11
#define SEG_T2        0X20   //�� 11
#define SEG_T3        0X10   //ƽ 11
#define SEG_T4        0X08   //��11
#define SEG_SHENGYU        0X04   //ʣ�� 11
#define SEG_SHE        0X02   //�� 11
#define SEG_QIAN         0X01   //Ƿ 11

//AUX8
#define SEG_DP7        0X80   //DP7 11
#if(PCB_LCD_NEW==0)
	#define SEG_ACTIVE        	0X40   //�й� 11
	#define SEG_INACTIVE        0X20   //�޹� 11
#else
	#define SEG_INACTIVE        0X40   //�޹� 11
	#define SEG_DEMAND        	0X20   //�� 11
#endif
#define SEG_DIAN      0X10   //�� 11
#define SEG_1D        0X08   //1D 11
#define SEG_1C        0X04   //1C 11
#define SEG_1B        0X02  //1B 11
#define SEG_1A        0X01   //1A 11

//AUX9
#define SEG_DP4        0X80   //DP4 11
#define SEG_S6        0X40   //S6�绰 11
#define SEG_CURRENT        0X20   //��ǰ 11
#define SEG_LAST        0X10   //�� 11
#define SEG_S0        0X08   //S0  11
#define SEG_1E        0X04   //1E 11
#define SEG_1G        0X02   //1G 11
#define SEG_1F        0X01   //1F 11

//AUX10
#define SEG_DP1        0X01   //DP1 11
#define SEG_DP2        0X02   //DP2 11
#define SEG_DP3        0X04   //DP3 11
#define SEG_DP5        0X08   //DP5 11
#define SEG_DP6        0X10   //DP6  11
#define SEG_DP8        0X20   //DP8 11
#define SEG_DP9        0X40   //DP9 11
#define SEG_TIAOZHA    0X80   //��բ  11

//���涨�� ÿ��������ͨ�� 11
#define SEG_H       SegB+SegC+SegE+SegF+SegG   //H 11
#define SEG_L       SegD+SegE+SegF   //L 11
#define SEG_        SegG
#define SEGR        SegE+SegG 
#define SEG_N       SegC+SegE+SegG 
#define SEGU        SegB+SegC+SegD+SegE+SegF 
#define SEGS        SegA+SegC+SegD+SegF+SegG
#define SEGNULL     0x00
#define SEGP        SegA+SegB+SegE+SegF+SegG 
#define SEGE        SegA+SegD+SegE+SegF+SegG
#define SEGC        SegA+SegD+SegE+SegF
#define SEGA        SegA+SegB+SegC+SegE+SegF+SegG
#define SEG0        SegC+SegD+SegE+SegG
#define SEGc        SegD+SegE+SegG 
#define SEGFULL     0xff
#define SEGb        SegC+SegD+SegE+SegF+SegG  
#define SEGd        SegB+SegC+SegD+SegE+SegG 
/******************************************************************************************/
	
//variables only used in this file
/******************************************************************************************/
//static uchar g_buf[LCD_NUM+LCD_RAM_OFFSET];

//Һ������ֲ����� 11
static const unsigned char LCDSEG_ARRAY[LCD_NUM]=
{
    11,12,16,17,
    18,19,20,21,
    6,7,8,22,23,
    33+1,34+1,35+1,36+1
};

	//����� 11
#if(SINGLE_OR_THREE_METER==1)

    //����Һ����ʾ˳������ 11
	static const unsigned char LCD_DISP_ARRAY[]=
	{
	#if(MODBUS_ENABLED==0)      
	    9,10,
	#else
		10,
	#endif
	    
	#ifdef RTC_8025T
		17,18,		//���� ʱ��  11
	#endif
	    
	    3,		//�ܵ��� 11
	#ifdef LADDER_ENABLE
		23,
	#endif
	
	#ifdef TARIFF_ENABLE
		19,20,21,22,
	#endif
	
	//���ѽ�� 11
	#ifdef BONUS_PADI_ENABLE
		24,
	#endif	
	
	#ifdef PRE_PAID_ENABLE
    	2,4,		//ʣ����͵�� 11
    #endif    
	    
	    13,		//���� 11
	    12,
	    11,
	#ifdef RTC_8025T
		26,27,28,	//��3���µ���  11
	#endif
	    
	#ifdef TARIFF_ENABLE
		25,		//ʱ�α� 11
	#endif     
			   
	};
#else
	//����Һ����ʾ˳������ 11
	static const unsigned char LCD_DISP_ARRAY[]=
	{
	#if(MODBUS_ENABLED==0)      
	    9,10,
	#else
		10,
	#endif

	#ifdef RTC_8025T
		27,28,		//���� ʱ��  11
	#endif
	    
	    3,		//�ܵ��� 11
	#ifdef LADDER_ENABLE
		33,		//�����ܵ��� 11
	#endif

	#ifdef TARIFF_ENABLE
		29,30,31,32,
	#endif

	//���ѽ�� 11
	#ifdef BONUS_PADI_ENABLE
		34,
	#endif

	#ifdef PRE_PAID_ENABLE
		2,4,		//ʣ����͵�� 11
	#endif

	    18,
	    19,20,15,16,17,
	    11,12,13,14,
	    23,24,25,26,  
	
	#ifdef RTC_8025T
		39,40,41,	//��3���µ���  11
	#endif 
	    	
		//������ʾ 11
	#ifdef DEMAND_ENABLE
		35,36,37,		
	#endif  
	
	#ifdef INACITVE_POWER_ENABLE
		#ifdef RTC_8025T
			42,
		#else
			35,
		#endif	
	#endif
	
	#ifdef TARIFF_ENABLE
		38,		//ʱ�α� 11
	#endif   
	};
#endif

static const unsigned char LCDCODE2_half1[]=
{
	SegA2+SegB2+SegC2+SegD2,	//0
    SegB2+SegC2,	//1
    SegA2+SegB2+SegD2,	//2
    SegA2+SegB2+SegC2+SegD2,	//3
    SegB2+SegC2,	//4
    SegA2+SegC2+SegD2,	//5
    SegA2+SegC2+SegD2,	//6
    SegA2+SegB2+SegC2,	//7
    SegA2+SegB2+SegC2+SegD2,	//8
    SegA2+SegB2+SegC2+SegD2,	//9
    SegA2+SegB2+SegC2,	//A
    SegC2+SegD2,	//b
    SegD2,	//C
    SegB2+SegC2+SegD2,	//d
    SegA2+SegD2,	//E
    SegA2,	//F
};

#define L21_NUM0	(SegA2+SegB2+SegC2+SegD2)	//0	
#define L21_NUM1	(SegB2+SegC2)	//1
#define L21_NUM2	(SegA2+SegB2+SegD2)	//2	
#define L21_NUM3	(SegA2+SegB2+SegC2+SegD2)	//3	
#define L21_NUM4	(SegB2+SegC2)	//4	
#define L21_NUM5	(SegA2+SegC2+SegD2)	//5	
#define L21_NUM6	(SegA2+SegC2+SegD2)	//6	
#define L21_NUM7	(SegA2+SegB2+SegC2)	//7	
#define L21_NUM8	(SegA2+SegB2+SegC2+SegD2)	//8	
#define L21_NUM9	(SegA2+SegB2+SegC2+SegD2)	//9	
#define L21_NUM10	(SegA2+SegB2+SegC2)	//10	
#define L21_NUM11	(SegC2+SegD2)	//11	
#define L21_NUM12	(SegD2)	//12	
#define L21_NUM13	(SegB2+SegC2+SegD2)	//13	
#define L21_NUM14	(SegA2+SegD2)	//14	
#define L21_NUM15	(SegA2)	//15	

static const unsigned char LCDCODE2_half2[16]=
{
	SegE2+SegF2,	//0
    0x00,	//1
    SegE2+SegG2,	//2
    SegG2,	//3
    SegF2+SegG2,	//4
    SegF2+SegG2,	//5
    SegE2+SegF2+SegG2,	//6
    0x00,	//7
    SegE2+SegF2+SegG2,	//8
    SegF2+SegG2,	//9
    SegE2+SegF2+SegG2,	//A
    SegE2+SegF2+SegG2,	//b
    SegE2+SegG2,	//C
    SegE2+SegG2,	//d
    SegE2+SegF2+SegG2,	//E
    SegE2+SegF2+SegG2,	//F
};

#define L22_NUM0	(SegE2+SegF2)	//0	
#define L22_NUM1	(0x00)	//1
#define L22_NUM2	(SegE2+SegG2)	//2	
#define L22_NUM3	(SegG2)	//3	
#define L22_NUM4	(SegF2+SegG2)	//4	
#define L22_NUM5	(SegF2+SegG2)	//5	
#define L22_NUM6	(SegE2+SegF2+SegG2)	//6	
#define L22_NUM7	(0x00)	//7	
#define L22_NUM8	(SegE2+SegF2+SegG2)	//8	
#define L22_NUM9	(SegF2+SegG2)	//9	
#define L22_NUM10	(SegE2+SegF2+SegG2)	//10	
#define L22_NUM11	(SegE2+SegF2+SegG2)	//11	
#define L22_NUM12	(SegE2+SegG2)	//12	
#define L22_NUM13	(SegE2+SegG2)	//13	
#define L22_NUM14	(SegE2+SegF2+SegG2)	//14	
#define L22_NUM15	(SegE2+SegF2+SegG2)	//15

typedef struct LCD_AUX_TABLE_tag
{
  	unsigned char 	aux1;	//��ʾ������1����Ӵ��� 11
  	unsigned char  	aux2;   //��ʾ������2 11
  	unsigned char  	aux3;   //��ʾ������3 11
  	unsigned char  	aux4;   //��ʾ������4 11
  	unsigned char  	aux5;   //��ʾ������5 11
  	unsigned char  	aux6;   //��ʾ������6 11
  	unsigned char  	aux7;   //��ʾ������7 11
  	unsigned char  	aux8;   //��ʾ������8 11
 	unsigned char  	aux9;   //��ʾ������9 11
 	
 	unsigned char  	aux10;   //��ʾ������10 С���� 11
}LCD_AUX_TABLE;

#if(PCB_LCD_NEW==0)   
static const LCD_AUX_TABLE LCD_AUX_DISPLAY[]={
    //aux1	aux2	aux3    aux4	aux5	aux6			aux7    aux8   	aux9	aux10 	 
    0x00,   0x00,   0x00,   0x00,	0x00,   SEG_METERID, 	0x00,   0x00,	0x00,	0x00,  	//��Ÿ�6λ 11
    0x00,   0x00,   0x00,   0x00,	0x00,   SEG_METERID, 	0x00,   0x00,	0x00,	0x00,  	//��ŵ�6λ  11 
    
#ifdef RTC_8025T
	//aux1	aux2	aux3    aux4	aux5	aux6					aux7    aux8   	aux9	aux10  
	0x00,   0x00, 	0x00,   0x00,   0x00,   SEG_TIME1+SEG_TIME2, 	0x00,   0x00,	0x00,	SEG_DP5+SEG_DP8,    //����  11
	0x00,   0x00, 	0x00,   0x00,   0x00,   SEG_TIME1+SEG_TIME2, 	0x00,   0x00,	SEG_DP4,	SEG_DP5,    //ʱ��  11	
#endif
	     
    //aux1	aux2	aux3    aux4				aux5	aux6		aux7    	aux8   					aux9	aux10           
    0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_vv,	SEG_h,	SEG_QUAN,	SEG_ZONG,	SEG_ACTIVE+SEG_DIAN,	0x00,	SEG_DP8,//�й��ܵ��� 11
#ifdef LADDER_ENABLE
	0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_vv,	SEG_h,	SEG_QUAN+SEG_MONTH,	SEG_ZONG,	SEG_ACTIVE+SEG_DIAN,	SEG_CURRENT,	SEG_DP8,//��ǰ���й��ܵ��� 11
#endif    
    
	      
#ifdef TARIFF_ENABLE
	//aux1	aux2	aux3    aux4				aux5	aux6		aux7    aux8   					aux9	aux10 
	0x00,  0x00,   	0x00, 	SEG_k+SEG_V+SEG_vv,	SEG_h,  SEG_QUAN,   SEG_T1, SEG_ACTIVE+SEG_DIAN,	0x00,	SEG_DP8,      //����� 11
	0x00,  0x00,   	0x00,  	SEG_k+SEG_V+SEG_vv,	SEG_h,  SEG_QUAN,   SEG_T2, SEG_ACTIVE+SEG_DIAN,	0x00,	SEG_DP8,     //����� 11
	0x00,  0x00,  	0x00,  	SEG_k+SEG_V+SEG_vv,	SEG_h,  SEG_QUAN,   SEG_T3, SEG_ACTIVE+SEG_DIAN,	0x00,	SEG_DP8,     //ƽ���� 11
	0x00,  0x00,  	0x00,  	SEG_k+SEG_V+SEG_vv,	SEG_h,  SEG_QUAN,   SEG_T4, SEG_ACTIVE+SEG_DIAN,	0x00,	SEG_DP8,     //�ȵ��� 11
#endif
	
//���ѽ�� 11
#ifdef BONUS_PADI_ENABLE
	//aux1	aux2	aux3    aux4		aux5	aux6		aux7    aux8   	aux9	aux10
	0x00, 	0x00,	SEG_S21,SEG_YUAN,	0x00,	SEG_JINE,	0x00,	0x00,	0x00,	SEG_DP8,	//ʣ�����ѽ�� 11	
#endif	
	
#ifdef PRE_PAID_ENABLE
    //aux1	aux2	aux3    aux4	aux5	aux6		aux7    		aux8   	aux9		aux10
    0x00, 	0x00,	0x00,SEG_YUAN,	0x00,	SEG_JINE,	SEG_SHENGYU,	0x00,	SEG_CURRENT,SEG_DP8,	//��ǰʣ���� 11
    0x00, 	0x00,	0x00,SEG_YUAN,	0x00,	SEG_PRICE,	0x00,			SEG_DIAN,SEG_CURRENT,SEG_DP8,	//��� 11
#endif
	
	//aux1	aux2	aux3    aux4	aux5	aux6	aux7    aux8   		aux9		aux10   
#if(SINGLE_OR_THREE_METER==1) 
	0x00,	0x00,   0x00,  	SEG_A,	0x00,	0x00,   0x00,  	0x00,	0x00,	SEG_DP6,	//A����� 11
	0x00,	0x00,   0x00,  	SEG_V,	0x00,	0x00,   0x00,  	0x00,	0x00,	SEG_DP9,	//A����� 11
	0x00, 0x00,   0x00, SEG_k+SEG_V+SEG_vv, 0x00,	0x00,   SEG_ZONG,0x00,0x00,   SEG_DP5,//�й��ܹ��� 11	 
#else
	0x00,	0x00,   0x00,  	SEG_A,	0x00,	0x00,   0x00,  	L21_NUM10,	L22_NUM10,	SEG_DP6,	//A����� 11
    
    0x00,	0x00,   0x00,  	SEG_A,	0x00,	0x00,   0x00,  	L21_NUM11,	L22_NUM11,	SEG_DP6,	//B����� 11  
    0x00,	0x00,   0x00,  	SEG_A,	0x00,	0x00,   0x00,  	L21_NUM12,	L22_NUM12,	SEG_DP6,	//C����� 11  
    
    //aux1	aux2	aux3    aux4	aux5	aux6	aux7    aux8   		aux9		aux10   	    
	0x00,	0x00,   0x00,  	SEG_V,	0x00,	0x00,   0x00,  	L21_NUM10,	L22_NUM10,	SEG_DP9,//A���ѹ 11    
    0x00,	0x00,   0x00,  	SEG_V,	0x00,	0x00,   0x00,  	L21_NUM11,	L22_NUM11,	SEG_DP9,//B���ѹ 11
    0x00,	0x00,   0x00,  	SEG_V,	0x00,	0x00,   0x00,  	L21_NUM12,	L22_NUM12,		SEG_DP9,//C���ѹ 11
    
    //aux1	aux2	aux3    aux4			aux5	aux6	aux7    aux8   	aux9		aux10   
    0x00, 0x00,   0x00, SEG_k+SEG_V+SEG_vv, 0x00,	0x00,   SEG_ZONG,SEG_ACTIVE,0x00,   SEG_DP5,//�й��ܹ��� 11
    0x00, 0x00,   0x00, SEG_k+SEG_V+SEG_vv, 0x00,	0x00,   0x00,L21_NUM10,	L22_NUM10,  SEG_DP5,//A���й�����
    0x00, 0x00,   0x00, SEG_k+SEG_V+SEG_vv, 0x00,	0x00,   0x00,L21_NUM11,	L22_NUM11,	SEG_DP5,//B���й�����
    0x00, 0x00,   0x00, SEG_k+SEG_V+SEG_vv, 0x00,	0x00,   0x00,L21_NUM12,	L22_NUM12,	SEG_DP5,//C���й�����
    
    //aux1	aux2	aux3    aux4	aux5	aux6	aux7    aux8   	aux9	aux10
    0x00,   0x00,   0x00, 	0x00,0x00,	0x00,   SEG_ZONG,0x00,	0x00,   	SEG_DP6,//�ܹ������� X.XXX11
    0x00,   0X00,   0x00,  	0x00,0x00,	0x00,   0x00,L21_NUM10,	L22_NUM10,  SEG_DP6,//A�๦������ X.XXX11
    0x00,   0X00,   0x00,  	0x00,0x00,	0x00,   0x00,L21_NUM11,	L22_NUM11,  SEG_DP6,//B�๦������ X.XXX11
    0x00,   0X00,   0x00,  	0x00,0x00,	0x00,   0x00,L21_NUM12,	L22_NUM12,	SEG_DP6,//C�๦������ X.XXX11
#endif     
    //SEG_S1+SEG_S2,  SEG_S13,0x00,//��ǰ�޹��ܵ��� 11
    
  
	
	//��3����ʷ���� 11
#ifdef RTC_8025T
	//aux1	aux2	aux3    aux4				aux5	aux6				aux7    aux8   							aux9				aux10           
    0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_vv,	SEG_h,	SEG_MONTH+SEG_QUAN,	0x00,	SEG_ACTIVE+SEG_DIAN+L21_NUM1,	SEG_LAST+L22_NUM1,	SEG_DP8,//��1���й��ܵ��� 11
    0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_vv,	SEG_h,	SEG_MONTH+SEG_QUAN,	0x00,	SEG_ACTIVE+SEG_DIAN+L21_NUM2,	SEG_LAST+L22_NUM2,	SEG_DP8,//��2���й��ܵ��� 11
    0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_vv,	SEG_h,	SEG_MONTH+SEG_QUAN,	0x00,	SEG_ACTIVE+SEG_DIAN+L21_NUM3,	SEG_LAST+L22_NUM3,	SEG_DP8,//��3���й��ܵ��� 11
#endif

#ifdef TARIFF_ENABLE
	//aux1	aux2	aux3    aux4	aux5	aux6				aux7    aux8   	aux9	aux10 
	0x00,  0x00,   	0x00, 	0x00,	0x00,  SEG_TIME1+SEG_DUAN, 0x00,	0x00,	SEG_DP4,SEG_DP5,      //ʱ�α� 11

#endif  
};
#else		//�µ���Ļ  ��Ҫ���й���ʾ��� 11
static const LCD_AUX_TABLE LCD_AUX_DISPLAY[]={
    //aux1	aux2	aux3    aux4	aux5	aux6			aux7    aux8   	aux9	aux10 	 
    0x00,   0x00,   0x00,   0x00,	0x00,   SEG_METERID, 	0x00,   0x00,	0x00,	0x00,  	//��Ÿ�6λ 11
    0x00,   0x00,   0x00,   0x00,	0x00,   SEG_METERID, 	0x00,   0x00,	0x00,	0x00,  	//��ŵ�6λ  11 
    
#ifdef RTC_8025T
	//aux1	aux2	aux3    aux4	aux5	aux6					aux7    aux8   	aux9	aux10  
	0x00,   0x00, 	0x00,   0x00,   0x00,   SEG_TIME1+SEG_TIME2, 	0x00,   0x00,	0x00,	SEG_DP5+SEG_DP8,    //����  11
	0x00,   0x00, 	0x00,   0x00,   0x00,   SEG_TIME1+SEG_TIME2, 	0x00,   0x00,	SEG_DP4,	SEG_DP5,    //ʱ��  11	
#endif
	     
    //aux1	aux2	aux3    aux4				aux5				aux6		aux7    	aux8   					aux9	aux10           
    0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_vv,	SEG_ACTIVE+SEG_h,	SEG_QUAN,	SEG_ZONG,	SEG_DIAN,	0x00,	SEG_DP8,//�й��ܵ��� 11
#ifdef LADDER_ENABLE
	0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_vv,	SEG_ACTIVE+SEG_h,	SEG_QUAN+SEG_MONTH,	SEG_ZONG,	SEG_DIAN,	SEG_CURRENT,	SEG_DP8,//��ǰ���й��ܵ��� 11
#endif    
    
	      
#ifdef TARIFF_ENABLE
	//aux1	aux2	aux3    aux4				aux5	aux6		aux7    aux8   					aux9	aux10 
	0x00,  0x00,   	0x00, 	SEG_k+SEG_V+SEG_vv,	SEG_ACTIVE+SEG_h,  SEG_QUAN,   SEG_T1, SEG_DIAN,	0x00,	SEG_DP8,      //����� 11
	0x00,  0x00,   	0x00,  	SEG_k+SEG_V+SEG_vv,	SEG_ACTIVE+SEG_h,  SEG_QUAN,   SEG_T2, SEG_DIAN,	0x00,	SEG_DP8,     //����� 11
	0x00,  0x00,  	0x00,  	SEG_k+SEG_V+SEG_vv,	SEG_ACTIVE+SEG_h,  SEG_QUAN,   SEG_T3, SEG_DIAN,	0x00,	SEG_DP8,     //ƽ���� 11
	0x00,  0x00,  	0x00,  	SEG_k+SEG_V+SEG_vv,	SEG_ACTIVE+SEG_h,  SEG_QUAN,   SEG_T4, SEG_DIAN,	0x00,	SEG_DP8,     //�ȵ��� 11
#endif
	
//���ѽ�� 11
#ifdef BONUS_PADI_ENABLE
	//aux1	aux2	aux3    aux4		aux5	aux6		aux7    aux8   	aux9	aux10
	0x00, 	0x00,	SEG_S21,SEG_YUAN,	0x00,	SEG_JINE,	0x00,	0x00,	0x00,	SEG_DP8,	//ʣ�����ѽ�� 11	
#endif	
	
#ifdef PRE_PAID_ENABLE
    //aux1	aux2	aux3    aux4	aux5	aux6		aux7    		aux8   	aux9		aux10
    0x00, 	0x00,	0x00,SEG_YUAN,	0x00,	SEG_JINE,	SEG_SHENGYU,	0x00,	SEG_CURRENT,SEG_DP8,	//��ǰʣ���� 11
    0x00, 	0x00,	0x00,SEG_YUAN,	0x00,	SEG_PRICE,	0x00,			SEG_DIAN,SEG_CURRENT,SEG_DP8,	//��� 11
#endif
	
	//aux1	aux2	aux3    aux4	aux5	aux6	aux7    aux8   		aux9		aux10   
#if(SINGLE_OR_THREE_METER==1) 
	0x00,	0x00,   0x00,  	SEG_A,	0x00,	0x00,   0x00,  	0x00,	0x00,	SEG_DP6,	//A����� 11
	0x00,	0x00,   0x00,  	SEG_V,	0x00,	0x00,   0x00,  	0x00,	0x00,	SEG_DP9,	//A����� 11
	0x00, 0x00,   0x00, SEG_k+SEG_V+SEG_vv, 0x00,	0x00,   SEG_ZONG,0x00,0x00,   SEG_DP5,//�й��ܹ��� 11	 
#else
	SEG_a,	0x00,   0x00,  	SEG_A,	0x00,	0x00,   0x00,  	0x00,	0x00,	SEG_DP6,	//A����� 11 
    SEG_b,	0x00,   0x00,  	SEG_A,	0x00,	0x00,   0x00,  	0x00,	0x00,	SEG_DP6,	//B����� 11  
    0x00,	SEG_c,   0x00,  SEG_A,	0x00,	0x00,   0x00,  	0x00,	0x00,	SEG_DP6,	//C����� 11  
    
    //aux1	aux2	aux3    aux4	aux5	aux6	aux7    aux8   		aux9		aux10   	    
	SEG_a,	0x00,   0x00,  	SEG_V,	0x00,	0x00,   0x00,  	0x00,	0x00,	SEG_DP9,//A���ѹ 11    
    SEG_b,	0x00,   0x00,  	SEG_V,	0x00,	0x00,   0x00,  	0x00,	0x00,	SEG_DP9,//B���ѹ 11
    0x00,	SEG_c,   0x00,  SEG_V,	0x00,	0x00,   0x00,  	0x00,	0x00,		SEG_DP9,//C���ѹ 11
    
    //aux1	aux2	aux3    aux4			aux5	aux6	aux7    aux8   	aux9		aux10   
    0x00, 0x00,   0x00, SEG_k+SEG_V+SEG_vv, SEG_ACTIVE+0x00,	0x00,   SEG_ZONG,	0x00,0x00,   SEG_DP5,//�й��ܹ��� 11
    SEG_a, 0x00,  0x00, SEG_k+SEG_V+SEG_vv, 0x00,	0x00,   0x00,0x00,	0x00,  SEG_DP5,//A���й�����
    SEG_b, 0x00,   0x00, SEG_k+SEG_V+SEG_vv, 0x00,	0x00,   0x00,0x00,	0x00,	SEG_DP5,//B���й�����
    0x00, SEG_c,   0x00, SEG_k+SEG_V+SEG_vv, 0x00,	0x00,   0x00,0x00,	0x00,	SEG_DP5,//C���й�����
    
    //aux1	aux2	aux3    aux4	aux5	aux6	aux7    aux8   	aux9	aux10
    0x00,   0x00,   0x00, 	0x00,0x00,	0x00,   SEG_ZONG,0x00,	0x00,   	SEG_DP6,//�ܹ������� X.XXX11
    SEG_a,   0X00,   0x00,  	0x00,0x00,	0x00,   0x00,0x00,	0x00,  SEG_DP6,//A�๦������ X.XXX11
    SEG_b,   0X00,   0x00,  	0x00,0x00,	0x00,   0x00,0x00,	0x00,  SEG_DP6,//B�๦������ X.XXX11
    0x00,   SEG_c,   0x00,  	0x00,0x00,	0x00,   0x00,0x00,	0x00,	SEG_DP6,//C�๦������ X.XXX11
#endif     
    //SEG_S1+SEG_S2,  SEG_S13,0x00,//��ǰ�޹��ܵ��� 11
//��3����ʷ���� 11
#ifdef RTC_8025T
	//aux1	aux2	aux3    aux4				aux5	aux6				aux7    aux8   							aux9				aux10           
    0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_vv,	SEG_ACTIVE+SEG_h,	SEG_MONTH+SEG_QUAN,	0x00,	SEG_DIAN+L21_NUM1,	SEG_LAST+L22_NUM1,	SEG_DP8,//��1���й��ܵ��� 11
    0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_vv,	SEG_ACTIVE+SEG_h,	SEG_MONTH+SEG_QUAN,	0x00,	SEG_DIAN+L21_NUM2,	SEG_LAST+L22_NUM2,	SEG_DP8,//��2���й��ܵ��� 11
    0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_vv,	SEG_ACTIVE+SEG_h,	SEG_MONTH+SEG_QUAN,	0x00,	SEG_DIAN+L21_NUM3,	SEG_LAST+L22_NUM3,	SEG_DP8,//��3���й��ܵ��� 11
#endif

#ifdef DEMAND_ENABLE
	//aux1	aux2	aux3    aux4	aux5	aux6		aux7    aux8   		aux9		aux10 
	0x00,  	0x00,   0x00, 	0x00,	0x00,  SEG_QUAN, 	0x00,	SEG_DEMAND,	SEG_CURRENT,SEG_DP5,      //������� 11
	0x00,  	0x00,   0x00, 	0x00,	0x00,  SEG_QUAN+SEG_TIME1+SEG_TIME2, 	0x00,	SEG_DEMAND,	SEG_CURRENT,SEG_DP5+SEG_DP8,      //�����������ʱ�� ������ 11
	0x00,  	0x00,   0x00, 	0x00,	0x00,  SEG_QUAN+SEG_TIME1+SEG_TIME2, 	0x00,	SEG_DEMAND,	SEG_CURRENT,SEG_DP8,      //�����������ʱ�� ʱ�� 11
#endif

#ifdef INACITVE_POWER_ENABLE
	//aux1	aux2	aux3    aux4				aux5		aux6		aux7    	aux8   					aux9	aux10           
    0x00,  0x00,   0X00, 	SEG_k+SEG_V+SEG_ar,	SEG_h,	SEG_QUAN,	SEG_ZONG,	SEG_INACTIVE+SEG_DIAN,	0x00,	SEG_DP8,//�޹��ܵ��� 11
#endif	
 	
#ifdef TARIFF_ENABLE
	//aux1	aux2	aux3    aux4	aux5	aux6				aux7    aux8   	aux9	aux10 
	0x00,  0x00,   	0x00, 	0x00,	0x00,  SEG_TIME1+SEG_DUAN, 0x00,	0x00,	SEG_DP4,SEG_DP5,      //ʱ�α� 11
#endif 

	
	

 
};	

#endif


static const unsigned char LCDCODE[16]=
{
	SegA+SegB+SegC+SegD+SegE+SegF,	//0
    SegB+SegC,	//1
    SegA+SegB+SegD+SegE+SegG,	//2
    SegA+SegB+SegC+SegD+SegG,	//3
    SegB+SegC+SegF+SegG,	//4
    SegA+SegC+SegD+SegF+SegG,	//5
    SegA+SegC+SegD+SegE+SegF+SegG,	//6
    SegA+SegB+SegC,	//7
    SegA+SegB+SegC+SegD+SegE+SegF+SegG,	//8
    SegA+SegB+SegC+SegD+SegF+SegG,	//9
    SegA+SegB+SegC+SegE+SegF+SegG,	//A
    SegC+SegD+SegE+SegF+SegG,	//b
    SegD+SegE+SegG,	//C
    SegB+SegC+SegD+SegE+SegG,	//d
    SegA+SegD+SegE+SegF+SegG,	//E
    SegA+SegE+SegF+SegG,	//F
};



#define RAM_POS     (0)
#define EEPROM_POS     (1)

//����� 11
#if ( SINGLE_OR_THREE_METER==1)

//��ʾ��Ŀ
static const LCD_TABLE LCD_DISPLAY[]=
{
	//len	position,   zeroPos	BCDOrNot	dataAddr
	0, 	RAM_POS,    0,		NO_Trans,	(uint16)&g_cardStatus,			//0 �忨��ʾ 11 
	0,	RAM_POS,    0,		NO_Trans,	(uint16)&g_buyPara.Status,		//1 ��״̬ 11
	4,	RAM_POS,    5,		BCDTransL4,	(uint16)&g_energy.Balance,	//2  ʣ���� 4λС�� 11	
	4,	RAM_POS,    5,		BCDTransL2,	(uint16)&g_energy.Accumulate,	//3 �ۼ��õ��� 11	
	4,	RAM_POS,    5,		BCDTransL4,	(uint16)&g_price,	//4  ��ǰ���� 11
		
	4,	RAM_POS,    5,		BCDTransL4,	(uint16)&g_energy.Overdraft,	//5 ����Ӧ������ 11
	4,	RAM_POS,    0,		NO_Trans,	(uint16)&g_addr.CardNum[0],		//6 ����Ӧ���� 11
	    	
	4,	RAM_POS,    5,		BCDTransL4,	(uint16)&g_buyPara.BuyQuan,		//7  ������ 11
	2,	RAM_POS,    0,		NO_Trans,	(uint16)&g_addr.Zone,			//8 �������� 11
	3,	RAM_POS,    0,		NO_Trans,	(uint16)&g_addr.MeterAddr[3],	//9 ����� 11
	3,	RAM_POS,    0,		NO_Trans,	(uint16)&g_addr.MeterAddr[0],	//10 ����� 11
	
	4,	RAM_POS,    3,		NO_Trans,	(uint16)&g_uip.PRms,	//11 ��ǰ���� 11
    4,	RAM_POS,    6,		NO_Trans,	(uint16)&g_uip.URms,	//12 ��ǰ��ѹ 11
    4,	RAM_POS,    4,		NO_Trans,	(uint16)&g_uip.IRms,	//13 ��ǰ���� 11 
	
	3,	RAM_POS,	3,		BCDTransI0,	(uint16)&g_para.LimitPower,		//14 L8����޵繦�� 11
	3,	RAM_POS,	3,		BCDTransI0,	(uint16)&g_buyPara.Alarm2,		//15 L9��������2 11
	
#ifdef RTC_8025T         
    4,	EEPROM_POS, 5,		BCDTransL2,	(uint16)offsetof(EEPROM_DATA,Last1_A1),	//16 �����ܵ��� 11
#else
	4,	EEPROM_POS, 5,		BCDTransL2,	(uint16)0,	//16 �����ܵ��� 11
#endif
	
	//len	position,   zeroPos	BCDOrNot	dataAddr
	3,		RAM_POS,	0,		NO_Trans,	(uint16)&g_date.day,		//17 ���� 11
	3,		RAM_POS,	0,		NO_Trans,	(uint16)&g_date.sec,		//18 ʱ�� 11
//#endif
    
	
		
//#ifdef TARIFF_ENABLE
    //len	position,   zeroPos	BCDOrNot	dataAddr
	4,		RAM_POS,    5,		BCDTransL2,(uint16)&g_energy.TariffQ1,	//19 ����1�õ��� 11
	4,		RAM_POS,    5,		BCDTransL2,(uint16)&g_energy.TariffQ2,	//20 ����2�õ��� 11 
	4,		RAM_POS,    5,		BCDTransL2,(uint16)&g_energy.TariffQ3,	//21 ����3�õ��� 11
	4,		RAM_POS,    5,		BCDTransL2,(uint16)&g_energy.TariffQ4,	//22 ����4�õ���	 11
//#endif	

	4,	    RAM_POS,	5,		BCDTransL2,	(uint16)&g_energy.CurrentQ,	//23  �����ۼ��õ��� 2λС�� 11	

//#ifdef BONUS_PADI_ENABLE	
	//len	position,   zeroPos	BCDOrNot	dataAddr
	4,		RAM_POS,    5,		BCDTransL4,	(uint16)&g_energy.BonusBalance,	//24 ����ʣ����	 11
//#endif

#ifdef RTC_8025T	
	//len	pos     	zeroPos	BCDOrNot	dataAddr
	//1,	    EEPROM_POS,	0,		NO_Trans,	offsetof(EEPROM_DATA,DayTalbeNum),	//25 ����ʱ�θ��� 11
	3,	    EEPROM_POS,	0,		NO_Trans,	offsetof(EEPROM_DATA,HHMMNN1),//25 ʱ�α� 24=3*8�ֽ� 11
	
	4,	    EEPROM_POS,5,		BCDTransL2,	offsetof(EEPROM_DATA,Last1_A1),	//26  ��1���ۼ��õ��� 2λС�� 11
	4,	    EEPROM_POS,5,		BCDTransL2,	offsetof(EEPROM_DATA,Last2_A1),	//27  ��3���ۼ��õ��� 2λС�� 11
	4,	    EEPROM_POS,5,		BCDTransL2,	offsetof(EEPROM_DATA,Last3_A1),	//28  ��3���ۼ��õ��� 2λС�� 11
#endif
};
#else	//����� 11
//��ʾ��Ŀ
static const LCD_TABLE LCD_DISPLAY[]=
{
	//len	pos     zeroPos	BCDOrNot	dataAddr
	0, 	    RAM_POS,0,		NO_Trans,	(uint16)&g_cardStatus,			//0 �忨��ʾ 11
	0, 	    RAM_POS,0,		NO_Trans,	(uint16)&g_buyPara.Status,		//1 ��״̬ 11

	4,	    RAM_POS,5,		BCDTransL4,	(uint16)&g_energy.Balance,	//2 ʣ���� 2λС�� 11
	4,	    RAM_POS,5,		BCDTransL2,	(uint16)&g_energy.Accumulate,	//3  �ۼ��õ��� 2λС�� 11

	
	4,	    RAM_POS,5,		BCDTransL4,	(uint16)&g_price,	//4  ��ǰ���� 11
	
	4,	    RAM_POS,5,		BCDTransL4,	(uint16)&g_energy.Overdraft,	//5 ����Ӧ������ 2λС�� 11
	4,	    RAM_POS,0,		NO_Trans,	(uint16)&g_addr.CardNum[0],		//6 ����Ӧ���� 11
	4,	    RAM_POS,5,		BCDTransL4,	(uint16)&g_buyPara.BuyQuan,		//7  ������ 2λС�� 11
	2,	    RAM_POS,0,		NO_Trans,	(uint16)&g_addr.Zone,			//8 ��������	 11
	3,	    RAM_POS,0,		NO_Trans,	(uint16)&g_addr.MeterAddr[3],	//9 ����� 11
	3,	    RAM_POS,0,		NO_Trans,	(uint16)&g_addr.MeterAddr[0],	//10 ����� 11
#if(LCD_TYPE_DEFINE==0x0823)
	//len	pos     zeroPos	BCDOrNot	dataAddr
	3,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[3].PRms,	//11 ��ǰ�й��ܹ��� 3λС�� 11
	3,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[0].PRms,	//12 A���й��ܹ��� 3λС�� 11
	3,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[1].PRms,	//13 B���й��ܹ��� 3λС��11
	3,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[2].PRms,	//14 C���й��ܹ��� 3λС�� 11
#elif(LCD_TYPE_DEFINE==0x0822)
    //len	pos     zeroPos	BCDOrNot	dataAddr
	3,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[3].PRms,	//11 ��ǰ�й��ܹ��� 2λС�� 11
	3,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[0].PRms,	//12 A���й��ܹ��� 2λС�� 11
	3,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[1].PRms,	//13 B���й��ܹ��� 2λС��11
	3,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[2].PRms,	//14 C���й��ܹ��� 2λС�� 11
#elif(LCD_TYPE_DEFINE==0x0806)
    //len	pos     zeroPos	BCDOrNot	dataAddr
	3,	    RAM_POS,3,		NO_Trans,	(uint16)&g_uip[3].PRms,	//11 ��ǰ�й��ܹ��� 4λС�� 11
	3,	    RAM_POS,3,		NO_Trans,	(uint16)&g_uip[0].PRms,	//12 A���й��ܹ��� 4λС�� 11
	3,	    RAM_POS,3,		NO_Trans,	(uint16)&g_uip[1].PRms,	//13 B���й��ܹ��� 4λС��11
	3,	    RAM_POS,3,		NO_Trans,	(uint16)&g_uip[2].PRms,	//14 C���й��ܹ��� 4λС�� 11
#endif

#if(LCD_TYPE_DEFINE==0x0823) 
	//len	pos     zeroPos	BCDOrNot	dataAddr
	2,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[0].URms,	//15 A���ѹ 1λС�� ���� 11
	2,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[1].URms,	//16 B���ѹ 1λС�� ���� 11
	2,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[2].URms,	//17 C���ѹ 1λС�� ���� 11
#elif( (LCD_TYPE_DEFINE==0x0822) || (LCD_TYPE_DEFINE==0x0806) )
    //len	pos     zeroPos	BCDOrNot	dataAddr
	2,	    RAM_POS,6,		NO_Trans,	(uint16)&g_uip[0].URms,	//15 A���ѹ 1λС��  11
	2,	    RAM_POS,6,		NO_Trans,	(uint16)&g_uip[1].URms,	//16 B���ѹ 1λС��  11
	2,	    RAM_POS,6,		NO_Trans,	(uint16)&g_uip[2].URms,	//17 C���ѹ 1λС��  11
#endif

#if( (LCD_TYPE_DEFINE==0x0823) || (LCD_TYPE_DEFINE==0x0806) )		
	//len	pos     zeroPos	BCDOrNot	dataAddr
	3,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[0].IRms,	//18 A����� 3λС�� 11
	3,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[1].IRms,	//19 B����� 3λС�� 11
	3,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[2].IRms,	//20 C����� 3λС�� 11
#elif (LCD_TYPE_DEFINE==0x0822)	
    //len	pos     zeroPos	BCDOrNot	dataAddr
	3,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[0].IRms,	//18 A����� 2λС�� 11
	3,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[1].IRms,	//19 B����� 2λС�� 11
	3,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[2].IRms,	//20 C����� 2λС�� 11    
#endif

	//len	pos     zeroPos	BCDOrNot	dataAddr
	3,	    RAM_POS,6,		BCDTransI0,	(uint16)&g_para.LimitPower,		//21 ����޵繦�� 11
	3,	    RAM_POS,6,		BCDTransI0,	(uint16)&g_buyPara.Alarm2,		//22 ��������2 11



#if( (LCD_TYPE_DEFINE==0x0823) || (LCD_TYPE_DEFINE==0x0806) )   
    //len	pos     zeroPos	BCDOrNot	dataAddr
	2,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[3].Factor,	//23 ��ǰ�й��ܹ������� 3λС�� 11
	2,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[0].Factor,	//24 A���й��ܹ�������  3λС�� 11
	2,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[1].Factor,	//25 B���й��ܹ�������  3λС�� 11
	2,	    RAM_POS,4,		NO_Trans,	(uint16)&g_uip[2].Factor,	//26 C���й��ܹ�������  3λС�� 11
#elif(LCD_TYPE_DEFINE==0x0822)	
    //len	pos     zeroPos	BCDOrNot	dataAddr
	2,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[3].Factor,	//23 ��ǰ�й��ܹ������� 2λС�� 11
	2,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[0].Factor,	//24 A���й��ܹ�������  2λС�� 11
	2,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[1].Factor,	//25 B���й��ܹ�������  2λС�� 11
	2,	    RAM_POS,5,		NO_Trans,	(uint16)&g_uip[2].Factor,	//26 C���й��ܹ�������  2λС�� 11    
    
#endif


//#ifdef RTC_8025T	
	//len	pos     zeroPos	BCDOrNot	dataAddr	
	3,	    RAM_POS,1,		NO_Trans,	(uint16)&g_date.day,		//27 ���� 11
	3,	    RAM_POS,1,		NO_Trans,	(uint16)&g_date.sec,		//28  ʱ�� 11
//#endif
	
//#ifdef TARIFF_ENABLE
	//len	pos     zeroPos	BCDOrNot	dataAddr
	4,	    RAM_POS,5,		BCDTransL2,	(uint16)&g_energy.TariffQ1,	//29 ����1�õ��� 11
	4,	    RAM_POS,5,		BCDTransL2,	(uint16)&g_energy.TariffQ2,	//30 ����2�õ��� 11
	4,	    RAM_POS,5,		BCDTransL2,	(uint16)&g_energy.TariffQ3,	//31 ����3�õ��� 11
	4,	    RAM_POS,5,		BCDTransL2,	(uint16)&g_energy.TariffQ4,	//32 ����4�õ���	 11
//#endif	

//#ifdef LADDER_ENABLE
	4,	    RAM_POS,5,		BCDTransL2,	(uint16)&g_energy.CurrentQ,	//33  �����ۼ��õ��� 2λС�� 11	
//#endif	

//#ifdef BONUS_PADI_ENABLE	
	//len	position,   zeroPos	BCDOrNot	dataAddr
	4,		RAM_POS,    5,		BCDTransL4,	(uint16)&g_energy.BonusBalance,	//34 ����ʣ����	 11
//#endif

#ifdef DEMAND_ENABLE
	3,	    EEPROM_POS,	3,		NO_Trans,	offsetof(EEPROM_DATA,CurDemand),//35 ������� 11
	3,	    EEPROM_POS,	1,		NO_Trans,	offsetof(EEPROM_DATA,CurDemand)+3,//36 �����������ʱ�� ������ 11
	2,		EEPROM_POS,	1,		NO_Trans,	offsetof(EEPROM_DATA,CurDemand)+6,//37 �����������ʱ�� ʱ�� 11
#else	
	3,	    EEPROM_POS,	3,		NO_Trans,	0,//35 ������� 11
	3,	    EEPROM_POS,	1,		NO_Trans,	0,//36 �����������ʱ�� ������ 11
	2,		EEPROM_POS,	1,		NO_Trans,	0,//37 �����������ʱ�� ʱ�� 11
#endif
	    	
#ifdef RTC_8025T
	//len	pos     	zeroPos	BCDOrNot	dataAddr
	3,	    EEPROM_POS,	0,		NO_Trans,	offsetof(EEPROM_DATA,HHMMNN1),//38 ʱ�α� 24=3*8�ֽ� 11
	
	4,	    EEPROM_POS,5,		BCDTransL2,	offsetof(EEPROM_DATA,Last1_A1),	//39  ��1���ۼ��õ��� 2λС�� 11
	4,	    EEPROM_POS,5,		BCDTransL2,	offsetof(EEPROM_DATA,Last2_A1),	//40  ��2���ۼ��õ��� 2λС�� 11
	4,	    EEPROM_POS,5,		BCDTransL2,	offsetof(EEPROM_DATA,Last3_A1),	//41  ��3���ۼ��õ��� 2λС�� 11
#endif

#ifdef INACITVE_POWER_ENABLE
    4,	    RAM_POS,5,		BCDTransL2,	(uint16)&g_Inactivenergy.InactiveQuan,	//42  �ۼ��޹��õ��� 2λС�� 11
#endif
};

#endif

/******************************************************************************************/
static uchar s_buf[LCD_NUM];

static uchar s_lcdPos;
static uchar s_cyclePos;

static uchar s_btnPos;

#ifdef TARIFF_ENABLE
	static uchar s_tariffCnt;
#endif

//function only used in this file
/******************************************************************************************/

static void TransBufData ( void );




/******************************************************************************************/

#if(PCB_VERSION_NEW==1)
__interrupt void ISRForKey1 ( void )
{	
	DI ( );
	
	if ( KEY1 == 1 )
	{
		g_cardFlag.Bit.KEY1BtnDown = 1;					
		g_cardFlag.Bit.BtnFreshLCD = 1;	
		g_btnFreezeLCD = BTN_FREEZE_LCD_TIME;	
		
		g_errLCDDispCnt = 0;		
	}
	else
	{
		LCDLED_ON();
	}
	EI ( );		
}
#endif

/*
*****************************************************************************************
* fuction name: DisplayFull
*
* Description : this function display the full segments
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void DisplayFull ( void )
{			   	   					
	uchar i,k;

    for ( i=0; i<LCD_NUM; i++ )
    {
		k = LCDSEG_ARRAY[i];
		LCDSEG[k] = 0xFf;	
	}

#ifdef TARIFF_ENABLE	
	s_tariffCnt = 0;
#endif
	
	return ;
}


/*
*****************************************************************************************
* fuction name: DisplayVersion
*
* Description : this function display the version
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void DisplayVersion ( void )
{			   	   						
	uchar i,k;

    for ( i=0; i<LCD_NUM; i++ )
	{
		k = LCDSEG_ARRAY[i];
		LCDSEG[k] = 0x00;	
	}
	
	LCDSEG[16] = SEGU;	
	
	LCDSEG[17] = LCDCODE[VERSION_HEAD_1];	
	
	LCDSEG[18] = LCDCODE[VERSION_HEAD_2];	
	LCDSEG[18] |= 0x01;	    //С���� 11
				
	LCDSEG[19] = LCDCODE[VERSION_UPDATE_1];	
										 					
	//������°汾 11
	LCDSEG[20] = LCDCODE[VERSION_UPDATE_2];	
	
	return ;
}

/*
*****************************************************************************************
* fuction name: DisplayClear
*
* Description : this function display the version
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void DisplayClear ( void )
{			   	   						
	uchar i,k;
	
    for ( i=0; i<LCD_NUM; i++ )
	{
		k = LCDSEG_ARRAY[i];
		LCDSEG[k] = 0x00;	
	}
	

	LCDSEG[16] = SEGC;	
	LCDSEG[17] = SEG_L;	
	LCDSEG[18] = SEGE;	
	LCDSEG[19] = SEGA;	
	LCDSEG[20] = SEGR;	
			       	    
    
	return ;
}

/*
*****************************************************************************************
* fuction name: DisplayCali
*
* Description : this function display the version
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void DisplayCali ( uchar num )
{			   	   						
	uchar i,k;

    for ( i=0; i<LCD_NUM; i++ )
	{
		k = LCDSEG_ARRAY[i];
		LCDSEG[k] = 0x00;	
	}
	
	LCDSEG[16] = SEGC;	
	LCDSEG[17] = SEGA;	
	LCDSEG[18] = SEG_L;	
	LCDSEG[19] = SEG_;	
	LCDSEG[20] = LCDCODE[num];	
				       	    
    
	return ;
}


/*
*****************************************************************************************
* fuction name: DisplayCardIn
*
* Description : this function display the version
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void DisplayCardIn ( void )
{			   	   						
	uchar i,k;

    for ( i=0; i<LCD_NUM; i++ )
	{
		k = LCDSEG_ARRAY[i];
		LCDSEG[k] = 0x00;	
	}
	
	LCDSEG[16] = SEGC;	
	LCDSEG[17] = SEGA;	
	LCDSEG[18] = SEGR;	
	LCDSEG[19] = SEGd;	
			       	    
    
	return ;
}
/*
*****************************************************************************************
* fuction name: DisplayNull
*
* Description : this function displays the error pulg-in symbol, 'C-'
*
* Arguments   : uchar number
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void DisplayInputNumber ( uchar *inbuf  )
{			   	   						
	uchar i,k;
	
	for ( i=0; i<LCD_NUM; i++ )
	{
		k = LCDSEG_ARRAY[i];
		LCDSEG[k] = 0x00;	
	}
	
	for ( i=0; i<4; i++ )
	{
    	s_buf[6-i*2+LCD_OFFSET] |= LCDCODE[(inbuf[i]>>4)&0x0f];
      	s_buf[7-i*2+LCD_OFFSET] |= LCDCODE[(inbuf[i])&0x0f];	
    			   	
	}
 	
 	for ( i=0; i<4; i++ )
	{
		k = LCDSEG_ARRAY[6-i*2+LCD_OFFSET];
    	LCDSEG[k] = LCDCODE[(inbuf[i]>>4)&0x0f];
    	
    	k = LCDSEG_ARRAY[7-i*2+LCD_OFFSET];
    	LCDSEG[k] = LCDCODE[(inbuf[i])&0x0f];	   	   	
	}
								
	return ;
}


/*
*****************************************************************************************
* fuction name: ManageLCDDisplay
*
* Description : This function visualize the request item according to the item NO.
*
* Arguments   : none
*
* returns     : none
*
* Notes     :
******************************************************************************************
*/
void ManageLCDDisplay ( void )
{  	 	  	  	  	
  	uchar i,temp,k;
  	uchar* pchar;
  	uint32 templong;
  	uchar buf[4];
  	
  	//��Ƴ�ʼ����ʾ 11
  	if ( g_initDispCnt > 0 )
  	{  	    
  	    return;
  	}
  	
  	if ( (g_commLCDDisp == 0) &&
  			(g_btnFreezeLCD==0) )
  	    LCDLED_OFF();
  	
  	//������ʾ���� ����  up 11
  	if ( g_cardFlag.Bit.KEY1BtnDown )
  	{
  		g_cardFlag.Bit.KEY1BtnDown = 0;
  		
  		s_btnPos = s_lcdPos;
  		s_btnPos++;
  	
  	#ifdef TARIFF_ENABLE	
  		if ( s_btnPos >= (sizeof(LCD_DISP_ARRAY)-1) )
  	#else
  		if ( s_btnPos >= sizeof(LCD_DISP_ARRAY) )
  	#endif
  		{
  		#ifdef TARIFF_ENABLE
  			//ʱ�α� 11
  			if ( s_btnPos == (sizeof(LCD_DISP_ARRAY)-1) )
  			{
  				I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,YearZoneNum), 
	    			(uchar*)&g_buf[0], 3, 1  ); 
	    		s_tariffCnt = ((g_buf[2]>>4)&0x0f)*10+g_buf[2]&0x0f;		//BCD�� 11
	    		
	    		if ( (s_tariffCnt>14) )	
	    		{
	    			s_tariffCnt = 0;
	    		} 		
  			}
  			
  			if ( (s_btnPos==(sizeof(LCD_DISP_ARRAY)+s_tariffCnt-1)) )
  			{ 				
  				if ( g_para.MeterTpye&(BIT0+BIT1) )
		  		{	  			
					g_LCDFrame = 1;		//������ʾ 11	
										
		  		}	
		  		else
		  		{
		  			s_btnPos = 0;	
	  				g_LCDFrame = LCD_DISP_ARRAY[s_btnPos];	
		  		}
  			}  			
	  		else
	  		{
	  			g_LCDFrame = LCD_DISP_ARRAY[sizeof(LCD_DISP_ARRAY)-1];
	  		} 
  			
  		#else  			  			
  			if ( g_para.MeterTpye&(BIT0+BIT1) &&
  				(s_btnPos==sizeof(LCD_DISP_ARRAY)) )
	  		{	  			
				g_LCDFrame = 1;		//������ʾ 11	
	  		}
	  		else
	  		{
	  			s_btnPos = 0;	
	  			g_LCDFrame = LCD_DISP_ARRAY[s_btnPos];
	  		} 
	  	#endif 			 			 			  			  		
  		}
  		else
  		{
  			g_LCDFrame = LCD_DISP_ARRAY[s_btnPos];	  			  			
  		}		
  		s_lcdPos = s_btnPos;  			
  	}
#if(SINGLE_OR_THREE_METER==3)
	//������ʾ���� �ݼ�  11
  	if ( g_cardFlag.Bit.KEY2BtnDown )
  	{
  		g_cardFlag.Bit.KEY2BtnDown = 0;
  		
  		s_btnPos = s_lcdPos;
  		
  		if ( s_btnPos > 0 )
  			s_btnPos--;
  		else
  		{
  		#ifdef TARIFF_ENABLE
  			I2CRead ( EEPROM_ADDR, offsetof(EEPROM_DATA,YearZoneNum), 
    			(uchar*)&g_buf[0], 3, 1  ); 
    		s_tariffCnt = ((g_buf[2]>>4)&0x0f)*10+g_buf[2]&0x0f;		//BCD�� 11
    		
    		if ( (s_tariffCnt>14) )	
    		{
    			s_tariffCnt = 0;
    		} 
    		
    		s_btnPos = sizeof(LCD_DISP_ARRAY)+s_tariffCnt;
  		#else
  			s_btnPos = 	sizeof(LCD_DISP_ARRAY)-1;
  		#endif	
  		}
  			
  		
  	#ifdef TARIFF_ENABLE	
  		if ( s_btnPos >= (sizeof(LCD_DISP_ARRAY)-1) )
  		{
  			  			
  			if ( (s_btnPos==(sizeof(LCD_DISP_ARRAY)+s_tariffCnt-1)) )
  			{
  				if ( g_para.MeterTpye&(BIT0+BIT1) )
		  		{	  			
					g_LCDFrame = 1;		//������ʾ 11	
		  		}	
		  		else
		  		{		  				
	  				s_btnPos--;
	  				g_LCDFrame = LCD_DISP_ARRAY[sizeof(LCD_DISP_ARRAY)-1];
		  		}
  			}  			
	  		else
	  		{
	  			g_LCDFrame = LCD_DISP_ARRAY[sizeof(LCD_DISP_ARRAY)-1];
	  		}
  		}
  		else
  		{
  			g_LCDFrame = LCD_DISP_ARRAY[s_btnPos];
  		}
  	#else		
  		if ( s_btnPos == (sizeof(LCD_DISP_ARRAY)-1) )
  		{ 		  		
  			if ( g_para.MeterTpye&(BIT0+BIT1) )
	  		{	  			
				g_LCDFrame = 1;		//������ʾ 11	
	  		}
	  		else
	  		{
	  				
	  			g_LCDFrame = LCD_DISP_ARRAY[s_btnPos];
	  		}  			 			 			  			  		
  		}
  		else
  		{
  			g_LCDFrame = LCD_DISP_ARRAY[s_btnPos];
  		}
  	#endif
  		
  		s_lcdPos = s_btnPos;
  			
  	}

#endif

 	if ( g_btnFreezeLCD > 0 )    
 	{
    	g_btnFreezeLCD--;
    	if ( g_btnFreezeLCD == 0 )
    	{
    		s_btnPos = 0;	
    		s_cyclePos = 0;	
    		s_lcdPos = 0;
    		
    		g_LCDFrame = LCD_DISP_ARRAY[s_cyclePos];
    	}
    }

  	g_lcdCycleCnt = LCD_CYCLE_TIME;
  	
  	if ( (g_meterFlag.Bit.CycleDisp == 1) &&
  		(( g_btnFreezeLCD == 0 )) &&
  		(g_cardDispCnt==0) )
  	{
  		g_meterFlag.Bit.CycleDisp = 0;
  		
  		s_cyclePos = s_lcdPos;
  		s_cyclePos++;
  	
  	#ifdef TARIFF_ENABLE	
  		//ѭ�Բ���ʾʱ��,����������-1 11
  		if ( s_cyclePos >= (sizeof(LCD_DISP_ARRAY)-1) )
  	#else
  		if ( s_cyclePos >= sizeof(LCD_DISP_ARRAY) )
  	#endif
  		{
  		#ifdef TARIFF_ENABLE	
  			if ( g_para.MeterTpye&(BIT0+BIT1) &&
  				(s_cyclePos==(sizeof(LCD_DISP_ARRAY)-1)) )
  		#else
  			if ( g_para.MeterTpye&(BIT0+BIT1) &&
  				(s_cyclePos==sizeof(LCD_DISP_ARRAY)) )
  		#endif 			
	  		{	  			
				g_LCDFrame = 1;		//������ʾ 11					
	  		}
	  		else
	  		{
	  			s_cyclePos = 0;	
	  			g_LCDFrame = LCD_DISP_ARRAY[s_cyclePos];
	  		}  
	  		
	  		if ( g_errStatus.Byte != 0 )
				g_errLCDDispCnt = 20;			 			 			  			  		
  		}
  		else
  		{
  			g_LCDFrame = LCD_DISP_ARRAY[s_cyclePos];
  		}
  		      		  		  	    	  	
  		s_lcdPos = s_cyclePos;
  	    
  	}	
 	
	//��ǰʣ���� �ж��Ƿ����0��������ʾ������ 11
	if ( g_LCDFrame == 2 )
	{
		if ( g_energy.Overdraft > 0 )
		{
			g_LCDFrame = 5;
		}
	}	
  	
#ifdef LADDER_ENABLE
	#if(SINGLE_OR_THREE_METER==1)
		if ( (g_LCDFrame==23) &&
  				(g_lcdFlag.Bit.LadderDisp==0) )	
  		{
  			s_lcdPos++;
  			g_LCDFrame = LCD_DISP_ARRAY[s_lcdPos];  		
  		}	
	#endif
	
	#if(LCD_TYPE_DEFINE==0x0806) 
  		if ( (g_LCDFrame==33) &&
  				(g_lcdFlag.Bit.LadderDisp==0) )	
  		{
  			s_lcdPos++;
  			g_LCDFrame = LCD_DISP_ARRAY[s_lcdPos];  		
  		}	
  	#endif
#endif

#ifdef TARIFF_ENABLE
//	#if(SINGLE_OR_THREE_METER==1)
//		if ( ((g_LCDFrame>=19)&&(g_LCDFrame<=22)) &&
//  				(g_lcdFlag.Bit.TariffDisp==0) )	
//  		{
//  			s_lcdPos += 23-g_LCDFrame;
//  			g_LCDFrame = LCD_DISP_ARRAY[s_lcdPos];  		
//  		}	
//	#endif
//	
//	#if(LCD_TYPE_DEFINE==0x0806) 
//  		if ( ((g_LCDFrame>=29)&&(g_LCDFrame<=32)) &&
//  				(g_lcdFlag.Bit.TariffDisp==0) )	
//  		{
//  			s_lcdPos += 33-g_LCDFrame;
//  			g_LCDFrame = LCD_DISP_ARRAY[s_lcdPos];  		
//  		}	
//  	#endif
#endif
 	
  	// 11
	if ( LCD_DISPLAY[g_LCDFrame].position == EEPROM_POS )
	{
#ifdef RTC_8025T	
	#if(SINGLE_OR_THREE_METER==1)
	    if ( g_LCDFrame == 25)
	#endif
	
	#if(LCD_TYPE_DEFINE==0x0806) 
	    if ( g_LCDFrame == 38)
	#endif
		{
			I2CRead ( EEPROM_ADDR, LCD_DISPLAY[g_LCDFrame].dataAddr+g_tariffOffset+3*(s_lcdPos+1-sizeof(LCD_DISP_ARRAY)), 
     			&buf[0], 3, 1  ) ;	
		}
	    else
	    {
#endif
	    	I2CRead ( EEPROM_ADDR, LCD_DISPLAY[g_LCDFrame].dataAddr, 
     			&buf[0], 4, 1  ) ;	
#ifdef RTC_8025T     			
	    }	
#endif	       
	    pchar = buf; 	           		    
	}
	else if ( LCD_DISPLAY[g_LCDFrame].position == RAM_POS )
	{
	    pchar = (uchar*)LCD_DISPLAY[g_LCDFrame].dataAddr;    		    
	}
  	
  	//�����ʾ�� 11
  	for ( i=0; i<LCD_NUM; i++ )
    {
		s_buf[i] = 0x00;
		k = LCDSEG_ARRAY[i];
		LCDSEG[k] = 0x00;	
	}
	
	  			
  	//���⴦��	�忨״̬ 11
  	if ( g_LCDFrame == 0)		
  	{
  		//if (  *pchar == 0 )	//������ȷ 11
  		if ( g_cardStatus == 0 )	//������ȷ 11
  		{
  			s_buf[2+LCD_OFFSET] = SEG_;
			s_buf[3+LCD_OFFSET] = SEGP;		    
			s_buf[4+LCD_OFFSET] = SEG_;		
  		}
  		else	//�������� 11
  		{
  			s_buf[2+LCD_OFFSET] = SEGE;		    
			s_buf[3+LCD_OFFSET] = SEG_;		            
			
			s_buf[4+LCD_OFFSET] = LCDCODE[(*pchar>>4)&0x0f];					
		 	s_buf[5+LCD_OFFSET] = LCDCODE[*pchar&0x0f];		
  		}
  	}
//  	else if (  g_commLCDDisp > 0 )//ͨѶ��ʾ 11
//  	{
//  		s_buf[2+5] = SEGE;
//		s_buf[3+5] = SEG_N;	//n	    
//		s_buf[4+5] = LCDCODE[13];	//d	
//		
//  	}	
  	else if ( g_errLCDDispCnt > 0 )//����״̬��ʾ 11
  	{
  		s_buf[1+LCD_OFFSET] = SEGE;	
  		s_buf[2+LCD_OFFSET] = SEGR;	
  		s_buf[3+LCD_OFFSET] = SEGR;    
		s_buf[4+LCD_OFFSET] = SEG_;		            
		
		s_buf[5+LCD_OFFSET] = LCDCODE[(g_errStatus.Byte>>4)&0x0f];					
	 	s_buf[6+LCD_OFFSET] = LCDCODE[g_errStatus.Byte&0x0f];		
  		LCDLED_ON();
  		
  		if ( g_errStatus.Byte==0 )
  			g_errLCDDispCnt = 0;	
  	}
  	
  	//�˷�״̬��ʾ 11
  	else if ( g_buyPara.Status == BACK_MONEY_STATUS )
  	{
  		s_buf[1+LCD_OFFSET] = SEG_;	
  		s_buf[2+LCD_OFFSET] = LCDCODE[7];
		s_buf[3+LCD_OFFSET] = LCDCODE[15];	    
		s_buf[4+LCD_OFFSET] = SEG_;			
  	}
  	
  	else if ( g_LCDFrame == 1 )		//�������բ��ʾ��ʾ 11
  	{
		s_buf[1+LCD_OFFSET] = SEG_;	
  		
  		if ( (g_para.MeterTpye&BIT0) == BIT0 )
  		{
  			s_buf[2+LCD_OFFSET] = LCDCODE[11];   //b 
			s_buf[3+LCD_OFFSET] = LCDCODE[13];   //d
  		}	
  		else if ( (g_para.MeterTpye&BIT1) == BIT1 )
  		{
  			s_buf[2+LCD_OFFSET] = SEG_L;   //L 
			s_buf[3+LCD_OFFSET] = LCDCODE[13];   //d
  		}		
  		     
		s_buf[4+LCD_OFFSET] = SEG_;		            			
  	}
  	

  	else
  	{
  		//hexת��λBCD 11
  		if ( LCD_DISPLAY[g_LCDFrame].BCDOrNot != NO_Trans )
  		{
  			if ( LCD_DISPLAY[g_LCDFrame].BCDOrNot == BCDTransL4 )
  			{
  				templong = *((uint32*)pchar)/100;	//2λ���� 11	
  			}
  			else if ( LCD_DISPLAY[g_LCDFrame].BCDOrNot == BCDTransL2 )
  			{
  				templong = *((uint32*)pchar);	//2λ���� 11	
  			}
  			else if ( LCD_DISPLAY[g_LCDFrame].BCDOrNot == BCDTransI0 )
  			{
  				templong = (uint32)*((uint16*)pchar);
  			}
  			
  			templong = Hex32ToBCD( templong );	
  			pchar = (uchar*)&templong;	  		 				
  		}  		
  	
#if(SINGLE_OR_THREE_METER==3)	 
  		//�������� ��ʾCoS 11
  	    if ( (g_LCDFrame>=23) && (g_LCDFrame<=26) )
  	    {  	          	         	        
  	        s_buf[LCD_OFFSET+0] |= SEGc;	
  	        s_buf[LCD_OFFSET+1] |= SEG0;	
  	        s_buf[LCD_OFFSET+2] |= SEGS;
  	    
  	    #if( (LCD_TYPE_DEFINE==0x0806) )   //3λС�� 11 
  	        	  	        
  	        s_buf[LCD_OFFSET+6] |= LCDCODE[(*pchar>>4)&0x0f];
  	        s_buf[LCD_OFFSET+7] |= LCDCODE[(*pchar)&0x0f];
  			pchar++;
  			
  			s_buf[LCD_OFFSET+4] |= LCDCODE[(*pchar>>4)&0x0f];
  	        s_buf[LCD_OFFSET+5] |= LCDCODE[(*pchar)&0x0f];
  	    #endif
  	    
	             
  	    }
  	   	       	      	    
  		else
#endif
  		{
  		    for	( i=0; i<LCD_DISPLAY[g_LCDFrame].len; i++ )
      		{
      			s_buf[6-i*2+LCD_OFFSET] |= LCDCODE[(*pchar>>4)&0x0f];
      			s_buf[7-i*2+LCD_OFFSET] |= LCDCODE[(*pchar)&0x0f];	
      			pchar++;
      		}   
  		    
  		}
  	  	      		
  		//���λ0 11
  		if ( LCD_DISPLAY[g_LCDFrame].zeroPos != 0 )
  		{
  			i = 0;
  			while( i < LCD_DISPLAY[g_LCDFrame].zeroPos )
			{
			    if ( (s_buf[i+LCD_OFFSET] != LCDCODE[0]) && 
			    		(s_buf[i+LCD_OFFSET] != SEGNULL)  )
			        break;
			    else
			        s_buf[i+LCD_OFFSET] = SEGNULL;
			    i++;    
			}			
  		}
  		
  		//��������ʾ 11	
  	//ʱ������ʾ 11	
	#if(SINGLE_OR_THREE_METER==1)
	    if ( g_LCDFrame == 25)
	#endif
	
	#if(LCD_TYPE_DEFINE==0x0806) 
	    if ( g_LCDFrame == 38)
	#endif
		{
			pchar = (uchar*)&LCD_AUX_DISPLAY[sizeof(LCD_AUX_DISPLAY)/sizeof(LCD_AUX_TABLE)-1].aux1;	
		} 
		else 
		{
			pchar = (uchar*)&LCD_AUX_DISPLAY[s_lcdPos].aux1;	
		}
  		
  		for	( i=0; i<9; i++ )
  			s_buf[8+LCD_OFFSET+i] = pchar[i];
  		//С������ʾ����բ��������ʾ  11
  	#if(SINGLE_OR_THREE_METER==1)
	    if ( g_LCDFrame == 25)
	#endif
	
	#if(LCD_TYPE_DEFINE==0x0806) 
	    if ( g_LCDFrame == 38)
	#endif
		{
			temp = LCD_AUX_DISPLAY[sizeof(LCD_AUX_DISPLAY)/sizeof(LCD_AUX_TABLE)-1].aux10;
		} 
		else 
		{
			temp = LCD_AUX_DISPLAY[s_lcdPos].aux10;	
		}
  		
  		for	( i=0; i<8; i++ )
  		{
  		    if ( (temp&BIT0)==BIT0 )
  		        s_buf[i+LCD_OFFSET] |= 0x01;
  		    temp >>= 1;       
  		}
    
  		//ʣ��������� 11
  		if ( (g_LCDFrame==5) )
  		{
  		#if(PCB_LCD_NEW==0)
  			s_buf[8+LCD_OFFSET] |= SEG_S10;  	//AUX1	8+0λ����ʼ 11
  		#else
  			s_buf[12+LCD_OFFSET] |= SEG_S10;  	//AUX5	8+4
  		#endif		
  		}
  		
  		//���� 11
  		if ( ((g_LCDFrame>=18) && (g_LCDFrame<=20)) ||
  			( (g_LCDFrame>=11) && (g_LCDFrame<=14)) ) //���� 11
  		{
  			if ( g_dirFlag.Bit.RevPFlag )
  				s_buf[9+LCD_OFFSET] |= SEG_S14;	//���ʷ��� 11
  		}
  		  		  		  	 	                   
        //��ַ��ʾҪ���⴦������H L 11
  		if ( g_LCDFrame == 9 )
  		{
  		    s_buf[LCD_OFFSET] |=  SEG_H;        		    
  		}
  		else if ( g_LCDFrame == 10 )
  		{
  		    s_buf[LCD_OFFSET] |=  SEG_L; 
  		    
  		#if(MODBUS_ENABLED==1) 	
  			s_buf[LCD_OFFSET+2] =  0; 
  			s_buf[LCD_OFFSET+3] =  0; 
  			s_buf[LCD_OFFSET+4] =  0;  
  		#endif       		    
  		}
  		
  		//ʱ�����ڼӵ� ��˸  11
  	#if (SINGLE_OR_THREE_METER==1 )
  		if ( g_LCDFrame == 18 )
  	#else
  		if ( g_LCDFrame == 28 )
  	#endif
  		{
  			if ( g_lcdFlag.Bit.TimerBlink )	
  			{
  				//s_buf[LCD_OFFSET+16] |=  0x80; 		//DP4
  				//s_buf[LCD_OFFSET+3] |=  0x01; 		//DP5
  				s_buf[LCD_OFFSET+15] |=  0x80; 		//DP7
  				s_buf[LCD_OFFSET+5] |=  0x01; 		//DP8
  			}
  		}
  		
		
		//���ѽ�� 11
		#if (SINGLE_OR_THREE_METER==1 )
			#ifdef BONUS_PADI_ENABLE
				if (g_LCDFrame==24)
				{
					s_buf[0] =  LCDCODE[15]; 		//F
					s_buf[1] =  LCDCODE[0]; 		//0	
				}
			#endif
		#endif		
		#if (LCD_TYPE_DEFINE==0x0806 )
			#ifdef BONUS_PADI_ENABLE	
				if (g_LCDFrame==34)
				{
					s_buf[0] =  LCDCODE[15]; 		//F
					s_buf[1] =  LCDCODE[0]; 		//0	
				}
			#endif
		#endif
		
		//ʱ������ʾ 11	
	#if(SINGLE_OR_THREE_METER==1)
	    if ( g_LCDFrame == 25)
	#endif
	
	#if(LCD_TYPE_DEFINE==0x0806) 
	    if ( g_LCDFrame == 38)
	#endif
		{
			temp = s_lcdPos+2-sizeof(LCD_DISP_ARRAY);
			
			if ( temp > 9 )
			{
				temp -= 10;
				
				s_buf[16] |= SEG_S0; 	
			}
			s_buf[15] |= LCDCODE2_half1[temp]; 
			s_buf[16] |= LCDCODE2_half2[temp]; 
		}
  	}
	
	
	
	//��ʾ�����ĸ�����ʱ���� 11
#ifdef TARIFF_ENABLE
	//if( (g_lcdFlag.Bit.TariffDisp==1) )		
	{
		if ( g_energy.TariffNum == 1 )
			s_buf[8] |= SEG_S7;
		else if ( g_energy.TariffNum == 2 )
			s_buf[8] |= SEG_S8;
		else if ( g_energy.TariffNum == 3 )
			s_buf[9] |= SEG_S11;			
		else if ( g_energy.TariffNum == 4 )
			s_buf[9] |= SEG_S12;	
	}					
#endif

  	 		
#if ( (LCD_TYPE_DEFINE==0x0806) || (SINGLE_OR_THREE_METER==1))	    
    //ͨѶ�绰��ʶ��ʾ 11
  	if (  (g_commLCDDisp > 0) && (g_relayCnt==0) )//ͨѶ��ʾ 11
  	{
		s_buf[16] |= SEG_S6;
  	}
  	
  	//��բ��ʾ ��˸�� 11
  	if ( (g_relayFlag.Bit.RelayOn == 0) && (g_relayCnt==0) )
    {
        s_buf[7] |= 0x01;
        LCDLED_ON(); 
        ALARM_ON();           
    }
  	else
    {
  	    if ( g_btnFreezeLCD == 0 )
  	    	LCDLED_OFF(); 	   
  	}  
#endif  	


	
    for ( i=0; i<LCD_NUM; i++ )
	{
		k = LCDSEG_ARRAY[i];
    	LCDSEG[k] = s_buf[i];		   	   	
	}
					
	return;
}


