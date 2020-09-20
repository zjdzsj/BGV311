/*
  -this file contains extern declarations of functions and variables used
  in energy.c
*/
#ifndef ENERGY_H

#define ENERGY_H

/******************************************************************************************/
#ifndef _ENERGY_DEFINE_
#define HEAD_MACRO_ENERGY	extern
#define ENERGY_NOT_INCLUDE 
#else	
#define HEAD_MACRO_ENERGY     	 
#endif
/******************************************************************************************/

//global variables statement
/*****************************************************************************************/

/*****************************************************************************************/

//global functions statement
/*****************************************************************************************/
HEAD_MACRO_ENERGY void InitEnergyPin ( void );
HEAD_MACRO_ENERGY void ManageEnergy ( void );
HEAD_MACRO_ENERGY void SaveEnergy ( uchar mode );
HEAD_MACRO_ENERGY void ManageTariff ( void );
HEAD_MACRO_ENERGY void ManageLadder ( void );

#ifdef DEMAND_ENABLE
	HEAD_MACRO_ENERGY void ManageDemand ( void );
#endif
/*****************************************************************************************/

#endif 