/*
  -this file contains declarations of functions and variables used in timer.c
*/

#ifndef _TIMER_H

#define _TIMER_H

/******************************************************************************************/
#ifndef _TIMER_DEFINE_
#define HEAD_MACRO_TIMER	extern
#else	
#define HEAD_MACRO_TIMER           	 
#endif
/******************************************************************************************/

//variables definition or statement 
/******************************************************************************************/


/******************************************************************************************/

//function definition or statement
/******************************************************************************************/
HEAD_MACRO_TIMER void InitTimerArray ( void );
HEAD_MACRO_TIMER void delay_4us ( void );
HEAD_MACRO_TIMER void delay_time ( void );

HEAD_MACRO_TIMER void Timer4_38k_start(void);
HEAD_MACRO_TIMER void Timer4_38k_stop(void);

HEAD_MACRO_TIMER void Timer5_1200bps_start(void);
HEAD_MACRO_TIMER void Timer5_1200bps_stop(void);

HEAD_MACRO_TIMER void Delay1MsTime ( uint16 n );
/******************************************************************************************/

#endif 