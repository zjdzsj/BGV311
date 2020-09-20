/*
  -this file contains declarations of functions and variables used in protocol.c.
*/

#ifndef _STRUCTUTRES_H

#define _STRUCTUTRES_H

#include <stddef.h>
#define SIZE_OF(TYPE, MEMBER) sizeof(((TYPE *)0)->MEMBER)

//���ʵ����Ϣ 11
typedef struct MeterTariff_tag
{			
	unsigned int TariffPrice1;		//���ʵ��1  2�ֽ� 11
	unsigned int TariffPrice2;		//���ʵ��2  2�ֽ� 11
	unsigned int TariffPrice3;		//���ʵ��3  2�ֽ� 11
	unsigned int TariffPrice4;		//���ʵ��4  2�ֽ� 11
	unsigned int TariffPrice5;		//���ʵ��5  2�ֽ� 11
	unsigned int TariffPrice6;		//���ʵ��6  2�ֽ� 11
	
	unsigned char Reserved[3];		//1�ֽ�	 11							
	unsigned char Checksum;			//16
}Meter_Tariff;

//����ֵ��Ϣ 11
typedef struct MeterLadder_tag
{			
	unsigned int Ladder1;		//����ֵ1  2�ֽ� 11
	unsigned int Ladder2;		//����ֵ2  2�ֽ� 11
	unsigned int Ladder3;		//����ֵ3  2�ֽ� 11
	unsigned int Ladder4;		//����ֵ4  2�ֽ� 11
	unsigned int Ladder5;		//����ֵ5  2�ֽ� 11
	
	unsigned char Reserved[5];		//1�ֽ�	 11							
	unsigned char Checksum;			//16
}Meter_Ladder;

//���ݵ����Ϣ 11
typedef struct MeterLadderPrice_tag
{			
	unsigned int LadderPrice1;		//���ݵ��1  2�ֽ� 11
	unsigned int LadderPrice2;		//���ݵ��2  2�ֽ� 11
	unsigned int LadderPrice3;		//���ݵ��3  2�ֽ� 11
	unsigned int LadderPrice4;		//���ݵ��4  2�ֽ� 11
	unsigned int LadderPrice5;		//���ݵ��5  2�ֽ� 11
	unsigned int LadderPrice6;		//���ݵ��6  2�ֽ� 11
	
	unsigned char Reserved[3];		//3�ֽ�	 11							
	unsigned char Checksum;			//16
}Meter_LadderPrice;


typedef struct ENERGY_DEMAND_TABLE_tag
{
  	unsigned char 	ID2;			// DL/T 645 communication protocal identifier 2 
  	unsigned int  	ramAddr;		// ram address  2
  	unsigned int  	extendAddr;		// eeprom or flash address  2
  	
  	//total 1+2+2=5
}ENERGY_DEMAND_TABLE_ELEMENT;	

//�������ݱ� 02
typedef struct VARIABLE_TABLE_tag
{
  	unsigned long 	ID;				// DL/T 645 communication protocal identifier 4
  	
  	unsigned char  	size:3;    		// number of bytes  1  	 	 
  	unsigned char 	type:3;			// data type: hex or BCD ,others.	
  	unsigned char  	readOrWrite:2;	// 2--read only;1--write only; 0--read and write
  		
  	unsigned char	location:4;		// 2--2208 ; 1--CPU ram; 0--eeprom, here you can define yourselves's location.  	
  	unsigned char 	bcdCheck:4;		//1
  	
  	unsigned int  	ramAddr;		// ram address  2
  	
  	//total 10
}VARIABLE_TABLE;

//�¼���¼ 11
typedef struct EVENT_TABLE_tag
{
  	unsigned long 	ID;				// DL/T 645 communication protocal identifier 4
  	
  	unsigned char  	size;    		// number of bytes  1  	 	 
  	
  	unsigned char 	type:4;			// data type: hex or BCD ,others.	
  	unsigned char  	readOrWrite:4;	// 2--read only;1--write only; 0--read and write
  		
  	unsigned char	location:4;		// 2--2208 ; 1--CPU ram; 0--eeprom, here you can define yourselves's location.  	
  	unsigned char 	bcdCheck:4;		//1
  	
  	unsigned int  	extendAddr;		// eeprom or flash address  2
  	
  	//total 8
}EVENT_TABLE;

//�α������ݱ� 11
typedef struct PARAMETER_TABLE_tag
{
  	unsigned long 	ID;				// DL/T 645 communication protocal identifier 4
  	
  	unsigned char  	size;    		// number of bytes  1  	 	 
  	unsigned char 	type:4;			// data type: hex or BCD ,others.	
  	unsigned char  	readOrWrite:4;	// 2--read only;1--write only; 0--read and write
  		
  	unsigned char	location:4;		// 2--2208 ; 1--CPU ram; 0--eeprom, here you can define yourselves's location.  	
  	unsigned char 	bcdCheck:4;		//1
  	
  	unsigned int  	ramAddr;		// ram address  2
  	unsigned int  	extendAddr;		// eeprom or flash address  2
  	
  	//total 10
}PARAMETER_TABLE;

typedef struct PROTOCOL_REG_tag
{
  	unsigned char  	size;    		// number of bytes  1
  	unsigned char 	type:4;			// data type: hex or BCD ,others.	
  	unsigned char  	readOrWrite:4;	// 2--read only;1--write only; 0--read and write
  	
  	unsigned char	location:4;		// 2--����оƬ ; 1--CPU ram; 0--eeprom, here you can define yourselves's location. 	
  	unsigned char 	CheckType:3;	//0��BCD, 1:hex;
  	unsigned char	keyOrNot:1;		//0:programme key;1:��Կ  2
  	unsigned char 	checksumOrNot;  //У��ģʽ	1	 11 
  	unsigned int  	addr;			// ram address  2
  	unsigned int  	eAddr;		// eeprom or flash address  2
  	//total 1+2=3
}PROTOCOL_REG;	

/*
typedef struct ENERGY_DEMAND_TABLE_tag
{
  	unsigned int 	registerNumber;	// DL/T 645 communication protocal identifier 2
  	unsigned char  	size;    		// number of bytes  1  	 
  	unsigned char 	type:2;			// data type: hex or BCD ,others.	
  	unsigned char  	readOrWrite:2;	// 2--read only;1--write only; 0--read and write
  	unsigned char	location:4;		// 2--����оƬ ; 1--CPU ram; 0--eeprom, here you can define yourselves's location.
  	unsigned char 	CheckType:4;	//0��BCD, 1:hex;
  	unsigned char	keyOrNot:4;		//0:programme key;1:��Կ  2
  	unsigned char 	checksumOrNot;  //У��ģʽ	1
  	unsigned int  	ramAddr;		// ram address  2
  	unsigned int  	extendAddr;		// eeprom or flash address  2
  	
  	//total 2+1+1+1+1+2+2=10
}ENERGY_DEMAND_TABLE_ELEMENT;	

*/	

typedef struct LCD_TABLE_tag
{
 	    
  	unsigned char   len:7;	//���ݳ��� 11
  	unsigned char   position:1;   //���ݴ洢λ�� 11
  	
  	unsigned char   zeroPos:4;	//��λ����λ�� 11
  	unsigned char   BCDOrNot:4;	//�Ƿ����BCDת�� 11
  			
  	unsigned int  	dataAddr;	// eeprom or flash address  2
  	
  	//total 4
}LCD_TABLE;



#endif
