#include "derivative.h"

#include "encoder.h"
#include "pwm.h"
#include "ad.h"
#include "servos.h"
#include "clock.h"


static void InitSystem (void) ;

unsigned int siguiente, periodo = 100 ;
float E, E_ant, U ;
float Y, Yd;



void main(void) {
 
 InitSystem () ;
 Init_Encoder ();
 Init_PWM () ;
 Init_AD () ;
 Init_Servos (periodo) ;
 
 Reset_Clock () ;
 Start_Clock () ;
 
 
 siguiente = Get_Time () ;


  E_ant = 0.0 ;
  U=0.0;
  
  while(1) {
    Y = velocity();
    Yd = Read_Value_Int_1();
    Yd=8.0*(float)Yd/1024.0;
    Yd-=4.0;
    E=R(Yd,Y);
    U = (0.572*E) - (0.286*E_ant) + U ;
    action (U) ;
    E_ant = E ;    
    siguiente += periodo ;
    delay_until (siguiente) ;
   }
}


void InitSystem (void) {

  /* Internal Clock Source Configuration */
	if(NVICSTRM == 0xFF)			// If programmed trim value is blank
		ICSTRM = 0x80;				  // load default value
	else
		ICSTRM = NVICSTRM;	  	// else load programmed trim value
  ICSC2_BDIV = 1 ;          // Divides selected clock by 2		
	                          // Bus clock = 5 MHz, CPU clock = 10MHz

  
  SOPT1_COPT = 0 ;          // WATCHDOG disable 
}
