/******************************************************************************************
*  Copyright (c) 2010, *** R&D Department
*  All rights reserved 
*
*  FileName£º   	LCD.h
*  Author£º     	Roger
*  Date£º  			2011/11/15 14:57
*  Revised Date£º  	
*  Main Function£º  this file contains all the header files used in this project.
*  Version:			
*  Note£º 			 			
*********************************************************************************************/

#ifndef LCD_H

#define LCD_H

/********************************************************************************************/
#ifndef _LCD_DEFINE_
#define HEAD_MACRO_LCD	extern
#else
#define HEAD_MACRO_LCD	  
#endif
/********************************************************************************************/

//global variables
/******************************************************************************************/

/******************************************************************************************/

//global functions
/******************************************************************************************/

HEAD_MACRO_LCD void DisplayFull ( void );
HEAD_MACRO_LCD void DisplayVersion ( void );

HEAD_MACRO_LCD void ManageLCDDisplay ( void );
HEAD_MACRO_LCD void DisplayInputNumber ( uchar *inbuf  );

HEAD_MACRO_LCD void DisplayClear ( void );
HEAD_MACRO_LCD void DisplayCali ( uchar num );
HEAD_MACRO_LCD void DisplayCardIn ( void );
/******************************************************************************************/

#endif
