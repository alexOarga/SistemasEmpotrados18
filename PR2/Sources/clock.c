
/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Area Arquitectura de Computadores
* Sistemas Empotrados I
*******************************************************************
* nombre fichero : clock.c 
*******************************************************************
* proyecto       : Realizacion de un reloj en el S08SV16
* programador    : 
* lenguaje       : ANSI C para CodeWarrior 
********************************************************************/


/*******************************************************************

Suponemos Internal Reference Clock
FLL engaged
BDIV = 1 => /2
BUSCLK = 5 MHz
El tick del reloj de sistema se fija en 1 ms

********************************************************************/

#include "derivative.h"
#include "clock.h"


/* Variables del modulo */

volatile static unsigned int tick_counter ;


/* Implementacion servicios */

void interrupt 6 Tick (void) {
	if (MTIMSC_TOF) MTIMSC_TOF = 0 ;
	tick_counter ++ ;
}



void Reset_Clock (void) {

  asm  sei ;
 
  tick_counter = 0 ;
/********************************************/
   MTIMSC_TRST = 1; // reset MTIM counter
/********************************************/
  asm cli ;
  
  return;
}


void Start_Clock (void) {

/********************************************/
  	MTIMCLK_CLKS = 0; // CLKS = 00, ClockSource = BUSCLK
	MTIMCLK_PS = 5;	// PS = 0101, ClockSource/32, 5 MHz/32 (6.4 us)
	MTIMMOD = 155; 	// tick = 1 ms (aprox)
	
	
	MTIMSC_TRST = 1; // reset MTIM counter
	MTIMSC_TOIE = 1; // enable MTIM interruption
	MTIMSC_TSTP = 0;  // start MTIM
/********************************************/	
	
  	return;
}

void Stop_Clock (void) {

  MTIMSC_TOIE=0;
  return;

}

unsigned int Get_Time (void) {
/********************************************/
	unsigned int tick_counter_copy ;
	asm sei ;
	tick_counter_copy = tick_counter ;
	asm cli ;
	return tick_counter_copy ;
/********************************************/
}

void delay_until(unsigned int T){

  while(!(T==tick_counter))
    asm WAIT ;
}

