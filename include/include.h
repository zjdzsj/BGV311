/******************************************************************************************
*  Copyright (c) 2010, Bona R&D Department
*  All rights reserved 
*
*  FileName£º   	include.h
*  Author£º     	Roger.
*  Date£º  			2010-3-8 20:11
*  Revised Date£º  	
*  Main Function£º  this file contains all the header files used in this project.
*  Version:			BonaV2.0
*  Note£º 			 			
*********************************************************************************************/




#include "macro.h"
#include "EEPROM.h"


#include "structures.h"
#include "para_structures.h"

#include "main.h"
#include "LCD.h"
#include "timer.h"
#include "common.h"
#include "I2C.h"

#if(IC_CARD_MODE==AT24_CARD)
	#include "ICCard.h"
	#include "CardI2C.h"
#endif

#if (IC_CARD_MODE==RF_CARD)
	#include "rf_SPI.h"
	#include "CV520.h"
	#include "rf_ICCard.h"
	
#endif

#include "energy.h"



#if(METER_CHIP_MODEL==0x2208)
	#include "cl2208.h"
	#include "uart2.h"
#else if (METER_CHIP_MODEL==0x8302)
	#include "RN8302.h"	
	#include "SPI.h"
#endif

#include "protocol_talbe.h"
#include "comm.h"
#include "buyprocess.h"
//#include <stdlib.h>

