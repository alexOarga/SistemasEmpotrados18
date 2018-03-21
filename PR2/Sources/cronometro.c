
/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Area Arquitectura de Computadores
* Sistemas Empotrados I
*******************************************************************
* nombre fichero : cronometro.c  
*******************************************************************
* descripcion    : Realizacion de un cronometro basado en un m�dulo
*                  de reloj de sistema
* programador    : 
* lenguaje       : ANSI C para CodeWarrior 
********************************************************************/

#include "derivative.h"
#include "clock.h"


/* Simbolos y constantes */

#define TRUE 1
#define FALSE 0
const unsigned int periodo = 100; // periodo cada 100 ms

/* Variables globales */

unsigned char Detenido ;
/********************************************/
volatile unsigned int pulsador_1 = 0;
volatile unsigned int pulsador_2 = 0;
/********************************************/

/* Funciones */

void InitSystem (void) ;

void Configuracion_pulsadores (void) ; 
	/* Activa las interrupciones del KBI 
	     PTA1 -> pulsador START/STOP
	     PTA2 -> pulsador RESET (puesta a cero) 
	*/



void display (unsigned char queDisplay, unsigned char valor) ;

void Configuracion_visualizacion (void) ;



void main (void) {

  InitSystem () ;
  
  Configuracion_visualizacion () ;
   Configuracion_pulsadores () ;
   Reset_Clock () ;
   Start_Clock () ;
   
   siguiente = Get_Time () ;

   while (1) {
/********************************************/
	volatile unsigned int time = Get_Time();
	volatile unsigned int milisec = (time%1000)/100;
	volatile unsigned int sec_d2 = (time/1000)/10; 
	volatile unsigned int sec_d1 = (time/1000)%10; 

	display (0, milisec);
	display (1, sec_d1);
	display (2, sec_d2);
	

	siguiente += periodo ;
	delay_until(siguiente) ;   
/********************************************/
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
    
  PTAPE = 0xFF ;            // Input ports with pullup
  PTBPE = 0xFF ;
}



void Configuracion_pulsadores (void) {
	/* Activa las interrupciones del KBI 
	     PTA1 -> pulsador START/STOP
	     PTA2 -> pulsador RESET (puesta a cero) 
	*/

/********************************************/
	/*Inicializar puerto A (entrada de datos) */ 		
  PTAPE |= (PTAPTE_PTAPE1_MASK | PTAPE_PTAPE2_MASK );
  KBIPE = 0x3				// Enable pin 2 1
  KBIES = 0x0				// Falling edge
  KBISC |= 0x2				// Enable interruptions
/********************************************/ 
  
}
  
void interrupt 4 KBI_handler (void) {
/********************************************/
	KBISC |= 0x4;		//clear KBACK
	if( PTAD & 0x1 == 0x1 ){	// si se ha pulsado
		pulsador_1 = 1;
	}else if( PTAD & 0x2 == 0x2 ){
		pulsador_2 = 1;
	}
/********************************************/
}

void Configuracion_visualizacion (void){

  /* Inicializar PTC (visualizacion):
        DDRC = 0x7F --> PTC como salidas 
                        PTC0..PTC3 -> codigo; 
                        PTC4..PTC6 -> seleccion display */
  PTCD = 0 ;
  PTCDS = 0xFF ;
  PTCDD = 0xFF ;

  return;
}



void display (unsigned char queDisplay, unsigned char valor) {

  PTCD = ((~(0x1 << queDisplay)) << 4) | (valor&0x0F) ;
  return ;
  
}  
