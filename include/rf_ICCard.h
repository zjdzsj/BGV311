/******************************************************************************************
*  Copyright (c) 2010, Bona Design Department(Tai Yuan)
*  All rights reserved 
*
*  FileName£º   	ICCard.h
*  Author£º     	Bona Meter
*  Date£º  			2010-3-8 20:11
*  Revised Date£º  	
*  Main Function£º  this file contains declarations of functions and variables used in ICCard.c.
*  Version:			BonaV2.0
*  Note£º 			 			
*********************************************************************************************/
#if (IC_CARD_MODE==RF_CARD)
	
#ifndef _ICCard_H

#define _ICCard_H

/******************************************************************************************/
#ifndef _ICCard_DEFINE_
#define HEAD_MACRO_ICCard	extern
#else	
#define HEAD_MACRO_ICCard           	 
#endif
/******************************************************************************************/

//variables definition or statement 
/******************************************************************************************/
//HEAD_MACRO_I2C uchar g_beepCnt;

/******************************************************************************************/

//function definition or statement
/******************************************************************************************/
#ifdef PRE_PAID_ENABLE
    HEAD_MACRO_ICCard void InitICCarVariables ( void );
#endif

HEAD_MACRO_ICCard void ManageRF_ICCard ( void );
//HEAD_MACRO_ICCard void ClearAllData ( uint32 preBuy, uint16 price );

HEAD_MACRO_ICCard uchar ReadBlockData ( uchar block, uchar *pbuf );     
HEAD_MACRO_ICCard uchar WriteBlockData ( uchar block, uchar *pbuf )
/******************************************************************************************/

#endif 

#endif
