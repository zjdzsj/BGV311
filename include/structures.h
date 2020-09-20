/*
  -this file contains declarations of functions and variables used in protocol.c.
*/

#ifndef _STRUCTUTRES_H

#define _STRUCTUTRES_H

#include <stddef.h>
#define SIZE_OF(TYPE, MEMBER) sizeof(((TYPE *)0)->MEMBER)

//费率电价信息 11
typedef struct MeterTariff_tag
{			
	unsigned int TariffPrice1;		//费率电价1  2字节 11
	unsigned int TariffPrice2;		//费率电价2  2字节 11
	unsigned int TariffPrice3;		//费率电价3  2字节 11
	unsigned int TariffPrice4;		//费率电价4  2字节 11
	unsigned int TariffPrice5;		//费率电价5  2字节 11
	unsigned int TariffPrice6;		//费率电价6  2字节 11
	
	unsigned char Reserved[3];		//1字节	 11							
	unsigned char Checksum;			//16
}Meter_Tariff;

//阶梯值信息 11
typedef struct MeterLadder_tag
{			
	unsigned int Ladder1;		//阶梯值1  2字节 11
	unsigned int Ladder2;		//阶梯值2  2字节 11
	unsigned int Ladder3;		//阶梯值3  2字节 11
	unsigned int Ladder4;		//阶梯值4  2字节 11
	unsigned int Ladder5;		//阶梯值5  2字节 11
	
	unsigned char Reserved[5];		//1字节	 11							
	unsigned char Checksum;			//16
}Meter_Ladder;

//阶梯电价信息 11
typedef struct MeterLadderPrice_tag
{			
	unsigned int LadderPrice1;		//阶梯电价1  2字节 11
	unsigned int LadderPrice2;		//阶梯电价2  2字节 11
	unsigned int LadderPrice3;		//阶梯电价3  2字节 11
	unsigned int LadderPrice4;		//阶梯电价4  2字节 11
	unsigned int LadderPrice5;		//阶梯电价5  2字节 11
	unsigned int LadderPrice6;		//阶梯电价6  2字节 11
	
	unsigned char Reserved[3];		//3字节	 11							
	unsigned char Checksum;			//16
}Meter_LadderPrice;


typedef struct ENERGY_DEMAND_TABLE_tag
{
  	unsigned char 	ID2;			// DL/T 645 communication protocal identifier 2 
  	unsigned int  	ramAddr;		// ram address  2
  	unsigned int  	extendAddr;		// eeprom or flash address  2
  	
  	//total 1+2+2=5
}ENERGY_DEMAND_TABLE_ELEMENT;	

//变量数据表 02
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

//事件记录 11
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

//参变量数据表 11
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
  	
  	unsigned char	location:4;		// 2--计量芯片 ; 1--CPU ram; 0--eeprom, here you can define yourselves's location. 	
  	unsigned char 	CheckType:3;	//0：BCD, 1:hex;
  	unsigned char	keyOrNot:1;		//0:programme key;1:密钥  2
  	unsigned char 	checksumOrNot;  //校验模式	1	 11 
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
  	unsigned char	location:4;		// 2--计量芯片 ; 1--CPU ram; 0--eeprom, here you can define yourselves's location.
  	unsigned char 	CheckType:4;	//0：BCD, 1:hex;
  	unsigned char	keyOrNot:4;		//0:programme key;1:密钥  2
  	unsigned char 	checksumOrNot;  //校验模式	1
  	unsigned int  	ramAddr;		// ram address  2
  	unsigned int  	extendAddr;		// eeprom or flash address  2
  	
  	//total 2+1+1+1+1+2+2=10
}ENERGY_DEMAND_TABLE_ELEMENT;	

*/	

typedef struct LCD_TABLE_tag
{
 	    
  	unsigned char   len:7;	//数据长度 11
  	unsigned char   position:1;   //数据存储位置 11
  	
  	unsigned char   zeroPos:4;	//高位清零位置 11
  	unsigned char   BCDOrNot:4;	//是否进行BCD转换 11
  			
  	unsigned int  	dataAddr;	// eeprom or flash address  2
  	
  	//total 4
}LCD_TABLE;



#endif
